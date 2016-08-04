/* File: Karel.cpp
 *Author: Francisco Lumbreras
 * A program that finds out how many paths home Karel can take.
 */
#include <iostream>
#include "console.h"
#include "simpio.h"
using namespace std;

/* Function: numPathsHome(int street, int avenue);
 * Usage: cout << numPathsHome(3, 2) << endl; // Prints 3
 * =================================================================
 * Returns how many paths home Karel can take from (street, avenue)
 * back to (1, 1).
 */
int numPathsHome(int street, int avenue);

int main() {
	
	//Initialise the row and column variables to ensure the while loop is accessed at first run.
	int street=0;
	int avenue=0;

	//Ask for user input iteratively until valid value or exit sentinel value. 
	while(street<1||street>5)
	{
	street=getInteger(" Please enter street where Karel sits (-1 to exit) \n");
	if(street==-1)break;
	}
	while(avenue<1||avenue>5&&street!=-1)
	{avenue=getInteger(" Please enter avenue where Karel sits \n");}

	//Print on screen number of paths home

	cout <<"Number of paths to home: "<< numPathsHome(street,avenue) <<endl;


	
	return 0;
}

int numPathsHome(int street, int avenue) {
	//Recursive code to calculate paths home
	if (street<1||avenue<1) return 0;
	if(street==1&&avenue==1) return 1;
	else { return  numPathsHome(street-1,avenue)+numPathsHome(street,avenue-1);}
}

