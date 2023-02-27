#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

const string face[] = { "Ace", "2", "3", "4", "5", "6", "7",
                        "8", "9", "10", "Jack", "Queen", "King" }; 
const string suit[] = { "Clubs", "Diamonds", "Hearts", "Spades" };

string random_card(bool verbose=false) {
	string card;

	card = face[ rand()%13 ];
	card += " of ";
	card += suit[ rand()%4 ];

	if (verbose)
	  cout << card << "\n";

	return card;
}

int main(int argc, char *argv[])
{
	bool verbose = false;
	int seedvalue = 0;

	for (int i=1; i<argc; i++) {
	  string option = argv[i];
	  if (option.compare(0,6,"-seed=") == 0) {
	    seedvalue = atoi(&argv[i][6]);
	  } else if (option.compare("-verbose") == 0) {
	    verbose = true;
	  } else 
	    cout << "option " << argv[i] << " ignored\n";
	}

	srand(seedvalue);

	//init table to have all 0s make a tmp var for ** check and a bool to break
	int table[4][13] = {};
	int tmp;
	bool over = false;
	while (1) {

		//check if done
	  if(over){
		  break;
	  }

	  string card = random_card(verbose);
	  
	  //getting string data with istringstream
	  string f;
	  string junk;
	  string s;
	  istringstream c;
	  c.str(card);
	  c >> f;
	  c >> junk;
	  c >> s;

	  //go through and fint the spot in the table that corresponds to the card values then add 1 card to that slot
	  for(int h = 0; h < 4; h++){

		  if(suit[h] == s){
			 for(int i = 0; i < 13; i++){

				 if(face[i] == f){

					 table[h][i]++;
				 }
			 }
		  }
	  }

	//check for break conditions
	for(int h = 0; h < 4; h++){
		
		if((table[h][10] > 0) && (table[h][11] > 0) && (table[h][12] > 0)){

			tmp = h;
			over = true;
		}
	}


      // break out of loop if stopping criteria met
	}

	//all the rest of this is to print in the correct format i used tmp to keep track of where the ** goes
	cout << "   Clubs :";
	for(int i = 0; i < 13; i++){

		cout << "   ";
		cout << table[0][i];
	}
	if(tmp == 0){

		cout << " **";
	}
	cout << endl;

	cout << "Diamonds :";
	for(int i = 0; i < 13; i++){

		cout << "   ";
		cout << table[1][i];
	}
	if(tmp == 1){

		cout << " **";
	}
	cout << endl;

	cout << "  Hearts :";
	for(int i = 0; i < 13; i++){

		cout << "   ";
		cout << table[2][i];
	}
	if(tmp == 2){

		cout << " **";
	}
	cout << endl;

	cout << "  Spades :";
	for(int i = 0; i < 13; i++){

		cout << "   ";
		cout << table[3][i];
	}
	if(tmp == 3){

		cout << " **";
	}
	cout << endl;
 
}
