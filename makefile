

all: ./bin/shell


./bin/shell: ./bin/shell.o ./bin/List.o ./bin/cmdLog.o ./bin/LineParser.o ./bin/pipeTools.o
	gcc -m32 -g -Wall -o ./bin/shell ./bin/LineParser.o ./bin/cmdLog.o ./bin/List.o ./bin/pipeTools.o ./bin/shell.o

 
./bin/LineParser.o: LineParser.c LineParser.h
	gcc -g -Wall -m32 -ansi -c -o ./bin/LineParser.o LineParser.c 

./bin/List.o: List.c List.h
	gcc -g -Wall -m32 -ansi -c -o ./bin/List.o List.c

./bin/cmdLog.o: cmdLog.c cmdLog.h
	gcc -g -Wall -m32 -ansi -c -o ./bin/cmdLog.o cmdLog.c

./bin/pipeTools.o: pipeTools.c pipeTools.h
	gcc -g -Wall -m32 -ansi -c -o ./bin/pipeTools.o pipeTools.c

./bin/shell.o: shell.c
	gcc -g -Wall -m32 -ansi -c -o ./bin/shell.o shell.c

.PHONY: clean


clean: 
	rm -f ./bin/*.o ./bin/shell
