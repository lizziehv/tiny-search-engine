/*
 * Author: Lizzie Hernandez Videa
 * May 19, 2020
 * Purpose: To define a new structure called tuple holding a pair of integers
 * 
 * See tuple.c for implementation
 * 
 * Usage: Used in querier.c to store pairs of (docID, score), which are inserted into 
 *        a querier.
 *  
 */

#ifndef __TUPLE_H
#define __TUPLE_H

#include <stdbool.h>

/********* data types *********/
typedef struct tuple tuple_t; 

/* @param - key
 * @param - score
 * 
 * @return a pointer to a tuple type <key, score>
 */
tuple_t *tuple_new(int key, int score);

/* @param pointer to a tuple 
 *
 * @return score of the tuple
 *         0 if tuple is NULL
 */
int tuple_get_score(tuple_t* pair);

/* @param pointer to a tuple 
 * 
 * @return key of the tuple
 *         0 if tuple is NULL
 */
int tuple_get_key(tuple_t* pair);

/* @param pointer to a tuple 
 *
 * Increase score by 1 for pair given
 * 
 * @return false if pair is NULL
 *         true otherwise
 */
bool tuple_update_score(tuple_t* pair);

#endif //__TUPLE_H