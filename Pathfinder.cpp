/*
 * File: Pathfinder.cpp
 * --------------------
 * Name: Francisco Lumbreras 
 * This file is the starter project for the Pathfinder application
 * on Assignment #6.
 */
 

#include <iostream>
#include <string>
#include <fstream> 
#include <cmath>
#include <cctype>
#include "console.h"
#include "graphtypes.h"
#include "gpathfinder.h"
#include "graph.h"
#include "console.h"
#include "simpio.h" 
#include "strlib.h" 
#include "tokenscanner.h"
#include "pqueue.h"
#include "path.h"
#include "vector.h"

using namespace std;

/* Function prototypes */

void quitAction();
string requestFile();
void readMap(Graph<Node,Arc> &graph);
void plotMap(Graph<Node,Arc> &graph);
void doDijkstra(Graph<Node,Arc> &graph);
void doDijkstraShortcut(GPoint selection,Graph<Node,Arc> &graph);
void doKruskal(Graph<Node,Arc> &graph);
bool isclose(GPoint P1,GPoint P2);
double getPathCost(const Vector<Arc *> &path);
Path findShortestPath(Node *start,Node *finish);
Vector<Arc *> findShortestPathVector(Node *start,Node *finish);

/* Main program */

int main() {
   initPathfinderGraphics();

   //define graph variable
   Graph<Node,Arc> graph;
  

   //add buttons and specify callbacks
   addButton("Read Map", readMap,graph);
   addButton("Dijkstra", doDijkstra,graph);
   addButton("Kruskal", doKruskal, graph);
   addButton("Quit", quitAction);

   // add listener to trigger auto Dijkstra if click on a node. See how the callback takes 
   //a GPoint variable plus additional data (graph)
	defineClickListener(doDijkstraShortcut,graph);
   // Wait for click on button
	pathfinderEventLoop();

   return 0;
}



void quitAction() {
   exit(0);
}

// function to extract file name using keyboard only
string requestFile()
{
	string filename,readfile;
	cout << "Please enter the name of an existing file containing a Map to process: \n"<<endl;
	filename=getLine();
	//check for txt extension and add if not present
	if(filename.size()>4&&filename.substr(filename.size()-4,4)==".txt")
	{ readfile=filename;
	}else
	{
		readfile=filename+".txt";
	}
	return readfile;

}
//function to extract map data from formatted txt file
void readMap(Graph<Node,Arc> &graph)
{
	ifstream infile;
	TokenScanner scanner;
	//temporaty variables to store read data and generate components of graph
	string line, readfile, nodeS, nodeF;
	Node *newnode;
	Arc *newarc;
	GPoint location;
	double xCoord,yCoord,arccost;

	//clean existing content if applicable
	graph.clear();

	//get file name
	readfile=requestFile();
	infile.clear();
	infile.open(readfile.c_str());
	if(infile.fail())
		//try again if the file was invalid
		return readMap(graph);
	else 
	{
	  if (infile.good())
	  {
		//variable to specify which section of txt file is being scanned
		int section=0;
		while(getline(infile,line))
		{
			if(section==0)
			{
				drawPathfinderMap(line);
				section++;
			}
			if(section==1&&line=="NODES")
			{section++;}//skip NODES line
			if(line!="NODES"&&section==2&&line!="ARCS")//read nodes
			{
				scanner.setInput(line);
				scanner.ignoreWhitespace();
				newnode=new Node;
	
					newnode->name=scanner.nextToken();
					scanner.scanNumbers();// must be added to scan numbers with decimal places
					xCoord=stringToReal(scanner.nextToken());
					scanner.scanNumbers();
					yCoord=stringToReal(scanner.nextToken());

				location=GPoint(xCoord,yCoord);
				cout<<"X Coord: "<<xCoord<<" & Y coord"<<yCoord<<endl;
				newnode->loc=location;
				//add new node
				graph.addNode(newnode);
				cout<<"Stored node name:"<<newnode->name<<endl;
			}
			if(line=="ARCS"&&section==2)
			{section++;}//ignore line ARCS
			if(line!="ARCS"&&section==3)//read arcs
			{	
				scanner.setInput(line);
				scanner.ignoreWhitespace();

				nodeS=scanner.nextToken();
				nodeF=scanner.nextToken();
				scanner.scanNumbers();
				arccost=stringToReal(scanner.nextToken());
				cout<<"new arc cost read: "<<arccost<<endl;

				//take on the arc only if the nodes exist
				if(graph.nodeExists(nodeS)&&graph.nodeExists(nodeF))
				{
					newarc=new Arc;
					newarc->start=graph.getNode(nodeS);
					newarc->finish=graph.getNode(nodeF);
					newarc->cost=arccost;
					//add arc in direction 1
					graph.addArc(newarc);

					newarc=new Arc;
					newarc->start=graph.getNode(nodeF);
					newarc->finish=graph.getNode(nodeS);
					newarc->cost=arccost;
					//add arc in inverse direction
					graph.addArc(newarc);

					cout<<"Stored double arc between "<<nodeS<<" - "<<nodeF<<endl;


				}else
				{
					//report that dasta in file is not consistent and ignore the arc that is inconsistent
					cout<<"Attention! Arc data ignored due to missing Node: "<<nodeS<<" / "<<nodeF<<endl;
				}

			}		
		}
	  }

	  //plot the nodes and arcs
	  plotMap(graph);
	}	
}

