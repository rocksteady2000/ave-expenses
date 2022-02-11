 # build an executable from expenses.cpp
 MAINPROG= bin/expenses
 CC = gcc
 CFLAGS = -Wall -std=c++2a 
 LDFLAGS = -lstdc++
 COVFLAGS = -fprofile-arcs -ftest-coverage
 OBJ_FILE = build/expenses.o
 SRC_FILE = src/expenses.cpp
 COV_FILE = *gcda *gcno *gcov

all: ${OBJ_FILE}
	${CC} ${COVFLAGS} ${LDFLAGS} $< -o ${MAINPROG}

${OBJ_FILE}: ${SRC_FILE}
	${CC} ${CFLAGS} -c  -o $@ $^

#takes command line parameter for filename
run:
	./${MAINPROG} ${filename}
clean: 
	${RM} ${OBJ_FILE} ${MAINPROG} ${COV_FILE}
