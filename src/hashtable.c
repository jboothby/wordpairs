#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crc64.h"

#define RESIZE_FACTOR 4		// multiple to enlarge table by for each growth
#define MAX_DEPTH 10		// max depth of any linked list in the hash table

/* typedef for node to use in linked list (buckets) */
typedef struct node{
	char* key;		// string key in key/value pair
	int value;		// integer value in key/value pair
	struct node *next; 	// pointer to next item in linked list
}node;

/* typedef for hashtable */
typedef struct hashtable{
	node** array;		// points to the first node in the array
	int contents;		// keeps track of current contents
	int collisions;		// holds number of collisions since last growth
	int maxDepth;		// holds max depth of lists
	int size;		// holds current size of table
}hashtable;

/* Function prototypes */
void hashtab_put(hashtable *ht, char *string, int val);	// put item into hashtable, if val supplied, add that too
void hashtab_display(hashtable *ht, int topNum);// output all information from hash table
hashtable *hashtab_create(int tableSize);	// create table of given size
void hashtab_empty(hashtable *ht);		// emtpy hashtable and free memory

int search(node *list, char *string);	 	// search the supplied list for the given word pair
int cmpfunc( const void * a, const void * b); 	// compare function for qsort
void hashtab_resize(hashtable *old);		// copies values from -> to, and frees from

/* This function initializes a hashtable and returns it */
hashtable* hashtab_create(int tableSize){

	/* Initialize table and allocate space on heap for hashtable and hashtable array */
	hashtable *ht = malloc( sizeof( hashtable ));
	ht->array = malloc(tableSize * sizeof(node**));

	/* Loop through array and initialize all values to null */
	for(int i = 0; i < tableSize; i++){
		ht->array[i] = 0x0;
	}

	/* initialize all values in hashtable */
	ht->size = tableSize;
	ht->contents = 0;
	ht->collisions = 0;
	ht->maxDepth = 0;

	return ht;
	
}

/* This table resizes the supplied hashtable when the depth is greather than MAX_DEPTH
   It does this in the following steps:
	1.) Create larger hashtable (new)
	2.) Copy nodes from old into new, and free memory allocated to old
	3.) Make old point to the new ( so future references to old will access new)
	4.) Free any references to new ( because now old points to that data)
*/
void hashtab_resize(hashtable *old){

	node* temp;	// node pointer for crawling over lists
	node* delNode;	// holds previous node so it can be freed properly

	// make a new hashtable that is larger than the size of previous
	hashtable *new = hashtab_create( old -> size * RESIZE_FACTOR );

	/* loop over hash table and traverse each non-null node */
	for( int i = 0; i < old -> size; i++){

		temp = old -> array[i];		// temp now points to head of linked list at location array[i]
		while( temp!= NULL ){		// loop until end of list is reached
			hashtab_put( new, temp -> key, temp -> value);	// copy node into new hashtable
			delNode = temp;		// copy temp node so it can be deleted
			temp = temp -> next;	// crawl to next node
			free( delNode -> key );	// free key (string) from last node
			free( delNode );	// free node struct itself
		}
	}

	new -> contents = old -> contents;	// copy number of contents from old to new ( same number of nodes )


	free( old -> array );	// free the array from the old table
	*old = *new;		// make the old hashtable point to contents of new hashtable
	free(new);		// free	hashtable structure from new (because contents are now addressed using old)
}

