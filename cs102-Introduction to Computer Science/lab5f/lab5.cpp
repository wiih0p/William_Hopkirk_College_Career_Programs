//will hopkirk
//2-6-19
//marz
//slot machine

#include <iostream>
#include <limits>
#include <cstdlib>
#include <vector>

using namespace std;

int main(){

	//initialize variables and array for the spinner and the two vectors for the wagers and winnings
	int spin[3];
	int wallet = 1000;
	int turns = 0;
	vector <int> bets;
	vector <int> winnings;
	int tBet = 1;
	int tWin;
	int sBet = 400000;
	int sWin = 400000;
	int lBet = 0;
	int lWin = 0;
	int seed;
	const int mn = 2;
	const int mx = 7;

	//check for and get a seed from the user for randomizer
	do{
		cout << "Input a seed: ";
		cin >> seed;
		if(cin.fail()){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
		}
	}while(!seed);

	srand(seed);

	//overarhcing while loop for continuously checking and getting values from the slot
	while(wallet > 0){

		//displays current balance
			cout << "Your money: $" << wallet << endl;

		// gets and checks the wager and if the wager is zero end the game without playing	
		do{
			cout << "Place a bet: $";
			cin >> tBet;
			if(tBet == 0){
				cout << '\n';
				if(turns == 0){
				cout << "You walked away without playing." << endl;
				return 0;
				}
				//outputs game info after user ends the session
				else if(turns > 0){
					for(unsigned int i = 0; i < bets.size(); i++){
						cout << "You wagered $" << bets.at(i);
						cout <<" and won $" << winnings.at(i) << endl;
					}
					cout << '\n';
					cout << "Your smallest wager was $" << sBet << endl;
					cout << "Your largest wager was $" << lBet << endl;
					cout << "Your smallest winnings was $" << sWin << endl;
					cout << "Your largest winnings was $" << lWin << endl;
								    
				    return 0;
				}
			}
			else if(cin.fail()){
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(),'\n');
			}
		}while(!tBet);
		
		//series of ifs for getting the smallest and largest wagers
		if(tBet > 0){
			turns++;
		}
		if(tBet > lBet){
			lBet = tBet;
		}
		else if(tBet < sBet){
			sBet = tBet;
		}

		// adds the wager tp the vector of wagers
		bets.push_back(tBet);

		//gets the random values for the spinner between 2 and 7
		for(int i = 0; i < 3; i++){

			spin[i] = (mn + rand() % (mx - mn + 1));
		}

		cout <<" " << spin[0] << " " << spin[1] << " "  << spin [2] << endl;

		//series of if statements to check win condition
		if(spin[0] != spin[1] && spin[1] != spin[2]){

			cout << "You didn't win." << endl << '\n';
			wallet -= tBet;
			tWin = 0;
			winnings.push_back(tWin);
			sWin = 0;
			
		}
		else if(spin[0] == spin[1] && spin[0] == spin[2] && spin[0] != 7) {

			cout << "You won $" << (tBet * 5) << '!' << endl << '\n';
			wallet += (tBet * 5);
			tWin = (tBet * 5);
			if(tWin > lWin){
				lWin = tWin;
			}
			if(tWin < sWin){
				sWin = tWin;
			}
			winnings.push_back(tWin);
		}
		else if((spin[0] == spin[1] || spin[0] == spin[2] || spin[1] == spin[2]) && (!(spin[0] == spin[1] && spin[0] == spin[2]))){

			cout << "You won $" << (tBet * 2) << '!' << endl << '\n';
			wallet += (tBet *2);
			tWin = (tBet * 2);
			if(tWin > lWin){
				lWin = tWin;
			}
			if(tWin < sWin){
				sWin = tWin;
			}
			winnings.push_back(tWin);
		}
		else if(spin[0] == 7 && spin[1] == 7 && spin[2] == 7){

			cout << "You won $" << (tBet * 10) << '!' << endl << '\n';
			wallet += (tBet *10);
			tWin = (tBet * 10);
			if(tWin > lWin){
				lWin = tWin;
			}
			if(tWin < sWin){
				sWin = tWin;
			}
			winnings.push_back(tWin);
		}

	}

	//check if user ran out of money
	if(wallet <= 0){
		cout << "You are out of money!" << endl << '\n';
	}
	
	//gives player game results
	if(turns > 0){
		for(unsigned int i = 0; i < bets.size(); i++){
			cout << "You wagered $" << bets.at(i);
			cout <<" and won $" << winnings.at(i) << endl;
		}
	
		cout << '\n';
	cout << "Your smallest wager was $" << sBet << endl;
	cout << "Your largest wager was $" << lBet << endl;
	cout << "Your smallest winnings was $" << sWin << endl;
	cout << "Your largest winnings was $" << lWin << endl;
	}
}
	


