/*************************************************************
 * File: pqueue-heap.cpp
 * Author: Francisco Lumbreras
 * Implementation file for the HeapPriorityQueue
 * class. Memory management is done via usage of standard array
 */
 
#include "pqueue-heap.h"
#include "error.h"

const int Size0=10;

/*************************************************************
// Method: Constructor
// -----------------------------------------------------------
// Usage: HeapListPriorityQueue queue;
//
*************************************************************/ 
HeapPriorityQueue::HeapPriorityQueue() {

	//Allocate default number of spaces, mark as empty and initialise pointer
	numallocated=Size0;
	numused=0;
	values=new string[numallocated];
	
}

/*************************************************************
// Method: Destructor
// -----------------------------------------------------------
// Usage: queue.~HeapPriorityQueue();
//
*************************************************************/ 
HeapPriorityQueue::~HeapPriorityQueue() {

	delete[] values;
	
}

/*************************************************************
// Method: Size
// -----------------------------------------------------------
// Usage: int n=queue.size();
// -----------------------------------------------------------
// Detail: Will return number of stored elements as integer
*************************************************************/ 
int HeapPriorityQueue::size() {

	return numused;

}

/*************************************************************
// Method: IsEmpty
// -----------------------------------------------------------
// Usage: queue.isEmpty();
// -----------------------------------------------------------
// Detail: Returns true if queue is empty and false otherwise
*************************************************************/ 
bool HeapPriorityQueue::isEmpty() {

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
void HeapPriorityQueue::enqueue(string value) {

	//add memory if required
	if(numused==numallocated)
	{increasespace();}

	//add element
	values[numused]=value;
	numused+=1;

	//sort alphabetically against parents
	bubbleup(numused-1);


}

/*************************************************************
// Method: Peek
// -----------------------------------------------------------
// Usage: string str=queue.peek();
// -----------------------------------------------------------
// Detail: Returns first alphabetically sorted string without
// removing it.
*************************************************************/ 
string HeapPriorityQueue::peek() {
	
	if(isEmpty()){error("Queue is empty!");}

	return values[0];
}


/*************************************************************
// Method: DequeueMin
// -----------------------------------------------------------
// Usage: string str=queue.dequeueMin();
// -----------------------------------------------------------
// Detail: Extracts and removes first alphabetically sorted
// element. Throws error if empty. 
*************************************************************/ 
string HeapPriorityQueue::dequeueMin() {
	if(isEmpty()){error("Queue is empty!");}

	string first=peek();

	string last=values[numused-1];
	// reduce number of used elements
	numused--;
	// bring last element to top
	values[0]=last;

	// sort down the new top element comparing with its children
	bubbledown(0);

	return first;
}


/*************************************************************
// Method: Increasespace
// -----------------------------------------------------------
// Usage: queue.increasespace();
// -----------------------------------------------------------
// Detail: Allocates more memory space when it is required.
// A similar method to deallocate memory if multiple elements are
// extracted has not been included but would follow same principles.
*************************************************************/ 
void HeapPriorityQueue::increasespace()
{
	numallocated*=2;
	string *values2=new string[numallocated];

	// copy values
	for(int i=0;i<numused;i++)
	{
		values2[i]=values[i];
	}
	// delete old array
	delete[] values;

	//update pointer
	values=values2;

}


/*************************************************************
// Method: Bubbledown
// -----------------------------------------------------------
// Usage: queue.bubbledown(int pos);
// -----------------------------------------------------------
// Detail: Brings down updated top element comparing it to its
// children. Notice the recursive implementation
*************************************************************/ 
void HeapPriorityQueue::bubbledown(int pos)
{
	// Locate the children
	int childleft=2*(pos+1)-1;
	int childright=2*(pos+1);

	//if out of range I am done
	if(childleft>numused-1) return;

	//Choose the minimum children
	int minchild;
	if(childright<=numused-1&&(values[childright]<values[childleft]))
		minchild=childright;
	else
	{
		minchild=childleft;
	}

	//swap elements if applicable
	if(values[pos]>values[minchild])
	{
		string aux=values[pos];
		values[pos]=values[minchild];
		values[minchild]=aux;

		//recursive function call
		bubbledown(minchild);
	}


}

/*************************************************************
// Method: Bubbleup
// -----------------------------------------------------------
// Usage: queue.bubbleup(int pos);
// -----------------------------------------------------------
// Detail: Brings up the latest included element comparing it to its
// parents. Notice the recursive implementation
*************************************************************/ 
void HeapPriorityQueue::bubbleup(int pos)
{
	// when head is reached, all done!
	if(pos!=0)
	{
	//locate parent
	int parent=int((pos+1)/2)-1;
	// swap if applicable
	if(values[pos]<values[parent])
	{
		string aux=values[pos];
		values[pos]=values[parent];
		values[parent]=aux;
		// Recursive call
		bubbleup(parent);
	}
	}

}





