#ifndef MYSTRUCT_H
#define MYSTRUCT_H


struct literal
{
    char p;
    int neg;
    bool operator<(literal &o)
    {
        if (neg != o.neg)
        {
            return neg < o.neg;
        }
        return p < o.p;
    }
};

struct node
{
    node *left, *right;
    bool type;
    char val;
    node(char val, bool type)
    {
        this->type = type;
        this->val = val;
        left = NULL;
        right = NULL;
    }
    node(bool type)
    {
        this->type = type;
        left = NULL;
        right = NULL;
    }
    void setop(char val)
    {
        this->val = val;
    }
};

struct proposition_set
{
    map<char, bool> P;
    void insert(char p)
    {
        P.insert({p, 0});
    }
    bool value(char p)
    {
        return P[p];
    }
    void set_values()
    {
        for (auto &x : P)
        {
            cout << "Enter value of " << x.first << ": ";
            cin >> x.second;
        }
    }
};

struct operators
{
    map<char, string> op_encoded;
    map<string, char> op_decoded;
    operators()
    {
        op_encoded['-'] = "negation";
        op_encoded['.'] = "and";
        op_encoded['+'] = "or";
        op_encoded['>'] = "implies";

        for (auto x : op_encoded)
            op_decoded[x.second] = x.first;
    }
    bool isop(char ch)
    {
        if (op_encoded.find(ch) == op_encoded.end())
            return 0;
        return 1;
    }
    bool isneg(char ch)
    {
        if (op_encoded.find(ch) == op_encoded.end())
            return 0;
        if (op_encoded[ch] == "negation")
            return 1;
        return 0;
    }
    bool isbop(char ch)
    {
        if (op_encoded.find(ch) == op_encoded.end())
            return 0;
        if (op_encoded[ch] == "negation")
            return 0;
        return 1;
    }
    bool isprop(char ch)
    {
        if (op_encoded.find(ch) != op_encoded.end())
            return 0;
        if (ch == '(' || ch == ')')
            return 0;
        return 1;
    }
};

struct formula_tree
{
    node *root;
    proposition_set P;
    operators op;
    formula_tree(string &alpha, operators &op)
    {
        if (build(alpha, 0, alpha.length() - 1, &root) < 0)
        {
            cout << "Not a wff!\n";
            exit(0);
        }
        this->op = op;
    }
    int build(string &alpha, int l, int r, node **root);
    void traverse(node *root);
    bool evaluate(node *root);
    bool satisfiability_check();
    bool complete_search(int, vector<char>);
};

struct cnf
{
    vector<vector<literal>> clauses;
    operators op;
    proposition_set P;
    cnf(operators &op, formula_tree &tr);
    vector<vector<literal>> convert(node *phi);
    void display();
    bool evaluate();
    bool satisfiability_check();
    bool DPLL(vector<vector<literal>> clauses, set<char> symbols, map<char, bool> model);
};


struct dnf
{
    vector<vector<literal>> clauses;
    operators op;
    proposition_set P;
    dnf(operators &op, formula_tree &tr);
    vector<vector<literal>> convert(node *phi);
    void display();
    bool evaluate();
    bool satisfiability_check();
};
#endif