#include <algorithm>
#include <sstream>

#include "gurobi_c++.h"

using namespace std;

int main(int argc, char const *argv[])
{
	int K = 47;
	int L = K;

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