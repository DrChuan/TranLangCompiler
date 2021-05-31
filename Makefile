LEX=lex  
YACC=yacc
CC=g++  
OBJECT=main #生成的目标文件  
  
$(OBJECT): lex.yy.o y.tab.o main.o utility.o AST.o TreeScanner.o SymbolTable.o
	$(CC) lex.yy.o y.tab.o main.o utility.o AST.o TreeScanner.o SymbolTable.o -o $(OBJECT) -g

main.o: main.cpp
	$(CC) -c main.cpp -g

AST.o: AST.cpp AST.h
	$(CC) -c AST.cpp -g

TreeScanner.o: TreeScanner.cpp TreeScanner.h
	$(CC) -c TreeScanner.cpp -g

utility.o: utility.cpp utility.h
	$(CC) -c utility.cpp -g

lex.yy.o: lex.yy.c y.tab.h utility.h AST.h
	$(CC) -c lex.yy.c  -g
  
y.tab.o: y.tab.c utility.h AST.h
	$(CC) -c y.tab.c  -g
  
y.tab.c: tran.y  
# bison使用-d参数编译.y文件  
	$(YACC) -d tran.y  

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"
  
lex.yy.c: tran.l  
	$(LEX) tran.l  
  
clean:  
	@rm -f $(OBJECT)  *.o  y.tab.* lex.yy.c

# a.out: main.o y.tab.o lex.yy.o utility.o
# 	g++ -g main.o y.tab.o lex.yy.o utility.o

# main.o: main.cpp utility.h
# 	g++ -g -c main.cpp

# y.tab.o: y.tab.c
# 	g++ -g -c y.tab.c

# y.tab.c: tran.y
# 	yacc -dv tran.y

# y.tab.h: y.tab.c
# 	echo "y.tab.h was created at the same time as y.tab.c"

# lex.yy.o: lex.yy.c y.tab.h utility.h
# 	g++ -g -c lex.yy.c

# lex.yy.c: tran.l
# 	lex tran.l

# utility.o: utility.cpp utility.h
# 	g++ -g -c utility.cpp

# clean: 
# 	rm -f a.out utility.o main.o lex.yy.o y.tab.c y.tab.h y.tab.o y.output lex.yy.c