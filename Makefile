CC	   = gcc
CXX    = g++
RM	   = rm
ECHO	 = echo

DEFINES= -DS2_LIST -DS2_SET -DS2_DEBUG

CFLAGS = -Wall -g -I./src -I./src/dynamiC/src $(DEFINES) #-Os
OBJLIB = libS2.so

SRC  = $(wildcard src/vm*.c)
OBJ  = $(patsubst %.c,%.o,$(SRC))

DYN_SRC = $(wildcard src/dynamiC/*.c)
DYN_OBJ = $(patsubst %.c,%.o,$(DYN_SRC))

EXE	 = src/main.out
MAIN = src/main.c

.PHONY: all test clean

all:
		$(CC) $(CFLAGS) $(DYN_SRC) $(SRC) $(MAIN) -o $(EXE)

lib: $(OBJ) $(DYN_OBJ)
		$(CC) $(CFLAGS) -shared $(DYN_OBJ) $(OBJ) -o $(OBJLIB)

%.o: %.c
		$(CC) $(CFLAGS) -c -fpic -o $@ $<

valgrind:
		valgrind --leak-check=full --track-origins=yes -v ./$(EXE)

run: all
		./$(EXE)

clean:
		$(RM) -f *.out *.o *.so
		cd src; $(RM) -f *.out *.o *.so

doc:
		doxygen

test: lib
		cd test; make
