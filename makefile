CC = gcc
CFLAGS = -g -Wall -std=c11

testLibrary: testLibrary.o
	$(CC) $(CFLAGS) testLibrary.c matrixLibrary.c -o testLibrary

clean:
	rm testLibrary