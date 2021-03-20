#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

using namespace std;


void printCSR(vector<vector<int>> &csr) {
	for(auto &v: csr){
		cout << "[";
	  for(int num: v){
			cout << num << ", ";
		}
		cout << "]" << endl;
	}
}

vector<vector<int>> make_csr(char *file_name) {
	ifstream file(file_name);

	string line;
	getline(file, line);

	vector<string> lineSplit;


	map<int,unordered_map<int,int>> cooRepresentation;

	while(getline(file, line)){
			boost::split(lineSplit, line, boost::is_any_of(" "));

			if(lineSplit[0] == "c") continue;

			int from = stoi(lineSplit[1]), to = stoi(lineSplit[2]), weight = stoi(lineSplit[3]);
			if(cooRepresentation.count(from) && cooRepresentation.count(to)){
				cooRepresentation[from][to] = max(cooRepresentation[from][to], weight);
			}else {
				cooRepresentation[from][to] = weight;
			}
	}

	vector<vector<int>> csr(3, vector<int>(0));
	csr[2].push_back(1);

	for(auto &it: cooRepresentation) {
		int row = it.first;
		int size = it.second.size();
		csr[2].push_back(csr[2].back() + size);

		unordered_map<int,int> colWeightMap = it.second;
		for(auto cols: colWeightMap) {
			csr[0].push_back(cols.second);
			csr[1].push_back(cols.first);
		}
	}

	printCSR(csr);

	return csr;
}



int main(int argv, char **argc) {
	if(argv < 2) {
		printf("You need to specify the DIMACS file to be converted\n");
		exit(0);
	}

	char  *file_name = argc[1];
	vector<vector<int>> csr = make_csr(file_name);
}
