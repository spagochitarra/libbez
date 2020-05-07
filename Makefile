
CC = gcc
CFLAGS = -g -O0 -I.

EXE = runme

all: $(EXE)

runme: runme.o librun.o
	$(CC) -o $@ $^ 

librun.o: librun.c 
	$(CC) -c $^

clean:
	rm -rf $(EXE) *.o *.gch

