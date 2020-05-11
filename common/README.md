# CS50 Tiny Search Engine (TSE) common library

## Usage

To build `common.a`, run `make`. 
To clean up, run `make clean`.

## Overview

 * `index` - the **index** module which includes methods shared by indexer, and querier. Includes the following:
 ```c
/* Modifies the given hashtable to include words in page_dir matched
 * with counters with document ids as keys and word count as values
 */
int index_build(char *page_dir, hashtable_t *htable);
 ```
 ```c
/*
 * Writes index into a file
 * one line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format
 * word docID count [docID count] ...
 */
int index_save(char *filename, hashtable_t *htable);
```
```c
/* 
 * Reads an index and inserts its contents (word, counters) items
 * into hashtable provided
 */
int file_to_index(FILE* f_in, hashtable_t *htable);
```

 * `pagedir` - the **pagedir** module which includes methods shared by crawler, indexer, and querier. Includes the following:


 ```c
 /* receives a page_dir (directory name) and checks 
  * if it exists by writing a file into it
  */
 bool directory_exists(char *page_dir);
 ```

  ```c
 /* receives a valid directory, a webpage, and a unique id
  * writes webpage into file named acoording to current_id
  * inside directory by including url, depth, and html
  */
int pagesaver(char *page_dir, webpage_t *page, int current_id);
 ```
```c
/*
 * Return true if directory is a crawler produced directory, false otherwise.
 */
 bool is_crawler_directory(char *page_dir);
```
```c
/*
 * Converts page with current_id in page_dir into a webpage
 */
webpage_t* file_to_webpage(char* page_dir, int current_id);
```
```c
/* 
 * check if fliname is a valid path
 */
bool is_valid_file(char* filename);
```
 * `words` - the **word** module which includes methods used by indexer. Includes the following:

```c
/* Converts every chracter in a string into lower case
 */
bool normalize_word(char* string)
```

## Assumptions

* `index_build` receives a crawler produced directory
* `index save` will overwrite the file given if it already exists