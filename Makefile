CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99 -I
IDIR = include
SDIR = src
DEPS = client.h parser.h
OBJ = crawler.o client.o parser.o
EXE = crawler

make: $(EXE)

%.o: $(SDIR)/%.c $(IDIR)/$(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	/bin/rm $(OBJ)

clobber: clean
	/bin/rm $(EXE)