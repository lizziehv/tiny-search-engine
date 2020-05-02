# Makefile for CS50 crawler
#   Builds and compiles all these utility functions into a library.
#
# Lizzie Hernandez, April 30, 2020

# object files, and the target library
LDIR=../libcs50
CDIR=../common

PROG1= crawler
PROG = testing.sh
OBJS = crawler.o 
LIB = $(LDIR)/libcs50.a $(CDIR)/common.a

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DMEMTEST  # -DNOSLEEP


CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I $(LDIR) -I $(CDIR)
CC = gcc
MAKE = make

# Build the program
$(PROG1): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@


.PHONY: clean test

test: $(PROG)
	  bash $(PROG)

# clean up after our compilation
clean:
	rm -f core
	rm -f *~ *.o
	rm -f stocks
	rm -fr data*

