a.out: main.o y.tab.o lex.yy.o utility.o
	cc -g main.o y.tab.o lex.yy.o utility.o

main.o: main.c utility.h
	cc -g -c main.c

y.tab.o: y.tab.c
	cc -g -c y.tab.c

y.tab.c: tran.y
	yacc -dv tran.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

lex.yy.o: lex.yy.c y.tab.h utility.h
	cc -g -c lex.yy.c

lex.yy.c: tran.l
	lex tran.l

utility.o: utility.c utility.h
	cc -g -c utility.c

clean: 
	rm -f a.out utility.o main.o lex.yy.o y.tab.c y.tab.h y.tab.o y.output lex.yy.c