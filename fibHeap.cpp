#include<iostream>
#include<cmath>
#include<string>
#include<stdlib.h>
#include<vector>
#include "fibHeap.h"
using namespace std;
FibHeap::FibHeap()
{
	max=NULL; 
	numNodes=0;
}
node* FibHeap::getNode(string hashtag,int freq)
{
	node* newNode=new node();
        newNode->degree=0;
	newNode->hashtag=hashtag;
        newNode->frequency=freq;
        newNode->child=NULL;
	newNode->left=newNode;				// left pointer points to itself
	newNode->right=newNode;				// right pointer points to itslef
        newNode->parent=NULL;
        newNode->childCut=false;
	return newNode;
}
void FibHeap::insert(node* newNode)
{
	if(max==NULL) 					// heap is just starting
		max=newNode;
	else
	{
		// insert in top level list
		node *next=max->right;
		max->right=newNode;
		newNode->left=max;
		newNode->right=next;
		next->left=newNode;
	}
	if(newNode->frequency > max->frequency)		// update max pointer
		max=newNode;
	numNodes++;					// increment number of nodes in heap
}

void FibHeap::increaseKey(node *ptr,int c)
{
	// increase frequency of node pointer to by ptr by c
	ptr->frequency+=c;
	// if ptr is not root and new frequency of ptr is more than its parent
	if(ptr->parent!=NULL && ptr->frequency>ptr->parent->frequency)
	{
		node *parent=ptr->parent; 		// need to store this because after cut this node will be at the top level list with parent pointer NULL
		cut(ptr);
		cascadingCut(parent);
	}
	// if after increase, current freq > max frequency, update max pointer
	if(ptr->frequency > max->frequency)
		max=ptr;
}
void FibHeap::cut(node *ptr)
{
	// if single child
	if(ptr->parent->degree==1)
	{
		ptr->parent->child=NULL;
		ptr->parent->degree--;
	}
	else
	{
		// remove current node from list at this level
		node *left=ptr->left;
		node *right=ptr->right;
		left->right=right;
		right->left=left;
		if(ptr->parent->child == ptr) 		// if ptr is the node the parent points to as the child
			ptr->parent->child=left;
		ptr->parent->degree--;
	}
	// parents child cut value will be examined/changed in cascadingCut method
	// add to top level list
	node *next=max->right;
	max->right=ptr;
	ptr->left=max;
	ptr->right=next;
	next->left=ptr;
	ptr->childCut=false; 				// since this has become root node, childCut should be undef/false
	ptr->parent=NULL;
}	
void FibHeap::cascadingCut(node *ptr)
{
	// if reached root node, no need to continue
	if(ptr->parent!=NULL)				// if reached a node which is not root
	{	
		if(ptr->childCut==false) 		// if childCut is false, this ptr is losing its first child, make childCut true; nothing else to do
			ptr->childCut=true;
		else 					// if childCut was true
		{
			node *parent=ptr->parent; 	// need to store this because after cut, this node will be at the top level list
			cut(ptr);
			cascadingCut(parent);
		}
	}
}
node* FibHeap::removeMax()
{
	node *ptr=max; 					// saving pointer to max node, to return it at the end
	if(max!=NULL)
	{
		node *leftSibling=max->left;
		node *rightSibling=max->right;
		if(leftSibling!=max) 			// not only a single tree in the heap
		{
			node *curChild=max->child;
			if(curChild!=NULL) 		// if atleast one child
			{
				node *firstChild=max->child; 
				node *lastChild=firstChild->left;
				// insert children list as it is by changing the following
				// right pointer of left sibling to firstChild
				// left pointer of firstChild to leftSibling
				// left pointer of right sibling to lastChild
				// right pointer of lastChild to rightSibling
				leftSibling->right=firstChild;
				firstChild->left=leftSibling;
				rightSibling->left=lastChild;
				lastChild->right=rightSibling;
				// reset ChildCut and parent pointer values for all children
				for(int i=max->degree;i>0;i--)
				{
					curChild->childCut=false; 	
					curChild->parent=NULL;
					curChild=curChild->right;
				}
			}
			else
			{
				leftSibling->right=rightSibling;
				rightSibling->left=leftSibling;
			}
			max=rightSibling;		
		}
		else 		// single tree in heap, reset parent pointers and childCut for children, remove the max node and reassign max
		{
			node* curChild=max->child;
			node* firstChild=max->child;
			if(curChild!=NULL) 	//atleast one child
			{
				do
				{	
					curChild->parent=NULL;
					curChild->childCut=false;
					curChild=curChild->right;
				}while(curChild!=firstChild);
			}
			max=curChild;
		}
		pairwiseCombine();
	}	
	else 						// heap is empty
	{
		cout<<"Cant do removeMax() from an empty heap"<<endl;	
	}
	numNodes>0?numNodes--:1; 			// cant reduce numNodes if there werent any nodes in the heap before
	// resetting values for the max node that will be returned
	ptr->degree=0;
	ptr->child=NULL;
	ptr->left=ptr;
	ptr->right=ptr;
	ptr->parent=NULL;
	ptr->childCut=false;
	return ptr;
}
void FibHeap::pairwiseCombine()
{
	float phi=((1.0+sqrt(5))/(2.0));
	int len=ceil(log((float)numNodes)/log(phi));	// log function returns natural log, converting it to log base phi
	node **arr=new node*[len]; 			// array of pointers to structure node
	int i;
	for(i=0;i<len;i++)
		arr[i]=NULL;
	
	if(max!=NULL) 					// if no node exists in the heap - nothing more to do
	{
		// vector to iterate over all the root nodes
		vector<node*> allNodes;

		node* curNode=max;
		do{
			allNodes.push_back(curNode);
			curNode=curNode->right;
		}while(curNode!=max);

		for(vector<node*>::iterator cur = allNodes.begin() ; cur != allNodes.end(); ++cur)
		{
			curNode=*cur;
			node* parentNode=curNode;
			int degree=curNode->degree;
			while(arr[degree]!=NULL) 	// while a node exists with degree curNode->degree
			{
				node *otherNode = arr[degree];
				if(otherNode->frequency > curNode->frequency)
				{
					parentNode=otherNode;
					move(otherNode,curNode);	// curNode becomes child of otherNode
				}
				else
				{
					parentNode=curNode;
					move(curNode,otherNode);	// otherNode becomes child of curNode
				}
				arr[degree]=NULL; 	// as there wont exist a node with this degree anymore(note that the new parents degree has incresed by 1)
				degree++;
				curNode=parentNode; 	// because whoever is the new parent will be compared next
			}
			arr[degree]=parentNode;
		}
	}
	//reset max
	max=NULL; 
	for(i=0;i<len;i++)
	{
		if(arr[i]!=NULL)
		{
			if(max==NULL)
				max=arr[i];
			else if(arr[i]->frequency > max->frequency)
				max=arr[i];
		}
	}
}
void FibHeap::move(node *otherNode, node *curNode)
{
	// curNode becomes child of otherNode
	// remove curNode from top level list	
	curNode->left->right=curNode->right;	
	curNode->right->left=curNode->left;
	// set curNode left and right pointers to itself
	curNode->left=curNode;
	curNode->right=curNode;
	curNode->childCut=false; 
	if(otherNode->child==NULL) 			// if prospective parent had no child
	{
		otherNode->child=curNode;
		curNode->parent=otherNode;
	}
	else
	{
		node* firstChild=otherNode->child;
		node* secondChild=otherNode->child->right;
		// insert new node on the right of first child
		firstChild->right=curNode;
		curNode->left=firstChild;
		curNode->right=secondChild;
		secondChild->left=curNode;
		curNode->parent=otherNode;
	}
	otherNode->degree++;
}