//function to plot all the nodes and arcs on top of the map image file
void plotMap(Graph<Node,Arc> &graph)
{
	if(!graph.isEmpty())
	{
		foreach(Node *node in graph.getNodeSet())
		{
			drawPathfinderNode(node->loc, "DARK_GRAY", node->name);
		}
		foreach(Arc *arc in graph.getArcSet())
		{
			drawPathfinderArc(arc->start->loc, arc->finish->loc, "DARK_GRAY");
		}

	}

}

//function to trigger Dijkstra minimum distance search from button
// this implementation uses the class Path to store the result of the search and
// its methods to reach that solution
void doDijkstra(Graph<Node,Arc> &graph)
{
	//repaint the map in case previously modified
	plotMap(graph);
	Node *start=NULL;
	Node *goal=NULL;
	//Initialise Path variable
	Path MinPath=Path();
	GPoint selection;

	cout<< "The algorithm will now calculate the shortest route"<<endl;
	cout<<"Please click on start node"<<endl;

	while(start==NULL)
	{
		selection=getMouseClick();

		//check if click was close to any node
		foreach(Node *node in graph.getNodeSet())
		{
			if(isclose(selection,node->loc))
			{
				start=node;
				drawPathfinderNode(node->loc, "ORANGE", node->name);
				break;
			}

		}
		cout<<"Invalid click, please choose start Node"<<endl;

	}

	cout<<"Please clik on destination Node"<<endl;
	while(goal==NULL)
	{
		selection=getMouseClick();
		//Check if a node was selected
		foreach(Node *node in graph.getNodeSet())
		{
			if(isclose(selection,node->loc)&&node!=start)
			{
				goal=node;
				drawPathfinderNode(node->loc, "ORANGE", node->name);
				break;
			}

		}
		cout<<"Invalid click, please choose destination Node"<<endl;

	}
	
	cout<<"Finding shortes path from "<<start->name<<" to "<<goal->name<<endl;

	//call the Dijsktra routine
	MinPath=findShortestPath(start,goal);
	if(MinPath.getCost()>0)
	{
		//print path
	cout<<"Found the path with cost "<<MinPath.getCost()<<endl;
	cout << MinPath.toString() << " of Total Cost " << MinPath.getCost() <<endl;
	//highlight path on map display
	foreach(Arc* arc in MinPath.extractPath())
	{
		drawPathfinderNode(arc->start->loc, "ORANGE", arc->start->name);
        drawPathfinderNode(arc->finish->loc, "ORANGE", arc->finish->name);
        drawPathfinderArc(arc->start->loc, arc->finish->loc, "ORANGE");

	}
	}else
	{
		cout<<"Impossible to find a Path linking the nodes provided"<<endl;
		drawPathfinderNode(start->loc, "DARK_GRAY", start->name);
		drawPathfinderNode(goal->loc, "DARK_GRAY", goal->name);
	}


}

