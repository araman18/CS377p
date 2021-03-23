#include <stdio.h>
#include <stdlib.h>
#include "conversions.cpp"
#include "pagerank.cpp"

using namespace std;

int main(int argv, char **argc) {
	char *desired_file_name = (char*)("test.dimacs");
	if(argv < 2) {
		printf("You need to specify the DIMACS file to be converted\n");
		exit(0);
	}
	if(argv > 2) {
		desired_file_name = argc[2];
	}

	char  *file_name = argc[1];
	vector<vector<int>> csr = DimacsToCsr(file_name);

	if(argv > 2) {
		CsrToDimacs(csr, desired_file_name);
		WriteNodeLabels(csr, desired_file_name);
	}

	vector<double> weights = pagerank(csr);
}
