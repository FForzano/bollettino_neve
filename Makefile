IDIR =./include
CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic

ODIR=./bin
LDIR =/usr/lib/x86_64-linux-gnu/libunistring.a
SDIR=./src

_DEPS = csexp.h main_func.h utils.h extraio.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ1 = bollettino_neve.o utils.o csexp.o extraio.o
OBJ1 = $(patsubst %,$(ODIR)/%,$(_OBJ1))

_OBJ2 = bollettino_neve_server.o utils.o main_func.o csexp.o extraio.o
OBJ2 = $(patsubst %,$(ODIR)/%,$(_OBJ2))

TARGET = ./bollettino_neve ./bollettino_neve_server


$(ODIR)/%.o: ${SDIR}/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: ${TARGET}

./bollettino_neve_server: $(OBJ2)
	$(CC) -o $@ $^ ${LDIR} $(CFLAGS)

./bollettino_neve: $(OBJ1)
	$(CC) -o $@ $^ ${LDIR} $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ 
