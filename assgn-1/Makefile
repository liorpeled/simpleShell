
target:
	flex lex.l
	cc -c myshell.c
	cc -c lex.yy.c
	cc -o shell myshell.o lex.yy.o -lfl
clean:
	rm shell myshell.o lex.yy.o