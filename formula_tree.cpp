#include <bits/stdc++.h>
using namespace std;
#include "header.h"

int formula_tree::build(string &alpha, int l, int r, node **root)
{
    if (l > r)
        return -1;
    if (l == r)
    {
        if (!op.isprop(alpha[l]))
            return -1;
        P.insert(alpha[l]);
        *root = new node(alpha[l], 1);
        return 0;
    }
    *root = new node(0);
    int cnt, i;
    if (alpha[l] == '(')
    {
        cnt = 0;
        for (i = l + 1; i <= r; ++i)
        {
            cnt += (alpha[i] == '(') - (alpha[i] == ')');
            if (cnt == -1)
                break;
        }
        if (i >= r)
            return -1;
        if (build(alpha, l + 1, i - 1, &((*root)->left)) < 0)
            return -1;
        i++;
    }
    else if (op.isneg(alpha[l]))
    {
        i = l + 1;
        (*root)->setop(alpha[l]);
        if (i == r)
        {
            if (build(alpha, i, i, &((*root)->left)) < 0)
                return -1;
            return 0;
        }
        else if (i < r)
        {
            if (alpha[i] != '(' || alpha[r] != ')')
                return -1;
            if (build(alpha, i + 1, r - 1, &((*root)->left)) < 0)
                return -1;
        }
        else
            return -1;
        return 0;
    }
    else if (op.isprop(alpha[l]))
    {
        P.insert(alpha[l]);
        if (build(alpha, l, l, &((*root)->left)) < 0)
            return -1;
        i = l + 1;
    }
    else
        return -1;

    if (!op.isbop(alpha[i]) || i >= r)
        return -1;
    (*root)->setop(alpha[i]);
    i++;

    if (i > r)
        return -1;

    if (alpha[i] == '(')
    {
        cnt = 0;
        l = i;
        for (i = i + 1; i <= r; ++i)
        {
            cnt += (alpha[i] == '(') - (alpha[i] == ')');
            if (cnt == -1)
                break;
        }
        if (i != r)
            return -1;
        if (build(alpha, l + 1, r - 1, &((*root)->right)) < 0)
            return -1;
    }
    else if (op.isprop(alpha[i]))
    {
        P.insert(alpha[i]);
        if (i != r)
            return -1;
        if (build(alpha, i, i, &((*root)->right)) < 0)
            return -1;
    }
    else
        return -1;
    return 0;
}
void formula_tree::traverse(node *root)
{
    if (root == NULL)
        return;
    cout << root->val << " ";
    traverse(root->left);
    traverse(root->right);
}
bool formula_tree::evaluate(node *root)
{
    if (root->type)
        return P.value(root->val);
    string opr = op.op_encoded[root->val];
    if (opr == "and")
        return evaluate(root->left) & evaluate(root->right);
    if (opr == "or")
        return evaluate(root->left) | evaluate(root->right);
    if (opr == "implies")
        return (!evaluate(root->left)) | evaluate(root->right);
    if (opr == "negation")
        return !evaluate(root->left);
    return 1;
}

bool formula_tree::satisfiability_check()
{
    vector<char>prop_list;
    for(auto x: P.P)
        prop_list.push_back(x.first);
    return complete_search(0, prop_list);
}

bool formula_tree::complete_search(int i, vector<char>prop_list)
{
    if(i==prop_list.size())
        return evaluate(root);
    P.P[prop_list[i]]=0;
    if(complete_search(i+1, prop_list))
        return 1;
    P.P[prop_list[i]]=1;
    return complete_search(i+1, prop_list);
}