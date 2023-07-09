all:
	gcc ./lex.c -o ./bin/lex
	gcc ./parser.c -o ./bin/parser

lex:./bin/lex
	./bin/lex $(ARGS)
	
parser:./bin/parser
	./bin/parser $(ARGS)

clean:
	rm ./bin/*