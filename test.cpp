#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cassert>
#include <climits>
#include <typeinfo>
#include "segmentTree.h"

using namespace std;

class Test{
    public:
        // print the test case generator information
        template<typename T>
        static SegmentTree<T>* test_case_generate_Stree(int const &case_num, vector<T> const &source){
            cout <<endl<<"test case "<<case_num<<endl;
            cout << "type: "<<typeid(T).name()<<"\ttotal size: " << source.size()<<endl;
            cout << "start to construct segment tree..."<<endl;

            clock_t begin,end;
            begin = clock();
            SegmentTree<T> *Stree = new SegmentTree<T>(source);
            end = clock();
            cout << "completed. Time cost: "<<end-begin<<"\tmicroseconds"<<endl;
            return Stree;
        };

        // print query information in each test case.
        template<typename T>
        static void test_case_query(int const &case_num, int const query_num,  SegmentTree<T>* const Stree,
                                    search_range_signal const left, T const & left_value,
                                    search_range_signal const right, T const & right_value,
                                    m_long_int const compare){
            clock_t begin,end;
            m_long_int result;
            begin = clock();
            result = Stree->query(left, left_value, right, right_value);
            end = clock();
            assert(result == compare);
            cout << "query "<<case_num<<"."<<query_num <<": "<<end - begin <<"\tmicroseconds"<<endl;
        }

        void test_integer(int case_num, m_long_int size){
            vector<int> source(size);
            for(auto i = 0; i < source.size();i++){
                if(i%3 == 1){
                    source[i] = INT_MAX;
                }
                if(i%3 == 2){
                    source[i] = INT_MIN;
                }
            }

            // tree leaves is like: 0,INT_MAX,INT_MIN, 0,INT_MAX,INT_MIN,.....
            SegmentTree<int> *Stree = test_case_generate_Stree<int>(case_num,source);
            int query_num = 0;
            // you can add or revise query here for this test case.

            // find number of int target = 0
            test_case_query(case_num, ++query_num, Stree,
                            CLOSE, 0, CLOSE, 0,
                            source.size()/3+(source.size()%3==0?0:1));
            // find number of  0 <= int target < 0
            test_case_query(case_num, ++query_num, Stree,
                            CLOSE, 0, OPEN, 0,
                            0);
            // find number of int target < 0
            test_case_query(case_num, ++query_num, Stree,
                            ENDLESS, 0, OPEN, 0,
                            source.size()/3);
            // find number of int target <= 0
            test_case_query(case_num, ++query_num, Stree,
                            ENDLESS, 0, CLOSE, 0,
                            source.size()/3*2+(source.size()%3==0?0:1));
            // find number of all
            test_case_query(case_num, ++query_num, Stree,
                            ENDLESS, 0, ENDLESS, 0,
                            source.size());
            // find number of INT_MIN < int target < 0
            test_case_query(case_num, ++query_num, Stree,
                            OPEN, INT_MIN, OPEN, 0,
                            0);
            // find number of 0 < int target < INT_MAX
            test_case_query(case_num, ++query_num, Stree,
                            OPEN, 0, OPEN, INT_MAX,
                            0);
            // find number of INT_MIN < int target < INT_MAX
            test_case_query(case_num, ++query_num, Stree,
                            OPEN, INT_MIN, OPEN, INT_MAX,
                            source.size()/3+(source.size()%3==0?0:1));
            // find number of 0 < int target <= INT_MAX
            test_case_query(case_num, ++query_num, Stree,
                            OPEN, 0, CLOSE, INT_MAX,
                            source.size()/3+(source.size()%3==2?1:0));
            delete(Stree);
        }

        void test_float(int case_num, m_long_int size){
            vector<float> source(size/3, -22.22);
            source.resize(size/3*2,0.15555);
            source.resize(size/3*3,111.15555);

            // leaves are like: -22.22, -22.22, ... , 0.1555, 0.1555, ... , 111.15555, 111.1555,...
            SegmentTree<float> *Stree = test_case_generate_Stree<float>(case_num, source);
            int query_num = 0;
            // you can add or revise query here for this test case.

            // find number of 111.555 <= float target
            test_case_query(case_num, ++query_num, Stree,
                            CLOSE, float(111.15555), ENDLESS,float(0.0),
                            source.size()/3);
            // find number of 111.554 <= float target
            test_case_query(case_num, ++query_num, Stree,
                            CLOSE, float(111.15554), ENDLESS,float(0.0),
                            source.size()/3);
            // find number of -22.22 < float target < 1
            test_case_query(case_num, ++query_num, Stree,
                            OPEN,float(-22.22), OPEN, float(1.0),
                            source.size()/3);
            delete(Stree);
        }

        void test_string(int case_num, m_long_int size){
            vector<string> source(size, "aabbccdd");
            source[0] = "hello";
            source[source.size()-1] = "aaaaaaaa";
            source[source.size()/2] = "aabbccddeeffgghhiijjkkllmm";

            // leaves are like: hello, aabbccdd, aabbccdd , ... , aabbccddeeffgghhiijjkkllmm , aabbccdd, aabbccdd, ...,
            //                  aaaaaaaa.
            SegmentTree<string> *Stree = test_case_generate_Stree<string>(case_num, source);
            int query_num = 0;
            // you can add or revise query here for this test case.

            // find number of a < string target < b
            test_case_query(case_num, ++query_num, Stree,
                            OPEN, string("a"), OPEN, string("b"),
                            source.size()-1);
            // find number of h <= string target < i
            test_case_query(case_num, ++query_num, Stree,
                            CLOSE, string("h"), OPEN,string("i"),
                            1);
            // find number of a < string target < aabbccdd
            test_case_query(case_num, ++query_num, Stree,
                            OPEN, string("a"), OPEN,string("aabbccdd"),
                            1);
            delete(Stree);

        }
};

int main(){
    int test_num = 0;
    Test test;
    // you can change the size to do the test. But the if the size is too large, the program may be killed by system.
    test.test_integer(++test_num,10000000);
    test.test_float(++test_num, 6000000);
    test.test_string(++test_num,20000000);
    cout <<endl<<"all test case passed."<<endl;
    return EXIT_SUCCESS;
}
