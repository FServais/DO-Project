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

		//Distance
		getline(input, tmp);
		vector<string> distance = split(tmp.substr(3), " ");
		vector<int> distInt(this->keyNumber, 0);
		for(int i = 0; i < distance.size() ; ++i){
			distInt[i] = atoi(distance[i].c_str());
		}		
		this->dk = distInt;

	}
}


void DataModel::setFreq(const char* filename){
	ifstream input;
	input.open(filename);

	if(input.is_open()) {
		double total = this->keyNumber;
		string tmp;
		string caract;
		int index;
		double nbr;
		vector<double> freq(this->keyNumber, 1); //Init of every counter at 1

		while(getline(input, tmp)){ // Add the counting
			caract = tmp.substr(0,1);
			nbr = atof(tmp.substr(2).c_str());
			index = this->alphabet.find_first_of(caract);
			
			if(index != string::npos){
				freq[index] += nbr;
				total += nbr;
			}				
		}		
		
		for(int i = 0; i < freq.size(); ++i){
			freq[i] /= total;
		}		
		this->fr = freq;	
	}
}

void DataModel::setBig(const char* filename){ //Here we take into account that the element in the file are ordered !
	ifstream input;
	input.open(filename);
	
	if(input.is_open()){
		string tmp;
		string current = "", caract1, caract2;
		int currentIndex, index;		
		double total = this->keyNumber; 
		double nbr;
		bool flag = true;
		vector<vector<double> > bigram(this->keyNumber, vector<double>(this->keyNumber, 1));
				
	
		while(getline(input, tmp)){
			caract1 = tmp.substr(0,1);
			if(current != "" && caract1 != current){ //We have take care of every bigram that begin by current.		
				for(int i = 0; i < this->keyNumber; ++i){
					bigram[currentIndex][i] /= total;		// normation			
				}
				total = this->keyNumber;
			}
			if(current == "" || caract1 != current){	
				current = tmp.substr(0,1);
				currentIndex = this->alphabet.find_first_of(current);
				
				while(currentIndex == string::npos){
					flag = getline(input, tmp);
					if(!flag)
						break;				
					current = tmp.substr(0,1);
					currentIndex = this->alphabet.find_first_of(current);
				} //If the first letter of the bigram is not in the alphabet we skip line until we find a nice one									
			}
			if(!flag)						
				break;

			caract2 = tmp.substr(1,1); //second caracter
			nbr = atof(tmp.substr(3).c_str());
			index = this->alphabet.find_first_of(caract2);
			if(index != string::npos){
				bigram[currentIndex][index] += nbr;
				total += nbr;					
			}
		}
		
		for(int i = 0; i < this->keyNumber; ++i){ //Last Verification if a caracter is not present in the first caract of any bigram we norm manually	
			if(bigram[i][0] == 1){
				for(int j = 0; j < this->keyNumber; ++j){
					bigram[i][j] /= this->keyNumber;
				}
			}
		} 
		this->big = bigram;	
	}
}
