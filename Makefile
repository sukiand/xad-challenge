all: run 

compile: test.cpp segmentTree.h
	g++ -std=c++11 test.cpp  -o test.out

run: compile test.out
	./test.out

clean:
	rm -rf *.o *.out
