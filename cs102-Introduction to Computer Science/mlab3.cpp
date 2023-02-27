//will hopkirk
//pre lab 3
//calculator that adds or multiplies 2 numbers
//1-22-19

#include <iostream>

using namespace std;

int main(){

	int num1;
	int num2;
	char calc;

	cout << "enter your first number: \n";
	cin >> num1;
	cout << "enter your second number: \n";
	cin >> num2;
	cout << "enter an a for addition or an m for multiplication: \n";
	cin >> calc;

	switch (calc){

		case 'a':
			    cout << num1 + num2 << "\n";
				break;
	
		case 'm':
				cout << num1 * num2 << "\n";
				break;

		default: cout << "yikes, try again" << "\n"; 
	}
    
	

}


