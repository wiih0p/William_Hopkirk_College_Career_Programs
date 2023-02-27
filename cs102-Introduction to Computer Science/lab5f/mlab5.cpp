//will hopkirk
//2/5/19
//marz
//adds a random list of numbers between -10 and 10

#include <iostream>
#include <limits>

using namespace std;

const int mn = -10;
const int mx = 10;
const int LENGTH = 25;
int main(){

	int list[LENGTH];
	int seed;
	int num;

	do{
	cout << "Enter a seed: ";
	cin >> seed;
		if(cin.fail()){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
		}
	}while(!seed);

	srand(seed);


	for(int i = 0; i < LENGTH; i++){
		
		list[i]=(mn + rand() % (mx - mn+1));
		num = (num + list[i]);
	}

	cout << "sum of array = " << num << '.' << '\n';

	return 0;

		





















}
