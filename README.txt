This program counts the frequency of word pairs in supplied text files
and outputs the most commonly occuring ones. It does this by storing all
word pairs from the supplied files in a hashtable, and then printing the
contents. If the <-count> parameter is ommitted, ALL wordpairs will be printed
If the <-count> parameter is supplied, just the top <-count> most frequent
pairs will be displayed.

If one of the supplied file names does not exist, or cannot be read,
the program will exit with a -1 exit status immediately and display
wordpairs from any other valid files.


All commands should be entered from the present directory of this readme

Run:
	./wordpairs <-count> fileName1 <fileName2>...

Compile:
	make

Test program on all datasets:
	make run

Test program on all datasets using valgrind to detect memory leaks
	make memcheck

Remove compiled files:
	make clean

Examples:
	Read file and print ALL word pairs:
		./wordpairs ./fileName.txt
	
	Read file and print 10 most common word pairs:
		./wordpairs -10 ./fileName.txt

	Read multiple files and display 25 most common pairs:
		./wordpairs -25 ./fileName.txt ./fileName2.txt

