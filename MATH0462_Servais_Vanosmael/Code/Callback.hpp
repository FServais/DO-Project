class Callback : public GRBCallback
{
private:
	/*
		Variables
	 */
	vector<vector<GRBVar> > kb, a;
	GRBVar vl;

    int sizeAlphabet;

    // Keep track of what has been added
	vector<vector<bool> > XOR1Added, XOR2Added, XOR3Added, XOR4Added;

	vector<int> sl;

	vector<GRBLinExpr> li;
	vector<GRBLinExpr> lj;

	vector<vector<GRBLinExpr> > constraints_XOR1;
	vector<vector<GRBLinExpr> > constraints_XOR2;
	vector<vector<GRBLinExpr> > constraints_XOR3;

	/*
		Methods
	 */
	bool isSatisfied(string constr, int i, int j){ 
		if(constr == "XOR1"){
			int sum = 0;
			for (int k = 0; k < sizeAlphabet; ++k)
				sum += ((to_int(getSolution(kb[k][i])) + to_int(getSolution(kb[k][j]))) * sl[k]);

			return to_int(getSolution(a[i][j])) <= sum;
		}

		else if(constr == "XOR2"){
			int sum = 0;
			for (int k = 0; k < sizeAlphabet; ++k)
				sum += ((to_int(getSolution(kb[k][i])) - to_int(getSolution(kb[k][j]))) * sl[k]);
			
			return to_int(getSolution(a[i][j])) >= sum;
		}

		else if(constr == "XOR3"){
			int sum = 0;
			for (int k = 0; k < sizeAlphabet; ++k)
				sum += ((to_int(getSolution(kb[k][j])) - to_int(getSolution(kb[k][i]))) * sl[k]);
			
			return to_int(getSolution(a[i][j])) >= sum;
		}

		else if(constr == "XOR4"){
			int sum = 0;
			for (int k = 0; k < sizeAlphabet; ++k)
				sum += ((to_int(getSolution(kb[k][i])) + to_int(getSolution(kb[k][j]))) * sl[k]);
			
			return to_int(getSolution(a[i][j])) <= 2 - sum;
		}

		return false;
	}



	bool isSatisfiedSol(string constr, int i, int j){ 
		if(constr == "XOR1"){
			int sum = 0;
			for (int k = 0; k < sizeAlphabet; ++k)
				sum += ((to_int(kb[k][i].get(GRB_DoubleAttr_X)) + to_int(kb[k][j].get(GRB_DoubleAttr_X))) * sl[k]);
				
			return to_int((a[i][j]).get(GRB_DoubleAttr_X)) <= sum;
		}

		else if(constr == "XOR2"){
			int sum = 0;
			for (int k = 0; k < sizeAlphabet; ++k)
				sum += ((to_int(kb[k][i].get(GRB_DoubleAttr_X)) - to_int(kb[k][j].get(GRB_DoubleAttr_X))) * sl[k]);
			
			return to_int((a[i][j]).get(GRB_DoubleAttr_X)) >= sum;
		}

		else if(constr == "XOR3"){
			int sum = 0;
			for (int k = 0; k < sizeAlphabet; ++k)
				sum += ((to_int(kb[k][j].get(GRB_DoubleAttr_X)) - to_int(kb[k][i].get(GRB_DoubleAttr_X))) * sl[k]);
			
			return to_int((a[i][j]).get(GRB_DoubleAttr_X)) >= sum;
		}

		else if(constr == "XOR4"){
			int sum = 0;
			for (int k = 0; k < sizeAlphabet; ++k)
				sum += ((to_int(kb[k][i].get(GRB_DoubleAttr_X)) + to_int(kb[k][j].get(GRB_DoubleAttr_X))) * sl[k]);

			return to_int((a[i][j]).get(GRB_DoubleAttr_X)) <= 2 - sum;
		}

		return false;
	}

	int to_int(double x){
		return (int)(x + 0.5);
	}


public:
	/*
		Constructor
	 */
	Callback(int _sizeAlphabet, vector<vector<GRBVar> > _kb, GRBVar _vl, vector<vector<GRBVar> > _a, vector<int> _sl) : 
			sl(_sl), kb(_kb), vl(_vl), a(_a), sizeAlphabet(_sizeAlphabet),
			XOR1Added(_sizeAlphabet, vector<bool>(_sizeAlphabet,false)), XOR2Added(_sizeAlphabet, vector<bool>(_sizeAlphabet,false)), XOR3Added(_sizeAlphabet, vector<bool>(_sizeAlphabet,false)), XOR4Added(_sizeAlphabet, vector<bool>(_sizeAlphabet,false))
	{

		li = vector<GRBLinExpr>(_sizeAlphabet);
		for (int i = 0; i < _sizeAlphabet; ++i)
			for (int k = 0; k < _sizeAlphabet; ++k)
				li[i] += (_kb[k][i] * _sl[k]);

		lj = vector<GRBLinExpr>(_sizeAlphabet);
		for (int j = 0; j < _sizeAlphabet; ++j)
			for (int k = 0; k < _sizeAlphabet; ++k)
				lj[j] += (_kb[k][j] * _sl[k]);

		constraints_XOR1 = vector<vector<GRBLinExpr> >(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));
		constraints_XOR2 = vector<vector<GRBLinExpr> >(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));
		constraints_XOR3 = vector<vector<GRBLinExpr> >(sizeAlphabet, vector<GRBLinExpr>(sizeAlphabet));

		for (int i = 0; i < _sizeAlphabet; ++i)
		{ 
			for (int j = 0; j < _sizeAlphabet; ++j)
			{
				constraints_XOR1[i][j] = li[i] + li[j];
				constraints_XOR2[i][j] = li[i] - li[j];
				constraints_XOR3[i][j] = li[j] - li[i];
			}
		}

	}

	bool isSatisfied(){
		for (int i = 0; i < sizeAlphabet; ++i)
		{
			for (int j = 0; j < sizeAlphabet; ++j)
			{
				if (!isSatisfiedSol("XOR1", i, j) || !isSatisfiedSol("XOR2", i, j) || !isSatisfiedSol("XOR3", i, j) || !isSatisfiedSol("XOR4", i, j))
					return false;
			}
		}

		return true;
	}

