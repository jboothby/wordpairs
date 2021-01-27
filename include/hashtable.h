#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "crc64.h"
#include <string.h>

/* Implementation of a hashtable where the key is a string
   and the value is a number. 

   This implemenation allows for actions such as the following:
	create table, put key/value pair, display table, empty table
*/

/* macros */
#define LOAD_FACTOR 0.75	// this number is how full the hashtable must be before automatically resizing
#define RESIZE_FACTOR 4		// the factor that the table will grow by when load factor threshold is reached

/* typedef for node to use in linked list (buckets) */
typedef struct node{
	char* key;
	int value;
	struct node *next; // pointer to next item in linked list
}node;

/* typdef for hashtable */
typedef struct hashtable{
	node** array;	// pointer to array of nodes
	int contents;	// holds number of contents in table
	int collisions;	// holds collisions since last growth
	int maxDepth;	// holds max depth of any list on the table
	int size;	// holds current size of table
}hashtable;


/* Function prototypes */

/* ht: the hashtable, string: the key, val: the value */
void 		hashtab_put( hashtable *ht, char *string, int val);

/* ht: the hasthable. Empties supplied hashtable and frees all associated memory */
void 		hashtab_empty( hashtable *ht);	

/* ht: the hashtable, topNum: how many items to display. This function first unloads the hashtable
   into an array, then sorts that array in decreasing order of value size. It will then display the
   first "topNum" values. In otherwords, it prints the "topNum" values in the hashtable with the highest
   values
*/
void 		hashtab_display(hashtable *ht, int topNum);		// output all information from hash table

/* tableSize: The initial size of the hashtable. The table will automatically grow when the load factor
   surpasses the threshold defined in the macro LOAD_FACTOR, however choosing a smaller initial table size
   will result in less memory being used on the heap if the table is being loaded with a small number of values.
   This function returns a pointer to a hashtable structure, so it should be invoked as following
	hashtable *ht = hashtab_create( size );
*/
hashtable*	hashtab_create(int tableSize);				// build table of given size

#endif
