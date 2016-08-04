/**********************************************************
 * File: HuffmanEncoding.cpp
 * Author: Francisco Lumbreras
 * Implementation of the functions from HuffmanEncoding.h.
 * Most (if not all) of the code that you write for this
 * assignment will go into this file.
 */

#include "HuffmanEncoding.h"

/* Function: getFrequencyTable
 * Usage: Map<ext_char, int> freq = getFrequencyTable(file);
 * --------------------------------------------------------
 * Given an input stream containing text, calculates the
 * frequencies of each character within that text and stores
 * the result as a Map from ext_chars to the number of times
 * that the character appears.
 *
 * This function will also set the frequency of the PSEUDO_EOF
 * character to be 1, which ensures that any future encoding
 * tree built from these frequencies will have an encoding for
 * the PSEUDO_EOF character.
 */
Map<ext_char, int> getFrequencyTable(istream& file) {
	
	
	if(!file){
		return Map<ext_char, int>();
	} else
	{
		Map<ext_char,int> FreqMap=Map<ext_char,int>();
		
		ext_char next;
		char c;
		while(file.get(c))
		{
			next=c;
			if(FreqMap.containsKey(next))
			{
				FreqMap[next]+=1;
			}else
			{
				FreqMap.put(next,1);
			}
		}
		FreqMap.put(PSEUDO_EOF,1);

		return FreqMap;
	}

}

/* Function: buildEncodingTree
 * Usage: Node* tree = buildEncodingTree(frequency);
 * --------------------------------------------------------
 * Given a map from extended characters to frequencies,
 * constructs a Huffman encoding tree from those frequencies
 * and returns a pointer to the root.
 *
 * This function can assume that there is always at least one
 * entry in the map, since the PSEUDO_EOF character will always
 * be present.
 */
Node* buildEncodingTree(Map<ext_char, int>& frequencies) {
	
	HuffmanPQueue NodeList;
	Node *head=NULL;
	Node *newNode=NULL;
	Node* tomerge1;
	Node* tomerge2;

	

	if(frequencies.isEmpty())
	{return head;}

	foreach(ext_char c in frequencies)
	{
		newNode=new Node;
		newNode->character=c;
		newNode->weight=frequencies[c];
		newNode->one=newNode->zero=NULL;
		NodeList.enqueue(newNode);
	}

	while(NodeList.size()>1)
	{
	
		tomerge1=NodeList.dequeueMin();
		tomerge2=NodeList.dequeueMin();

		newNode=new Node;
		newNode->character=NOT_A_CHAR;
		newNode->weight=tomerge1->weight+tomerge2->weight;
		newNode->zero=tomerge1;
		newNode->one=tomerge2;
		NodeList.enqueue(newNode);
	}

	head=NodeList.dequeueMin();

	if(head->character!=NOT_A_CHAR)
	{
		newNode=new Node;
		newNode->character=NOT_A_CHAR;
		newNode->weight=head->weight;
		newNode->zero=head;
		newNode->one=NULL;

		head=newNode;
	}

	return head;
}

/* Function: freeTree
 * Usage: freeTree(encodingTree);
 * --------------------------------------------------------
 * Deallocates all memory allocated for a given encoding
 * tree.
 */
void freeTree(Node* root) {

	if(root!=NULL)
	{
		freeTree(root->one);
		freeTree(root->zero);

		delete root;
		
	
	}
}

/* Function: encodeFile
 * Usage: encodeFile(source, encodingTree, output);
 * --------------------------------------------------------
 * Encodes the given file using the encoding specified by the
 * given encoding tree, then writes the result one bit at a
 * time to the specified output file.
 *
 * This function can assume the following:
 *
 *   - The encoding tree was constructed from the given file,
 *     so every character appears somewhere in the encoding
 *     tree.
 *
 *   - The output file already has the encoding table written
 *     to it, and the file cursor is at the end of the file.
 *     This means that you should just start writing the bits
 *     without seeking the file anywhere.
 */ 
void encodeFile(istream& infile, Node* encodingTree, obstream& outfile) {
	

	if(!infile)
	{
		error("Error in file to read!");
	}else
	{
		char c;
		ext_char next;
		Vector<int> code;
		while(infile.get(c))
		{
			next=c;
			code.clear();
			if(getCode(next,encodingTree,code))
			{
				foreach(int member in code)
				{
					outfile.writeBit(member);
				}
			}else
			{
				error("Could not find encoding for Character! "+c);
			}
		}
		code.clear();
		if(getCode(PSEUDO_EOF,encodingTree,code))
		{
			foreach(int member in code)
			{
				outfile.writeBit(member);
			}
		}else
		{
			error("Could not find encoding for Pseudo_EOF!");
		}


	}

}

/* Function: decodeFile
 * Usage: decodeFile(encodedFile, encodingTree, resultFile);
 * --------------------------------------------------------
 * Decodes a file that has previously been encoded using the
 * encodeFile function.  You can assume the following:
 *
 *   - The encoding table has already been read from the input
 *     file, and the encoding tree parameter was constructed from
 *     this encoding table.
 *
 *   - The output file is open and ready for writing.
 */
void decodeFile(ibstream& infile, Node* encodingTree, ostream& file) {
	if(!infile)
	{
		error("Error in file to read!");
	}else
	{
		Vector<int> code;
		int next;
		ext_char c;
		char c_con;
		bool EOF_found=false;
		while(!EOF_found)
		{
			next=infile.readBit();
			code.add(next);
			c=findCode(encodingTree,code);
			if(c!=NOT_A_CHAR)
			{
				if(c==PSEUDO_EOF)
				{EOF_found=true;
				}else
				{
					c_con=c;
					file.put(c);
					code.clear();
				}	
			}
		}
	}
}

