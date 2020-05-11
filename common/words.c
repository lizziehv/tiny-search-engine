/**
 * words.c
 * See words.h for information about function usage
 * 
 * Author: Lizzie Hernandez Videa
 * Date: May 10, 2020
 */ 

#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* ################ normalize_word() ############# */
/* ########### See words.h for details ########### */
bool normalize_word(char* string){
    /* loop through characters in string */
    for(int i = 0; i<strlen(string); i++){
        char c  = string[i];
        if(isalpha(c)){
            string[i] = tolower(c);
        }
        else
            return false;
    }
    return true;
}