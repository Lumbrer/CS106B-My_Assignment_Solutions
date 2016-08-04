/* File: UniversalHealthCoverage.cpp
 * Name: Francisco Lumbreras
 * A program that determines whether or not it is possible to provide
 * coverage to everyone using a fixed number of hospitals.
 */
#include <iostream>
#include <string>
#include "set.h"
#include "simpio.h"
#include "vector.h"
#include "console.h"
using namespace std;

/* Function: canOfferUniversalCoverage(Set<string>& cities,
 *                                     Vector< Set<string> >& locations,
 *                                     int numHospitals,
 *                                     Vector< Set<string> >& result);
 * Usage: if (canOfferUniversalCoverage(cities, locations, 4, result)
 * ==================================================================
 * Given a set of cities, a list of what cities various hospitals can
 * cover, and a number of hospitals, returns whether or not it's
 * possible to provide coverage to all cities with the given number of
 * hospitals.  If so, one specific way to do this is handed back in the
 * result parameter.
 * This problem shows a very similar structure as sudoku solver. Make a
 * decision and stick to it until solution or undo. 
 */
bool canOfferUniversalCoverage(Set<string>& cities,
                               Vector< Set<string> >& locations,
                               int numHospitals,
                               Vector< Set<string> >& result);

bool checkCoverage(Set<string>& cities,Vector< Set<string> >& result);


int main() {
	
	//Populate dummy cities Set//

	Set<string> cities;
	cities.add("A");
	cities.add("B");
	cities.add("C");
	cities.add("D");
	cities.add("E");
	cities.add("F");
	cities.add("G");

	//Define vectors of sets for both candidate and result locations/
	Vector<Set<string> > locations(5);
	Vector<Set<string> > result;

	//Populate dummy candidate locations/
	locations[0].add("A");locations[0].add("B");locations[0].add("C");
	locations[1].add("A");locations[1].add("C");locations[1].add("D");
	locations[2].add("B");locations[2].add("F");
	locations[3].add("C");locations[3].add("E");locations[3].add("F");
	locations[4].add("G");


	while(true)
	{
		//Variable to store number of hospitals 
		int numHos=0;

		cout<<"Select number of hospitals to build (min 1-max "<<locations.size()<<") [0 to exit]"<<endl;
		numHos=getInteger();
		if (numHos<1||numHos>locations.size()) break;


		//Call recursive function/
		bool possible=canOfferUniversalCoverage(cities,locations,numHos,result);

		//Display if possible to cover. There is no output of the hospitals in result for simplicity/
		if(possible) {cout<<"It is possible to cover all locations with "<<numHos<<" Hospitals"<<endl;}
		else {cout<<"Imposible to cover all locations with "<< numHos<<" Hospitals"<<endl;}

	}
	return 0;
}

bool canOfferUniversalCoverage(Set<string>& cities,
                               Vector< Set<string> >& locations,
                               int numHospitals,
                               Vector< Set<string> >& result) 
{
	//Have we covered all cities? if so success!
	if(checkCoverage(cities,result)) return true;

	//Have we run out of hospitals? if so failure/
	if(numHospitals==0) return false;

	//Recursive loop. Take one hospital and call recursively your function using the remaining.
	for(int i=0;i<locations.size();i++)
	{
		//Make copy of locations. This step could be removed and the code made more elegant if
		//the function call below uses a subvector of locations where location[i] has been removed so a copy is not needed./
		Vector< Set<string> > RemainLoc=locations;
		//take a tentative hospital/
		result.add(locations[i]);
		//Remove chosen hospital from remaining candidates/
		RemainLoc.removeAt(i);

		//if once taken this decision I can build a train of decisions that leads me to full coverage
		// I am done and success!/

		//Recursive call with remaining candidate locations and number of hospitals minus one available./
		if(canOfferUniversalCoverage(cities,RemainLoc,numHospitals-1,result))
		{return true;}

		//if my decision was poor, remove the candidate from my tentative solution and try next
		result.removeAt(result.size()-1);
	}

	//if I tried all possible routes and could not cover all my cities, failure/
	return false;
}

//define a function to check if you have reached your goal and exit recursion
// same as function solvesudoku for sudoku backtracking problem
bool checkCoverage(Set<string>& cities,Vector< Set<string> >& result)
{
	//compile all covered cities in a set/
	Set<string> Covered;
	for(int i=0;i<result.size();i++)
	{Covered+=result[i];
	}

	//check if the set created contains the cities we must cover, if so success!/
	return cities.isSubsetOf(Covered);
}


