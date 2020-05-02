
# Testing script for crawler.c
# Author: Lizzie Hernandez Videa
# Date: April 30, 2020
#
# usage: bash -v testing.sh

# Define variables
seedURL1=http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html
seedURL3=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
seedURL2=http://old-www.cs.dartmouth.edu/~cs50/index.html
seedURL4=http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-coming-woman-a-novel-based-on-the-life-of-the-infamous-feminist-victoria-woodhull_993/index.html
wiki=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/

externalURL=www.google.com

madeupserver=http://www.randomURLthatshouldntexist.com
madeupURL=http://old-www.cs.dartmouth.edu/~cs50/randomabc.html

#####################################
####### Tests that should fail ######
echo "####### Tests that should fail ######"
mkdir data
# less than 4 arguments
echo 
echo "1. less than 4 arguments"
./crawler
./crawler $seedURL1
./crawler $seedURL1 data

# more than 4 arguments
echo 
echo "2. more than 4 arguments"
./crawler $seedURL1 data 2 3 4 

# Negative max depth
echo 
echo "3. Negative max depth"
./crawler $seedURL1 data -2

# 4 using external url
echo 
echo "4. Using external url"
./crawler $externalURL data 2

# Non existing directory
echo 
echo "5. Non existing directory"
./crawler $seedURL1 random123abc 2

# Incorrect third parameter
echo 
echo "6. Incorrect third parameter"
./crawler $seedURL1 data helloWorld

# Non existing server
echo 
echo "7. Non-existing server"
./crawler $madeupserver data 3

# Valid server but non-existent page
echo
echo "8. Valid server but non-existent page"
./crawler $madeupURL data 3

######################################
### These tests should pass ####
echo
echo "### These tests should pass ####"

# at depth 0
echo
echo "At depth 0 >>>>>>>>>>>> see data1 directory for output"
mkdir data1
./crawler $seedURL1 data1/ 0

# at depth 2
echo
echo "At depth 2 >>>>>>>>>>>> see data2 directory for output"
mkdir data2
./crawler $seedURL2 data2 2

# at depth 5
echo
echo "Closed set with repeating pages..."
echo "At depth 5 >>>>>>>>>>>> see data3 directory for output"
mkdir data3
./crawler $seedURL3 data3 5

# at depth 5
echo
echo "Different starting url..."
echo "At depth 1 >>>>>>>>>>>> see data4 directory for output"
mkdir data4
./crawler $seedURL4 data4 1

# at depth 5
echo
echo "Wikipedia page..."
echo "At depth 1 >>>>>>>>>>>> see wiki directory for output"
mkdir data5
./crawler $seedURL4 data5 1