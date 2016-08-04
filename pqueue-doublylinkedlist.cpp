/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 * Author: Francisco Lumbreras Arcos
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-doublylinkedlist.h"
#include "error.h"


/*************************************************************
// Method: Constructor
// -----------------------------------------------------------
// Usage: DoublyLinkedListPriorityQueue queue;
//
*************************************************************/ 
DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
	//Assign NULL pointers
	head=NULL;
	tail=NULL;
}

/*************************************************************
// Method: Destructor
// -----------------------------------------------------------
// Usage: queue.~DoublyLinkedListPriorityQueue();
//
*************************************************************/ 
DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
	while(head!=NULL)
	{deleteNode(head);
	}
}

/*************************************************************
// Method: Size
// -----------------------------------------------------------
// Usage: int n=queue.size();
// -----------------------------------------------------------
// Detail: Will return number of stored elements as integer
*************************************************************/ 
int DoublyLinkedListPriorityQueue::size() {

	if(head==NULL) return 0;

	// Use pointer cursor to run along list
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
bool DoublyLinkedListPriorityQueue::isEmpty() {
	// TODO: Fill this in!
	if(head==NULL)return true;

	else{
		return false;}
}

/*************************************************************
// Method: Enqueue
// -----------------------------------------------------------
// Usage: queue.enqueue(string str);
// -----------------------------------------------------------
// Detail: Add string to vector. Memory management is done by
// the Vector class.
*************************************************************/
void DoublyLinkedListPriorityQueue::enqueue(string value) {
	
	//Add new node to list. Nodes are not sorted in this occasion.
	// Hence these are added at the head by default.
	Node *newpointer= new Node;
	newpointer->val=value;

	if(head==NULL)
	{
		head=newpointer;
		tail=newpointer;
		head->next=NULL;
		head->previous=NULL;
	}else //no need to observe the tail in the case list is not empty.
	{
	head->previous=newpointer;
	newpointer->next=head;
	newpointer->previous=NULL;
	head=newpointer;
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
string DoublyLinkedListPriorityQueue::peek() {
	
	if(head==NULL) {error("The queue is empty!");}

	// Use a pointer cursor to find the first string and store it.
	Node *cursor=head;
	string candidate=cursor->val;
	while(cursor->next!=NULL)
	{
		if(cursor->next->val<candidate)
		{candidate=cursor->next->val;
		
		}
		cursor=cursor->next;
	}
	
	return candidate;
}

/*************************************************************
// Method: DequeueMin
// -----------------------------------------------------------
// Usage: string str=queue.dequeueMin();
// -----------------------------------------------------------
// Detail: Extracts and removes first alphabetically sorted
// element. Throws error if empty. 
*************************************************************/ 
string DoublyLinkedListPriorityQueue::dequeueMin() {
	
	if(head==NULL) error("The queue is empty!");

	// Use pointer cursor to find minimum and delete Nodde once found.
	Node *cursor=head;
	Node *Nodetodelete=head;
	string candidate=cursor->val;

	while(cursor->next!=NULL)
	{
		if(cursor->next->val<candidate)
		{candidate=cursor->next->val;
		Nodetodelete=cursor->next;
		
		}
		cursor=cursor->next;
	}

	deleteNode(Nodetodelete);
	return candidate;	
}

/*************************************************************
// Method: deleteNode
// -----------------------------------------------------------
// Usage: queue.deleteNode(Node *todelete);
// -----------------------------------------------------------
// Detail: Removes the node specified by the pointer todelete. 
*************************************************************/ 

void DoublyLinkedListPriorityQueue::deleteNode(Node *todelete) {

	// am I deleting the first (head) node?
	if (todelete->previous==NULL)
	{
		head=todelete->next;
		//was it the only node? if so update tail. Else just adjust
		// new head
		if(head==NULL)
		{tail=NULL;

		}else
		{head->previous=NULL;}
	// was it the tail node? Update tail then. 
	}else if(todelete->next==NULL)
	{
		todelete->previous->next=NULL;
		tail=todelete->previous;
	// default, it is an intermediate node.
	}else {
	
		// bridge node connections.
		todelete->previous->next=todelete->next;
		todelete->next->previous=todelete->previous;

	}
	// manage memory
	delete todelete;

}


