CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99
OBJ = crawler.o client.o parser.o
EXE = crawler

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(CFLAGS)

object/crawler.o: src/crawler.c
	$(CC) -c $@ $< src/crawler.c $(CFLAGS)

object/client.o: src/client.c include/client.h
	$(CC) -c -o $@ $< $(CFLAGS)

object/parser.o: src/parser.c include/parser.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	/bin/rm $(OBJ)

clobber: clean
	/bin/rm $(EXE)