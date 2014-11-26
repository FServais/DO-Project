#include "parser.hpp"


vector<string> split(string toSplit, string delimiter){
	vector<string> returnS;
	size_t current;
	size_t next = -1;
	do
	{
	  current = next + 1;
	  next = toSplit.find_first_of(delimiter, current);
	  returnS.push_back(toSplit.substr(current, next - current));
	}while (next != string::npos);
	return returnS;
}

DataModel::DataModel(const char* filename){	
	ifstream input;
	input.open(filename);

	if(input.is_open()) {
		string tmp;
		vector<string> stringTab1, stringTab2;	
		int count;


		//Get Ke
		getline(input, tmp);
		tmp = tmp.substr(3);
		this->keyNumber = atoi(tmp.c_str());
	
		//Get alphabet
		getline(input, tmp);
		this->alphabet = tmp.substr(3);

		
		//Hands sr and sl	
		getline(input, tmp);
		tmp = tmp.substr(3);
		stringTab1 = split(tmp, " "); //Left side
		getline(input, tmp);
		tmp = tmp.substr(3);		
		stringTab2 = split(tmp, " "); //Rigth side
		vector<int> tmp1(this->keyNumber, 0), tmp2(this->keyNumber,0); //Vectors with only 0
		for(int i = 0; i < stringTab1.size(); ++i){
			tmp1[atoi(stringTab1[i].c_str())] = 1;
		}
		for(int i = 0; i < stringTab2.size(); ++i){
			tmp2[atoi(stringTab2[i].c_str())] = 1;
		}
		this->sl = tmp1;
		this->sr = tmp2;

		//Vowels
		getline(input, tmp);
		tmp = tmp.substr(3);
		this->vowels = tmp;
		vector<int> tmp3(this->keyNumber, 0);		
		for(int i = 0; i < tmp.size(); ++i){
			int index =  this->alphabet.find_first_of(tmp[i]);
			tmp3[index] = 1;
		}
		this->vl = tmp3;

		getline(input, tmp);
		this->vowelsNumber = atoi(tmp.substr(3).c_str());

		//Finger and strength
		getline(input, tmp);
		vector<string> fingerStr = split(tmp.substr(3), " "); 
		
		vector<double> keyToStrength(this->keyNumber, 0.0);
		for(int f = 0; f < 10; ++f){
			getline(input, tmp);
			vector<string> keyToFingerF = split(tmp.substr(4), " ");
			for(int i = 0; i < keyToFingerF.size(); ++i){
				keyToStrength[atoi(keyToFingerF[i].c_str())] = atof(fingerStr[f].c_str());			
			}		
		}
		this->ks = keyToStrength;
/*
		//Distance
		getline(input, tmp);
		vector<string> distance = split(tmp.substr(3), " ");
		vector<int> distInt(this->keyNumber - 1, 0);
		for(int i = 0; i < this->keyNumber; ++i){
			distInt[i] = atoi(distance[i].c_str());
		}		
*/

	}
}




