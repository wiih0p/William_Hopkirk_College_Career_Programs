// lab 1 part 1

#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

int main(){

	//parameters for checkerboard
	int r;
	int c;
	int sc;
	int cs;
	int w;

	// error check vars
	bool err = false;
	bool end = false;
	string clear;

	//error check to see if input is valid else print error message
	while(!end){

		if((!(cin >> r)) || (!(cin >> c)) || (!(cin >> sc)) || (!(cin >> cs)) || (!(cin >> w))){
			err = true;
		}

		if(err){

			cin.clear();
			
			while(cin >> clear){
			}

			cout << "usage: checkerboard  - strdin should contain R, C, SC, CS and W" << endl;

			err = false;
		}else{

			end = true;
		}
	}
	
	//check if any are 0 or if starting char is illegal
	if((r <= 0) || (c <= 0) || (cs <= 0) || (w <= 0) || (sc > 127)){

		return -1;
	}

	// construct and print the checkerboard with rows and columns 
	for(unsigned int i = 0; i < r; i++){

		for(unsigned int j = 0; j < w; j++){

			for(unsigned int h = 0; h < c; h++){

				for(unsigned int k = 0; k < w; k++){

					cout << char(sc + (i + h) % cs);
				}
			}
			cout << endl;
		}
	}



	return 0;
}