//function to trigger Dijkstra minimum distance search by clickling on a node at the home screen.
// This implementation uses the class Path to store the result of the search and
// its methods to reach that solution
void doDijkstraShortcut(GPoint selection,Graph<Node,Arc> &graph)
{
	plotMap(graph);
	Node *start=NULL;
	Node *goal=NULL;
	Path MinPath=Path();
	GPoint selection2;

	
	// check if click happened near any node
	foreach(Node *node in graph.getNodeSet())
	{
			if(isclose(selection,node->loc))
			{
				start=node;
				drawPathfinderNode(node->loc, "ORANGE", node->name);
				break;
			}

	}
	
	if(start!=NULL)
	{
	
		cout<< "The Dijkstra algorithm will now calculate the shortest route"<<endl;

	

	cout<<"Please clik on destination Node"<<endl;
	while(goal==NULL)
	{
		selection2=getMouseClick();

		foreach(Node *node in graph.getNodeSet())
		{
			if(isclose(selection2,node->loc)&&node!=start)
			{
				goal=node;
				drawPathfinderNode(node->loc, "ORANGE", node->name);
				break;
			}

		}

		cout<<"Invalid click, please choose destination Node"<<endl;

	}
	
	cout<<"Finding shortes path from "<<start->name<<" to "<<goal->name<<endl;
	
	MinPath=findShortestPath(start,goal);
	if(MinPath.getCost()>0)
	{
		//print path
	cout<<"Found the path with cost "<<MinPath.getCost()<<endl;
	cout << MinPath.toString() << " of Total Cost " << MinPath.getCost() <<endl;
	//highlight path on map display
	foreach(Arc* arc in MinPath.extractPath())
	{
		drawPathfinderNode(arc->start->loc, "ORANGE", arc->start->name);
        drawPathfinderNode(arc->finish->loc, "ORANGE", arc->finish->name);
        drawPathfinderArc(arc->start->loc, arc->finish->loc, "ORANGE");

	}
	}else
	{
		cout<<"Impossible to find a Path linking the nodes provided"<<endl;
		drawPathfinderNode(start->loc, "DARK_GRAY", start->name);
		drawPathfinderNode(goal->loc, "DARK_GRAY", goal->name);
	}
	
	}

}
//Function to produce the Kruskal minimum spanning tree on the graph as triggered by button on application
void doKruskal(Graph<Node,Arc> &graph)
{
	//rapint the map
	plotMap(graph);
	//PQueue to sort all arcs to extract them based on lowest cost first
	PriorityQueue<Arc *> SortedArcs;
	//Pointer to processed Arc
	Arc * arc2proc;
	//Tree with Result (set of Arcs picked)
	Vector<Arc *> Tree;
	//Map node to all the already reachable nodes as you iterate
	Map<string,Set<Node *> > nodesMap;

	//firstly populate the PQueue
	foreach (Arc * arc in graph.getArcSet())
	{
		SortedArcs.enqueue(arc,arc->cost);
	}

	//Initialise the Map, each node can only reach itself
	foreach (Node * node in graph.getNodeSet())
	{
		Set<Node *> set;
		set.add(node);
		nodesMap.put(node->name,set);
	}

	//until all arcs are processed...
	while(!SortedArcs.isEmpty())
	{
		//take lowest cost arc of remaining
		arc2proc=SortedArcs.dequeue();
		//if I can already link start and end using a path included ignore the arc, otherwise add it
		if(!nodesMap[arc2proc->start->name].contains(arc2proc->finish))
		{
			//add the arc
			Tree.add(arc2proc);
			//merge the reachable nodes from start and end without producing duplicity (taken care by Set class)
			Set<Node *> merged=nodesMap[arc2proc->start->name]+nodesMap[arc2proc->finish->name];
			// now all the nodes in the new merged set can access the rest of nodes.
			foreach(Node *node in merged)
			{
				nodesMap.put(node->name,merged);
			}

			cout << arc2proc->start->name <<"->" <<arc2proc -> finish -> name <<"with cost " <<arc2proc->cost << " added." << endl;

		}else
		{
			cout << arc2proc->start->name <<"->" <<arc2proc -> finish -> name <<"with cost " <<arc2proc->cost << " ignored." << endl;
		}

	}

	//compute total cost of the MST
	double TreeCost=0;
	foreach(Arc * arc in Tree)
	{
		drawPathfinderNode(arc->start->loc, "green", arc->start->name);
        drawPathfinderNode(arc->finish->loc, "green", arc->finish->name);
        drawPathfinderArc(arc->start->loc, arc->finish->loc, "green");
		TreeCost+=arc->cost;

	}
	cout<< "Total Cost for Minimum Spanning Tree is "<<TreeCost<<endl;




}

