/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name: Francisco Lumbreras				
 * This file is the starter project for the random writer problem
 * on Assignment #2. This program receives a source sourcefile.txt file as specified
 * by the user and creates a random file saved as sourcefilenameRandom.txt by writing 
 * an initial seed (that most present in given file and starting with an alphanumeric 
 * character) followed by 2000 characters generated using the MDP model of order as 
 * specified by the user (from 1 to 10);
 *
 * The map uses a simple data structure where all the following char elements for a seed are
 * stored in a Vector<char>. A more efficient way to store the data would be to map seeds to
 * a new map of following char to number of occurrences (int).
 */

//relevant include/
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"

using namespace std;

//define error message for invalid file/
const string Invalid_File="Invalid file specified. Please ensure file is located in current directory";

//Declare functions/
Map <string,Vector<char> > createModel(ifstream &myfile,int order);
void printMap(Map <string,Vector<char> > &map);
string getinitialSeed(Map<string,Vector<char> > &map);
void writerandomFile(Map<string,Vector<char> > &map,string filename);

//Entry point/
int main() {
	// 

	//file name of input file/
	string filename;
	//ifstream to open file/
	ifstream myfile;
	//Map strings to vector of following char elements to build MDP model/
	Map <string,Vector<char> > model;
	//order of MDP model
	int MDPorder=-1;
	
	//Until valiud filename.../
	while(myfile.fail()||filename.empty())
	{
		myfile.clear();
		cout<<Invalid_File<<endl;
		filename=getLine("Please enter filename to train model: (RETURN to exit) \n");
		//If sentinel close program/
		if(filename==""){break;}
		//Note the c string required when opening the file/
		myfile.open(filename.c_str());

	}
	//If sentinel, end of execution/
	if (filename==""){return -1;}

	//Until a valid MDP order (could also have chosen default 5 if input corrupted for example)/
	while(MDPorder<1||MDPorder>10)
	{MDPorder=getInteger("Please insert MDP order from 1 to 10: \n");}
	//Read data in your input file and build model/
	model=createModel(myfile,MDPorder);
	//Check if Map is correctly built/
	/*printMap(model);*/
	
	//Write new file/
	writerandomFile(model,filename);
	
	return 0;
}



Map <string,Vector<char> > createModel(ifstream &myfile,int order)
{
	/*This function generates a map for random writing based on an opened file
	and the order fo the MDP desired.
	*/
	//variable to read char/
	char ch;
	//seed and seed copy to update/
	string seed, newseed;
	//empty map constructor/
	Map <string,Vector<char> > map;
	
	//while there are more characters.../
	while(myfile.get(ch))
	{
		//read first seed/
		if(seed.empty()||seed.size()!=order)
		{
			seed+=ch;
		}else{
			//if entry already exists, add subsequent char/
			if(map.containsKey(seed))
			{
				(map[seed]).add(ch);
			}//else generate map entry/
			else
			{
				map.put(seed,Vector<char>(1,ch));
			}
			//Update your seed, careful if order is 1/
			if(order!=1)
			{
				newseed=seed.substr(1);
				newseed+=ch;
				seed=newseed;
				newseed.clear();

			}else{
				seed=ch;
			}
		
		}
	}
	//close file/
	myfile.close();
	return map;
}


void printMap(Map <string,Vector<char> > &map)
{
	//function used to display seeds and number of occurrences of each/
	int n;
	foreach (string s in map)
	{
		n=map[s].size();
		cout<< "Map contains key "<<s<<" which appears "<<n<<" times."<<endl;
	}
}

string getinitialSeed(Map<string,Vector<char> > &map)
{
	/* This function is used in order to produce the first seed for the output file
	and it looks for the most frequent seeds that begin with an alphanumeric character*/
	//find max number of occurrences for valid seed/
	int max=0;
	int ind;
	string seed0="";//initialise your output string (initial seed)/
	//Create a vector to stoire candidates in case multilpe seeds have the same number of occurrences, which is maximum 
	Vector<string> candidates;
	//loop through map/
	foreach (string s in map)
	{
		if(map[s].size()>max&&isalpha(s[0])){
		max=map[s].size();
		}
	}
	//once max number of occurrences known, find the corresponding seed(s)/
	foreach (string s in map)
	{
		if(map[s].size()==max&&isalpha(s[0])){
		candidates.add(s);
		}
	}

	//choose a random seed from those proposed/
	if(!candidates.isEmpty()){
	ind= randomInteger(0, candidates.size()-1);
	seed0.clear();
	seed0=candidates[ind];
	}
	return seed0;

}

void writerandomFile(Map<string,Vector<char> > &map,string filename)
{
	/*this function is used to write the randomly generated file, it takes 
	the MDP map by reference as well as the input file name. The order of the MDP is
	deduced from the length of the seeds in the map*/

	//define variables/
	string seed, newseed, ofilename;
	ofstream myfile;
	int order; // MDP order/
	int numchar; //number of following char in the map for a specific seed, will change during loop/
	int ind; //index to random char chosen/
	char ch; //variable to write file/

	//create name of output file from input file name/
	ofilename=filename.substr(0,filename.find_first_of("."));
	ofilename+="Random.txt";

	seed=getinitialSeed(map);
	order=seed.length();
	//cout<<"Initial seed will be "<<seed<<endl;/

	//note again the c string command when opening the new file/
	myfile.open(ofilename.c_str());
	//clean file in case the same was written before/
	myfile.clear();
	//write initial seed/
	myfile << seed;
	
	//write additional 2000 char elements/
	for(int i=0;i<2000;i++)
	{
		if(!map.containsKey(seed)) //stop writing if seed not found in map/
		{
			break;
			cout<<"Writing finished before 2000 char reached"<<endl;
		}
		//how many char elements I found after seed?/
		numchar=map[seed].size();
		//take random index/
		ind=randomInteger(0,numchar-1);
		//get random element and write to file (most frequent element is most probable)/ 
		ch=map[seed].get(ind);
		myfile << ch;
		
		//update your seed/
		if(order!=1)
			{
				newseed=seed.substr(1);
				newseed+=ch;
				seed=newseed;
				newseed.clear();

			}else{
				seed=ch;
			}
	}
	//close file
	myfile.close();

	cout<<"Finished writing on file "<<ofilename<<endl;
	

	
}