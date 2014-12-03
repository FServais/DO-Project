#include "Callback.hpp"

Callback::Callback(vector<GRBLinExpr> _li, vector<GRBLinExpr> _lj, vector<vector<GRBLinExpr> > _constraints_XOR1, vector<vector<GRBLinExpr> > _constraints_XOR2, vector<vector<GRBLinExpr> > _constraints_XOR3, vector<vector<GRBLinExpr> > _constraints_XOR4){

	numberOfXOR1Added = 0;
	numberOfXOR2Added = 0;
	numberOfXOR3Added = 0;
	numberOfXOR4Added = 0;

	li = _li;
	lj = _lj;
	constraints_XOR1 = _constraints_XOR1;
	constraints_XOR2 = _constraints_XOR2;
	constraints_XOR3 = _constraints_XOR3;
	constraints_XOR4 = _constraints_XOR4;
}
