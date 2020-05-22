# CS50 Lab 4 Indexer Implementation
## Lizzie Hernandez, CS50 Spring 2020

### Data structures used

* Hashtable: A hashtable is an array of non-null (key, item) pairs, with unique keys, implemented using a set of (key, item) pairs. The indexer uses a hashtable to map each word found in a crawler produced directory to a set of counters with (documentID, count) pairs, where each documentID is a unique file name in the directory provided and count is the number of times the word was seen in the page.

* Counters: A counters set if a set of (key, count pairs), both integers. By inserting a key into the counters set, the count is initialized to 1, and every time the key is inserted again, its count increases by 1. The indexer maps every word sen in the directory to a counters set, where the keys are given by the document id of the files in the directory given, to keep track of the number of times the word is seen oin each file.

### Pseudocode for each component

The indexer runs as follows:

- Execute from a command line and validate parameters
- Initialize a hashtable data structure used for the inverted indexer
- While there are still files to visit in the directory provided
    - For every word in the file with length >= 3 and with alphabetic characters only
        - If the word is not in the hashtable yet...
            - Insert the word with a counter for this file id starting at 1
        - If the word was already in the hashtable...
            - Increment its count foe this file id
- Call indexSaver

The indextester runs as follows:

- Execute from command line and validate arguments
- Create an inverted-index structure from file name provided
    - For every line in the index
        - read the word and its counters by reading (%d $d) pairs
        - insert the word and its counters into the hashtable
- Use index save to save the index into file name provided

The indexsaver runs as follows:

- Use an inverted-indes structure
- For every item in the hashtable
     - Write the word and its counters into a new line of the file

### Definition of functions and parameters used 

```c
/* @param page_dir- Crawler produced directory to create index from
 * @param htable- Hashtable structure used to build the index
 * 
 * Modifies the given hashtable to include words in page_dir matched
 * with counters with document ids as keys and word count as values
 * 
 * @return 1 if error
 * @return 0 else
 */
int index_build(char *page_dir, hashtable_t *htable);
```

```c
/* @param filename- File to save index into
 * @param htable- Hashtable structure where index was built
 * 
 * Writes index into a file
 * one line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format
 * word docID count [docID count] ...
 * 
 * @return 1 if error opening file
 * @return 0 if success
 */
int index_save(char *filename, hashtable_t *htable);
```

See ../common/README.md for definition of functions in *pagedir.c* and *words.c*.

### Memory allocation in C and modules used

1. The cleanup is done by deleting the hashtable and the counters inside it.

### Error handling

* The indexer will return a non zero exit status if the index file name given is not valid before intializing data structures.

* The indexer will assume that if the directory given contains a crawler file, then it is a crawler produced directory.

* It is assumed that the files inside the crawler produced directory are named 1, 2, 3, ..., without gaps. If there is a gap, the indexer will stop executing and only words found before the gap will be saved into the index.

* The indexer will return a non zero exit status upon any error initializing data structures.

* The indexer will ignore words that can't be normalized (ie. include non-alphabetic characters).

* The indexer will stop executing if it comes across a file in the directory that does noot have the correct format: url, depth, html.


### Resource management

Libraries used are `common.a` and `libcs50.a`, found in common/ and libcs50/, respectively. Each can be compiled by running `make` from the directory root.


### Files required

The directory provided must be a crawler produced directory and must follow these conventions:
* File names are consecutive numbers starting from 1.
* Each file must have the following format : url + \n + depth + \n + html