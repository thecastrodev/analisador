all:
	gcc ./src/lexer.c -o ./bin/lexer.out
	gcc ./src/parser.c -o ./bin/parser.out

lexer:./bin/lexer.out
	./bin/lexer.out $(ARGS)
	
parser:./bin/parser.out
	./bin/parser.out $(ARGS)

clean:
	rm ./bin/*