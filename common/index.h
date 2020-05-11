/**
 * index.h
 * 
 * Author: Lizzie Hernandez Videa
 * Date: May 10, 2020
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "memory.h"
#include "hashtable.h"
#include "pagedir.h"

/* @param *page_dir- Crawler produced directory to create index from
 * @param *htable- Hashtable structure used to build the index
 * 
 * Modifies the given hashtable to include words in page_dir matched
 * with counters with document ids as keys and word count as values
 * 
 * @return 1 if error
 * @return 0 else
 */
int index_build(char *page_dir, hashtable_t *htable);

/* @param *filename- File to save index into
 * @param *htable- Hashtable structure where index was built
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

/* @param *f_in - File to read index from
 * @param *htable - Hashtable to insert inverted index into
 * 
 * Reads an index and inserts its contents (word, counters) items
 * into hashtable provided
 * 
 * @return 1 on any formatting error
 * @return 0 if success
 */
int file_to_index(FILE* f_in, hashtable_t *htable);

/* @param *item - a pointer to a counter structure
 * 
 * Wrapper for counters delete to delete items in
 * inverted-index structure
 * 
 * @return nothing
 */
void index_item_delete(void *item);

#endif