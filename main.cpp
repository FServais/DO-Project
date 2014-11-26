#include <algorithm>
#include "parser.hpp"


using namespace std;

int main(int argc, char const *argv[])
{
	DataModel m("datas");
	cout << "Number of Keys : " << m.keyNumber;
	cout << endl;
	cout << "Set of the letters : " << m.alphabet;
	cout << endl;

	cout << "sr : ";
	 for (vector<int>::iterator it = m.sr.begin() ; it != m.sr.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;
	
	cout << "sl : ";
	 for (vector<int>::iterator it = m.sl.begin() ; it != m.sl.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;

	cout << "Vowels : ";
	cout << m.vowels << endl;
	cout << "vl : ";
	 for (vector<int>::iterator it = m.vl.begin() ; it != m.vl.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;

	cout << "ks : ";
	 for (vector<double>::iterator it = m.ks.begin() ; it != m.ks.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;

	cout << "dk : ";
	 for (vector<int>::iterator it = m.dk.begin() ; it != m.dk.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;

	m.setFreq("out1.txt");
	cout << "fr : ";
	 for (vector<double>::iterator it = m.fr.begin() ; it != m.fr.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;

	m.setBig("out2.txt");
	cout << "big : ";
	 for (vector<vector<double> >::iterator it = m.big.begin() ; it != m.big.end(); ++it){
	for (vector<double>::iterator it2 = it->begin() ; it2 != it->end(); ++it)
	    cout << ' ' << *it2;
	  cout << endl;

	}
	  cout << endl;

	return 0;
}
