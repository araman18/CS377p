#include <stdio.h>
#include <stdlib.h>
#include "conversions.cpp"
#include "pagerank.cpp"

using namespace std;

int main(int argv, char **argc) {
	char *desired_file_name;
	char* label_file_name;
	if(argv < 2) {
		printf("You need to specify the DIMACS file to be converted\n");
		exit(0);
	}
	char  *file_name = argc[1];
	vector<vector<int>> csr = DimacsToCsr(file_name);

	if(argv > 2) {
		desired_file_name = argc[2];
		CsrToDimacs(csr, desired_file_name);
		if(argv > 3) {
			label_file_name = argc[3];
			WriteNodeLabels(csr, label_file_name);
		}
	}


	vector<double> weights = pagerank(csr);
}
