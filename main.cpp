#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include "lexical_analyzer.h"
#include "dag.h"

using namespace std;

void pm(string exp)
{
    string exp1 = "", exp2 = "";
    int l = exp.length(), i = 0, j = 0;
    while (exp[i] != '+' && exp[i] != '-')
    {
        i++;
    }
    exp2.append(exp, 0, i);
    reverse(exp.begin(), exp.end());
    j = l - i - 1;
    exp1.append(exp, 0, j);
    reverse(exp1.begin(), exp1.end());
    cout << "\tThree address code:\n";
    cout << "\ttemp=" << exp1 << endl;
    cout << "\ttemp1=" << exp[j + 1] << exp[j] << "temp\n";
}

void div(string exp) 
{
    string exp1 = "";
    int l = exp.length(), i = 0;
    while (exp[i] != '/' && exp[i] != '*')
    {
        i++;
    }
    exp1.append(exp, 0, i + 2);
    cout << "\tThree address code:\n";
    cout << "\ttemp=" << exp1 << endl;
    cout << "\ttemp1=temp" << exp[i + 2] << exp[i + 3] << endl;
}

void pplus(string exp)
{
    string exp1 = "";
    int l = exp.length(), i = 0;
    while (exp[i] != '+' && exp[i] != '-')
    {
        i++;
    }
    exp1.append(exp, 0, i + 2);
    cout << "\tThree address code:\n";
    cout << "\ttemp=" << exp1 << endl;
    cout << "\ttemp1=temp" << exp[i + 2] << exp[i + 3] << endl;
}

int main()
{
    system("cls");
    // Lexical analysis
    cout << "---------------------------------------------------------------------------------------------------------------------\n";
    cout << "\t\t\t\t\t\tLEXICAL ANALYZER\n";
    cout << "---------------------------------------------------------------------------------------------------------------------\n\n";
    LexicalAnalyzer lex("program.txt");

    Token t;
    do
    {
        t = lex.getNextToken();
        cout << t.value << " - ";
        switch (t.type)
        {
        case IDENTIFIER:
            cout << "Identifier" << endl;
            break;
        case OPERATOR:
            cout << "Operator" << endl;
            break;
        case LITERAL:
            cout << "Literal" << endl;
            break;
        case KEYWORD:
            cout << "Keyword" << endl;
            break;
        case PUNCTUATION:
            cout << "Punctuation" << endl;
            break;
        case END_OF_FILE:
            cout << "End of File" << endl;
            break;
        }
    } while (t.type != END_OF_FILE);

    cout << endl;
    system("pause");

    cout << "\n---------------------------------------------------------------------------------------------------------------------\n";
    cout << "\t\t\t\t\t\tTHREE ADDRESS CODE\n";
    cout << "---------------------------------------------------------------------------------------------------------------------\n";

    // Three address code generation
    ifstream infile("program.txt");
    if (!infile)
    {
        cerr << "Error: Could not open input file\n";
        return 1;
    }
    string exp;
    int sn = 1;
    while (infile >> exp)
    {
        cout << '\n'
             << sn << ". Expression: " << exp << "\n\n";
        int l = exp.length();
        string exp1 = "";
        for (int i = 0; i < l; i++)
        {
            if (exp[i] == '+' || exp[i] == '-')
            {
                if (exp[i + 2] == '/' || exp[i + 2] == '*')
                {
                    pm(exp);
                    break;
                }
                else
                {
                    pplus(exp);
                    break;
                }
            }
            else if (exp[i] == '/' || exp[i] == '*')
            {
                div(exp);
                break;
            }
        }
        sn++;
    }

    cout << endl;
    system("pause");

    cout << "\n---------------------------------------------------------------------------------------------------------------------\n";
    cout << "\t\t\t\t\t\tDIRECTED ACYCLIC GRAPH\n";
    cout << "---------------------------------------------------------------------------------------------------------------------\n";

    // Directed acyclic graph generation
    ifstream inputFile("program.txt");
    if (!inputFile)
    {
        cerr << "Error: Input file could not be opened." << endl;
        return 1;
    }

    int n = 0;
    string line;
    while (getline(inputFile, line))
    {
        n++;
    }

    string *st = new string[n];

    inputFile.clear();
    inputFile.seekg(0, ios::beg);

    for (int i = 0; i < n; i++)
    {
        getline(inputFile, st[i]);
    }

    inputFile.close();

    unordered_map<char, DAG *> labelDAGNode;

    for (int i = 0; i < n; i++)
    {
        string stTemp = st[i];
        for (int j = 0; j < 5; j++)
        {
            char tempLabel = stTemp[0];
            char tempLeft = stTemp[2];
            char tempData = stTemp[3];
            char tempRight = stTemp[4];
            DAG *leftPtr;
            DAG *rightPtr;
            if (labelDAGNode.count(tempLeft) == 0)
            {
                leftPtr = new DAG(tempLeft);
            }
            else
            {
                leftPtr = labelDAGNode[tempLeft];
            }
            if (labelDAGNode.count(tempRight) == 0)
            {
                rightPtr = new DAG(tempRight);
            }
            else
            {
                rightPtr = labelDAGNode[tempRight];
            }
            DAG *nn = new DAG(tempLabel, tempData, leftPtr, rightPtr);
            labelDAGNode.insert(make_pair(tempLabel, nn));
        }
    }

    cout << "\n\tLabel      ptr      leftPtr       rightPtr\n"
         << endl;
    cout<<"   .....................................................\n";
    sn = 1;
    for (int i = 0; i < n; i++)
    {
        if (i % 4 == 0)
        {
            cout << sn << ".";
            sn++;
        }
        DAG *x = labelDAGNode[st[i][0]];
        if ((i + 1) % 4 == 0)
        {
            cout << '\n';
            continue;
        }
        cout << "\t" << st[i][0] << "             " << x->data << "            ";
        if (x->left->label == '_')
            cout << x->left->data;
        else
            cout << x->left->label;
        cout << "          ";
        if (x->right->label == '_')
            cout << x->right->data;
        else
            cout << x->right->label;
        cout << endl;
    }

    for (auto &it : labelDAGNode)
    {
        delete it.second;
    }

    delete[] st;

    cout << endl;
    system("pause");

    return 0;
}