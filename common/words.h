/**
 * words.h
 * See words.c for implementation
 * 
 * Author: Lizzie Hernandez Videa
 * Date: May 10, 2020
 */ 

#ifndef __WORDS_H
#define __WORDS_H

#include <stdbool.h>

/* @param string- a strign to normalize
 * 
 * Converts every chracter in a string into lower case
 * 
 * @return true if string was correctly normalized
 * @return false if string contains non-alphabetic characters
 */
bool normalize_word(char* string);

#endif