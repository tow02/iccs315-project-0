CC = gcc
CFLAGS = -g -Wall -std=c99 -D_BSD_SOURCE

all: clean findminmax_seq findminmax_par1 findminmax_par2

findminmax_par1: findminmax_par1.c
	$(CC) $(CFLAGS) -o findminmax_par1 findminmax_par1.c

findminmax_par2: findminmax_par2.c
	$(CC) $(CFLAGS) -o findminmax_par2 findminmax_par2.c

findminmax_seq: findminmax_seq.c
	$(CC) $(CFLAGS) -o findminmax_seq findminmax_seq.c

clean:
	rm -f *~
	rm -f findminmax_seq
	rm -f findminmax_par1
	rm -f findminmax_par2
