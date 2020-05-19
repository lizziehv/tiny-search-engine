# Makefile for crawler.c
#
# Lizzie Hernandez Videa - April 2020

LDIR=../libcs50
CDIR=../common

PROG1= querier
PROG = testing.sh
OBJS = querier.o iterating.o
LIB = $(CDIR)/common.a $(LDIR)/libcs50.a 

FLAGS =
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I $(LDIR) -I $(CDIR)
CC = gcc
MAKE = make

$(PROG1): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

# Dependencies object files depend on header files
iterating.o: iterating.h

.PHONY:  clean test

test: $(PROG)
	  bash $(PROG)

clean:
	rm -f core
	rm -f *~ *.o
	rm -f stocks