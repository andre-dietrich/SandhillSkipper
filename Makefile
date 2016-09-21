CC	   = gcc
CXX    = g++
RM	   = rm
ECHO	 = echo

DEFINES= -DS2_DEBUG -DS2_LIST -DS2_SET

CFLAGS = -Wall -g -I./dynamic $(DEFINES) #-Os
OBJLIB = lib2s.so

SRC  = $(wildcard vm*.c)
OBJ  = $(patsubst %.c,%.o,$(SRC))

DYN_SRC = $(wildcard ./dynamic/*.c)
DYN_OBJ = $(patsubst %.c,%.o,$(DYN_SRC))

EXE	 = main.out
MAIN = main.c

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
		cd dynamic; make clean
		$(RM) -f *.out *.o *.so

doc:
		doxygen

test: lib
		cd test; make
