/*
 * Author: Lizzie Hernandez Videa
 * May 17, 2020
 * Purpose: To define item function for iterating through counters
 * 
 * See iterating.c for implementation
 * 
 * Usage: Note that all itemfuncs take as parameters 
 *        void *arg
 *        const int key
 *        const int count
 *        And return void
 * 
 *        These are the the parameters and return values of any itemfunc
 *        in the method from counters
 *        void counters_iterate(counters_t *ctrs, void *arg, 
 *                     void (*itemfunc)(void *arg, 
 *                                      const int key, const int count));
 *        
 *        These functions are used as any of the item functions (third parameter
 *        of counter_iterate)
 *  
 */

#include "counters.h"
#include "tuple.h"


/* If called on counter1 (should be an empty counter) with arg counter2, 
 * it will add counters in counter2 to counter1 if count > 0
 */
void counters_add_all(void *arg, const int key, const int count);

/* If called on counter1 with arg counter2, it will add key, count pair
 * in counter1 and add the count of the key in counter2 (0 if key is not 
 * in counter2)
 */
void counters_merge(void *arg, const int key, const int count);

/* If called on counter1 with arg counter2, it will 'merge' with count 
 * so that all keys have as count min(counter1.key, counter2.key).
 *
 * Note that the count will be 0 if a key in counter1 was not found in
 * counter2. Use add_all afterwards to move all keys with non-zero counts
 * to an empty counters set.
 */ 
void counters_intersect(void *arg, const int key, const int count);

/* If called on counter1 with argument to an integer pointer *count,
 * it will update the integer for every key
 * 
 * (Use to count the number of counters in the counters set)
 */
void counters_count(void *arg, const int key, const int count);

/* If called on counter1 with argument to an empty array of tuple pointers
 * tuple_t *array[], will insert a (key, item) pair into array in decreasing 
 * order using an insertion sort approach.
 * 
 * array must have as the first element a tuple with score=1 and any integer
 * value key
 * 
 * (Used to insert counters into array starting at index 1)
 */
void counters_to_array(void *arg, const int key, const int count);
