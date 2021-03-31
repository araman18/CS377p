#include <stdio.h>
#include <stdlib.h>
#include "conversions.cpp"
#include "pagerank.cpp"

using namespace std;

int main(int argv, char **argc) {
	if(argv < 2) {
		printf("You need to specify the DIMACS file to be converted\n");
		exit(0);
	}
	char *file_name = argc[1];
	string name(file_name);

	vector<string> to_process;
	if(name == "all") {
		to_process.push_back("wiki.dimacs");
		to_process.push_back("road-NY.dimacs");
		to_process.push_back("rmat15.dimacs");
	} else {
		to_process.push_back(name);
	}
	for(string &str: to_process) {

		const char *to_read = str.c_str();
		vector<vector<int>> csr = DimacsToCsr(to_read);


		str = str.substr(0, str.size() - 7);

		string desired_file_name = (str + "csr.txt");
		CsrToDimacs(csr, desired_file_name.c_str());


		string hist_file_name = (str + "hist.txt");
		histogramLabels(csr, hist_file_name.c_str());

		vector<double> weights = pagerank(csr);

		string label_file_name = (str + "labels.txt");
		writeLables(weights, label_file_name.c_str());
	}


}
