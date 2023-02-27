#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include "PPM.h"

void PPM::read(string file) { 
  
	ifstream fin;
	fin.open(file.c_str());

	if(!(fin.is_open())){

		cerr << "bad file" << endl;
		return;
		}

	istringstream sin;
	string line;
	string p66;

	getline(fin, line);

	if(p66 != "P6"){

		cerr << "bad file" << endl;
		return;
	}

	int width;
	int height;
	int maxin;
	int tmps;
	vector<int> tmp;
	RGB rgb;
	bool dimen;
	bool mxi = false;
	
	while(getline(fin, line)){

		if(!mxi && !dimen){

			sin >> width >> height;
			int dimension = width * height;
			img.reserve(dimension);

			dimen = true;
		}
		else if(dimen && !mxi){

			sin >> maxin;
			mxi = true;
		}else{

			while(sin >> tmps){

				tmp.push_back(tmps);
			}
		}

		sin.clear();
	}

	for(unsigned int i = 0; i < img.size(); i++){

		RGB pix;

		int p = (i % 3);

		if(p == 0){

			pix.R = tmp[i];

		}
		if(p == 1){

			pix.G = tmp[i];
		}
		if(p == 2){

			pix.B = tmp[i];
		}
	}




}

void PPM::write(string file) { 
 
	ofstream fout;
	fout.open(file.c_str());

	fout << p6 << '\n' << w << " " << h << '\n' << maxi << '\n';

	for(unsigned int i = 0; i <= img.size(); i++){

		fout << img[i].R << " " << img[i].G << " " << img[i].B << '\n';
	}

}

int main(int argc, char **argv){

	return 0;
}



