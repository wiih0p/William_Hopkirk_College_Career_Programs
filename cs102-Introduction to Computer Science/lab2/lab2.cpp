//will hopkirk
//lab 2
//1-16-19
//ask for name, age, and adress. first name, age, and adress are modified in various ways in output

//adding libraries
#include <iostream>

#include <string>

using namespace std;

//main function
int main(){

	//initializing variables
	string firstname;
	string middlename;
	string streetname;
	string streettype;
	int age;
	int streetnumber;

	//series of cout and cin statements to obtain user's first and middle name, age, and address information
	cout<<"Enter your first and your middle names:";

	cin>>firstname>>middlename;

	cout<<"Enter your age:";

	cin>>age;

	cout<<"Enter your street number, name, and type:";

	cin>>streetnumber>>streetname>>streettype;

	//outputs the user's name as the address street name as well as their middle name
	cout<<"Your movie star name is "<<streetname<<" "<<middlename<<". \n";
	
	//outputs the age of the user as the remainder of their numerical address divided by their age times 3 
	cout<<"you will play a "<<((streetnumber%age)*3)<<" year old. \n";
	
	//outputs the user's address as their age times 700 divided by their numerical address with their first name as the street name and the street type remaining unchanged
    cout<<"your address is "<<((age*700)/streetnumber)<<" "<<firstname<<" "<<streettype<<". \n";

	return 0;
}
