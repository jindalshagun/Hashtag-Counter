#include<string>
using namespace std;
struct node
{
	int degree;
	string hashtag;
	int frequency;
	node *child;
	node *left;
	node *right;
	node *parent;
	bool childCut;
};
class FibHeap{
	int numNodes;
	node *max;
	public:
		FibHeap();
		node* getNode(string hashtag,int freq); // create a node 
		void insert(node* newNode); 		// insert a node into the heap
		void cut(node *ptr); 
		void cascadingCut(node *ptr);
		void increaseKey(node *ptr,int c); 	// increase frequency of node given by ptr by amount c
		node* removeMax(); 			// remove the max element from the heap
		void pairwiseCombine(); 
		void move(node *ptr1,node *ptr2); 	// make ptr2 a child of ptr1
};
