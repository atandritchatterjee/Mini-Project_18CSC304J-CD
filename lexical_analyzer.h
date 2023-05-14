#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <fstream>
#include <string>
#include <unordered_map>

enum TokenType
{
    IDENTIFIER,
    OPERATOR,
    LITERAL,
    KEYWORD,
    PUNCTUATION,
    END_OF_FILE
};

struct Token
{
    TokenType type;
    std::string value;
};

class LexicalAnalyzer
{
public:
    LexicalAnalyzer(const std::string &input_file)
    {
        input_stream.open(input_file);
        if (!input_stream.is_open())
        {
            std::cerr << "Error opening input file: " << input_file << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    Token getNextToken()
    {
        std::string token_value;
        char c = input_stream.get();
        TokenType token_type;

        while (isspace(c))
        {
            c = input_stream.get();
        }

        if (isalpha(c) || c == '_')
        {
            token_type = IDENTIFIER;
            while (isalnum(c) || c == '_')
            {
                token_value += c;
                c = input_stream.get();
            }
            input_stream.putback(c);
            if (keyword_map.count(token_value) > 0)
            {
                token_type = keyword_map[token_value];
            }
        }
        else if (isdigit(c))
        {
            token_type = LITERAL;
            while (isdigit(c) || c == '.')
            {
                token_value += c;
                c = input_stream.get();
            }
            input_stream.putback(c);
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
                 c == '=' || c == '<' || c == '>' || c == '&' || c == '|' ||
                 c == '!' || c == '^' || c == '~')
        {
            token_type = OPERATOR;
            token_value += c;
            char lookahead = input_stream.peek();
            if ((c == '+' && lookahead == '+') ||
                (c == '-' && lookahead == '-') ||
                (c == '=' && lookahead == '=') ||
                (c == '<' && lookahead == '=') ||
                (c == '>' && lookahead == '=') ||
                (c == '&' && lookahead == '&') ||
                (c == '|' && lookahead == '|'))
            {
                input_stream.get();
                token_value += lookahead;
            }
        }
        else if (c == '(' || c == ')' || c == '{' || c == '}' ||
                 c == '[' || c == ']' || c == ';' || c == ',')
        {
            token_type = PUNCTUATION;
            token_value += c;
        }
        else if (input_stream.eof())
        {
            token_type = END_OF_FILE;
        }
        else
        {
            std::cerr << "Error: Unknown token found" << std::endl;
            exit(EXIT_FAILURE);
        }

        return Token{token_type, token_value};
    }

private:
    std::ifstream input_stream;
    std::unordered_map<std::string, TokenType> keyword_map = {
        {"int", KEYWORD},
        {"float", KEYWORD},
        {"double", KEYWORD},
        {"char", KEYWORD},
        {"bool", KEYWORD},
        {"if", KEYWORD},
        {"else", KEYWORD},
        {"while", KEYWORD},
        {"for", KEYWORD},
        {"do", KEYWORD},
        {"return", KEYWORD},
        {"true", LITERAL},
        {"false", LITERAL}};
};

#endif
