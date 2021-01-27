#include <stdio.h>	// for printing output
#include <string.h>	// for strcat and strcpy
#include <stdlib.h>	// for free/exit/ etc..
#include <ctype.h>	// for isdigit() to parse arguments
#include "getWord.h"	// for getNextWord
#include "hashtable.h"	// for hashtable functionality

/* Function Prototypes */
int isNum( char *string ); // returns 1 if string is a number, 0 if not

int main(int argc, char **argv){

	// define help message to output when program not invoked properly
	char *errorMessage = "\n\n\tUsage: ./wordpairs <-count> fileName1 <fileName2> <fileName3> ...\n\n\tWhere: count is the number of words to display\n\n";
	
	int displayNum = 0;	// number of word pairs to display (if left at 0, displays all contents of hashtable)
	int fileStart = 1;	// this number holds the beginning of the fileNames in argv[1]. by default, assumes first argument is file


	/* Command line argument checking */
	// If there are no arguments, print error message and exit
	if( argc == 1 ){
		fprintf(stderr, "Error: This program requires at least one file argument%s", errorMessage);
		exit(-1);
	// if there is one argument, but it's a number, print error message and quit
	}else if ( argc == 2){
		if ( isNum( argv[1] ) ){
			fprintf(stderr, "Error: This program requires at least one file argument%s", errorMessage);
			exit(-1);
		}
	// if there are two or more arguments, check to see if first is a number
	}else{
		// if the first argument starts with a dash
		if ( argv[1][0] == '-' ){
			// if the first argument is a number
			if ( isNum( argv[1] ) ){
				// convert argument (excluding dash) to integer
				displayNum = atoi( argv[1]+1 );
				fileStart = 2;
			}
		}
	}

	/* initialize hashtable and fill with NULL values */
	int tablesize = 128;
	hashtable *ht  = hashtab_create(tablesize); 	// init hashtable with start size of 1024

	/* loop through all file arguments and add them to the hashtable*/
	for( int i = fileStart; i < argc; i++){ 

		/*Open file*/
		FILE *fp; 
		fp = fopen( argv[i], "r"); 	// open file in read mode	
		// if the file does not exist, print error, free hashtable, and exit
		if( fp == NULL ){
			fprintf(stderr, "Error: file '%s' does not exist, or cannot be opened%s", argv[i], errorMessage);
			hashtab_empty(ht);
			exit(-1);
		}

		/* read through file, print each word pair and it's hash*/

		char* temp = getNextWord(fp);	// get next word from file (malloc'd by getNextWord);
		char* lastWord = (char*)calloc( 1024, sizeof( char* )); // allocate space for temp to copy into
		strcpy(lastWord, temp);		// cpy contents into buffer
		free( temp );			// free memory from temp (because getNextWord uses strdup)

		char* thisWord;	// holds current word
		char* pair;	// holds word pair to add to table

		while( (thisWord = getNextWord(fp)) != NULL ){ 	// continue reading until EOF

			// allocate memory for pair big enough for each word and the space
			// +1 is for the space
			pair = (char *) calloc( strlen(lastWord) + strlen(thisWord) + 1, sizeof(char*));
		
			strcpy(pair, lastWord); // add space after last word
			strcat(pair, " ");	// add space between words
			strcat(pair, thisWord);	// concat this word to finish word pair

			hashtab_put(ht, pair, 1);	// add current word pair to the hashtable

			strcpy(lastWord, thisWord);	// copy this word into last word		

			free( thisWord ); 	// free malloc from strdup in getNextWord
			free( pair );		// free the space allocated for pair

		}

		free( lastWord );// free temp var that was holding last word
		fclose(fp);	// close current fil
	}

	// display hashtable and print end values
	hashtab_display(ht, displayNum);

	// empty the hashtable to free data
	hashtab_empty(ht);

	exit(0);
}

/* This function returns 1 if the string is a number, and 0 if it is not 
   Ignores '-' character for parameters
*/
int isNum( char *string ){
 	int flag = 1; 	// 1 if number, 0 if not
	for( int i = 0; i < strlen(string); i++){
		// check for digit, ignore dashes
		if( string[i] != '-' && !isdigit( string[i] )) flag = 0;
	}
	return flag;
}
	
