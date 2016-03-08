#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

// used as parameters passed into SegmentTree::query() function
// seach range from [0,100) can be presented as  query(CLOSE,0,OPEN,100)
typedef int search_range_signal;
#define OPEN    1
#define CLOSE   0
#define ENDLESS -1

// used by SegmentTree::whether_in_range(), to judge the relation of the query range and the node range
// eg: node.min = 0, node.max=100,  search range is (-1,101), then return OVERLAP_RANGE
typedef int range_signal;
#define OVERLAP_RANGE 1
#define PARTIALLY_IN_RANGE 0
#define OUT_OF_RANGE -1

// define the type of vector length, index, etc. long int is enough in this case.
#define max_size_of_vector 100000000
typedef long int m_long_int;

// template for SegmentTreeNode
template<class T>
struct SegmentTreeNode{
    T min;
    T max;
    m_long_int index_start;
    m_long_int index_end;
    SegmentTreeNode* left;
    SegmentTreeNode* right;

    // construct a new node from a vector element
    SegmentTreeNode(const T value, m_long_int index){
        this->min = value;
        this->max = value;
        this->index_start = index;
        this->index_end = index;
        this->left = nullptr;
        this->right = nullptr;
    };

    // construct a father node from single child node
    SegmentTreeNode(SegmentTreeNode* const left_child){
        this->min = left_child->min;
        this->max = left_child->max;
        this->index_start = left_child->index_start;
        this->index_end = left_child->index_end;
        this->left = left_child;
        this->right = nullptr;
    };

    //construct a father node from two children 
    SegmentTreeNode(SegmentTreeNode* const left_child, SegmentTreeNode* const right_child){
        this->min = left_child->min < right_child->min? left_child->min : right_child->min;
        this->max = left_child->max > right_child->max? left_child->max: right_child->max;
        this->index_start = left_child->index_start;
        this->index_end = right_child->index_end;
        this->left = left_child;
        this->right = right_child;
    };
};

// template for SegmentTree class
template<class T>
class SegmentTree{
    private:
        SegmentTreeNode<T>* root;
    public:
        // construct from vector with the same type, make it a balance tree.
        // use queue to store node, level by level. Until only one node left in the queue, tree is established.
        SegmentTree(vector<T> const &x){
            queue<SegmentTreeNode<T> *> q;
            for(auto i = x.begin(); i!=x.end();i++){
                SegmentTreeNode<T>* node = new SegmentTreeNode<T>(*i, i-x.begin());
                q.push(node);
            }
            while(q.size()>1){
                m_long_int size = q.size();
                while(size>1){
                    SegmentTreeNode<T>* left_child = q.front();
                    q.pop();
                    SegmentTreeNode<T>* right_child = q.front();
                    q.pop();
                    SegmentTreeNode<T>* node = new SegmentTreeNode<T>(left_child, right_child);
                    q.push(node);
                    size -=2;
                }
                if(1 == size){
                    SegmentTreeNode<T>* left_child = q.front();
                    q.pop();
                    SegmentTreeNode<T>* node = new SegmentTreeNode<T>(left_child);
                    q.push(node);
                }
            }
            this->root = q.empty()?nullptr:q.front();
        };

        // delete all nodes once the tree is destroyed.
        ~SegmentTree(){
            if(!this->root){
                return;
            }
            queue<SegmentTreeNode<T> *> q;
            q.push(this->root);
            while(!q.empty()){
                SegmentTreeNode<T> *node = q.front();
                q.pop();
                if(node->left) q.push(node->left);
                if(node->right) q.push(node->right);
                delete(node);
            }
            //cout <<"destroyed";
        }

        // find the number of objects that lies in arange of values
        // input example: get object >= 1000   -->  [1000, +endless)  --> query(CLOSE,1000,ENDLESS,0)
        //                get object >1000 and <= 2000  --> (1000,2000]  --> query(OPEN,1000,CLOSE,2000)
        // output: m_long_int, number of objects that satisfy the query.
        m_long_int query(search_range_signal left_signal, T const &left_value, search_range_signal right_signal, T const & right_value){
            if(!this->root){
                return 0;
            }
            if(!check_range_valid(left_signal, left_value, right_signal, right_value)){
                return 0;
            }
            queue<SegmentTreeNode<T> *> q;
            q.push(this->root);
            m_long_int count = 0;
            while(!q.empty()){
                SegmentTreeNode<T> * node = q.front();
                q.pop();
                switch(whether_in_range(node->min, node->max, left_signal,left_value,right_signal,right_value)){
                    case OVERLAP_RANGE:
                        count += node->index_end - node->index_start+1;
                        break;
                    case OUT_OF_RANGE:
                        break;
                    case PARTIALLY_IN_RANGE:
                        if(node->left){
                            q.push(node->left);
                        }
                        if(node->right){
                            q.push(node->right);
                        }
                        break;
                    default:
                        break;
                }
            }
            return count;
        };

        // check if the range is valid. static function, independent with SegmentTree objects.
        // input: the same as input in query. eg: [100,100] is valid. [100,100) is not valid.
        // output: bool, is valid
        static bool check_range_valid(search_range_signal left_signal, T const &left_value, search_range_signal right_signal, T const & right_value){
            if(left_signal == ENDLESS || right_signal == ENDLESS){
                return true;
            }
            if(left_value < right_value){
                return true;
            }
            else if(left_value > right_value){
                return false;
            }

            if(left_signal == CLOSE && right_signal == CLOSE){
                return true;
            }
            return false;
        }

        // judge the relation between search range and the node range.
        // eg: search range [10,20], node min and max is [11,19], then return OVERLAP_RANGE
        //     search range [10,20), node min and max is [20,30], the return OUT_OF_RANGE
        //     search range [10,20], node min and max is [15,100], then return PARTIALLY_IN_RANGE
        static range_signal whether_in_range(T const &min, T const &max, search_range_signal left_signal, T const &left_value, search_range_signal right_signal, T const &right_value){
            //in range
            bool left_overlap = (left_signal == ENDLESS) ||
                                (left_signal == OPEN && left_value < min) ||
                                (left_signal == CLOSE && left_value <= min);
            bool right_overlap =(right_signal == ENDLESS) ||
                                (right_signal == OPEN && right_value > max) ||
                                (right_signal == CLOSE && right_value >= max);
            if(left_overlap && right_overlap){
                return OVERLAP_RANGE;
            }

            bool out_of_range = (right_signal == OPEN && right_value <= min) ||
                                (right_signal == CLOSE && right_value < min) ||
                                (left_signal == OPEN && left_value >= max) ||
                                (left_signal == CLOSE && left_value > max);
            if(out_of_range){
                return OUT_OF_RANGE;
            }
            return PARTIALLY_IN_RANGE;
        }

        // override the << operator.
        friend ostream& operator<<(ostream& os, SegmentTree<T> const &tree){
            if(!tree.root){
                os<<"nullptr" << endl;
            }
            else{
                queue<SegmentTreeNode<T> *> q;
                q.push(tree.root);
                while(!q.empty()){
                    m_long_int size = q.size();
                    while(size > 0){
                        SegmentTreeNode<T>* node = q.front();
                        q.pop();
                        os << "min:"<<node->min << " max:"<<node->max << " [" << node->index_start <<","<<node->index_end <<"]\t";
                        if(node->left) q.push(node->left);
                        if(node->right) q.push(node->right);
                        size--;
                    }
                    os << endl<< endl;
                }
            }
        };
};
