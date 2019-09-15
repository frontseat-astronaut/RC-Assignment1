#include<bits/stdc++.h>
using namespace std;
#include "header.h"

cnf::cnf(operators &op, formula_tree &tr)
{
    this->op = op;
    this->P = tr.P;
    clauses = convert(tr.root);
}
vector<vector<literal>> cnf::convert(node *phi)
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
    if (binary_operator == "and")
    {
        vector<vector<literal>> P = convert(phi->left);
        vector<vector<literal>> Q = convert(phi->right);
        ret = P;
        ret.insert(ret.end(), Q.begin(), Q.end());
        return ret;
    }
    if (binary_operator == "or")
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
void cnf::display()
{
    cout<<"CNF form:\n{ ";
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
bool cnf::evaluate()
{
    bool ret = 1;
    for (int i = 0; i < clauses.size(); ++i)
    {
        bool c = 0;
        for (int j = 0; j < clauses[i].size(); ++j)
        {
            c |= (clauses[i][j].neg) ^ (P.value(clauses[i][j].p));
            if (c)
                break;
        }
        ret &= c;
        if (!ret)
            break;
    }
    return ret;
}
bool cnf::satisfiability_check()
{
    set<char> symbols;
    for (auto x : P.P)
        symbols.insert(x.first);
    map<char, bool> model;
    return DPLL(clauses, symbols, model);
}
bool cnf::DPLL(vector<vector<literal>> clauses, set<char> symbols, map<char, bool> model)
{
    vector<vector<literal>> modified_clauses;
    for (int i = 0; i < clauses.size(); ++i)
    {
        vector<literal> clause;
        int keep = 2;
        for (int j = 0; j < clauses[i].size(); ++j)
        {
            if (model.find(clauses[i][j].p) != model.end())
            {
                bool value = (clauses[i][j].neg) ^ (model[clauses[i][j].p]);
                if (value)
                {
                    keep = 0;
                    break;
                }
            }
            else
            {
                keep = 1;
                clause.push_back(clauses[i][j]);
            }
        }
        if (keep == 2)
            return 0;
        if (keep == 1)
        {
            modified_clauses.push_back(clause);
        }
    }
    if (modified_clauses.empty())
        return 1;

    //pure symbols
    map<char, bool> pure_symbols;
    for (int i = 0; i < modified_clauses.size(); ++i)
    {
        for (int j = 0; j < modified_clauses[i].size(); ++j)
        {
            if (pure_symbols.find(modified_clauses[i][j].p) != pure_symbols.end())
            {
                if (pure_symbols[modified_clauses[i][j].p] == modified_clauses[i][j].neg)
                    pure_symbols.erase(modified_clauses[i][j].p);
            }
            else
                pure_symbols[modified_clauses[i][j].p] = !modified_clauses[i][j].neg;
        }
    }
    if (!pure_symbols.empty())
    {
        for (auto x : pure_symbols)
        {
            model[x.first] = x.second;
            symbols.erase(x.first);
        }
        return DPLL(modified_clauses, symbols, model);
    }

    map<char, bool> unit_clauses;
    for (int i = 0; i < modified_clauses.size(); ++i)
    {
        if (modified_clauses[i].size() == 1)
        {
            if (unit_clauses.find(modified_clauses[i][0].p) != unit_clauses.end())
            {
                if (unit_clauses[modified_clauses[i][0].p] == modified_clauses[i][0].neg)
                    return 0;
            }
            unit_clauses[modified_clauses[i][0].p] = !modified_clauses[i][0].neg;
        }
    }
    if (!unit_clauses.empty())
    {
        for (auto x : unit_clauses)
        {
            model[x.first] = x.second;
            symbols.erase(x.first);
        }
        return DPLL(modified_clauses, symbols, model);
    }
    char p = *(symbols.begin());
    symbols.erase(p);
    model[p] = 1;
    if (DPLL(modified_clauses, symbols, model))
        return 1;
    model[p] = 0;
    return DPLL(modified_clauses, symbols, model);
}