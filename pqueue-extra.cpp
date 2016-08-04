/*************************************************************
 * File: pqueue-extra.cpp
 * Author: Francisco Lumbreras
 * Implementation file for the ExtraPriorityQueue class.  You
 * do not need to implement this class, but we're happy to
 * give you extra credit if you do!
 */
 
#include "pqueue-extra.h"
#include "error.h"
#include <iostream>

/*************************************************************
// Method: Constructor
// -----------------------------------------------------------
// Usage: ExtraPriorityQueue queue;
//
*************************************************************/ 
ExtraPriorityQueue::ExtraPriorityQueue() {
	head=NULL;
}

/*************************************************************
// Method: Destructor
// -----------------------------------------------------------
// Usage: queue.~ExtraPriorityQueue();
//
*************************************************************/
ExtraPriorityQueue::~ExtraPriorityQueue() {
	//recursively delete all leaves
	deleteLeaf(head);
}

/*************************************************************
// Method: Size
// -----------------------------------------------------------
// Usage: int n=queue.size();
// -----------------------------------------------------------
// Detail: Will return number of stored elements as integer
*************************************************************/
int ExtraPriorityQueue::size() {
	//recursively count all leaves
	return countLeaves(head);
}

/*************************************************************
// Method: IsEmpty
// -----------------------------------------------------------
// Usage: queue.isEmpty();
// -----------------------------------------------------------
// Detail: Returns true if queue is empty and false otherwise
*************************************************************/ 
bool ExtraPriorityQueue::isEmpty() {
	
	return (size()==0);
}


/*************************************************************
// Method: Enqueue
// -----------------------------------------------------------
// Usage: queue.enqueue(string str);
// -----------------------------------------------------------
// Detail: Add string to vector. Memory management is done by
// the Vector class.
*************************************************************/
void ExtraPriorityQueue::enqueue(string value) {
	if(head==NULL)//is it empty? then add head node
	{ 
		Node* newnode=new Node;
		newnode->val=value;
		newnode->left=NULL;
		newnode->right=NULL;
		head=newnode;
	}else
	{
		//scan the tree in search of an insertion point and
		//add the value
		insertValue(head,value);
	}
}

/*************************************************************
// Method: Peek
// -----------------------------------------------------------
// Usage: string str=queue.peek();
// -----------------------------------------------------------
// Detail: Returns first alphabetically sorted string without
// removing it.
*************************************************************/ 
string ExtraPriorityQueue::peek() {
	if(head==NULL) {error("Queue is empty!");}
	return recursivepeek(head);
	
}

/*************************************************************
// Method: DequeueMin
// -----------------------------------------------------------
// Usage: string str=queue.dequeueMin();
// -----------------------------------------------------------
// Detail: Extracts and removes first alphabetically sorted
// element. Throws error if empty. 
*************************************************************/ 
string ExtraPriorityQueue::dequeueMin() {
	
	if(head==NULL) {error("Queue is empty!");}

	// notice the use of pointer cursor here as it is more robust for 
	// insertion of big amounts of data.
	return scandequeue(head,NULL);
}

/*************************************************************
// Method: deleteLeaf
// -----------------------------------------------------------
// Usage: queue.deleteLeaf(Node *leaf);
// -----------------------------------------------------------
// Detail: Removes the node specified by the pointer leaf.
// Recursively deletes all tree.
*************************************************************/ 
void ExtraPriorityQueue::deleteLeaf(Node *leaf){

	if(leaf!=NULL)
	{
		deleteLeaf(leaf->left);
		deleteLeaf(leaf->right);
		delete leaf;
	}
}

/*************************************************************
// Method: countLeaves
// -----------------------------------------------------------
// Usage: int n=queue.countLeaves(Node *leaf);
// -----------------------------------------------------------
// Detail: Recursively counts how many nodes are present in
// the tree
*************************************************************/ 
int ExtraPriorityQueue::countLeaves(Node *leaf){

	if(leaf==NULL) return 0;
	else{
		return 1+countLeaves(leaf->left)+countLeaves(leaf->right);
	}
}

/*************************************************************
// Method: InsertValue
// -----------------------------------------------------------
// Usage: queue.insertValue(Node *leaf, string value);
// -----------------------------------------------------------
// Detail: Inserts the string value into the tree whose head is
// referenced by the pointer leaf. leaf cannot be NULL, which is
// handled by the public non recursive method Enqueue
*************************************************************/ 
void ExtraPriorityQueue::insertValue(Node * leaf,string value)
{ 
	//cursor to scan tree
	Node *cursor=leaf;
	//sentinel to indicate position found
	bool searching=true;

	while(searching)
	{
		//if lower go to left
		if(value<cursor->val)
		{
			//is the tree populated? then continue searching
			if(cursor->left!=NULL)
			{cursor=cursor->left;}
			else
			{
				//found position, insert value.
				searching=false;
				cursor->left=new Node;
				cursor->left->val=value;
				cursor->left->left=NULL;
				cursor->left->right=NULL;
			}
			//else go to right.
		}else if(value>=cursor->val)
		{
			//is populated? continue scanning with cursor
			if(cursor->right!=NULL)
			{cursor=cursor->right;}
			else
			{
				//found position, insert new node with value
				searching=false;
				cursor->right=new Node;
				cursor->right->val=value;
				cursor->right->left=NULL;
				cursor->right->right=NULL;
			}
		}
	}
}
	
