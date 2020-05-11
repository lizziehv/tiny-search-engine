# CS50 Lab 3
## Lizzie Hernandez Videa

## Index

An index is a module that builds an inverted-index data structure mapping from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given webpage.

### Usage
Both `indexer.c` and `indextest.c`, simply contain a main function which validates command line arguments. The main functions used are found in `common.a` and include `fileToWebpage`, `fileToIndex`, `indexSave`, and ``indexBuild`.

### Compilation

Before compiling, make sure `libcs50/libcs50.a` and `common/common.a` exist. These can be compiled by running `make` from the root of their corresponding directories.
Run `make all` from `indexer` directory to compile both indexer and indextest, or  `make programName` to compile only one of them, and `make test` to see a sample run. Run `make clean` after running the test to delete unwanted files.

### Indexer
To run, execute from a command line with the following syntax:
```bash
$ ./indexer pageDirectory indexFilename
```
#### Input 
The arguments given to this command line are the following:
- pageDirectory: path of a crawler-produced directory
- indexFilename: path of a file into which index is written. (The indexer creates the file (if needed) and overwrites the file (if it already exists.)

#### Output 
`indexer.c` will read every file in a crawler-produced directory and map every unique, normaizable word with three or more characters to a set of counters keeping track of how many times the word has been seen in each file. The index is saved to the file name provided in the format:
        - one line per word, one word per line
        - each line provides the word and one or more (docID, count) pairs, in the format "word docID count [docID count]…"
        - where word is a string of lower-case letters,
        - where docID is a positive non-zero integer,
        - where count is a positive non-zero integer,
        - where the word and integers are separated by spaces.


### Indextest
To run, execute from a command line with the following syntax:
```bash
$ ./indextest oldIndexFilename newIndexFilename
```

#### Input 
The arguments given to this command line are the following:
- oldIndexFilename: path of a file containing an index, whose format is assumed to be that of an index with
"word docId count [docId count] ..."
items on every new line.

- newIndexFilename: name of a file into which the index should be written(The indexer creates the file (if needed) and overwrites the file (if it already exists.)

#### Output 
`indextest.c` will read the old file (which is a saved index) and recreate an index from that file. The new file is written into newIndexFilename. Once complete, the files can be compared by executing
```bash
$ diff oldIndexFilename newIndexFilename
```
All errors are logged into standard error.

### Exit Status:
#### Indexer
0: Success, no errors <br/>
1: Wrong arguments/parameters given <br/>
2: Error creating modules necessary (hashtable) <br/>
3: Error building or saving index  <br/>

#### Indextest
0: Success, no errors <br/>
1: Wrong arguments/parameters given <br/>
2: Error creating modules necessary (hashtable) <br/>
3: Error loading or saving index  <br/>

### Implementation

For more imformation, see [IMPLEMENTATION](IMPLEMENTATION.md)


### Assumptions
`indexer.c` assumes that:
- pageDirectory has files named 1, 2, 3, …, without gaps.
- The content of files in pageDirectory follow the format as defined in the specs

`indextest.c` assumes that the content of the given file follows the format specified above.

### Testing

To test, simply `make test`. The file `testing.sh` is require.
See [TESTING](TESTING.md) for details of testing and an example test run.