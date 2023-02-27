//lab1d
//will Hopkirk


#include <cstdio>
#include <ctype.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

//function prototypes
void enc(string &input, int shift);
void dec(string &input, int shift);


int main(int argc, char** argv){

	//check for right ammount of args
	if(argc != 3){

		cerr << "Usage: ./Prog1d (-encode/-decode) (0-9)" << endl;
		return -1;
	}

	//vars for command and shift
	string cmd;
	int sft;

	//stringstream in shift as an int and later command as a string
	istringstream sin;
	sin.str(argv[2]);
	sin >> sft;

	//check if shift is valid
	if(sft > 9 || sft < 0){

		cerr << "Usage: ./Prog1d (-encode/-decode) (0-9)" << endl;
		return -1;
	}

	sin.clear();
	sin.str(argv[1]);
	sin >> cmd;

	//init input string
	string inp;

	//check command take in the input line and run function
	if(cmd == "-encode"){

		getline(cin, inp);
		enc(inp, sft);
	}
	else if(cmd == "-decode"){

		getline(cin, inp);
		dec(inp, sft);
	}

	//last check to see if command was correct
	else{

		cerr << "Usage: ./Prog1d (-encode/-decode) (0-9)" << endl;
		return -1;
	}


}


void enc(string &input, int shift){

	//loop through line ignoring non-alpha chars and adjusting upper and lower case letters via ascii
	for(int i = 0; i < input.length(); i++){

		if(isalpha(input[i])){

			if(isupper(input[i])){

				input[i] = (input[i] + shift);
				if(input[i] > 90){
					input[i] = (input[i] - 26);
				}

			}

			if(islower(input[i])){

				input[i] = (input[i] + shift);
				if(input[i] > 122){
					input[i] = (input[i] - 26);
				}
			}
		}

	}

	//print results
	cout << input << endl;
}

void dec(string &input, int shift){


	//same as encode but subtracting shift and checking overload adjustment the other way
	for(int i = 0; i < input.length(); i++){

		if(isalpha(input[i])){

			if(isupper(input[i])){

				input[i] = (input[i] - shift);
				if(input[i] < 65){
					input[i] = (input[i] + 26);
				}

			}

			if(islower(input[i])){

				input[i] = (input[i] - shift);
				if(input[i] < 97){
					input[i] = (input[i] + 26);
				}
			}
		}

	}


	cout << input << endl;

}
