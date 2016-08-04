/*************************************************************
 * File: pqueue-vector.cpp
 * Author: Francisco Lumbreras
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"


/*************************************************************
// Method: Constructor
// -----------------------------------------------------------
// Usage: VectorPriorityQueue queue;
//
*************************************************************/ 

VectorPriorityQueue::VectorPriorityQueue() {

}

/*************************************************************
// Method: Destructor
// -----------------------------------------------------------
// Usage: queue.~VectorPriorityQueue();
//
*************************************************************/ 

VectorPriorityQueue::~VectorPriorityQueue() {

}

/*************************************************************
// Method: Size
// -----------------------------------------------------------
// Usage: int n=queue.size();
// -----------------------------------------------------------
// Detail: Will return number of stored elements as integer
*************************************************************/ 

int VectorPriorityQueue::size() {
	
	return values.size();
}

/*************************************************************
// Method: Isempty
// -----------------------------------------------------------
// Usage: queue.isempty();
// -----------------------------------------------------------
// Detail: Returns true if queue is empty and false otherwise
*************************************************************/ 

bool VectorPriorityQueue::isEmpty() {
	
	return (values.size()==0);
}

/*************************************************************
// Method: Enqueue
// -----------------------------------------------------------
// Usage: queue.enqueue(string str);
// -----------------------------------------------------------
// Detail: Add string to vector. Memory management is done by
// the Vector class.
*************************************************************/ 

void VectorPriorityQueue::enqueue(string value) {

	values+=value;
}

/*************************************************************
// Method: Peek
// -----------------------------------------------------------
// Usage: string str=queue.peek();
// -----------------------------------------------------------
// Detail: Returns first alphabetically sorted string without
// removing it.
*************************************************************/ 

string VectorPriorityQueue::peek() {

	string res;
	if (isEmpty()) {
        error("Queue is empty!");
	}
	
	res=values[0];
	for (int i = 1; i < values.size(); i++) {
        if (values[i] < res) {
            res = values[i];
        }
        
    }
    return res;

}

/*************************************************************
// Method: DequeueMin
// -----------------------------------------------------------
// Usage: string str=queue.dequeueMin();
// -----------------------------------------------------------
// Detail: Extracts and removes first alphabetically sorted
// element. Throws error if empty. 
*************************************************************/ 

string VectorPriorityQueue::dequeueMin() {

	string res;
	if (isEmpty()) {
		error("Queue is empty!");
	}
	
	// First locate the minimum by inspection
	int minind=0;
	res=values[minind];
	for (int i = 1; i < values.size(); i++) {
        if (values[i] < res) {
            res = values[i];
			minind=i;
        }
        
    }
	// Remove element with exeption of this being the only element remaining.
	if(size()==1)
	{values.clear();
	}else
	{
	values.removeAt(minind);
	}
    return res;
}
