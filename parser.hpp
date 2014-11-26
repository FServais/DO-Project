#include <fstream>
#include <string>
#include <vector>

using namespace std;

class DataModel {

public:
	int keyNumber, vowelsNumber;
	string alphabet, vowels;
	vector<int> sr, sl,   // sX[i] = 1 if key x is on hand X
	      ks,	// ks[i] = force of the finger associate to key i
	      vl, 	// vl[i] = 1 if the letter i is a vowel
	      dk,	// dk[i] = distance that the finger attributed to the key i has to cross to reach that key		
	      fr;	// fr[i] = frequence of the key i in the language
	vector<vector<int> > big;    // big[i][j] = probability that the letter j follows the letter i in a word

	
	DataModel(ifstream input);
};
