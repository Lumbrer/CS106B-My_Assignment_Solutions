/* File: EvenWords.cpp
 * Author: Francisco Lumbreras
 * A program that uses recursion to determine which words are
 * even words.
 */
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

/* Function: isEvenWord(string input);
 * Usage: if (isEvenWord("tattletale")) { ... }
 * ==================================================================
 * Returns whether the given word is an "even word;" that is, whether
 * every letter in the word appears an even number of times.
 */
bool isEvenWord(string word); 

int main() {

	string word, sentinel;
	//Exit snetinel string/
	sentinel="-1";

	// Recursively ask for word and display if it is an even word unless sentinel is input/
	while(word!=sentinel){
	word=getLine("Please enter word to check (-1 to exit) \n");
	if(word==sentinel)break;
	cout << "Is input word '"<<word<<"' an even word?: - "<<isEvenWord(toLowerCase(word))<<endl;
	}
	
	return 0;
	
}

bool isEvenWord(string word) {
	//Simplest case, empty string is even/
	if(word=="")
		return true;

	//Check if first char is repeated in string/
	for(int i=1;i<word.length();i++){
	
		//When found call recursively the function once the pair found has been removed/
		if(word[0]==word[i])
		{return isEvenWord(word.substr(1,i-1)+word.substr(i+1));}
	}

	//When unable to find repetition of first char, failure/
	return false;
}

