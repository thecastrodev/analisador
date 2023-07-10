all:
	gcc ./src/lexer.c -o ./bin/lexer.elf
	gcc ./src/parser.c -o ./bin/parser.elf

lexer:./bin/lexer.elf
	./bin/lexer.elf $(ARGS)
	
parser:./bin/parser.elf
	./bin/parser.elf $(ARGS)

clean:
	rm ./bin/*