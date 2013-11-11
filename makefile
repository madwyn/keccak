CC = gcc
CFLAGS = -O3 -Wall
TARGET = keccak_test

all: $(TARGET)

$(TARGET): keccak.o main.o
	$(CC) $(CFLAGS) -o $(TARGET) keccak.o main.o

keccak.o: keccak.c keccak.h
	$(CC) $(CFLAGS) -c -o keccak.o keccak.c

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

.PHONY: clean

clean:
	rm -rf *.o *.gch $(TARGET)
