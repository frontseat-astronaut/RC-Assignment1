#include <bits/stdc++.h>
using namespace std;
#include "header.h"
dnf::dnf(operators &op, formula_tree &tr)
{
    this->op = op;
    this->P = tr.P;
    clauses = convert(tr.root);
}
vector<vector<literal>> dnf::convert(node *phi)
{
    vector<vector<literal>> ret;
    if (phi->type)
    {
        vector<literal> clause;
        clause.push_back({phi->val, 0});
        ret.push_back(clause);
        return ret;
    }
    string binary_operator = op.op_encoded[phi->val];
    if (binary_operator == "or")
    {
        vector<vector<literal>> P = convert(phi->left);
        vector<vector<literal>> Q = convert(phi->right);
        ret = P;
        ret.insert(ret.end(), Q.begin(), Q.end());
        return ret;
    }
    if (binary_operator == "and")
    {
        vector<vector<literal>> P = convert(phi->left);
        vector<vector<literal>> Q = convert(phi->right);
        for (auto &pi : P)
        {
            for (auto &qi : Q)
            {
                vector<literal> clause(pi.size() + qi.size());
                set_union(pi.begin(), pi.end(), qi.begin(), qi.end(), clause.begin());
                while (!clause.back().p)
                    clause.pop_back();
                ret.push_back(clause);
            }
        }
        return ret;
    }
    if (binary_operator == "negation")
    {
        node *psi = phi->left;
        if (psi->type)
        {
            vector<literal> clause;
            clause.push_back({psi->val, 1});
            ret.push_back(clause);
            return ret;
        }
        string operator_1 = op.op_encoded[psi->val];
        if (operator_1 == "negation")
        {
            return convert(psi->left);
        }
        if (operator_1 == "or")
        {
            node *new_phi = new node(0);
            new_phi->setop(op.op_decoded["and"]);
            node *left_neg = new node(0);
            left_neg->setop(op.op_decoded["negation"]);
            node *right_neg = new node(0);
            right_neg->setop(op.op_decoded["negation"]);

            left_neg->left = psi->left;
            right_neg->left = psi->right;
            new_phi->left = left_neg;
            new_phi->right = right_neg;
            return convert(new_phi);
        }
        if (operator_1 == "and")
        {
            node *new_phi = new node(0);
            new_phi->setop(op.op_decoded["or"]);
            node *left_neg = new node(0);
            left_neg->setop(op.op_decoded["negation"]);
            node *right_neg = new node(0);
            right_neg->setop(op.op_decoded["negation"]);

            left_neg->left = psi->left;
            right_neg->left = psi->right;
            new_phi->left = left_neg;
            new_phi->right = right_neg;
            return convert(new_phi);
        }
        if (operator_1 == "implies")
        {
            node *new_phi = new node(0);
            new_phi->setop(op.op_decoded["and"]);
            node *right_neg = new node(0);
            right_neg->setop(op.op_decoded["negation"]);

            right_neg->left = psi->right;
            new_phi->right = right_neg;
            new_phi->left = psi->left;
            return convert(new_phi);
        }
    }
    if (binary_operator == "implies")
    {
        node *new_phi = new node(0);
        new_phi->setop(op.op_decoded["or"]);
        node *left_neg = new node(0);
        left_neg->setop(op.op_decoded["negation"]);

        left_neg->left = phi->left;
        new_phi->left = left_neg;
        new_phi->right = phi->right;
        return convert(new_phi);
    }
    return ret;
}
void dnf::display()
{
    cout<<"DNF form:\n{ ";
    for(int i=0; i<clauses.size(); ++i)
    {
        cout<<"{";
        for(int j=0; j<clauses[i].size(); ++j)
        {
            if(clauses[i][j].neg)
                cout<<'-';
            cout<<clauses[i][j].p<<" ";
        }
        cout<<"} ";
    }
    cout<<"}\n";
}
bool dnf::evaluate()
{
    bool ret = 0;
    for (int i = 0; i < clauses.size(); ++i)
    {
        bool c = 1;
        for (int j = 0; j < clauses[i].size(); ++j)
        {
            c &= (clauses[i][j].neg) ^ (P.value(clauses[i][j].p));
            if (!c)
                break;
        }
        ret |= c;
        if (ret)
            break;
    }
    return ret;
}
bool dnf::satisfiability_check()
{
    for (int i = 0; i < clauses.size(); ++i)
    {
        bool f = 1;
        map<char, bool> p;
        for (auto x : clauses[i])
        {
            if (p.find(x.p) != p.end())
            {
                if (p[x.p] != x.neg)
                {
                    f = 0;
                    break;
                }
                continue;
            }
            p[x.p] = x.neg;
        }
        if (f)
            return 1;
    }
    return 0;
}