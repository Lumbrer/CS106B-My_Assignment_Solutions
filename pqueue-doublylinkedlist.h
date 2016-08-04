/*******************************************************
 * File: pqueue-doublylinkedlist.h
 * Author: Francisco Lumbreras
 * A priority queue class backed by an unsorted, doubly-linked
 * list.
 */
#ifndef PQueue_DoublyLinkedList_Included
#define PQueue_DoublyLinkedList_Included

#include <string>
using namespace std;

/* A class representing a priority queue backed by an unsorted,
 * doubly-linked list.
 */
class DoublyLinkedListPriorityQueue {
public:
	/* Constructs a new, empty priority queue backed by a doubly-linked list. */
	DoublyLinkedListPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~DoublyLinkedListPriorityQueue();
	
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
	
	//Define struct for list node
	typedef struct Node{
	string val;
	Node *next;
	Node *previous;} Node;

	//Define pointers for head and tail.
	Node *head;
	Node *tail;

	//Method to delete a node. 
	void deleteNode(Node *todelete);
};

#endif
