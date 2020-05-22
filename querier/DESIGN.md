---
written by: Lizzie Hernandez
layout: page
title: TSE Querier Design Spec
---

# CS50 Lab 4 Indexer Design
## Lizzie Hernandez, CS50 Spring 2020


Recall the [Querier Requirements Spec](REQUIREMENTS.md); the **querier** reads an index from an index-produced file to produce an inverted index structure and satisfy queries by ranking results found according to a score.

### User interface

The querier's only interface with the user is on the command-line; it must always have two arguments.

```
./querier pageDirectory indexFilename

```

### Inputs and outputs

Input: The program takes the command-line parameters indicated above and queries from standard input (one line per query).

Output: For every query (new line) from stdin the querier will output the results for pages from the pageDirectory provided and index provided ranked by score. For example:

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
```
The document pages are listed along with their scores, ids, and urls. The pages are ranked by a score determined by the Backus-Naur form, in which:
    - An individual word has a score equal to the number of times it appears in the document.
    - An "and sequence", which is a sequence of words optionally separated by and's has a score of the minimum number of times any of the words appear in the document.
    - A query, which is a sequence of "and sequences" separated by or's has a score per document equal to the sum of the scores for each the document in each of the "and sequences".


### Functional decomposition into modules

We anticipate the following modules or functions:

querier.c - main querier program
 1. *main*, which parses arguments and initializes other modules, initialize inverted index data structure
 2. *divideQuery(string)* separates a single string or query into an array of words
 3. *parseQuery(string)* which validates a query
 4. *solveAndSequence(sequence)* which determines the common documents and corresponding scores of the words in an "and sequence"
 5. *solveQuery(sequence)* which separates a query into "and sequences", resolves each, and combines results adding up scores
 6. *printResults(results)* which outputs the results of a query in the correct format
 7. *indexDelete* clean up data structures

iterating.c - functions for iterating through counters sets for each word
 1. *addAll(counter2, counter1 pair)* which merges one counter into another one
 2. *intersect(counter2, counter1 pair)* which merges one counter into another using smaller values
 3. *count(counter pair)* which counts the number of counters in a counters set
 4. *counterSort(array, counter pair)*  which inserts a counter into an array in decreasing order

Other important functions:
 1. *indexLoad*, which creates an inverted-index structure from an index file
 2. *isCrawlerDirectory*, which verifies whether the given directory is a Crawler-produced directory
 3. *NormalizeWord*, which converts all letters in a word to lowercase 
 4. *isValidFile* which verifies whether a file is a valid path for a file
 5. *getURL* which reads a page from a directory and gives the URL of the page

And some helper modules that provide data structures:

 1. *hashtable* filled with sets of counters to keep track of the words seen
 2. *set* maps from a word to a list of (docID, count) pairs
 3. *counter* keeps track of how many times a word was seen in a specific docID
 4. *tuple* stores a pair of integers (key, value)

### Pseudo code for logic/algorithmic flow
The Indexer will run as follows:
- Execute from a command line as shown in the User Interface
- Process and validate command-line parameters
- Initialize data structure inverted index from file
- While end of standard input has not been reached
    - Read query
    - Separate words in the query
    - Validate query and normalize words
    - Initialize a result set (initially empty)
    - For every and sequence in the query
        - Initialize a new set called res_and (initially empty)
        - For every word not "and" in the sequence
            - find the set of (docId, ocurrences) pairs of the word
            - If the set is not empty
                - Find the intersection between this set and res_and
                - Update res_and to be the intersection found before
            - If the set is empty
                - break loop and move to next and sequence
        - Merge res_and with the total result
    - Sort the results found
    - Print results in decreasing order and with specified format
- Clean up data structures

 
The querier completes and exits when it has nothing left to read from stdin and the hashtable is clean.

### Dataflow through modules
indexer.c
 1. *main*, parses parameters (uses ``isCrawlerDirectory``), initializes inverted index data structure. Keeps reading queries while there are more left to read.
 2. *divideQuery(string)* separates valid query string into an array of words
 3. *parseQuery(array)* goes through each of the word in the array to see if the format of the query is valid
 4. *solveQuery(array)* goes through each word to identify ""and sequences"
 5. *solveAndQuery(array)* goes thorugh and sequence to find intersection of the sets
 6. *solveQuery(array)* finds the union of all and sequences results
 7. *printResults(counters)* turns the set found into an array arranged in decreasing order and outputs the results


### Major data structures

4 helper modules provide data structures:

 1. *hashtable* of sets with words as keys and counters as items
 2. *counters* of document names. Count is how many times a word was seen in a specific document
 3. *sets* which will map from words to counters of (docID, count)
 4. *tuple* which stores a pair of integers

### Testing plan

*Unit testing*.  A small test program in `testing.sh` to test each module to make sure it does what it's supposed to do.

*Integration testing*.  Assemble the querier and test it as a whole using `testing.sh`.
With fuzzquery, examine the output files carefully to be sure they have the correct output. Also test with queries for which results are known and compare expected output with actual ouput using the diff command. 

### querier.c Testing
#### Incorrect Inputs
- Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.


#### Correct Inputs
- Test the index with a `indexFilename` that points to a file that already exists.
- Test the index with a `pageDirectory` 
- Use fuzzquery to satisfy random queries
- Test queries for which results are known and compare expected output with actual ouput
