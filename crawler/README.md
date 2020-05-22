# CS50 Lab 4 crawler
## Lizzie Hernandez, CS50 Spring 2020

### Crawler

Crawler implements the Requirements Spec and is used to explore internal URLs starting from a 
seed url and visiting linked urls.

### Usage

Crawler will execute from command line using the following syntax:
```
    ./crawler seedURL pageDirectory maxDepth
```
It will crawl all pages reachable starting from seedURL up to a maximum depth of maxDepth (where a depth
of 0 will only include the seedURL).

- Parameters
1. seedURL is the URL to start crawling from. It must be internal and must be able to normalize it.
2. pageDirectory is an existing directory where retrieved pages will be written into, each under a unique
file named after the pages' corresponding unique ids, which start from 1(seedURL) and increment on every page.
3. maxDepth is the maximum number of "level" to explore.

- Output<br />
<br />
Any errors will be logged into standard error. Each page is written into the directory provided inside 
files named after a unique id. Each output file contains the url, depth, and html of a reachable page.

- Exit status

0. success<br />
1. Usage: Incorrect argument input.<br />
2. Error creating any of the needed modules<br />
3. Unable to create webpage from  seedURL<br />


### Implementation

See [IMPLEMENTATION.md](IMPLEMENTATION.md)

### Assumptions

- No assumptions besides those that are clear from the spec

### Compilation

Before compiling, make sure `libcs50/libcs50.a` and `common/common.a` exist. These can be compiled by running `make` from the root of their corresponding directories.
Run `make` from `crawler` directory to compile the program and `make test` to see a sample run. Run `make clean` after running the test to delete unwanted files.

### Testing

To test, simply `make test`.
See [TESTING](TESTING.md) for details of testing and an example test run.

### Limitations

- I've included a maximum allowable depth of 200, since the hashtable is of size 200 to prevent overloading the hashtable.