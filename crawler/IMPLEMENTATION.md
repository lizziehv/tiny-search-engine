# CS50 Lab 4 Crawler Implementation
## Lizzie Hernandez, CS50 Winter 2020

### Data structures used

* Hashtable: A hashtable is an array of non-null (key, item) pairs, with unique keys, implemented using a set of (key, item) pairs. I used a hashtable to keep track of which URLs have already been visited by the crawler, using each URL as a key and an empty string ("") as the item, since null items are not allowed. Each unique URL is added into the hashtable so that no page is visited twice.

* Bag: A bag is an unordered set of non-null items. It provides the finction `bag_extract()`, which pops one item on every call. I used a bag to keep track of which pages to visit next, just like a stack. Note that `bag_extract()` will pop items in no particular order. Therefore, pages are visited in no particular order starting from depth 0.

### Pseudocode for each component

The crawler runs as follows:

- Create a hashtable to keep track of visited urls and a bag to keep track of which web pages to visit.
- Insert the seedURL into both the hashtable and the bag
- While there are still webpages inside the bag...
    - Extract a webpage
    - Fetch its data (using *pagesaver*)
    - If its depth is less than the maximum depth given...
        - For each of the linked pages embedded as anchors in the current page's html...
            - If the linked url is internal, can be normalized, and had not been inserted into the hasthable...
                - Insert into hashtable and into bag

The pagesaver runs as follows:

- Create a new file with name of the webpage's unique id
- Copy the page's url
- Copy the page's depth
- Copy the page's html


### Definition of functions and parameters

```c
/* @param seed_url - url to start fetching from
 * @param page_dir - directory to write files into
 * @param max_depth - maximum recursion depth of pages to visit
 * @param bag - initialized bag module to keep track of which pages to visit
 * @param htable - initialized hashtable to keep track of which pages have been visited
 * 
 * Use to crawl a website and retrieve webpages starting with a specified URL
 * 
 * @return 0 if success
 * @return 1 if function was aborted because webpage could not be created from seedURL
 */
int crawler(char *seed_url, char *page_dir, int max_depth, bag_t *bag, hashtable_t *htable);
```

See ../common/README.md for definition of functions in *pagedir.c*.

### Memory allocation in C and modules used

1. In *webpage.c*, the `webpage_getNextURL` function allocates memory for the URL. This is later freed once a webpage is deleted after being extracted from bag and explored.

2. All modules will be newed and freed in `main`.

3. Memory was allocated for the `seedURL` in order to prevent errors in `webpage_delete`, which frees the page's URL.

4. New files were created by first allocating memory for the file's name, using the C string library.


### Error handling

* If a webpage's html cannot be fetched, the error will be logged and the page will be ignored.

* If a URL found cannot be normalized, the error will be logged and the page will not be explored.

* If a URL found is not internal, it will be ignored as a security measure. The crawler will only go into internal URLs.

* *Pagesaver* will stop upon any error writing page into file.

* The crawler will stop upon any error creating the needed modules.

* Incorrect parameters given will abort the function call, for example, any errors creating webpage from `seedURL`, non-existing directories, etc.

### Security and privacy properties

Our crawler purposely slows down its behavior adding a 1 second delay between html fetches. Also, to avoid bothering other servers, the crawler will only explore internal URLs inside the CS playground.

### Resource management

Libraries used are `common.a` and `libcs50.a`, found in common/ and libcs50/, respectively. Each can be compiled by running `make` from the directory root.


### Persistant storage (files, database, etc)

None required.
