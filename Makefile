IDIR =include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = frontierset.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = frontierset.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

BUILDDIR=build

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -fPIC -c -o $@ $< $(CFLAGS)

libfrontierset.a: $(OBJ)
	ar rcs $(BUILDDIR)/libfrontierset.a $(OBJ)

.PHONY: clean all

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(BUILDDIR)/*.a

all: libfrontierset.a
