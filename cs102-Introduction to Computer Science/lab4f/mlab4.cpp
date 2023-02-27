//will hopkirk
//mlab4
//1/29/19
//banking application
//cite alex baldwin

#include <iostream>
#include <string>

using namespace std;

int main(){

	double accBal;
	double addBal;
	string option;

	cout << "Enter your initial account amount: ";
	cin >> accBal;

	while( option != "quit"){

		cout << "What do you want to do? ";
		cin >> option;

		if(option == "deposit" || option == "balance" || option == "withdrawl" || option == "quit"){

		if(option == "deposit"){

			cout << "How much? ";
			cin >> addBal;
			(accBal += addBal);
		}
		else if(option == "balance"){

			cout << "You have $" << accBal << '\n';
		}
		else if(option == "withdrawl"){

			cout << "How much? ";
			cin >> addBal;
			(accBal -= addBal);	
		}
		}
		else{

			cout << "invalid input. exiting bank.\n";
			option = "quit";
		}
	}
		









}
