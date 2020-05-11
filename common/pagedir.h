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
#include "pagedir.h"
#include "webpage.h"
#include "memory.h"

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

/* @param Provide page_dir name
 * Return true if directory is a crawler produced directory, false otherwise.
 */
bool is_crawler_directory(char *page_dir);

/* Provide path to a file 
 * Returns true if the path to file is valid
 * Returns false otherwise
 * (File must not necessarily exist.)
 */
bool is_valid_file(char* filename);

/* @param page_dir - Directory name where crawler files are located
 * @param current_id - ID of a page in the directory
 * 
 * Converts page with current_id in page_dir into a webpage
 * 
 * @return NULL if page_dir does not contain a file with name
 *         current_id, or if the page's format is not a valid
 *         crawler produced file:
 *         <url>
 *         <depth>
 *         <html>
 * @return a pointer to a webpage created from crawler produced file
 */
webpage_t* file_to_webpage(char* page_dir, int current_id);

#endif