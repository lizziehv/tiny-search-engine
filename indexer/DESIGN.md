---
written by: Maria Paula Mora, Lizzie Hernandez, Ethan Trepka, and Kshitij Jain
layout: page
title: TSE Indexer Design Spec
---

Recall the [Indexer Requirements Spec](REQUIREMENTS.md); the **indexer**  reads all of the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

### User interface

The indexer’s only interface with the user is on the command-line; it must always have two arguments.

```
./indexer pageDirectory indexFilename

```
Note: Do not assume the pageDirectory is entered with slash at the end. Hence, it could be entered as `./data/` or `./data` both of which are equivalent and should be handled correctly.

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: we output indexFileName and write the index to that file in the following format: 
* one line per word, one word per line
* each line provides the word and one or more (docID, count) pairs, in the format *word docID count [docID count]...*
* where word is a string of lower-case letters,
* where docID is a positive non-zero integer,
* where count is a positive non-zero integer,
* where the word and integers are separated by spaces.


### Functional decomposition into modules

We anticipate the following modules or functions:

indexer.c
 1. *main*, which parses arguments and initializes other modules, initialize data structure index
 3. *indexBuild(directory, index)* builds an index from the directory given by reading and processing the files inside
 4. *indexSave(file, index)* saves the index into a file
 5. *indexDelete* clean up data structures

indextest.c
 1. *main*, process and validate command-line parameters, initialize data structure index
 3. *indexLoad(file1, index)*, reads an index based file and creates a hashtable index
 4. *indexDave(file2, index)*, saves this new index into a file
 5. *indexDelete*, clean up data structures

Other important functions:
 1. *pageLoad*, which creates a webpage from crawler output files
 2. *isCrawlerDirectory*, which verifies whether the given directory is a Crawler-produced directory
 3. *NormalizeWord*, which converts all letters in a word to lowercase 

And some helper modules that provide data structures:

 1. *hashtable* filled with sets of counters to keep track of the words seen
 2. *set* maps from a word to a list of (docID, count) pairs
 3. *counter* keeps track of how many times a word was seen in a specific docID

### Pseudo code for logic/algorithmic flow
The Indexer will run as follows:
1. Execute from a command line as shown in the User Interface
2. Process and validate command-line parameters
3. Initialize data structure index
4. indexBuild(directory, index)
	5. for each file in (pageDirectory)
		6. convert the document ID to a webpage
		create counters 
		7. while there are more words in a website
			8. if the word has more than 3 characters
				9. Normalize the word (convert to lowercase)
			10. if hashtable doesn’t contain word 
				11. add word to hashtable 
			12. if hashtable does contain word
				13. if counters at that word contains docID
					14. increment count
				15. else 
					16. add counter to ht with (docID, 1)
		
17. indexSave(file, index);
18. for each index created
19. write the index into the pathname indexFilename (creates the file (if needed) and overwrites the file (if it already exists))
20. clean up data structures
	21. for every counter in hashtable
22. free the counter
	23. free the hashtable
 
The indexer completes and exits when it has nothing left to read in the directory and the hashtable is clean - no more files to be indexed.

### Dataflow through modules
indexer.c
 1. *main*, parses parameters (uses ``isCrawlerDirectory``) and  initialize data structure index, passes them to the ``indexBuild``
 3. *indexBuild(directory, index)*  builds an index from the directory given by reading and processing the files inside (uses ``pageLoad`` and ``NormalizeWord``)
 4. *indexSave(file, index)* saves the index into a file
 5. *indexDelete* once index is saved to file, clean up by freeing hashtable and constituent data structures

indextest.c
 1. *main*, process and validate command-line parameters and passes them to the ``indexLoad``, initialize data structure index
 3. *indexLoad(file1, index)*, reads an index based file and creates a hashtable index 
 4. *indexSave(file2, index)*, saves this new index into a file
 5. *indextestDelete*, once index is saved to file, clean up the data structures

### Major data structures

Three helper modules provide data structures:

 1. *hashtable* of sets with words as keys and counters as items
 2. *counters* of document names. Count is how many times a word was seen in a specific document
3. *sets* which will map from words to counters of (docID, count)

### Testing plan

*Unit testing*.  A small test program in `testing.sh` to test each module to make sure it does what it's supposed to do.

*Integration testing*.  Assemble the indexer and test it as a whole using `testing.sh`.
In each case, examine the output files carefully to be sure they have the correct representation of the index data structure. 

### Indexer.c Testing
#### Incorrect Inputs
- Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.
Test the indexer with a `pageDirectory` that points to a non-existing directory
Test the indexer with a `pageDirectory` that points to non-crawler-created directory  
Test the indexer with an invalid filename given as a second parameter
More than 3 parameters
Less than 3 parameters

#### Correct Inputs
- Test the index with a `indexFilename` that points to a file that already exists.
- Test the index with a `pageDirectory` in different formats (./directory, /directory,directory/)

### Indextest.c Testing
#### Incorrect Inputs
- Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.
Test the indexer with a nonexistent oldIndexFilename
Test the indexer with an empty file given as oldIndexFilename (or a non crawler format file)
Test the indexer with an invalid filename given as a second parameter
More than 3 parameters
Less than 3 parameters

#### Correct Inputs
- Test the index with any of the files that we created before in indexer test
- Compare the files created

Verify that these tests files created valid results created match expectations.



