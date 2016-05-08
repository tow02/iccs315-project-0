CC = gcc
CFLAGS = -g -Wall

all: findminmax_seq findminmax_par1

findminmax_par1: findminmax_par1.c
	$(CC) $(CFLAGS) -o findminmax_par1 findminmax_par1.c

findminmax_seq: findminmax_seq.c
	$(CC) $(CFLAGS) -o findminmax_seq findminmax_seq.c
