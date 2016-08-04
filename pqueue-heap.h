/**********************************************
 * File: pqueue-heap.h
 * Author: Francisco Lumbreras
 * A priority queue class backed by a binary
 * heap.
 */
#ifndef PQueue_Heap_Included
#define PQueue_Heap_Included

#include <string>
using namespace std;

/* A class representing a priority queue backed by an
 * binary heap.
 */
class HeapPriorityQueue {
public:
	/* Constructs a new, empty priority queue backed by a binary heap. */
	HeapPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~HeapPriorityQueue();
	
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
	
	// Integers to store number of allocated and used memory blocks
	int numallocated;
	int numused;

	// Pointer to array of strings
	string *values;

	//method to allocate more space
	void increasespace();

	// Method to sort alphabetically from bottom
	void bubbleup(int childpos);

	//Method to sort alphabetically from top
	void bubbledown(int parentPos);

};

#endif
