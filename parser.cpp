#include "parser.hpp"
#include <algorithm>

struct sort_second_pair
{
	bool operator()(const pair<string, int> &left, const pair<string, int> &right){
		return left.second > right.second;
	}
};

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

DataModel::DataModel(const char* datafile, const char* freq_file, const char* bigram_file){	
	this->invalidModel = false;	
	this->keyNumber = 47;
	setFreq(freq_file);

	if(this->invalidModel)
		return;
	
	ifstream input;
	input.open(datafile);

	if(input.is_open()) {
		string tmp;
		vector<string> stringTab1, stringTab2;	
		int count;
	
		//Get alphabet
		getline(input, tmp);
		//this->alphabet = tmp.substr(3);
		
		//Hands sr and sl	
		getline(input, tmp);
		tmp = tmp.substr(3);
		stringTab1 = split(tmp, " "); //Left side

		getline(input, tmp);
		tmp = tmp.substr(3);		
		stringTab2 = split(tmp, " "); //Right side

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
			string s = tmp.substr(i,1);
			int index = find(alphabet.begin(), alphabet.end(), s) - alphabet.begin();
			tmp3[index] = 1;
		}
		this->vl = tmp3;

		//getline(input, tmp);
		//this->vowelsNumber = atoi(tmp.substr(3).c_str());
		this->vowelsNumber = this->vowels.size();

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
		setBig(bigram_file);
	}
	else{
		this->invalidModel = false;
	}


}


void DataModel::setFreq(const char* filename){
	ifstream input;
	input.open(filename);
	if(input.is_open()) {
		
		string line;
		int total = 0;
		vector<pair<string, int> > freqs;

		while(getline(input, line)){
			vector<string> splitted = split(line, " "); // 2 parts : <char ; freq>

			int f = atoi(splitted[1].c_str());

			total += f;
			freqs.push_back(make_pair(splitted[0], f));
		}

		sort(freqs.begin(), freqs.end(), sort_second_pair());

		vector<pair<string, int> >::iterator it = freqs.begin();
		vector<double> freq(this->keyNumber, 0);

		for (int i = 0; i < this->keyNumber && it != freqs.end() ; ++i){
			this->alphabet.push_back(it->first);
			freq[i] = (double)it->second / (double)total;
			it++;
		}


		this->fr = freq;
	}
	else{
		this->invalidModel = true;
	}
}

void DataModel::setBig(const char* filename){ //Here we take into account that the element in the file are ordered !
	ifstream input;
	input.open(filename);
	if(input.is_open()){
		string tmp;
		string current = "", caract1, caract2;
		vector<string>::iterator currentIndex;	
		double total = this->keyNumber; 
		double nbr;
		bool flag = true;
		vector<vector<double> > bigram(this->keyNumber, vector<double>(this->keyNumber, 1));
				
	
		while(getline(input, tmp)){
			caract1 = tmp.substr(0,1);
			if(current != "" && caract1 != current){ //We have take care of every bigram that begin by current.		
				for(int i = 0; i < this->keyNumber; ++i){
					bigram[currentIndex - alphabet.begin()][i] /= total;		// normalization			
				}
				total = this->keyNumber;
			}
			if(current == "" || caract1 != current){	
				current = tmp.substr(0,1);
				currentIndex = find(alphabet.begin(), alphabet.end(), current);
				
				while(currentIndex == alphabet.end()){
					flag = getline(input, tmp);
					if(!flag)
						break;				
					current = tmp.substr(0,1);
					currentIndex = find(alphabet.begin(), alphabet.end(), current);
				} //If the first letter of the bigram is not in the alphabet we skip line until we find a nice one									
			}
			if(!flag)						
				break;

			caract2 = tmp.substr(1,1); //second caracter
			nbr = atof(tmp.substr(3).c_str());
			vector<string>::iterator index = find(alphabet.begin(), alphabet.end(), caract2);
			if(index != alphabet.end()){
				bigram[currentIndex - alphabet.begin()][index - alphabet.begin()] += nbr;
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
	else{
		this->invalidModel = true;
	}
}