/*************************************************************
// Method: InsertValueRecursively
// -----------------------------------------------------------
// Usage: queue.insertValueRecursively(Node * &leaf, string &value);
// -----------------------------------------------------------
// Detail: Inserts the string value into the tree using recursion from the
// leaf indicated by the pointer received. Leaf cannot be NULL, which is
// handled by the public non recursive method Enqueue
*************************************************************/ 
void ExtraPriorityQueue::insertValueRecursively(Node * &leaf,string &value)
{
	//if lower, go to left
	if(value<(leaf->val))
	{
		// if there are more leaves, go on iterating
		if(leaf->left!=NULL)
		{insertValue(leaf->left,value);}
		else{
			// add new node to left
			leaf->left=new Node;
			leaf->left->val=value;
			leaf->left->left=NULL;
			leaf->left->right=NULL;

		}
		// else go to right
	}else if (value>=(leaf->val))
	{
		// if tree has more leaves, keep seeking
		if(leaf->right!=NULL)
		{insertValue(leaf->right,value);}
		else{
			//add new node to right
			leaf->right=new Node;
			leaf->right->val=value;
			leaf->right->left=NULL;
			leaf->right->right=NULL;

		}
	}
	
}

/*************************************************************
// Method: Recursivepeek
// -----------------------------------------------------------
// Usage: string str= queue.recursivepeek(Node *leaf);
// -----------------------------------------------------------
// Detail: Peeks at the first alphabetically sorted element
// and returns it by using recursion. Leaf cannot be NULL, this
// check is done at the public method peek
*************************************************************/ 
string ExtraPriorityQueue::recursivepeek(Node *leaf)
{
	//if there are no more leaves to left, return value
	if(leaf->left==NULL)
	{
		return leaf->val;
	}else //keep searching
	{return recursivepeek(leaf->left);}
}

/*************************************************************
// Method: Recursivedequeue
// -----------------------------------------------------------
// Usage: string str= queue.recursivedequeue(Node *leaf, Node *previous);
// -----------------------------------------------------------
// Detail: Returns the first alphabetically sorted element
// and removes it by using recursion, note how the rest of the tree needs
// to be rearranged. Leaf cannot be NULL, this
// check is done at the public method dequeueMin
*************************************************************/ 
string ExtraPriorityQueue::recursivedequeue(Node * leaf,Node *previous)
{
	string aux;
	// if nothing more to left, assign value to temporary container
	if(leaf->left==NULL)
	{
		aux=leaf->val;
		// am I at the head?
		if(leaf==head)
		{
			//reassign head
			head=leaf->right;
			//clear memory
			delete leaf;
			
		}else //past the rest of the tree to the left of previous leaf
		{
			previous->left=leaf->right;
			//clear memory
			delete leaf;
		}
		return aux;
	
	}else //if there are more leaves to the left, keep searching for minimum
	{
		return recursivedequeue(leaf->left,leaf);
	}



}

/*************************************************************
// Method: Scandequeue
// -----------------------------------------------------------
// Usage: string str= queue.scandequeue(Node *leaf, Node *previous);
// -----------------------------------------------------------
// Detail: Returns the first alphabetically sorted element
// and removes it by using a pointer cursor to run along the tree,
// note how the rest of the tree needs to be rearranged.
// Leaf cannot be NULL, this check is done at the public method dequeueMin().
*************************************************************/ 
string ExtraPriorityQueue::scandequeue(Node * leaf,Node *previous)
{
	//container for value
	string aux;
	// cursor to scan tree
	Node *cursor=leaf;
	// cursor to point previous node
	Node *precursor=previous;
	//sentinel to indicate when valid position found
	bool searching=true;

	while(searching)
	{
		//nothing more to left, done 
		if(cursor->left==NULL)
		{
			aux=cursor->val;
			searching=false;
			//if at head, reassign to right
			if(cursor==head)
			{
				head=cursor->right;
				//clean memory
				delete cursor;
			
			}else
			{
				//paste the tree leaves bridging node deleted
				precursor->left=cursor->right;
				//clean memory
				delete cursor;
			}
			
	
		}else //if there is more tree to left, keep searching
		{
			// update cursor pointers
			precursor=cursor;
			cursor=cursor->left;
		}
	}
	return aux;



}


