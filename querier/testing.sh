# Create wiki to work in if it doesn't exist
TESTDIR=test1/
INV_DIR=randomName
wiki=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/


if [ ! -d $TESTDIR ]; then
    echo "Making directory for testing ..." 
    mkdir $TESTDIR
    # run crawler
    ../crawler/crawler $wiki $TESTDIR 1 &>/dev/null
    # run querier
    ../indexer/indexer $TESTDIR $TESTDIR/index
    echo
fi

########### Check validity of arguments ########### 
echo "########### Check validity of arguments ###########" 
echo

echo "1. Incorrect number of arguments" 
./querier $TESTDIR $TESTDIR/index 5
./querier $TESTDIR
echo

echo "2. Invalid directory" 
./querier $INV_DIR index
echo

echo "3. Invalid filename" 
./querier $TESTDIR $TESTDIR/index-null
echo 

echo "4. Not a crawler directory" 
./querier . index
echo 

echo "########### Use querier on different query inputs ###########" 
echo "Testing queries that should fail..."
./querier $TESTDIR $TESTDIR/index < testdir/badchars

echo "Using input for which results are known..."
if [ ! -f testdir/example ]; then
    echo "Needed 'example' file to test known outputs"
else
    cat testdir/examplein | ./querier $TESTDIR $TESTDIR/index &> testdir/exampleout
    echo "Comparing files..."
    echo "Note that files might differ in the order in which some results appear."
    diff testdir/example testdir/exampleout

    if [ $? -eq 0 ]; then
        echo "No differences found."
    fi
fi
echo

echo "Generating random queries with fuzzquery..."
if [ ! -d testinput ]; then
    mkdir testinput/
    ./fuzzquery $TESTDIR/index 10 2 > testinput/test1
    ./fuzzquery $TESTDIR/index 10 5 > testinput/test2
fi

./querier $TESTDIR $TESTDIR/index < testinput/test1
./querier $TESTDIR $TESTDIR/index < testinput/test2




