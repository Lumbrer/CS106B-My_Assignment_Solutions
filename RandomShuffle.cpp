/* File: RandomShuffle.cpp
 * Author: Francisco Lumbreras
 * A program that uses recursion to randomly scramble the contents of
 * a string.
 */
#include <iostream>
#include <string>
#include "random.h"
#include "console.h"
#include "simpio.h"
using namespace std;

/* Function: randomShuffle(string input);
 * Usage: cout << randomShuffle("keith") << endl; // Might get htiek
 * =================================================================
 * Returns a random permutation of the given string.
 */
string randomShuffle(string input);

int main() {
	//Get word from user and shuffle unless exit sentinel is chosen, then exit.
	string input=getLine("Please enter word to shuffle (Return to exit) \n");
	if(input=="") return -1;
	else
	{ cout<< "Shuffled input word " <<input<<" to random: "<<randomShuffle(input)<< endl;
	return 0;
	}
}

string randomShuffle(string input) {
	// if last char left, only one shuffle possible
	if(input.length()==1)
		return input;
	else {
		// Random extraction of one element out of string/
		int ind=randomInteger(0,input.length()-1);

		//Recursively call randomShuffle on remaining string and append it to chosen char/
		return input[ind]+randomShuffle(input.substr(0,ind)+input.substr(ind+1));
	}
}

