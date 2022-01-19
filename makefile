 # build an executable from expenses.cpp
  all: src/expenses.cpp 
	gcc src/expenses.cpp -lstdc++ -std=c++11 -o bin/a.out

  clean: 
	  $(RM) expenses