/*  This function will insert a new node into the hashtable
    with the specified key/value pair. It is also responsible for calling
    the resize function if the LOAD_FACTOR threshold is reached
*/
void hashtab_put(hashtable *ht, char *string, int val){
	

	/* Resize table if it's depth is too great  */
	if( ht->maxDepth >= MAX_DEPTH ){
		hashtab_resize( ht );
	}


	ht->contents += 1;	// increment size of hashtable

	/* compute index using crc64 and make another pointer to the list at that index*/
	int index = crc64(string) % ( ht->size );
	node *list = ht->array[index];		

	// if the node is already in list, just return
	// search function handles incrementing counter for node if it's a duplicate
	if( search(list, string) ){
		return;
	}
	
	/* create new node */
	node *newnode;			// pointer to new node	
	newnode = malloc(sizeof(node)); // allocate space for new node
	newnode -> key = malloc((strlen(string) + 1) * sizeof(char*));//allocate space for key
	strcpy(newnode -> key, string);		// copy string into the key for the node 
	newnode -> value = val;			
	newnode -> next = NULL;

	// if the location in the hashtable is null, add the new node
	if( list == NULL ){
		if( ht->maxDepth < 1) ht->maxDepth = 1;	// set max depth to 1 is this is first node
		ht->array[index] = newnode;
	}


	// if the node is not null, traverse to the end and append
	else{
		ht->collisions += 1; 	// increment collisions counter
		int depth = 1;

		/* traverse list to end, append node */
		node *temp;			// temporary node allows for traversing list
		temp=list;			// begin traversal at list (pointer to head)
		while( temp -> next != NULL ){	// traverse list until last node
			depth++;		// increment depth counter
			temp = temp->next;
		}
		temp->next = newnode; 	// append new node

		if( depth > ht->maxDepth ) ht->maxDepth = depth;
	}

	return;
}

/* this function outputs the hashtable by traversing each existing node */
void hashtab_display(hashtable *ht, int topNum){

	node* sortArr[ht->contents];	// holds array of all data for sorting
	node *temp;			// crawler for linked lists
	int j = 0; 			// holds current index for array

	/* loop over hash table and traverse each non-null node */
	for( int i = 0; i < ht->size; i++){
		temp = ht->array[i];		
		while( temp!= NULL ){		// loop until end of list is reached
			sortArr[j] = temp;	// add current node to array for sorting
			temp = temp -> next;	// crawl to next node
			++j;			// increment array index
		}
	}

	// print out values to specified (topNum), or j (end of data), whichever is smaller
	int limit = j;
	if( topNum != 0 && topNum < j) limit = topNum;

	// call qsort to sort the array using the cmpfunc
	qsort(sortArr, j, sizeof(node*), cmpfunc);


	// output sorted array of data
	for(int k = 0; k < limit; k++){
		printf("%10d %s\n", sortArr[k] -> value, sortArr[k] -> key);
	}
}

/* this is the compare function for qsort to sort the output array */
int cmpfunc( const void * p, const void * q){

	// cast void pointers from function call to node pointers
	node *a = *(node**)p;
	node *b = *(node**)q;

	// compare values of a and b. 
	if( a -> value < b -> value )		// if current is less than next, return 1 to swap
		return 1;

	else if ( a -> value > b -> value)	// if current is greather than next, return -1 to leave
		return -1;

	return 0;				// if values are same, do nothing
}

/*  This function searches the given list for the supplied word pair
    and increments the counter it found. returns 1 if found, 0 if not found
*/
int search(node *list, char *string){
	
	/* handle error case */
	if( list == NULL ) return 0;
	
	/* initialize a temp node to step through the list */	
	node *temp;
	temp = list; 	// point temp to list (header node)

	/* iterate through list */
	while( temp != NULL  ){
		if( strcmp(temp -> key, string) == 0){	// if key is in list, increment value
			temp -> value += 1;		// increment value (frequency) of current node because it's a repeat	
			return 1;			// pair found, return 1
		}
		temp = temp -> next;
	}
	
	return 0;	// if pair not found, return 0
}

/*  This function empties the hashtable and frees memory allocated to all nodes
*/
void hashtab_empty( hashtable *ht){
	
	node *this;	// pointer to current node
	node *next;	// pointer to next node

	// iterate over hashtable array and each linked list
	for( int i = 0; i < ht->size; i++){
		// crawl through each node and free it
		this = ht->array[i];		//current node
		while( this != NULL ){		//while the node isn't null
			next = this->next;	//save next node before deleting current
			free( this -> key );	//delete key we calloc'd
			free( this );		//free current node
			this = next;		//move next into current
		}
		
		free(this);	//free last node
	};
	
	free( ht->array );	// free the array
	free( ht );		// free the pointer itself
}
