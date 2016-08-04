/**********************************************
 * File: pqueue-extra.h
 * Author: Francisco Lumbreras
 * This file implements a priority queue by means of a binary tree in which 
 * strings are stored in alphaetic order following the rule: if lower to 
 * parent, store to left, else stored to right. The big drawback here is the
 * selection of the first element conditions the balance of the tree.
 *
 * Methods are implemented in both a recursive way as well as using a pointer 
 * cursor (like linked lists).
 */
#ifndef PQueue_Extra_Included
#define PQueue_Extra_Included

#include <string>
using namespace std;

class ExtraPriorityQueue {
public:
	/* Constructs a new, empty priority queue. */
	ExtraPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~ExtraPriorityQueue();
	
	/* Returns the number of elements in the priority queue. */
	int size();
	
	/* Returns whether or not the priority queue is empty. */
	bool isEmpty();
	
	/* Enqueues a new string into the priority queue. */
	void enqueue(string value);
	
	/* Returns, but does not remove, the lexicographically first string in the
	 * priority queue.
	 */
	string peek();
	
	/* Returns and removes the lexicographically first string in the
	 * priority queue.
	 */
	string dequeueMin();

private:
	
	// define the node structure
	typedef struct Node{
	string val;
	Node *left;
	Node *right;}Node;

	// head
	Node *head;


	// recursive deletion function
	void deleteLeaf(Node *leaf);

	//recursive counting function
	int countLeaves(Node *leaf);

	// insert value function - uses pointer cursor to travel along
	// the tree
	void insertValue(Node * leaf,string value);

	// recursive insert function.
	void insertValueRecursively(Node * &leaf,string &value);

	// recursive peek function
	string recursivepeek(Node *leaf);

	// recursive dequeue function 
	string recursivedequeue(Node * leaf,Node *previous);

	// dequeue function using pointer to node
	string scandequeue(Node * leaf,Node *previous);
};

#endif
