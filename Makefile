CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99
IDIR = include
SDIR = src
OBJ = crawler.o client.o parser.o
EXE = crawler

make: $(EXE)

_DEPS = client.h parser.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	/bin/rm $(OBJ)

clobber: clean
	/bin/rm $(EXE)