/**
 * pagedir.h
 * See pagedir.c for implementation
 * 
 * Author: Lizzie Hernandez Videa
 * Date: April 30, 2020
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"

/* @param dirname - naem of a directory
 *
 * Write into directory to determine if it exists
 * 
 * @return true if directory exists and is writeable
 * @return false otherwise
 */
bool directory_exists(char *page_dir);

/* @param page_dir - Directory to write file on
 * @param page - webpage to write onto file
 * @param current_id - id of page being written (must be unique)
 * 
 * Saves page to pagedir directory under file with name current_id
 * by writing its url, depth, and html
 * 
 * @return 0 if success
 * @return 1 on error
 */
int pagesaver(char *page_dir, webpage_t *page, int current_id);

#endif