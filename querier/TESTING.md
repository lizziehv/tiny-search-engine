# CS50 Lab 3
## Lizzie Hernandez

### Testing 'querier'

To test my *querier* module I wrote a test program, [testing.sh](testing.sh), to explore the capabilities of the *querier* program and visualize output of running the program with valid or invalid parameters, and give different queries.

### Test run
An example test run is in `testing.out`, showing output from `testing.sh`, built with

	make test &> testing.out

In that file one can see different results of running the crawler program to explore webpages according to the testing methods explained below.
<br />
* Please note that testing requires compiling both the *crawler* and *indexer* programs. Run make from their corresponding directories to compile the programs.

### Test file explanation
The `testing.sh` file will test the following capabilities of the *querier* program:

1. Validation of arguments: 
	- Tests incorrect number of parameters given
	- Tests any incorrect parameter, including nonexistent directory, invalid filename, and non-crawler directory

2. (On tests that should pass) Testing on example queries for which results are known
    - Uses [testdir](testdir) to test:
        - Output for bad characters in query, using queries in [badchars](testdir/badchars) from this directory
        - Output for known queries from [examplein](testdir/examplein), by creating a file *exampleout* and comparing the results from this ouput file with those in [example](testdir/example).
            - Note that when the files are compared, some differences might be found because the rankings will make no distinction between documents with the same score, so these might appear in different positions on different runs.

3. (On tests that should pass) Testing on random queries for which results are unknown
    - Uses [fuzzquery.c](fuzzquery.c) to produce a directory of random queries and test querier on these


### Limitations

- Since the rankings make no distinction between documents with the same score, testing differences on queries for which results are known might be ineffective.

- Tested on wiki page with depth 1 only.

