#include <algorithm>
#include <sstream>
#include <vector>
#include <cmath>

#include "gurobi_c++.h"
#include "parser.hpp"
#include "Callback.hpp"

using namespace std;

string findChar(int i, vector<vector<GRBVar> >& kb, vector<string> alphabet, int sizeAlphabet){
	for (int j = 0; j < sizeAlphabet; ++j)
	{
		/*
		if(abs(kb[i][j].get(GRB_DoubleAttr_X) - 1.0) < 0.00000000001)
			return alphabet[j];
			*/
		int value = (int)(kb[i][j].get(GRB_DoubleAttr_X) + 0.5);
		if(value == 1)
			return alphabet[j];
	}

	cout << "No no no no no :( Pour i = " << i << endl;
	return " ";
}

int main(int argc, char const *argv[])
{
	/**
	 * ============== Parsing ============== 
	 */
	
	DataModel m("datas", "fl-out1.txt", "fl-out2.txt");


	int numberKeys = m.getNumberKeys();
	int sizeAlphabet = numberKeys;
	int numberVowels = m.getNumberVowels();

	vector<int> dk = m.getDistanceKey();
	vector<double> fr = m.getFrequencies();
	vector<double> ks = m.getStrength();
	vector<int> sl = m.getLeftHandKeys();
	vector<int> sr = m.getRightHandKeys();
	vector<int> v = m.getLeftSideLetters(); 

	vector<double> dif(dk.size());
	for (int i = 0; i < dk.size() ; ++i)
		dif[i] = dk[i]*ks[i];

	vector<vector<double> > w = m.getBigramsFreq();

	/**
	 * ==============  Computation ============== 
	 */
	try{
		// Set new environment
		cout << "Creating environment" << endl;
		GRBEnv env = GRBEnv();

		// Create new model
		cout << "Creating model" << endl;
		GRBModel model = GRBModel(env);

		model.getEnv().set(GRB_IntParam_LazyConstraints, 1);

		/**
		 * Variables
		 */
		cout << "Creating variables" << endl;
		vector<vector<GRBVar> > kb(numberKeys, vector<GRBVar>(sizeAlphabet)); // Assignment of the letters with the keys
		GRBVar vl; // Vowel on the left side or not
		vector<vector<GRBVar> > a(numberKeys, vector<GRBVar>(sizeAlphabet)); // Not the same hand to type one then another

		// kb_{k,l}
		for (int k = 0; k < numberKeys; ++k){
			for (int l = 0; l < sizeAlphabet; ++l){
				stringstream ss;
				ss << "kb_" << k << "_" << l;
				kb[k][l] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, ss.str());
			}
		}

		// vl
		vl = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "vl");

		// a
		for (int k = 0; k < numberKeys; ++k){
			for (int l = 0; l < sizeAlphabet; ++l){
				stringstream ss;
				ss << "a_" << k << "_" << l;

				if (k == l)
					a[k][l] = model.addVar(0.0, 0.0, 0.0, GRB_BINARY, ss.str());
				else
					a[k][l] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, ss.str());
			}
		}

		model.update();

		/**
		 * Objective function
		 */
		cout << "Creating objective function" << endl;
		GRBLinExpr objFunction;

		// First term
		for (int i = 0; i < sizeAlphabet; ++i){
			GRBLinExpr term;

			for (int j = 0; j < numberKeys; ++j)
				term += (dif[j] * kb[j][i]);

			term *= fr[i];
			objFunction += term;
		}

		// Second term
		for (int i = 0; i < sizeAlphabet; ++i)
			for (int j = 0; j < sizeAlphabet; ++j)
				objFunction += (w[i][j] * (1 - a[i][j]));

		// Third term
		for (int i = 0; i < sizeAlphabet; ++i){
			for (int j = 0; j < sizeAlphabet; ++j){
				GRBLinExpr term;

				for (int k = 0; k < numberKeys; ++k)
					term += (dk[k] * kb[k][i]);

				for (int l = 0; l < numberKeys; ++l)
					term += (dk[l] * kb[l][j]);
				

				objFunction += (term * w[i][j]);
			}
		}

		model.setObjective(objFunction, GRB_MINIMIZE);

		/**
		 * Contraints
		 */
		cout << "Creating constraints" << endl;

		GRBLinExpr side_constr;
		for (int l = 0; l < sizeAlphabet; ++l)
		{
			GRBLinExpr term;

			for (int k = 0; k < numberKeys; ++k)
				term += (kb[k][l] * (sr[k] - sl[k]));

			side_constr += (term * fr[l]);
		}
		model.addConstr(side_constr >= 0, "side_constraint");

		// One key per letter
		for (int l = 0; l < sizeAlphabet; ++l)
		{
			GRBLinExpr keyPerLetter;
			
			for (int k = 0; k < numberKeys; ++k)
				keyPerLetter += kb[k][l];
			
			model.addConstr(keyPerLetter == 1, "One key per letter");
		}

		// One letter per key
		for (int k = 0; k < numberKeys; ++k)
		{
			GRBLinExpr letterPerKey;
			
			for (int l = 0; l < sizeAlphabet; ++l)
				letterPerKey += kb[k][l];
			
			model.addConstr(letterPerKey == 1, "One letter per key");
		}

		// Vowels on the same hand
		GRBLinExpr vowels_side;
		for (int l = 0; l < sizeAlphabet; ++l)
		{
			GRBLinExpr term;

			for (int k = 0; k < numberKeys; ++k)
				term += (kb[k][l] * sl[k]);

			vowels_side += (term * v[l]);
		}
		model.addConstr(vowels_side == numberVowels * vl, "Vowels on the same hand");

		for (int i = 0; i < sizeAlphabet; ++i)
		{
			model.addConstr(a[i][i] == 0, "a_i_i constr");
		}

		vector<GRBLinExpr> li(sizeAlphabet);
		for (int i = 0; i < sizeAlphabet; ++i)
			for (int k = 0; k < numberKeys; ++k)
				li[i] += (kb[k][i] * sl[k]);

		vector<GRBLinExpr> lj(sizeAlphabet);
		for (int j = 0; j < sizeAlphabet; ++j)
			for (int k = 0; k < numberKeys; ++k)
				lj[j] += (kb[k][j] * sl[k]);

		vector<vector<GRBLinExpr> > constraints_XOR1(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));
		vector<vector<GRBLinExpr> > constraints_XOR2(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));
		vector<vector<GRBLinExpr> > constraints_XOR3(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));
		vector<vector<GRBLinExpr> > constraints_XOR4(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));

		for (int i = 0; i < sizeAlphabet; ++i)
		{ 
			for (int j = 0; j < sizeAlphabet; ++j)
			{
				constraints_XOR1[i][j] = li[i] + lj[j];
				constraints_XOR2[i][j] = li[i] - lj[j];
				constraints_XOR3[i][j] = lj[j] - li[i];
				constraints_XOR4[i][j] = 2 - li[i] - lj[j];
			}
		}

		Callback cb(sizeAlphabet, kb, vl, a, sl, li, lj, constraints_XOR1, constraints_XOR2, constraints_XOR3, constraints_XOR4);
		model.setCallback(&cb);

		model.optimize();


		/*
		// Row generation 
		
		int numberOfAddedContraints = 0;
		bool constr_violated = true;

		// Links between the variables
		vector<GRBLinExpr> li(sizeAlphabet);
		for (int i = 0; i < sizeAlphabet; ++i)
			for (int k = 0; k < numberKeys; ++k)
				li[i] += (kb[k][i] * sl[k]);

		vector<GRBLinExpr> lj(sizeAlphabet);
		for (int j = 0; j < sizeAlphabet; ++j)
			for (int k = 0; k < numberKeys; ++k)
				lj[j] += (kb[k][j] * sl[k]);

		vector<vector<GRBLinExpr> > constraints_XOR1(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));
		vector<vector<GRBLinExpr> > constraints_XOR2(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));
		vector<vector<GRBLinExpr> > constraints_XOR3(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));
		vector<vector<GRBLinExpr> > constraints_XOR4(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));

		for (int i = 0; i < sizeAlphabet; ++i)
		{
			for (int j = 0; j < sizeAlphabet; ++j)
			{
				constraints_XOR1[i][j] = li[i] + lj[j];
				constraints_XOR2[i][j] = li[i] - lj[j];
				constraints_XOR3[i][j] = lj[j] - li[i];
				constraints_XOR4[i][j] = 2 - li[i] - lj[j];
			}
		}

		vector<vector<bool> > added_XOR1(sizeAlphabet, vector<bool>(sizeAlphabet, false));
		vector<vector<bool> > added_XOR2(sizeAlphabet, vector<bool>(sizeAlphabet, false));
		vector<vector<bool> > added_XOR3(sizeAlphabet, vector<bool>(sizeAlphabet, false));
		vector<vector<bool> > added_XOR4(sizeAlphabet, vector<bool>(sizeAlphabet, false));

		do{
			numberOfAddedContraints = 0;
			// Checking of contraints are violated
			for (int i = 0; i < sizeAlphabet && numberOfAddedContraints < 10; ++i)
			{
				for (int j = 0; j < sizeAlphabet && numberOfAddedContraints < 10; ++j)
				{
					if(added_XOR1[i][j])
						continue;

					if(!(a[i][j].get(GRB_DoubleAttr_X) <= constraints_XOR1[i][j].getValue())){
						model.addConstr(a[i][j] <= li[i] + lj[j], "XOR1");
						numberOfAddedContraints++;
						added_XOR1[i][j] = true;
					}	
				}
			}

			if(numberOfAddedContraints > 0)
				model.optimize();
 
		}while(numberOfAddedContraints > 0);

			//model.addConstr(a[i][j] <= li[i] + lj[j], "XOR1");
			//model.addConstr(a[i][j] >= li[i] - lj[j], "XOR2");
			//model.addConstr(a[i][j] >= lj[j] - li[i], "XOR3");
			//model.addConstr(a[i][j] <= 2 - li[i] - lj[j], "XOR4");		
		*/
		
		/**
		 * Solve
		 */
		//cout << "Solving" << endl;
		//model.optimize();
		//cout << "End of solving" << endl;

		/**
		 * Print
		 */
		
		cout << "======== kb ========" << endl;
		for (int k = 0; k < numberKeys; ++k)
			for (int l = 0; l < sizeAlphabet; ++l)
				cout << kb[k][l].get(GRB_StringAttr_VarName) << " = " << kb[k][l].get(GRB_DoubleAttr_X) << endl;
			/*
		cout << endl;
		
		cout << "======== vl ========" << endl;
		cout << vl.get(GRB_StringAttr_VarName) << " = " << vl.get(GRB_DoubleAttr_X) << endl;

		cout << endl;

		cout << "======== a ========" << endl;
		for (int i = 0; i < sizeAlphabet; ++i)
			for (int j = 0; j < sizeAlphabet; ++j)
				cout << a[i][j].get(GRB_StringAttr_VarName) << " = " << a[i][j].get(GRB_DoubleAttr_X) << endl;
		*/
		cout << endl;
		
		cout << "Objective function = " << model.get(GRB_DoubleAttr_ObjVal) << endl;

		// Display keyboard
		cout << endl;
		
		for (int i = 0; i < 12; ++i)
		{
			cout << findChar(i, kb, m.getAlphabet(), sizeAlphabet) << " ";
		}
		cout << endl << " ";
		for (int i = 12; i < 24; ++i)
		{
			cout << findChar(i, kb, m.getAlphabet(), sizeAlphabet) << " ";
		}
		cout << endl << "  ";
		for (int i = 24; i < 36; ++i)
		{
			cout << findChar(i, kb, m.getAlphabet(), sizeAlphabet) << " ";
		}
		cout << endl;
		for (int i = 36; i < sizeAlphabet; ++i)
		{
			cout << findChar(i, kb, m.getAlphabet(), sizeAlphabet) << " ";
		}
		cout << endl;
		
	} catch(GRBException e){
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...){
		cout << "Exception during optimization" << endl;
	}


	
	return 0;
}
