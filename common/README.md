# CS50 Tiny Search Engine (TSE) common library

## Usage

To build `common.a`, run `make`. 
To clean up, run `make clean`.

## Overview

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



