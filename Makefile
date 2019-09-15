make_assignment: cnf.cpp dnf.cpp formula_tree.cpp header.h demo.cpp 
	@echo "Compiling..."
	@echo ""
	@g++ -c cnf.cpp
	@g++ -c dnf.cpp
	@g++ -c formula_tree.cpp
	@g++ -c demo.cpp
	@g++ cnf.o dnf.o formula_tree.o demo.o -o go
	@./go