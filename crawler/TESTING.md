# CS50 Lab 4
## Lizzie Hernandez, CS50 Winter 2020

### Testing 'crawler'

To test my *crawler* module I wrote a test program, [testing.sh](testing.sh), to explore the capabilities of the *crawler* program and visualize output of running the program with valid or invalid parameters.

I first tested boundary cases, including: less than 4 arguments given, more than 4 arguments given.

Then I tested the validity of each argument: negative maximum depth, external URL, non-existing directory, incorrect third parameter.

After these boundary cases, I made example test runs using different internal URLs and different maximum depths.

### Test run

An example test run is in `testing.out`, showing output from `testing.sh`, built with

	make test &> testing.out

In that file one can see different results of running the crawler program to explore webpages.

### Limitations

- Hashtable is of fixed size. I used 200 slots, which might become less efficient as more webpages 
are explored.