CC = gcc
CFLAGS = -O3 -Wall

all: keccak_test keccaksum

keccak_test: keccak.o test.o
	$(CC) $(CFLAGS) -o keccak_test keccak.o test.o

keccaksum: keccak.o keccaksum.o
	$(CC) $(CFLAGS) -o keccaksum keccak.o keccaksum.o

keccak.o: keccak.c keccak.h
	$(CC) $(CFLAGS) -c -o keccak.o keccak.c

test.o: test.c
	$(CC) $(CFLAGS) -c -o test.o test.c

keccaksum.o: keccaksum.c
	$(CC) $(CFLAGS) -c -o keccaksum.o keccaksum.c

.PHONY: clean

clean:
	rm -rf *.o *.gch keccak_test keccak_sum
