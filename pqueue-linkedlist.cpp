/*************************************************************
 * File: pqueue-linkedlist.cpp
 * Author: Francisco Lumbreras
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"

/*************************************************************
// Method: Constructor
// -----------------------------------------------------------
// Usage: LinkedListPriorityQueue queue;
//
*************************************************************/ 

LinkedListPriorityQueue::LinkedListPriorityQueue() {
	//assign NULL pointer to head
	head=NULL;
}

/*************************************************************
// Method: Destructor
// -----------------------------------------------------------
// Usage: queue.~LinkedListPriorityQueue();
//
*************************************************************/ 

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	// Memory management. Erase elements one by one.	
	while(head!=NULL)
	{deleteFirst();
	}
}

/*************************************************************
// Method: Size
// -----------------------------------------------------------
// Usage: int n=queue.size();
// -----------------------------------------------------------
// Detail: Will return number of stored elements as integer
*************************************************************/ 
int LinkedListPriorityQueue::size() {
	if(head==NULL) return 0;

	//Use pointer cursor to count number of nodes
	int count=1;
	Node *point=head;
	while(point->next!=NULL)
	{
		count++;
		point=point->next;
	}
	return count;
}

/*************************************************************
// Method: IsEmpty
// -----------------------------------------------------------
// Usage: queue.isEmpty();
// -----------------------------------------------------------
// Detail: Returns true if queue is empty and false otherwise
*************************************************************/ 
bool LinkedListPriorityQueue::isEmpty() {
	
	return (head==NULL);
}

/*************************************************************
// Method: Enqueue
// -----------------------------------------------------------
// Usage: queue.enqueue(string str);
// -----------------------------------------------------------
// Detail: Add string to vector. Memory management is done by
// the Vector class.
*************************************************************/ 
void LinkedListPriorityQueue::enqueue(string value) {
	Node *point =new Node;
	point->val=value;
	point->next=NULL;

	//Add node at beginning (empty list or first item in order introduced)
	if(head==NULL||head->val>value){
		point->next=head;
		head=point;
	}// else add position where corresponds or end of list!
	else{
		Node *cursor=head;
		while(cursor->next!=NULL&&cursor->next->val<value)
		{cursor=cursor->next;
		}
		point->next=cursor->next;
		cursor->next=point;
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
string LinkedListPriorityQueue::peek() {
	
	if(head==NULL) error("Queue is empty!");
	return head->val;
}

/*************************************************************
// Method: DequeueMin
// -----------------------------------------------------------
// Usage: string str=queue.dequeueMin();
// -----------------------------------------------------------
// Detail: Extracts and removes first alphabetically sorted
// element. Throws error if empty. 
*************************************************************/ 
string LinkedListPriorityQueue::dequeueMin() {
	if(head==NULL) error("Queue is empty!");
	string str=head->val;
	//Pointer to new head
	Node *point;
	point=head->next;
	//clear first element
	delete head;
	//assign new head
	head=point;
	return str;
	
}


/*************************************************************
// Method: deleteFirst()
// -----------------------------------------------------------
// Usage: queue.deleteFirst();
// -----------------------------------------------------------
// Detail:  Removes first alphabetically sorted
// element. Equivalent to dequeueMin but returns void. 
*************************************************************/ 
void LinkedListPriorityQueue::deleteFirst(){
	if(head!=NULL)
	{
		Node *point;
		point=head->next;

		delete head;

		head=point;

	}
}


