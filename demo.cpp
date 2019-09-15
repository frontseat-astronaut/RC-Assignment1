#include <bits/stdc++.h>
using namespace std;
#include "header.h"
int main()
{
    operators op;
    cout<<"Notation: \n";
    for(auto x: op.op_encoded)
    {
        cout<<""<<x.second<<": '"<<x.first<<"'\n";
    }
    cout<<"\nSome notes regarding input:\n1. Don't add outer parentheses\n2. No spaces between characters\n3. Propositions can only be represented by single characters\n";
    string alpha;
    cout<<"\nEnter a well formed formula: ";
    cin >> alpha;
    formula_tree tr = formula_tree(alpha, op);
    cout<<"\nPrefix representation: ";
    tr.traverse(tr.root);
    cout<<"\n\n";
    cnf C = cnf(op, tr);
    C.display();
    cout<<'\n';
    dnf D = dnf(op, tr);
    D.display();
    cout<<'\n';
    if(C.satisfiability_check())
        cout<<"CNF is Satisfiable (checked by DPLL)!\n";
    else 
        cout<<"CNF is Unsatisfiable (checked by DPLL)!\n";
    if(D.satisfiability_check())
        cout<<"DNF is Satisfiable!\n";
    else 
        cout<<"DNF is Unsatisfiable!\n";
    if(tr.satisfiability_check())
        cout<<"Formula is Satisfiable (checked by complete search)!\n";
    else 
        cout<<"Formula is Unsatisfiable (checked by complete search)!\n";
    
    return 0;
}