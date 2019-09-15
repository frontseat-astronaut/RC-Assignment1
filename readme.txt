Run 'make' command on any linux terminal to run the code.

The program takes as input a well formed formula (wff) as a string (and print out an error if the format is incorrect) and forms a formula tree out of it. It then can convert the formula into CNF and DNF. For DNF, a simple linear algorithm and for CNF, the DPLL algorithm is implemented to check satisfiability of the formula. 

A guide for usage of symbols:
  '+' : OR
  '-' : NEGATION
  '.' : AND
  '>' : IMPLIES

A few correct sample strings:
  1. p.q
  2. p+(q+(r.p)) 
  3. ((-q).q)+r
A few incorrect strings:
  1. (p.q) [outer parentheses are not supported] 
    correction: p.q
  2. p.-p [add parentheses!] 
    correction: p.(-p)
  3. p+q+r [add parentheses!]
    correction: p+(q+r) *or* (p+q)+r
