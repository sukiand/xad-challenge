### Information
This is a simple c++ program to solve xad challenge problem: given a list of object, find the number of objects that lies in arange of values. Object length can be 10k to 100M.

### Author
Changyun Gong

### Content

#### Environment
ubuntu 14.04 in virtual machine with 4GB RAM with single processor.
c++ 11  

run the program:
```bash
cd PROGRAM_DIR
make
```

#### Files
**segmentTree.h**  
contians template struct of SegmentTreeNode and template class of SegmentTree. SegmentTree can be construct from the same type of vector. SegmentTree is the main data structure to store objects. Member function SegmentTree.query() is the function to find the number of objects the lies in a certain range.  
Each node record 6 message: min value of the segment, max value of the segment, start index, end index, left child and right child.  


usage of the query function:  
```bash
SegmentTree Tree(a_type_of_vector);
//get object >= 1000
//once the signal is ENDLESS, the function does not care the value folloing with the signal.
Tree.query(CLOSE,1000,ENDLESS,0);

//get object >1000 and <120000
Tree.query(OPEN,1000,OPEN,120000);

//get object = 1000
Tree.query(CLOSE,1000,CLOSE,1000);
```

**test.cpp**  
Contains the test cases.  
- test case 1
type: int  
feature: 0,INT_MAX,INT_MIN,0,INT_MAX,INT_MIN,0,.....  
queries:   
target = 0  
0 <= target < 0  
target < 0  
target <= 0  
everything  
INT_MIN < target < 0  
0 < target < INT_MAX  
INT_MIN < target < INT_MAX  
0 < target <= INT_MAX  

- test case 2
type: float  
feature:  -22.22,-22.22,..., 0.1555,0.1555,...,111.1555,111.1555...  
queries:  
111.1555 <= target  
111.1554 <= target  
-22.22 < target  

- test case 3
type: string  
feature: hello, aabbccdd, aabbccdd,..., aabbccddeeffgg, aabbccdd, aabbccdd ,....,aaaaaa.  
queries:  
a < target < b  
h <= target < i  
a < target < aabbccdd  

#### Analysis
1. When doinf the test, the max length of vector which can be dealed with is 40M. After that, the program will be killed due to out of memory(4GB memory).  
2. When construct the tree or search the tree where traversal is needed, I use queue to do level traversal other than recursive traversal. Recursive way is slower.  
3. For the worst case, such as the query target = 0 in test case 1, the query need to go through all leaves to sum the answer, so that the time cost is nlogn.  
4. The query will perform better in the case 2 and 3 where the search range can overlap a large cluster to return the index number directly, or the search range is out of the cluster range then return 0 directly.









