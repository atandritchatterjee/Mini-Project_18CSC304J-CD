#ifndef DAG_H
#define DAG_H

#include <iostream>
#include <unordered_map>
#include <string>

class DAG
{
public:
    char label;
    char data;
    DAG *left;
    DAG *right;

    DAG(char x);
    DAG(char lb, char x, DAG *lt, DAG *rt);
};

DAG::DAG(char x)
{
    label = '_';
    data = x;
    left = nullptr;
    right = nullptr;
}

DAG::DAG(char lb, char x, DAG *lt, DAG *rt)
{
    label = lb;
    data = x;
    left = lt;
    right = rt;
}

#endif
