CURR_DIR=../indexer
CDIR=../crawler
DIR1=$CDIR/data1
DIR2=$CDIR/data2
DIR3=$CDIR/data3
INV_DIR=$CDIR/randomName

# check if crawler had been ran before
# if not, run crawler
if [ ! -d $DIR1 ]; then
    cd $CDIR
    make clean
    make
    make test &> testing.out
    cd $CURR_DIR
    echo "changed"
fi

echo "Testing index.c ..." 

########### Check validity of arguments ########### 
echo "########### Check validity of arguments ###########" 
echo

echo "1. Incorrect number of arguments" 
./indexer $DIR1 index 4 6
./indexer $DIR1 
echo

echo "2. Invalid directory" 
./indexer $INV_DIR index
echo

echo "3. Invalid filename" 
./indexer $DIR1 $DIR2
echo 

echo "4. Not a crawler directory" 
./indexer $CDIR index
echo 

echo "########### Use Indexer on different directories ###########" 
echo

if [ ! -d RES ]; then
    mkdir RES
fi

# Directory with slash
./indexer $DIR1/ RES/index1
echo "See RES/index1 for output."

# Given an existing file
touch $DIR2/index2
./indexer $DIR2 RES/index2
echo "See RES/index2 for output."

./indexer $DIR3 RES/index3
echo "See RES/index3 for output."

echo
echo "Testing indextest.c ..." 

########### Check validity of arguments ########### 
echo "########### Check validity of arguments ###########" 
echo

echo "1. Incorrect number of arguments" 
./indextest RES/index1 RES/index1-1 RES/index1-2
./indextest RES/index1
echo

########### Tests that should pass ########### 
echo "########### Tests that should pass ###########" 
echo

echo "Testing with RES/index1 and RES/new-index1 ..."
./indextest RES/index1 RES/new-index1

echo "Sorting files..."
gawk -f ./indexsort.awk RES/index1 > RES/index1.sorted
gawk -f ./indexsort.awk RES/new-index1 > RES/new-index1.sorted

echo "Comparing files..."
diff RES/index1.sorted RES/new-index1.sorted

if [ $? -eq 0 ]; then
    echo "No differences found."
fi