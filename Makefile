CC=gcc
CFLAGS=-I.

DEPS = frontierset.h
OBJ = frontierset.o

%.o: %.c $(DEPS)
	$(CC) -fPIC -c -o $@ $< $(CFLAGS)

libfrontierset.a: $(OBJ)
	ar rcs libfrontierset.a frontierset.o
