/*
 * File: Boggle.cpp
 * ----------------
 * Name: Francisco Lumbreras
 * This file is the main starter file for Assignment #4, Boggle.
 * In this script you will be playing boogle against your computer. This is a game focused on finding words by means of concatenating adjacent words
 * from a randomly produced board. 
 * Please request instructions at the beginning of the game for more details. 
 */
 
#include <iostream>
#include "gboggle.h"
#include "graphics.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "error.h"

using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

//Define a char constant to mark used cubes on board for iterative calls/

const char USED ='1';

// Data to shuffle the cubes randomly

const string STANDARD_CUBES[16]  = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

//Define structure to store cube locations in order to highlight them/
typedef struct{
	int row;
	int col;
}Location;

/* Function prototypes */

void welcome();
void giveInstructions();
void askforInstructions();
bool askforManual();
bool askforExtended();
bool isAlphaString(string str);
void labelBoard(Grid<char> Board);
Grid<char> fillManualBoard(int BoardSize);
Grid<char> shuffleCubes(Vector<string> Cubes, int BoardSize);
bool findWord(Grid<char> &Board,string word,Vector<Location> &Locations);
bool areAdjacent(Location a,Location b);
void  highlightWord(Vector<Location> &Locations,bool flag);
Vector<string> ComputerWordSearch(Grid<char> &Board,string Candidate,Location Cursor,Lexicon &English);

//template function to swap elements as passed by reference/
template <typename Type>
void swapElements(Type &a,Type &b);

/* Main program */

