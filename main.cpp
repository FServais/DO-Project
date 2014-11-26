#include <algorithm>
#include "parser.hpp"


using namespace std;

int main(int argc, char const *argv[])
{

	string s = "1 2 3 4 5";
	vector<string> lol;
	lol = split(s, " ");
	for (int i=0; i<lol.size(); i++)
    		cout << lol[i];

	cout << endl;

	DataModel m("datas");
	cout << m.keyNumber;
	cout << endl;
	cout << m.alphabet;
	cout << endl;

	 for (vector<int>::iterator it = m.sr.begin() ; it != m.sr.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;
	

	 for (vector<int>::iterator it = m.sl.begin() ; it != m.sl.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;

	cout << m.vowels << endl;

	 for (vector<int>::iterator it = m.vl.begin() ; it != m.vl.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;


	 for (vector<double>::iterator it = m.ks.begin() ; it != m.ks.end(); ++it)
	    cout << ' ' << *it;
	  cout << endl;

	/* code */
	return 0;
}
