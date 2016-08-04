/* File: InverseGenetics.cpp
 * Name: Francisco Lumbreras
 * A program that, given a sequence of amino acids, returns all possible
 * RNA strands that could encode that amino acid sequence.
 */
#include <iostream>
#include <string>
#include <fstream>
#include "set.h"
#include "map.h"
#include "simpio.h"
#include "strlib.h"
#include "console.h"
using namespace std;

/* Function: allRNAStrandsFor(string protein,
 *                            Map<char, Set<string> >& codons);
 * Usage: foreach (string rna in allRNAStrandsFor("PARTY", codons);
 * ==================================================================
 * Given a protein and a map from amino acid codes to the codons for
 * that code, returns all possible RNA strands that could generate
 * that protein.
 */
Set<string> allRNAStrandsFor(string protein, Map<char, Set<string> >& codons);

/* Function: loadCodonMap();
 * Usage: Map<char, Lexicon> codonMap = loadCodonMap();
 * ==================================================================
 * Loads the codon mapping table from a file.
 */
Map<char, Set<string> > loadCodonMap();

int main() {
	/* Load the codon map. */
	Map<char, Set<string> > codons = loadCodonMap();

    Set <string> Results;
	string input;

	while(true)
	{
		input=getLine("Please enter combination of proteins to run (Return to exit)\n");
		if(input=="") break;
		//Convert input to upper case, assume all letters are included in codons.txt 
		// no need to discard any cases/
		input=toUpperCase(input);

		//Call recursive function/
		Results=allRNAStrandsFor(input,codons);

		//Plot all possible combinations/
		cout <<"The RNA strans that could represent protein "<<input<<" are:\n"<<endl;
		
		foreach(string str in Results)
		{
			cout << str << endl;
		}
	}
	
	return 0;
}

Set<string> allRNAStrandsFor(string protein, Map<char, Set<string> >& codons) {

	//Define auxiliary set to collect results. This variable can be avoided if results are written 
	//to a set passed by reference.
	Set<string> Aux;
	//If I am on the last char, just return all possible codons for such protein.
	// could have added "if(protein.length()<1) return Aux;" to protect against empty input case/
	if(protein.length()==1) return codons[protein[0]];

	//Populate the combinations after fixing one protein (first in string)
	foreach(string s1 in codons[protein[0]])
	{
		//Call recursively after removing the char that has already been fixed, this will ensure all combinations are generated.
		foreach(string s2 in allRNAStrandsFor(protein.substr(1),codons))
		{
			//Add combinations to Auxiliary Set/
			Aux.add(s1+s2);
		}
	}

	//Return output combinations
	return Aux;
	

}

/* You do not need to change this function. */
Map<char, Set<string> > loadCodonMap() {
	ifstream input("codons.txt");
	Map<char, Set<string> > result;
		
	/* The current codon / protein combination. */
	string codon;
	char protein;
	
	/* Continuously pull data from the file until all data has been
	 * read.  We did not cover this syntax in class, but it means
	 * "while reading a codon/protein pair succeeds, add the pair to
	 * the map."
	 */
	while (input >> codon >> protein) {
		result[protein] += codon;
	}
	
	return result;
}

