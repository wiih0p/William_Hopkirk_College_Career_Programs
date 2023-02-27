#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <set>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <utility>
#include <sstream>
#include <cmath>
#include <map>

using namespace std;

//LCS class
class LCS {
	public:
		void text1_push_back(char *f);
		void text2_push_back(char *f);
		void compute_alignment();
		//void report_difference();

		//private:
		// support functions
		vector<string> text1; 
		vector<string> text2; 
		vector<string> ops;

		//lcs cost function

};

//reading in text buffers
void LCS::text1_push_back(char *f){

	ifstream fin;

	fin.open(f);

	string tmp1;

	while(getline(fin, tmp1)){

		//cout << tmp1 << endl;
			this->text1.push_back(tmp1);

	}

	fin.close();
}

void LCS::text2_push_back(char *f){

	ifstream fin;

	fin.open(f);

	string tmp2;

	while(getline(fin, tmp2)){

		//cout << tmp2 << endl;
		
			this->text2.push_back(tmp2);

	}

	fin.close();
}

//added all functionality in one function
void LCS::compute_alignment(){

	vector<string> ex = this->text1;
	vector<string> ey = this->text2;

	int m = ex.size();
	int n = ey.size();

	//vector< vector<int> > cost;
	//vector< vector<int> > link;

//	cost.assign(m+1, n+1);
//	link.assign(m+1, n+1);

//	cost[0][0] = 0;
//	link[0][0] = 0;

	unsigned int j = 0;

	for(unsigned int i = 0; i < ey.size(); i++){

		//if not a match
		if(ey[i] != ex[j]){

			//add case
			if(ex[j] == ""){

				//add inserts from txt2 into txt1
				cout << j << "a" << (i + 1) << endl;
				cout << "> " << ey[i] << endl;
				j--;
			}
			//change case
			else{

				//change takes all lines till a break in the file deletes from txt1 and adds from txt2
				vector<string> tmpx;
				vector<string> tmpy;
				unsigned int tmp = 0;
				bool skp = false;
				for(unsigned int h = (i - 1); ex[h] != ""; h++){

					tmpx.push_back(ex[h]);

					if(ey[h] == ""){

						tmpy.push_back(ey[(h + 1)]);
						skp = true;
					}
					else if(skp == true){

						tmpy.push_back(ey[(h + 1)]);
					}
					else{

						tmpy.push_back(ey[h]);
					}
					tmp = (h - (i - 1));
				}

				cout << (j + 1) << "," << (j + tmp + 1) << "c" << (i + 1) << "," << (i + tmp + 1) << endl;
				for(unsigned int x = 0; x < tmpx.size(); x++){

					cout << "< " << tmpx[x] << endl;
				}
				cout << "---" << endl;
				for(unsigned int y = 0; y < tmpy.size(); y++){

					cout << "> " << tmpy[y] << endl;
				}
				
				i += tmp;
				j += tmp;
			}	
		
		}
		j++;
	}

	//delete case
	if(j < ex.size()){

		//delete checks if there is leftover in txt1 and takes it out
		cout << n << "," << m << "d" << n << endl;
		for(unsigned int i = (n - 1); i < m; i++){

			cout << "< " << ex[i] << endl;
		}
	}
		

}




int main(int argc, char *argv[])
{

	//err check
	if(argc != 3){

		cout << "USAGE: ./Diff8 file1.txt file2.txt" << endl;
	}

	//read int files and perform operations
	char *file1 = argv[1];
	char *file2 = argv[2];

	LCS lcs;

	lcs.text1_push_back(file1);
	lcs.text2_push_back(file2);

	lcs.compute_alignment();

	// check two input files are specified on command line

	//LCS lcs;  // instantiate your "LCS based diff" object

	// read the text from file1 into the lcs.text1 buffer
	// read the text from file2 into the lcs.text2 buffer

	//lcs.compute_alignment();
	//lcs.report_difference();
}