int main() {

	//Board size (cubes per dimension)
	int BoardSize;
	// Candidate to find as provided by the user
	string Candidate;
	// Vector of letter locations in the cube to highlight them
	Vector<Location> Locations=Vector<Location>();
	//Board, grid of char type. A copy used for iterative calls
	Grid<char> Board=Grid<char>();
	Grid<char> BoardCopy=Grid<char>();
	// Vector of strings to populate board
	Vector<string> Cubes=Vector<string>();
	// Vector of string to store the words found by the computer iteratively
	Vector<string> ComputerWords=Vector<string>();
	// English words
	Lexicon English("EnglishWords.dat");
	if(English.isEmpty()) error("Unable to open EnglishWords.dat file");
	// Additional lexicons to store words already used by each player
	Lexicon UserWords=Lexicon();
	Lexicon MachineWords=Lexicon();
	// Variable to exit iterative loop/
	string playagain="y";
	// Cursor to call iterative the computer search . This way the next function call
	// Knows where to begin its search
	Location MachineCursor;
	
   //Only welcome on the first go
   welcome();
   

   while(playagain=="y"||playagain=="yes")
   {
	   //Draw graphics, ask for instructions and clear all data from previous game.
	   initGraphics(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
	   askforInstructions();
	   Cubes.clear();
	   UserWords.clear();
	   MachineWords.clear();
	   ComputerWords.clear();

	
	   //If user wants big boogle... 
   if(askforExtended())
   {
	   BoardSize=5;
	   //This loop is only needed if cubes shuffling is chosen, so it could be within the loop for that selection. 
	   for(int i=0;i<BoardSize*BoardSize;i++)
	   {
		   Cubes.add(BIG_BOGGLE_CUBES[i]);
	   }

   }else //Use data for standard...
   {
	   BoardSize=4;
	   for(int i=0;i<BoardSize*BoardSize;i++)
	   {
		   Cubes.add(STANDARD_CUBES[i]);
	   }
   }

   //Reset the board and draw it again.
   Board.resize(BoardSize,BoardSize);
   drawBoard(BoardSize,BoardSize);

   //Choose how to fill the board and ask in consequence
   if(askforManual())
   {
	   Board=fillManualBoard(BoardSize);
		
   }else
   {
		Board=shuffleCubes(Cubes,BoardSize);
   }
   //Add letter labels
   labelBoard(Board);

   cout<<"Player's turn begins now!"<<endl;
   while(true)
   {
	   //Reset location of word
	   Locations.clear();
	   //Take candidate from user
	   Candidate=getLine("Please Enter a word:\n");
	   //Exit if no candidate
	   if(Candidate==""||Candidate==" ") break;

	   if(Candidate.length()<4) cout << "Minimum word length must be 4 characters"<<endl;

	   //If it is valid, search for it on the table
	   if(Candidate.length()>=4&&English.contains(Candidate)&&(!UserWords.contains(Candidate)))
	   {
		   //Convert to uppercase as board is in upper case
		   Candidate=toUpperCase(Candidate);
		   //Call recursive search of word, if found, add it and highlight it
		   if(findWord(Board,Candidate,Locations))
		   {
			   //cout<<"you got a new word!!"<<endl;
			   UserWords.add(Candidate);
			   recordWordForPlayer(Candidate,HUMAN);
			   highlightWord(Locations,true);
			   pause(200);
			   highlightWord(Locations,false);

		   }
	   }else if(UserWords.contains(Candidate))
	   {
			cout << "That word has been used previously!"<<endl;
	   }else
	   {
			cout << "That's not a word!"<<endl;
	   }
   }

    cout<<"Machine's turn begins now!"<<endl;

	//Iterative machine search, machine must start a search from each of the cubes on the board
	for(int i=0;i<Board.numRows();i++)
	{
		for(int j=0;j<Board.numCols();j++)
		{
			//Initiate your candidate
			Candidate="";
			Candidate+=Board[i][j];

			//Mark the board initial cube as used 
			BoardCopy=Board;
			BoardCopy[i][j]=USED;

			//Initiate the cursor
			MachineCursor.row=i;
			MachineCursor.col=j;

			//Iterative call to search for words
			ComputerWords=ComputerWordSearch(BoardCopy,Candidate,MachineCursor,English);

			//Add the words found from that initial cube, if not used by the human player or duplicated
				foreach(string str in ComputerWords)
			{
				if(!MachineWords.contains(str)&&!UserWords.contains(str))
				{
					recordWordForPlayer(str,COMPUTER);
					MachineWords.add(str);
				}
			}

		}
	}

   //Ask for new game
   playagain=getLine("Would you like to play again? \n");
   playagain=toLowerCase(playagain);
   }
 
return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
   cout << "Welcome!  You're about to play an intense game ";
   cout << "of mind-numbing Boggle.  The good news is that ";
   cout << "you might improve your vocabulary a bit.  The ";
   cout << "bad news is that you're probably going to lose ";
   cout << "miserably to this little dictionary-toting hunk ";
   cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
   cout << endl;
   cout << "The boggle board is a grid onto which I ";
   cout << "I will randomly distribute cubes. These ";
   cout << "6-sided cubes have letters rather than ";
   cout << "numbers on the faces, creating a grid of ";
   cout << "letters on which you try to form words. ";
   cout << "You go first, entering all the words you can ";
   cout << "find that are formed by tracing adjoining ";
   cout << "letters. Two letters adjoin if they are next ";
   cout << "to each other horizontally, vertically, or ";
   cout << "diagonally. A letter can only be used once ";
   cout << "in each word. Words must be at least four ";
   cout << "letters long and can be counted only once. ";
   cout << "You score points based on word length: a ";
   cout << "4-letter word is worth 1 point, 5-letters ";
   cout << "earn 2 points, and so on. After your puny ";
   cout << "brain is exhausted, I, the supercomputer, ";
   cout << "will find all the remaining words and double ";
   cout << "or triple your paltry score." << endl << endl;
   cout << "Hit return when you're ready...";
   getLine();
}

/** 
Ask the user if instructions should be displayed
**/
void askforInstructions()
{
	 string opt;
	 cout<<"Would you like to read the instructions? "<<endl;
	 while(opt!="y"&&opt!="yes"&&opt!="n"&&opt!="no")
   {
	opt=getLine("");
	opt=toLowerCase(opt);
	if(opt=="y"||opt=="yes") {
		giveInstructions();
	}else if (opt!="n"&&opt!="no"){cout<<"Please choose Yes (Y) / No (N)"<<endl;}
   }
}

/** 
Ask the user if big boogle 5x5 is preferred, returns a bool variable to indicate the decision form the user
**/
bool askforExtended()
{
	cout<<"There is an option to play Extended Boogle with a 5x5 board"<<endl;
	string opt;
	bool res=false;
	opt=getLine("Would you like to play Entended Boogle? \n");
	opt=toLowerCase(opt);
	if(opt=="y"||opt=="yes") {
		res=true;
	}
   return res;
}

/** 
Ask the user if the board will be created manually. Returns bool. 
**/
bool askforManual()
{
	 string opt;
	 bool res=false;
	 cout<<"There is an option to enter the board manually, would you like to do so?"<<endl;
	 while(opt!="y"&&opt!="yes"&&opt!="n"&&opt!="no")
   {
	opt=getLine("");
	opt=toLowerCase(opt);
	if(opt=="y"||opt=="yes") {
		res=true;
	}else if (opt!="n"&&opt!="no"){cout<<"Please choose Yes (Y) / No (N)"<<endl;}
   }
	 return res;	
}

/** 
Function to populate board manually, it returns a grid contaning the board letters and takes only the board size as input
**/
Grid<char> fillManualBoard(int BoardSize)
{
	Grid<char> Board=Grid<char>(BoardSize,BoardSize);
	string str;
	//Take only a string with valid number of elements and all being alphabetic characters
	while(str.length()!=BoardSize*BoardSize||(!isAlphaString(str)))
	{
		str.clear();
		cout<<"Please enter a "<<BoardSize*BoardSize<<" ALPHABETIC elements string to assign the board letters"<<endl;
		str=getLine();
	}
	//Fill in the grid once valid input respecting rows and columns
	for(unsigned int i=0;i<str.length();i++)
	{
		Board[i/BoardSize][i%BoardSize]=str[i];
	}
	return Board;
}

/**
Verify if a string is full of alphabetic elements. Recursive function
**/
bool isAlphaString(string str)
{
	if(str.empty()) return true;
	if(str.length()==1) 
	{
		return((str[0] <= 'z' && str[0] >= 'a') || (str[0] <= 'Z' && str[0] >= 'A'));
	}
	//Return iterative call. Take first character, check it and continue check. 
	return isAlphaString(str.substr(0,1))&&isAlphaString(str.substr(1));
}

/**
Generate board by shuffling cubes
**/
Grid<char> shuffleCubes(Vector<string> Cubes, int BoardSize)
{
	Grid<char> Board=Grid<char>(BoardSize,BoardSize);
	int ind;
	//For each of the cubes given, swap them with another randomly chosen cube in order to
	// locate each cube on a different board position
	for(int i=0; i<Cubes.size();i++)
	{
		ind=randomInteger(0,Cubes.size()-1);
		swapElements(Cubes[i],Cubes[ind]);
	}

	//for each of the relocated cubes, pick a face randomly and add it to the board grid
	for(int i=0;i<Cubes.size();i++)
	{
		ind=randomInteger(0,(Cubes[i]).length()-1);
		Board[i/BoardSize][i%BoardSize]=(Cubes[i])[ind];
	}
	return Board;
}

/**
Function to display all board char elements
**/
void labelBoard(Grid<char> Board)
{
	int nf=Board.numRows();
	int nc=Board.numCols();

	for(int i=0;i<nf;i++)
	{
		for(int j=0;j<nc;j++)
		{
			labelCube(i,j,Board[i][j]);
		}
	}
}

/**
Swap elements, typename template function. 
**/
template <typename Type>
void swapElements(Type &a,Type &b)
{
	Type aux=a;
	a=b;
	b=aux;
}

/**
Recursive function used to find a word as given by the user
**/
bool findWord(Grid<char> &Board,string word,Vector<Location> &Locations)
{
	//if you found all letters, the word is valid
	if(word=="") return true;

	Grid<char> NewBoard=Grid<char>();
	//Location placeholder
	Location pos;

	//search for the first character in the word around the board
	for(int i=0;i<Board.numRows();i++)
	{
		for(int j=0;j<Board.numCols();j++)
		{
			
			if(Board[i][j]==word[0])
			{
				//Once found, record where it is located, make a decision
				pos.row=i;
				pos.col=j;

				//If this is the first word or it is adyacent to the last position, record it
				if(Locations.isEmpty()||areAdjacent(pos,Locations[Locations.size()-1]))
				{
				Locations.add(pos);
				//Copy board for recursive call and mark as used the taken char
				NewBoard=Board;
				NewBoard[i][j]=USED;

				// if you can find the rest of the word using this path, go for it
				if (findWord(NewBoard,word.substr(1),Locations))
				{
					return true;
				}
				//backtracking, undo your action
				Locations.removeAt(Locations.size()-1);
				}	
			}
			
		}
	}
	//Impossible to find the character, failure
	return false;
}

/**
Check it two Location structures are adjacent 
**/
bool areAdjacent(Location a,Location b)
{
	if(a.col-b.col<=1&&a.col-b.col>=-1&&a.row-b.row<=1&&a.row-b.row>=-1) return true;
	else return false;

}

/**
Use this function to highlight the word given by the user once found, we pass a flag in order
to call the function given in gboogle.h
**/
void  highlightWord(Vector<Location> &Locations,bool flag)
{
	for(int i=0;i<Locations.size();i++)
	{
		highlightCube((Locations[i]).row, (Locations[i]).col, flag);
	}
}

/**
Recursive function used to populate a vector of words found from an initial position on the board and used to search words for 
the computer. This function returns a Vector of string elements.
**/
Vector<string> ComputerWordSearch(Grid<char> &Board,string Candidate,Location Cursor,Lexicon &English)
{
	//Board copy to make recursive call
	Grid<char> NewBoard=Grid<char>();
	Location Pos;
	//Vector to store results
	Vector<string> Aux=Vector<string>();
	//vector to indicate failure, empty vector
	Vector<string> EmptyVec=Vector<string>();

	//if the search is heading to failure finish it
	if((!English.containsPrefix(Candidate))||Candidate.length()>8) return EmptyVec;

	//if you found a valid word, add it to your vector
	if(Candidate.length()>3&&English.contains(Candidate))
	{
		Aux.add(Candidate);
	}
	//search the adjacent cubes in search of a valid word path
	for(int dx=-1;dx<=1;dx++)
	{
		for(int dy=-1;dy<=1;dy++)
		{
			//if I point to the same cube or I am out of the board or I already used the cube do not search that way
			if(!(dx==0&&dy==0)&&Board.inBounds(Cursor.row+dx,Cursor.col+dy)&&(Board[Cursor.row+dx][Cursor.col+dy]!=USED))
			{
				//add char to candidate
				Candidate+=Board[Cursor.row+dx][Cursor.col+dy];
				//create board copy and mark used the char taken
				NewBoard=Board;
				NewBoard[Cursor.row+dx][Cursor.col+dy]=USED;
				Pos.row=Cursor.row+dx;
				Pos.col=Cursor.col+dy;

				//if you can find a word using the path chosen, add the word to aux
				if(!ComputerWordSearch(NewBoard,Candidate,Pos,English).isEmpty())
				{
					Aux+=ComputerWordSearch(NewBoard,Candidate,Pos,English);
				}
				//backtracking, if I could not find a word this way I need to remove the last char from my candidate
				Candidate=Candidate.substr(0,Candidate.length()-1);
			}
		}
	}
	//return your vector full of valid words
	return Aux;
}


