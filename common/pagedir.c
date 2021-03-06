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
#include "file.h"

/* local functions (Not visible outside)*/


/* ################ directory_exists() ############# */
/* ########### See pagedir.h for details ########### */
bool directory_exists(char *page_dir){
    /* Test if directory exists by trying to create a new file */
    char *filename =  assertp(count_malloc(strlen(page_dir)+sizeof(char)+strlen(".crawler")+1), "Error: Could not create file."); 
    sprintf(filename, "%s/%s", page_dir, ".crawler");
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
    char *filename = assertp(count_malloc(strlen(page_dir)+sizeof(char)*(char_counter(current_id)+1)+1), "Error: Could not create file.\n");
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

/* ############## is_crawler_directory() ########### */
/* ########### See pagedir.h for details ########### */
bool is_crawler_directory(char *page_dir){
    /* Create file with filename */
    char *filename =  assertp(count_malloc(strlen(page_dir)+sizeof(char)+strlen(".crawler")+1), "Error: Could not create file.\n"); 
    sprintf(filename, "%s/%s", page_dir, ".crawler");
    FILE *test = fopen(filename, "r");

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

/* ################# is_valid_file() ############### */
/* ########### See pagedir.h for details ########### */
bool is_valid_file(char* filename){
    FILE *f_in = fopen(filename, "w");

    if(f_in == NULL)
        return false;

    fclose(f_in);
    return true;
}

/* ################ file_to_webpage() ############## */
/* ########### See pagedir.h for details ########### */
webpage_t* file_to_webpage(char* page_dir, int current_id){
    /* Open file by allocating memory for name */
    char *filename = assertp(count_malloc(strlen(page_dir)+sizeof(char)*(char_counter(current_id)+1)+1), "Error: Could not create file.\n");
    sprintf(filename, "%s/%d", page_dir, current_id);
    FILE *current_file = fopen(filename, "r");

    /* return on error */
    if(current_file == NULL){
        count_free(filename);
        return NULL;
    }

    /* else create webpage from file */
    char* url = freadlinep(current_file);           // read url
    if(url == NULL){                                // return on error
        count_free(filename);
        fclose(current_file);
        return NULL;
    }

    int depth;                                      // read depth
    char* second_line = freadlinep(current_file);
    if(second_line == NULL || 
        sscanf(second_line, "%d", &depth)==0){      // return on error
        if(second_line != NULL){
            free(second_line);
        }
        /* clean up */
        free(url);
        count_free(filename);
        fclose(current_file);    
        return NULL;
    }
    free(second_line);

    char* html = freadfilep(current_file);         // read html
    if(html == NULL){      // return on error
        /* clean up */
        free(url);
        count_free(filename);
        fclose(current_file); 
        return NULL; 
    }

    /* Create webpage */
    webpage_t *page = webpage_new(url, depth, html);
    
    /* clean up */
    count_free(filename);
    fclose(current_file);
    return page;
}

/* ##################### get_url() ################## */
/* ########### See pagedir.h for details ########### */
char * get_url(char *page_dir, int current_id){
    /* Open file by allocating memory for name */
    char *filename = assertp(count_malloc(strlen(page_dir)+sizeof(char)*(char_counter(current_id)+1)+1), "Error: Could not create file.\n");
    sprintf(filename, "%s/%d", page_dir, current_id);
    FILE *current_file = fopen(filename, "r");

    if(current_file == NULL){
        fprintf(stderr, "Error: file not found in directory.\n");
        count_free(filename);
        return NULL;
    }
    char *url = freadlinep(current_file);
    count_free(filename);
    fclose(current_file);
    return url;
}

/* ################## char_counter() ############### */
/* ########### See pagedir.h for details ########### */
int char_counter(int n) {
    int res = 0;   // current number of digit places seen
    int place = 1; // digits place to start looking from

    // until last place hasn't been reached
    while (place <= n) {
        res +=1;     // increase number of character
        place *= 10; // increase place (tenth? hundreth? thousand?)
    }

    return res; 
}
