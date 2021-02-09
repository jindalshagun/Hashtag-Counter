CC=g++
CFLAGS=-I.

hashtagcounter: fibHeap.o hashtagcounter.o
	$(CC) -o hashtagcounter fibHeap.o hashtagcounter.o $(CFLAGS)