/* Function: writeFileHeader
 * Usage: writeFileHeader(output, frequencies);
 * --------------------------------------------------------
 * Writes a table to the front of the specified output file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to decompress input files once they've been
 * compressed.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * readFileHeader function defined below this one so that it
 * can properly read the data back.
 */
void writeFileHeader(obstream& outfile, Map<ext_char, int>& frequencies) {
	/* The format we will use is the following:
	 *
	 * First number: Total number of characters whose frequency is being
	 *               encoded.
	 * An appropriate number of pairs of the form [char][frequency][space],
	 * encoding the number of occurrences.
	 *
	 * No information about PSEUDO_EOF is written, since the frequency is
	 * always 1.
	 */
	 
	/* Verify that we have PSEUDO_EOF somewhere in this mapping. */
	if (!frequencies.containsKey(PSEUDO_EOF)) {
		error("No PSEUDO_EOF defined.");
	}
	
	/* Write how many encodings we're going to have.  Note the space after
	 * this number to ensure that we can read it back correctly.
	 */
	outfile << frequencies.size() - 1 << ' ';
	
	/* Now, write the letter/frequency pairs. */
	foreach (ext_char ch in frequencies) {
		/* Skip PSEUDO_EOF if we see it. */
		if (ch == PSEUDO_EOF) continue;
		
		/* Write out the letter and its frequency. */
		outfile << char(ch) << frequencies[ch] << ' ';
	}
}

/* Function: readFileHeader
 * Usage: Map<ext_char, int> freq = writeFileHeader(input);
 * --------------------------------------------------------
 * Reads a table to the front of the specified input file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to reconstruct the encoding tree for that file.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * writeFileHeader function defined before this one so that it
 * can properly write the data.
 */
Map<ext_char, int> readFileHeader(ibstream& infile) {
	/* This function inverts the mapping we wrote out in the
	 * writeFileHeader function before.  If you make any
	 * changes to that function, be sure to change this one
	 * too!
	 */
	Map<ext_char, int> result;
	
	/* Read how many values we're going to read in. */
	int numValues;
	infile >> numValues;
	
	/* Skip trailing whitespace. */
	infile.get();
	
	/* Read those values in. */
	for (int i = 0; i < numValues; i++) {
		/* Get the character we're going to read. */
		ext_char ch = infile.get();
		
		/* Get the frequency. */
		int frequency;
		infile >> frequency;
		
		/* Skip the space character. */
		infile.get();
		
		/* Add this to the encoding table. */
		result[ch] = frequency;
	}
	
	/* Add in 1 for PSEUDO_EOF. */
	result[PSEUDO_EOF] = 1;
	return result;
}

/* Function: compress
 * Usage: compress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman compressor.  Compresses
 * the file whose contents are specified by the input
 * ibstream, then writes the result to outfile.  Your final
 * task in this assignment will be to combine all of the
 * previous functions together to implement this function,
 * which should not require much logic of its own and should
 * primarily be glue code.
 */
void compress(ibstream& infile, obstream& outfile) {
	
	Map<ext_char,int> Frequencies=getFrequencyTable(infile);

	infile.rewind();

	Node *head=buildEncodingTree(Frequencies);

	writeFileHeader(outfile,Frequencies);
	encodeFile(infile,head,outfile);

	freeTree(head);

}

/* Function: decompress
 * Usage: decompress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman decompressor.
 * Decompresses the file whose contents are specified by the
 * input ibstream, then writes the decompressed version of
 * the file to the stream specified by outfile.  Your final
 * task in this assignment will be to combine all of the
 * previous functions together to implement this function,
 * which should not require much logic of its own and should
 * primarily be glue code.
 */
void decompress(ibstream& infile, ostream& outfile) {
	
	Map<ext_char,int> Frequencies=readFileHeader(infile);
	Node *head=buildEncodingTree(Frequencies);
	decodeFile(infile,head,outfile);

	freeTree(head);


}


/* Function: getCode
 * Usage: getCode(c,leaf, values);
 * --------------------------------------------------------
 * This function is used to recursively find the compression 
 * code for a given ext_char that has previously been asserted
 * different to NOT_A_CHAR. The code, once recursively found is
 * stored in a Vector of integers that is passed by reference.
 */
bool getCode(ext_char c,Node *leaf,Vector<int> &values)
{
	if(leaf->character!=NOT_A_CHAR)
		return (leaf->character==c);

	values.add(0);
	if(getCode(c,leaf->zero,values))
	{return true;
	}

	values.removeAt(values.size()-1);

	values.add(1);
	if(getCode(c,leaf->one,values))
	{return true;
	}

	values.removeAt(values.size()-1);
	
	return false;

}

/* Function: findCode
 * Usage: ext_char c=findCode(leaf, values);
 * --------------------------------------------------------
 * This function is used to reverse the encoding by means of  
 * examining the complete encoded file. It will return an ext_char 
 * variable if a valid code has been given. In order to be called
 * recursively it returns NOT_A_CHAR when the code in values is not
 * linked to a char or PSEUDO_EOF. 
 */
ext_char findCode(Node *leaf,Vector<int> &values)
{
	Node *cursor=leaf;
	for(int i=0;i<values.size();i++)
	{
		if(cursor==NULL){return NOT_A_CHAR;}

		if(values[i]==0)
		{
			cursor=cursor->zero;
		}else if(values[i]==1)
		{
			cursor=cursor->one;
		}else
		{
			error("Corrupted code found while decoding");
		}
	}
	return cursor->character;

}
