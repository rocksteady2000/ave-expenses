 # build an executable from expenses.cpp
 MAINPROG=../bin/expenses
 CC = gcc
 CFLAGS = -Wall -std=c++2a 
 LDFLAGS = -lstdc++
 OBJ_FILE = ../build/expenses.o
 SRC_FILE = expenses.cpp

all: ${OBJ_FILE}
	${CC}  ${LDFLAGS} $< -o ${MAINPROG}

${OBJ_FILE}: ${SRC_FILE}
	${CC} ${CFLAGS} -c  -o $@ $^

run:
	./${MAINPROG} ${filename}

 clean: 
	${RM} ${OBJ_FILE} ${MAINPROG}
