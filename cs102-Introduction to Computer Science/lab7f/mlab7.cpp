//will hopkirk
//marz
//2/27/2019
//playing card locater

#include <iostream>
#include <string>

using namespace std;

int main(){


	string deck[52] = {"2 of Hearts", "3 of Hearts", "4 of Hearts", "5 of Hearts", "6 of Hearts", "7 of Hearts", "8 of Hearts", "9 of Hearts", "10 of Hearts", "jack of Hearts", "queen of Hearts", "king of Hearts", "ace of Hearts", "2 of Diamonds", "3 of Diamonds", "4 of Diamonds", "5 of Diamonds", "6 of Diamonds", "7 of Diamonds", "8 of Diamonds", "9 of Diamonds", "10 of Diamonds", "jack of Diamonds", "queen of Diamonds", "king of Diamonds", "ace of Diamonds",  "2 of Spades", "3 of Spades", "4 of Spades", "5 of Spades", "6 of Spades", "7 of Spades", "8 of Spades", "9 of Spades", "10 of Spades", "jack of Spades", "queen of Spades", "king of Spades", "ace of Spades", "2 of Clubs", "3 of Clubs", "4 of Clubs", "5 of Clubs", "6 of Clubs", "7 of Clubs", "8 of Clubs", "9 of Clubs", "10 of Clubs", "jack of Clubs", "queen of Clubs", "king of Clubs", "ace of Clubs"};

	int index;
	

	cout << "Enter a card index: ";
	cin >> index;

	if(index < 52 && index > -1){
	cout << "That's the " << deck[index] << endl;
	}
	else{
		cout << "index must be within deck (0-51)" << endl;
	}








}
