Lexer: main.o lexer.o lexer.h
	gcc -Wall -g $< lexer.o -o $@
main.o: main.c lexer.h
	gcc -Wall -c $<
lexer.o: lexer.c lexer.h
	gcc -Wall -c $<