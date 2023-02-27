//will hopkirk
//marz
//2/27/2019
//blackjack game
//cite alex baldwin

//headers for libraries needed
#include <iostream>
#include <string>
#include <iomanip>
#include <time.h>

using namespace std;

//function prototypes
int GetRandom(int mn, int mx);
string DealCard(int d[], int p);
int ScoreHand(int d[], int p);

//global constants for the card values and card suits
const string CVALS[13] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
const string CSUITS[4] = {"H","D","S","C"};

int main(){

	//initalizing vars for players, the deck, and for rand
	int deck[52] = {0};
	int players;
	srand(time(NULL));

	//initializing the cards as well as the value of the players hand
	string c1;
	string c2;
	int hand;

	//ask player for number of players
	cout << "How many players? ";
	cin >> players;
	players++;

	//calling functions to get cards and the hand value
	for(int i = 1; i < players; i++){


		c1 = DealCard(deck, i);
		c2 = DealCard(deck, i);
		hand = ScoreHand(deck, i);
		cout << "Player " << i << " has cards: ";
		cout << c1 << " ";
		cout << setw(20) << left << c2 << '(' << hand << ')' << endl;
	}
}

//definition for getting a random number function
int GetRandom(int mn, int mx){

	int index = (mn + rand() % (mx-mn+1));
	return index;
}

//function to deal the cards to each player
string DealCard(int d[], int p){

	string hand;
	int index = GetRandom(0, 51);
	if(d[index] == 0){
		hand = CVALS[index % 13] + CSUITS[index % 13];
		d[index] = p;
	}
	return hand;

}

//function to score the hand of each players cards
int ScoreHand(int d[], int p){

	int points = 0;
	for(int i = 0; i <= 51; i++){
		if(d[i] == p){
			if((i % 13) >= 10){
				points = (points + 10);
			}
			else{
				points += (i%13) +1;
			}
		}
	}
	return points;
}
