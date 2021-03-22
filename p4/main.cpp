#include <stdio.h>
#include <stdlib.h>
#include "conversions.cpp"

using namespace std;

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