protected: 
	void callback(){
		try{
			if (where == GRB_CB_MIPSOL)
			{
				int countAdded = 0;
				for (int i = 0; i < sizeAlphabet && countAdded < 16; ++i)
				{
					for (int j = 0; j < sizeAlphabet && countAdded < 16; ++j)
					{
						if(!XOR1Added[i][j]){
							if(!isSatisfied("XOR1", i, j)){
								addLazy(a[i][j] <= constraints_XOR1[i][j]);
								XOR1Added[i][j] = true;
								countAdded++;
							}
						}

						if(!XOR2Added[i][j]){
							if(!isSatisfied("XOR2", i, j)){
								addLazy(a[i][j] >= constraints_XOR2[i][j]);
								XOR2Added[i][j] = true;
								countAdded++;
							}
						}

						if(!XOR3Added[i][j]){
							if(!isSatisfied("XOR3", i, j)){
								addLazy(a[i][j] >= constraints_XOR3[i][j]);
								XOR3Added[i][j] = true;
								countAdded++;
							}
						}

						if(!XOR4Added[i][j]){
							if(!isSatisfied("XOR4", i, j)){
								addLazy(a[i][j] <= 2 - constraints_XOR1[i][j]);
								XOR4Added[i][j] = true;
								countAdded++;
							}
						}
					}
				}
			}
			
		} catch (GRBException e){
			cout << "Error number: " << e.getErrorCode() << endl;
			cout << "Message erreur : " << e.getMessage() << endl;
		} catch (...){
			cout << "Error during callback." << endl;
		}
		
	}
	
};
