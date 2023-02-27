
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

#include "Rnumgen.h"

//from handout
using namespace std;

int main(int argc, char *argv[]) {  
	if (argc < 4) {    

		cerr << "usage: " << argv[0]          << " seed N label1=ratio1 ... labelN=ratioN\n";    

		return 0;  
	}  

	int seedvalue = atoi(argv[1]);  
	int N = atoi(argv[2]);  

	vector<string> label;  
	vector<int> ratio;  
	for (int i=3; i<argc; i++) {    

		string new_label = strtok(argv[i], "=");
		int new_ratio = atoi(strtok(NULL, "="));
		label.push_back(string(new_label));
		ratio.push_back(new_ratio);  
	}  
	if (seedvalue == 0) seedvalue = time(NULL);  
	rnumgen RNG(seedvalue, ratio);  
	for (int i=0; i<N; i++)    
		cout << " " << label[RNG.rand()] << "\n";
}
