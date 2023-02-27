//will hopkirk
//lab3
//calculator that takes two numbers and adds, subtacts, multiplies or divides depending on user input
//1-27-19
//citation - Alex Baldwin

#include <iostream>

using namespace std;

int main(){

	// initializing local variables
	double num1;
	double num2;
	char op;

	//ask user for first number
	cout << "Enter left operand: ";

	//check if the entry is a number
	if (!(cin >> num1)){

		cout << "Invalid left operand.\n";

		return -1;
	}

	//ask user for an operation
	cout << "Enter operation: ";

	cin >> op;

	//check if the given character matches an operation
	switch(op){
		
		case 'a':
		case 'A':
		case '+':
		case 's':
		case 'S':
		case '-':
		case 'm':
		case 'M':
		case '*':
		case 'x':
		case 'd':
		case 'D':
		case '/':
			break;
		default:
			cout << "Unknown operation.\n";
			return -1;

	}
	
	//ask user for second number
	cout << "Enter right operand: ";

	//check if given number is a number
	if (!(cin >> num2)){

		cout << "Invalid right operand.\n";

		return -1;

	}

	//check for operation and complete it
	switch (op){

		case 'a':
		case 'A':
		case '+':

			cout << "Result = " << num1 + num2 << '\n';
			break;

		case 's':
		case 'S':
		case '-':

			cout << "Result = " << num1 - num2 << '\n';
			break;
		
		case 'm':
		case 'M':
		case 'x':
		case '*':

			cout << "Result = " << num1 * num2 << '\n';
			break;

		case 'd':
		case 'D':
		case '/':

			cout << "Result = " << num1 / num2 << '\n';
			break;
		}	

	return 0;






}
