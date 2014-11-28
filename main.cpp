#include <algorithm>
#include <sstream>

#include "gurobi_c++.h"
#include "parser.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
	/**
	 * ============== Parsing ============== 
	 */

	int K = 47;
	int L = K;

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
	for (int i = 0 ; i < m.keyNumber; ++i){
		for (int j = 0; j < m.keyNumber; ++j)
		    cout << ' ' << m.big[i][j];
	  cout <<  endl;

	}
	  cout << endl;

	/**
	 * ============== Computation ============== 
	 */
	try{
		// Set new environment
		GRBEnv env = GRBEnv();

		// Create new model
		GRBModel model = GRBModel(env);

		// Variables
		// kb_{k,l}
		for (int i = 0; i < K; ++i){
			for (int j = 0; j < L; ++j){
				stringstream ss;
				ss << "k_" << i << "_" << j;
				GRBVar kij = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, ss.str());
			}
		}

	} catch(GRBException e){
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...){
		cout << "Exception during optimization" << endl;
	}

	
	return 0;
}
