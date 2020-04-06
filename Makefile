CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99 -I
DEPS = client.h parser.h
OBJ = crawler.o client.o parser.o
EXE = crawler

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	/bin/rm $(OBJ)

clobber: clean
	/bin/rm $(EXE)