/**
 * pagedir.c
 * See pagedir.h under common for more information
 * 
 * Author: Lizzie Hernandez Videa
 * Date: April 30, 2020
 * Purpose: To implement a library with functions used in crawler, indexer, and querier
 */


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "pagedir.h"
#include "webpage.h"
#include "memory.h"

/* local functions (Not visible outside)*/
static int char_counter(int n);

/* ################ directory_exists() ############# */
/* ########### See pagedir.h for details ########### */
bool directory_exists(char *page_dir){
    /* Test if directory exists by trying to create a new file */
    char *filename =  assertp(count_malloc(strlen(page_dir)+sizeof(char)+strlen("crawler")+1), "Error: Could not create file."); 
    sprintf(filename, "%s/%s", page_dir, "crawler");
    FILE *test = fopen(filename, "w");

    if(test == NULL){
        /* clean up */
        count_free(filename);
        return false;
    }

    /* clean up */
    count_free(filename);
    fclose(test);
    return true;
}

/* #################### pagesaver() ################ */
/* ########### See pagedir.h for details ########### */
int pagesaver(char *page_dir, webpage_t* page, int current_id){
    /* Create file with filename */
    char *filename = assertp(count_malloc(strlen(page_dir)+sizeof(char)*(char_counter(current_id)+1)+1), "Error: Could not create file.");
    sprintf(filename, "%s/%d", page_dir, current_id);
    FILE *new = fopen(filename, "w");

    /* Check for errors */
    if (new == NULL){
        count_free(filename);
        fprintf(stderr, "Error creating file '%s'.\n", filename); 
        return 1;
    }

    /* Writing into file
     * First line is url
     * Second line is depth
     * Third line onwards is html
     */
    fprintf(new, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));

    /* Clean up */
    count_free(filename);
    fclose(new);
    
    return 0;
}

/* ################## char_counter() ############### */
/* ########### See pagedir.h for details ########### */
static int char_counter(int n) {
    int res = 0;   // current number of digit places seen
    int place = 1; // digits place to start looking from

    // until last place hasn't been reached
    while (place <= n) {
        res +=1;     // increase number of character
        place *= 10; // increase place (tenth? hundreth? thousand?)
    }

    return res; 
}
