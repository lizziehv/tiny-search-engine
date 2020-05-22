# CS50 Lab 4 Querier Implementation
## Lizzie Hernandez, CS50 Winter 2020

### Data structures used

* Hashtable: A hashtable is an array of non-null (key, item) pairs, with unique keys, implemented using a set of (key, item) pairs. The querier uses an inverted index structure which is a hashtable mapping each word found in a crawler produced directory to a set of counters with (documentID, count) pairs, where each documentID is a unique file name in the directory provided and count is the number of times the word was seen in the page.

* Counters: A counters set if a set of (key, count pairs), both integers. By inserting a key into the counters set, the count is initialized to 1, and every time the key is inserted again, its count increases by 1. The indexer maps every word sen in the directory to a counters set, where the keys are given by the document id of the files in the directory given, to keep track of the number of times the word is seen in each file.

* Tuple: A tuple stores a pair of integers (key, score) and it supports the functions of retrieving key and score. Implementation for a tuple is found in [tuple.c](tuple.c). The querier uses a tuple once the query results are stored in a counters set to insert each (docID, score) pair in an array using insertion sort.

### Resource management

Libraries used are `common.a` and `libcs50.a`, found in common/ and libcs50/, respectively. Each can be compiled by running `make` from the directory root.

### Dependencies
The querier includes the following dependencies from libcs50 library: `hashtable.h`,`counters.h`, `file.h`. It also uses `pagedir.h`, `index.h`, `words.h`from the common library. See README.md from common directory for information about supported functions in common.a. Additionally, it depends on two files from querier directory explained below:

* `iterating.h` - header file for `iterating.c` which includes all item function used for iterating through counters, and supports the following functions:
```c
/* To move all counters from which iterate is called to an empty counters set */
void counters_add_all(void *arg, const int key, const int count);
/* To merge two counters set on the counters called */
void counters_merge(void *arg, const int key, const int count);
/* To merge with lesser counts */
void counters_intersect(void *arg, const int key, const int count);
/* To count the number of (key, count) pairs */
void counters_count(void *arg, const int key, const int count);
/* To insert each (key, count) tuple into an array using insertion sort */
void counters_to_array(void *arg, const int key, const int count);
```
Please check the file for detailed information about usage.

* `tuple.h` - header file for tuple structure which stores a pair of integers and supports:
```c
/* To create a new tuple structure */
tuple_t *tuple_new(int key, int score);
/* To retrieve second integer */
int tuple_get_score(tuple_t* pair);
/* To retrieve first integer */
int tuple_get_key(tuple_t* pair);
/* To increase tuple's score by 1 */
bool tuple_update_score(tuple_t* pair);
```

### Pseudocode for each component

The querier runs as follows:

- Execute from a command line and validate parameters
- Create an inverted-index structure (hashtable) from index file name provided
- Prompt a query
    - While there are more queries to read
        - Divide query (See below)
        - Validate query using Backus-Naur form, normalize words (stopping on any error)
        - Initialize a counters set to store results
        - Satisfy query (See below)
        - Print query results (See below)
        - Clean up memory space allocated
- Clean up hashtable memory

Divide query runs as follows:

- For every character in the query
    - Go to next alpahbetic character
    - Insert that character pointer to array of words
    - Skip all alphabetic characters and go to the first space seen
    - Make the space a null character

Satisfy query runs as follows: 

- For every and sequence in the query
    - Initialize a counters structure to hold the intersection of the counters set in this sequence -> res_and
    - For every word in the and sequence
        - find the counters set in the index corresponding to the word
        - if the word's counters set exist
            - Find the intersection of the current res_and the words's counters set by ...
            - Iterating through the current res_and and adding every key, item pair to the word's counters with lesser values (0 if not contained in counters)
            - Iterate through the counter set and adding every key, item pair to res_and with lesser values (0 if not contained in res_and)
            - Now that res_and contains all the keys in both counters so that the value of a key is 0 if it is not in either of them, move all keys with a non-zero value to a new intersection and update res_and to point to this new intersection.
        - else, if the counters set does not exist
            - Since the intersection of the counters set in this sequence will empty, move to the next sequence and break from this
    - Clean up structure used (res_and)
    - Find union between current result and res_and and update result to be this nrw union

Print query runs as follows:

- Find the size of the result counters set by iterating through counters
- If size is 0, report documents found
- Else
    - Initialize an array of tuples of size
    - For every key, count pair in the result counter set
        - create a tuple holding (key, count) --> "current"
        - insert current at the end of the array
        - For every tuple already in the array --> "other"
            - If other has lower score
                - move lower to right and current to the left
    - For every tuple in the array
        - Find the url by accessing the document from the directory
        - Print score, document ID, and url



### Definition of functions and parameters used 

```c
/* @param char *line
 * 
 * Count the number of words in a line for size of word array
 * 
 * @return number of words in line
 */
int word_counter(char *line);

/* @param char *line - pointer to line to be divided
 * @param char *words[] - array of strings (empty)
 * @param int count - number of words in string array
 * 
 * Function to divide query line into an arrray of words
 * 
 * @return void
 */ 
void divide_query(char *line, char *words[], int count);

/* @param char *words[] - array of strings (empty)
 * @param int count - number of words in string array
 * 
 * Normalize each word in words
 * Output query
 * Check if the format is valid
 * <and sequence> or <and sequence> etc.
 * 
 * @return false if a word contains a non alphabetic character or
 *         format is invalid: consecutive ANDs, ORs, or query starts
 *         with AND, OR
 * @return true if success (valid format ans)
 */ 
bool parse_query(char *words[], int count);

/* @param result - empty counters set to insert query results
 * @param index - inverted-index structure
 * @param words - array of strings containing words
 * @param count - number of words
 * 
 * Find query results by merging and_sequences results
 * 
 * @return false if any error initializing structures
 * @return true if success
 */ 
bool query(counters_t* result, hashtable_t *index, char *words[], int count);

/* Helper function for query()
 * 
 * @param result - empty counters set to insert query results
 * @param index - inverted-index structure
 * @param words - array of strings containing words
 * @param count - number of words
 * @param current - index in words where current index was found
 * 
 * Function will find the intersection of all words in a sequence
 * Update current index to point to the beginning of the next sequence
 * 
 * @return pointer to counters containing intersection
 *         NULL for any error
 */ 
static counters_t * resolve_sequence(hashtable_t *index, char *words[], int count, int* current);

/* @param result - counters with results from query
 * @param page_dir - crawler produced directory where pages come from 
 * 
 * Print results to stdout. Pages ranked with format
 * score <score> doc <doc>: <url>
 * 
 */ 
void print_results(counters_t* result, char* page_dir);
```

See ../common/README.md for definition of functions in *pagedir.c* and *words.c*.

### Memory allocation in C and modules used

1. The cleanup is done by deleting the hashtable index and the counters inside it.

2. Note that there is no tuple_delete function because free(tuple) will work instead. 

3. A new counter set is initialized and deleted for every query.

### Error handling

* The querier will return a non zero exit status on any usage error or on any error allocating memory for structures.

* The querier will assume that if the directory given contains a crawler file, then it is a crawler produced directory, and that the index provided is a valid indexer produced index.

* The querier will ignore a query containing non-white-space or non-alphabetic characters


### Files required

The directory provided must be a crawler produced directory and must follow these conventions:
* File names are consecutive numbers starting from 1.
* Each file must have the following format : url + \n + depth + \n + html
The index provided must be an indexer produced index and follow the following conventions:
* One word per line with space-separated docID, occurrences 