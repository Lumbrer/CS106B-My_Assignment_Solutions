/*
 * File: path.cpp
 * --------------
 * Name: Francisco Lumbreras
 * This file implements the path.h interface.
 */

/////////////////////////////////////////////////////////////////////
// This class could be easily implemented using a doubly linked    //
// that would allow access to initial elements for plotting and    //
// access to tail elements for extraction during Djikstra          //
// algorithm. A vector has been chosen for memory management and   //
// code simplification.
/////////////////////////////////////////////////////////////////////

#include "path.h"
#include "gpathfinder.h"

//Basic constructor, set cost to zero 
Path::Path() {
  Pcost = 0;
}

//Destructor, no need to delete any memory as the data
// has been passed by reference from the graph and must not be lost 
// during operations
Path::~Path() {
}

//Return cost
double Path::getCost() {
	return Pcost;
}

// function to produce a string that visually describes the path
string Path::toString() {
    string pString = "";
    if (ArcsVector.size()==0) return pString;
	//add start
    pString = ArcsVector[0]->start->name;
	//for all the arcs, add the finish node label
	foreach (Arc *arc in ArcsVector) {
        pString += "->";
        pString += arc->finish->name;
    }
	return pString;
}

//function to add arc to path and update cost. Note how it allows duplicates
void Path::addArc(Arc* arc) {
	ArcsVector.add(arc);
	Pcost += arc->cost;
}

//return size of Path (how many arcs?)
int Path::size()
{
	return ArcsVector.size();
}

// check if Path is empty
bool Path::isEmpty() {
	return ArcsVector.isEmpty();
}

// remove last element of the Path
void Path::removeLast(){
    Pcost = Pcost - (ArcsVector[size()-1]-> cost);
    ArcsVector.removeAt(size()-1);
}

// empty the Path
void Path::clear(){
    ArcsVector.clear();
	Pcost=0;
}
// return Path as a Vector of arcs
Vector<Arc *> Path::extractPath(){
    return ArcsVector;
}

// Peek at the last element without removing it.
Arc *Path::getLast() {
	return ArcsVector[size() - 1];
}

