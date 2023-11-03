CC = gcc
CFLAGS = -Wall -std=c11
LFLAGS = -lm -pthread

testLibrary: testLibrary.o
	$(CC) $(CFLAGS) testLibrary.c matrixLibrary.c -o test $(LFLAGS)

clean:
	rm testLibrary
