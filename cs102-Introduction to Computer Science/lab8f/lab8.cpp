//will hopkirk
//marz
//3/26/2019
//blackjack game
//cite alex baldwin

//lib headers
#include <iostream>
#include <string>
#include <iomanip>
#include <time.h>
#include <vector>

using namespace std;

//function prototypes
int GetRandom(int mn, int mx);
string DealCard(int d[], int p);
int ScoreHand(int d[], int p);

//global constants for the card values and card suits
const string CVALS[13] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
const string CSUITS[4] = {"H","D","S","C"};

int main(){

	        //initalizing vars for player&dealer, the deck, and for rand
			int deck[52] = {0};
			int dealer = 1;
			int player = 2;
			int dscore;
			int pscore;
			bool end = false;
			string act = "hit";
			srand(time(NULL));
		
			//initializing vectors for players hand and dealer's hand
			vector<string> phand;
			vector<string> dhand;
		
		
			//pushback to give player and dealer one card each initially
			phand.push_back((DealCard(deck, player)));
			dhand.push_back((DealCard(deck, dealer)));
			dhand.push_back((DealCard(deck, dealer)));

			//continually play game by hitting and getting cards for player
			do{
				cout << "Dealer has cards: "; 
				dscore = ScoreHand(deck, dealer);
				for(unsigned int i = 0; i < dhand.size(); i++){
					cout << dhand[i] << " ";
				}
				cout << setw(20) << left << " " << '(' << dscore << ')' << endl;
				cout << "You have cards  : ";
				phand.push_back(DealCard(deck, player));
				pscore = ScoreHand(deck, player);
				for(unsigned int i = 0; i < phand.size(); i++){
					cout << phand[i] << " ";
				}
				cout << setw(20) << left << " " << '(' << pscore << ')' << endl;
				if(pscore > 21){
					cout << "Player busts, dealer wins!" << endl;
					end = true;
					return -1;
				}
				if(pscore == 21){
					cout << "Player wins!" << endl;
					end = true;
					return -1;
				}

				//while(act != "hit" || act != "stand"){
				cout << "Hit or stand   ? ";
				cin >> act;
				//}
			}while(act == "hit");
			

			//same as player loop but for dealer after player stands
			if(end == false){
				do{
					if(dscore < 18){
						dhand.push_back(DealCard(deck, dealer));
					}
					else{
						end = true;
					}
					dscore = ScoreHand(deck, dealer);
					cout << "Dealer hits     :";
					for(unsigned int i = 0; i < dhand.size(); i++){
						cout << dhand[i] << " ";
					}
					cout << setw(20) << left << " " << '(' << dscore << ')' << endl;
					if(dscore > 21){
						cout << "Dealer busts, player wins!" << endl;
						end = true;
						return -1;
					}
				}while (end == false);	
			}

			//check win conditions
					if(dscore > 21){
						cout << "Dealer busts, player wins!" << endl;
					}
					else if(pscore > dscore){
						cout << "Player wins!" << endl;
					}
					else if(pscore < dscore){
						cout << "Dealer wins!" << endl;
					}
					else if(pscore == dscore){
						cout << "Player and dealer draw." << endl;
					}

}


//definition for getting a random number function
int GetRandom(int mn, int mx){

	int index = (mn + rand() % (mx-mn+1));
		return index;
}

//function to deal the cards to each player
string DealCard(int d[], int p){

	string card;
	int index;
	do{
		index = GetRandom(0, 51);
		card = CVALS[index % 13] + CSUITS[index / 13];
	}while (d[index] !=0);

	d[index] = p;
	return card;

}

//function to score the hand of each players cards
int ScoreHand(int d[], int p){

	int points = 0;
	for(int i = 0; i < 52; i++){
		if(d[i] == p){
			if((i % 13) > 9){
				points = (points + 10);
			}
			else{
				points += ((i%13) +1);
			}
		}
	}
	return points;

	
}

//tried extra credit, didnt work out so good...

