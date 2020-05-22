# CS50 Lab 3
## Lizzie Hernandez Videa

## Querier

Querier is the final module for the tiny search engine, which uses a crawler-produced directory and an indexer-produced index file to satisfy a query by ranking results according to a score.


### Compilation

Before compiling, make sure `libcs50/libcs50.a` and `common/common.a` exist. These can be compiled by running `make` from the root of their corresponding directories.
<br />
Run `make all` from `indexer` directory to compile both *fuzzquery.c* and *querier.c*, or  `make programName` to compile only one of these programs, or `make` to compile only *querier.c*. If you're planning to run the testing script, please note that compiling *fuzzquery.c* is required.
<br />
For testing, `make test` to see a sample run. Please note that testing requires compiling both the *crawler* and *indexer* programs. Run make from their corresponding directories to compile each of these programs.
<br />
Run `make clean` after to delete all compiled and reproducable files.
<br />

### Usage
To run, execute from a command line with the following syntax:
```bash
$ ./querier pageDirectory indexFilename
```

#### Arguments
- pageDirectory: path of a crawler-produced directory
- indexFilename: path of an indexer-produced index

#### Input 
The querier will read queries from standard input (one per line) until end of file is reached.

#### Output 
For each query read from stdin, the query will output results the following format:
```bash
Query: <query>
Matches 7 documents (ranked):
score <score> doc <id> : <url>
_________________________________________
```

For example:
```
Query? dartmouth or computer science or programming or unix                
Query: dartmouth or computer science or programming or unix 
Matches 7 documents (ranked):
score 422 doc   4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
score 222 doc   5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Unix.html
score 195 doc   7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score  93 doc   6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
score  13 doc   2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Linked_list.html
score  10 doc   3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
score   4 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
-----------------------------------------------
Query? Backus-Naur Form
Error: bad character '-' in query.
```

### Exit Status:
#### Indexer
0: Success, no errors <br/>
1: Usage errors (wrong number of parameters or invalid parameters)<br/>
2: Error allocating memory and creating modules necessary (hashtable, counters)<br/>


### Implementation

For more imformation, see [IMPLEMENTATION](IMPLEMENTATION.md)

### Design

For more imformation, see [DESIGN](DESIGN.md)

### Testing
To test, simply `make test`. The files and directories required are [testing.sh](testing.sh), [example](testdir/example), [examplein](testdir/examplein), [badchars](testdir/badchars). The programs required are (compiled files of) [fuzzquery.c](fuzzquery.c),[crawler](../crawler/crawler.c), and [indexer](../indexer/indexer.c).
For more imformation, see [TESTING](TESTING.md)


### Assumptions
- It is assumed that if `pageDirectory` contains a *.crawler* file, then it is a crawler-produced directory and follows appropriate conventions.

- It is assumed that if `indexFileName` is readable, then it is an index-produced file and follows appropriate conventions.

