CC=gcc

CFLAGS = -Wall -O2

all: huffmanCoder

huffmanCoder: huffmanCoder.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f huffmanCoder