#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include <stdlib.h>

using namespace std;

class DataModel {

public:
	/**
	 * keyNumber : Number of keys
	 * vowelsNumber : Number of vowels
	 */
	int keyNumber, vowelsNumber;
	string alphabet, vowels;
	vector<int> sr, sl,	         // sX[i] = 1 if key i is on hand X
			    vl,		 	// vl[i] = 1 if the letter i is a vowel
			    dk;			// dk[i] = distance that the finger attributed to the key i has to cross to reach that key		
	vector<vector<double> > big;    // big[i][j] = probability that the letter j follows the letter i in a word
	vector<double> ks,		// ks[i] = force of the finger associate to key i
				   fr;		// fr[i] = frequence of the key i in the language

	
	DataModel(const char* filename);

	int getNumberKeys(){ return keyNumber; }
	int getNumberVowels(){ return vowelsNumber; }

	string getAlphabet(){ return alphabet; }
	
	vector<int> getDistanceKey() { return dk; }
	vector<int> getLeftHandKeys(){ return sl; }
	vector<int> getRightHandKeys(){ return sr; }
	vector<int> getLeftSideLetters(){ return vl; }
	vector<double> getFrequencies(){ return fr; }
	vector<double> getStrength(){ return ks; }
	vector<vector<double> > getBigramsFreq() {return big; }

	void setFreq(const char* filename);
	void setBig(const char* filename);

};

vector<string> split(string toSplit, string delimiter);
