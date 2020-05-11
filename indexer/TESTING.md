# CS50 Lab 3
## Lizzie Hernandez

### Testing 'indexer'

To test my *indexer* module I wrote a test program, [testing.sh](testing.sh), to explore the capabilities of the *indexer* program and visualize output of running the program with valid or invalid parameters.

### Test file explanation
The `testing.sh` file will test the follwoing capabilities of the *indexer* program:

1. Validation of arguments (for both `indexer.c` and `indextest.c`): 
	- Tests incorrect number of parameters given
	- Tests any incorrect parameter, including nonexistent directory, invalid filename, noncrawler directory, nonexistent oldIndexFilename.

2. Writing index from a crawler produced directory (tests that should pass)
    - Will run `make test` for crawler, if it hadn't been ran before
    - Will create an index for output directories in crawler testing and store into RES/ directory.

3. Creating and saving inverted index structure from an index using `indextest.c`
    - Uses `indextest.c` to store a new index created from one fo the indices in the RES/ directory.
    - Uses indexsort.awk` to sort both the indexer produce files and the indextester files. 
    - Compares both files using the diff command 


### Test run
An example test run is in `testing.out`, showing output from `testing.sh`, built with

	make test &> testing.out

In that file one can see different results of running the crawler program to explore webpages.


### Limitations
- Hashtable is of fixed size. I used 900 slots, which might become less efficient as more webpages 
are explored.
- Only tests output directories from crawler `testing.sh`