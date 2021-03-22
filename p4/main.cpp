#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

using namespace std;


void PrintCSR(vector<vector<int>> &csr) {
	for(auto &v: csr){
		cout << "[";
	  for(int num: v){
			cout << num << ", ";
		}
		cout << "]" << endl;
	}
}

vector<vector<int>> DimacsToCsr(char *file_name) {
	ifstream file(file_name);

	string line;
	getline(file, line);

	vector<string> line_split;
  boost::split(line_split, line, boost::is_any_of(" "));

	int num_nodes = stoi(line_split[2]);

	map<int,unordered_map<int,int>> coo_representation;
	int num_duplicates = 0;
	while(getline(file, line)){
			boost::split(line_split, line, boost::is_any_of(" "));

			if(line_split[0] != "a") continue;

			int from = stoi(line_split[1]), to = stoi(line_split[2]), weight = stoi(line_split[3]);
			if(coo_representation.count(from) && coo_representation[from].count(to)){
				coo_representation[from][to] = max(coo_representation[from][to], weight);
				++num_duplicates;
			}else {
				coo_representation[from][to] = weight;
			}
	}

	printf("Number of duplicate edges: %d\n", num_duplicates);

	vector<vector<int>> csr(2, vector<int>(0));
	csr.push_back(vector<int>(num_nodes + 1, 0));

	for(int row_index = 1; row_index <= num_nodes; ++row_index) {

		if(!coo_representation.count(row_index)) {
			csr[2][row_index] = csr[2][row_index - 1];
		}else{
			unordered_map<int,int> outgoing_edges = coo_representation[row_index];
			int size = outgoing_edges.size();

			for(auto &it: outgoing_edges) {
				csr[0].push_back(it.second);
				csr[1].push_back(it.first);
			}
			csr[2][row_index] = csr[2][row_index  - 1] + size;
		}
	}

	//PrintCSR(csr);

	return csr;
}

void CsrToDimacs(vector<vector<int>> &csr, char *desired_file_name) {
	ofstream dimacs_file;
	//dimacs_file.open(file_name);

	int num_nodes = csr[2].size() - 1;
	int num_edges = csr[0].size();
	printf("Number of non duplicate edges %d\n", num_edges);

	
}



int main(int argv, char **argc) {
	if(argv < 2) {
		printf("You need to specify the DIMACS file to be converted\n");
		exit(0);
	}

	char  *file_name = argc[1];
	vector<vector<int>> csr = DimacsToCsr(file_name);

	char *desired_file_name = (char*)("test.dimacs");
	CsrToDimacs(csr, desired_file_name);
}