//This function is used to check if a click has ocurred close to a Node of the map
// for this check the constant NODE_RADIUS is used as a threshold radius
bool isclose(GPoint P1,GPoint P2)
{
	if((abs(P1.getX()-P2.getX())<=NODE_RADIUS)&&(abs(P1.getY()-P2.getY())<=NODE_RADIUS))
	{return true;}

	return false;


}

// Dijkstra minimum distance search from start to finish node 
// and using the Path class
Path findShortestPath(Node *start,Node *finish)
{
	Path MinPath=Path();
	//Pqueue to store paths from one node
	PriorityQueue<Path> queue;
	//Map to store minimum distance to each node of the graph from the
	// starting node
	Map<string,double> fixed;

	while(start!=finish)
	{
		//if I do not have a path to start node..
		if(!fixed.containsKey(start->name))
		{
			//store the minimum cost to start node in PQueue
			fixed.put(start->name,MinPath.getCost());
			//for all arcs from start, explore all new paths
			foreach(Arc *arc in start->arcs)
			{
				if(!fixed.containsKey(arc->finish->name))
				{
					//add arc to new destination
					MinPath.addArc(arc);
					//store path as a candidate to explore
					queue.enqueue(MinPath,MinPath.getCost());
					//undo your decision to continue exploring the graph
					MinPath.removeLast();
				}
			}
		}
		//if I have no more alternatives to consider...
		if(queue.isEmpty())
		{
			//return empty Path
			MinPath.clear();
			return MinPath;
		}
		//continue exploring the shortest path from start node
		MinPath=queue.dequeue();
		start=MinPath.getLast()->finish;
	}

	return MinPath;
}


// Dijkstra minimum distance search from start to finish node 
// and using the a vector of pointers to Arc variables, see comments
// on function above for functionality
Vector<Arc *> findShortestPathVector(Node *start,Node *finish)
{
	Vector<Arc *> MinPath;
	PriorityQueue<Vector<Arc *> > queue;
	Map<string,double> fixed;

	while(start!=finish)
	{
		if(!fixed.containsKey(start->name))
		{
			fixed.put(start->name,getPathCost(MinPath));
			foreach(Arc *arc in start->arcs)
			{
				if(!fixed.containsKey(arc->finish->name))
				{
					MinPath.add(arc);
					queue.enqueue(MinPath,getPathCost(MinPath));
					MinPath.removeAt(MinPath.size()-1);
				}
			}
		}
		if(queue.isEmpty())
		{
			MinPath.clear();
			return MinPath;
		}
		MinPath=queue.dequeue();
		start=MinPath[MinPath.size()-1]->finish;
	}

	return MinPath;
}
//Function to produce the cost of a path if the latter is stored
// by means of a Vector of pointers to Arc.
double getPathCost(const Vector<Arc *> &path) {
    double Totalcost = 0;
    foreach (Arc *arc in path)   {
        Totalcost += arc->cost;
    }
    return Totalcost;
}

