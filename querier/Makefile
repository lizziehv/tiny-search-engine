# Makefile for crawler.c
#
# Lizzie Hernandez Videa - April 2020

LDIR=../libcs50
CDIR=../common

# For main program
PROG1= querier
PROG = testing.sh
OBJS = tuple.o iterating.o querier.o 
LIB = $(CDIR)/common.a $(LDIR)/libcs50.a 

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I $(LDIR) -I $(CDIR)
CC = gcc
MAKE = make

# For testing program
TEST_PROG = fuzzquery
TEST_OBJ = fuzzquery.o
TEST_LIB = $(LDIR)/libcs50.a
TEST_FLAGS = -Wall -pedantic -std=gnu11 -ggdb $(FLAGS) -I $(LDIR)

$(PROG1): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

$(TEST_PROG): $(TEST_OBJ)
	$(CC) $(TEST_FLAGS) $^ $(TEST_LIB) -o $@

# Dependencies object files depend on header files
iterating.o: iterating.h
tuple.o: tuple.h

.PHONY: all clean test

all:
	$(MAKE) $(PROG1)
	$(MAKE) $(TEST_PROG)

test: $(PROG)
	  bash $(PROG)

clean:
	rm -f core
	rm -f *~ *.o
	rm -f stocks
	rm -f $(PROG1)
	rm -f $(TEST_PROG)
	rm -rf test1
	rm -rf testinput