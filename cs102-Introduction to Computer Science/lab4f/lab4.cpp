//will hopkirk
//lab 4
//2-6-19
//make a fleet of cars and trucks
//cite: alex baldwin

#include <iostream>
#include <limits>
#include <string>

using namespace std;

int main(){

	//initializing variables
	double cmi;
	double cgall;
	double tgall;
	double tmi;
	bool cars = false;
	bool trucks = false;
	string cmd;

	//large while loop to ask for commands until the user types "done"
	while(cmd != "done"){
		
		//gets command input
		cout << "Enter a command: ";
		cin >> cmd;

		
		if(cmd == "car"){
			
			//if the user types "car" sets that the fleet has cars to true
			cars = true;

			//loop that gets and checks for the car's miles
			do{
				cout << "Enter car's miles: ";
				cin >> cmi;
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
				}
			}while(!cmi);

			//loop that gets and checks for the car's gallons
			do{
				cout << "Enter car's gallons: ";
				cin >> cgall;
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
				}
			}while(!cgall);
					
		}
		if(cmd == "truck"){
			
			//if the user types "truck" sets that the fleet has trucks to true
			trucks = true;

			//loop that gets and checks for the truck's miles
			do{
				cout << "Enter truck's miles: ";
				cin >> tmi;
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
				}
			}while(!tmi);

			//loop that gets and checks for the truck's gallons
			do{
				cout << "Enter truck's gallons: ";
				cin >> tgall;
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
				}
			}while(!tgall);

		}
	
		//fail statement to retry to get command from user if the command is not one for this code
		if(cin.fail()){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
		}
	}
	
	//checking to see if there are cars or trucks in the fleet and if so printing out the avg mile per gallon for each
	if(cars == true){
		cout << "Average car MPG = " << (cmi/cgall) << endl;
	}
	else{
		cout << "Fleet has no cars." << endl;
	}
	if(trucks == true){
		cout << "Average truck MPG = " << (tmi/tgall) << endl;
	}
	else{
		cout << "Fleet has no trucks." << endl;
	}
}









