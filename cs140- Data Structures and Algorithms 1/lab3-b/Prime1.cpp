#include <iostream>
#include <cstdio>

using namespace std;

bool isprime(int number) 
{
	//0 and 1 are NOT prime
	if((number == 1) || (number == 0)){

		return false;
	}
	//start with 2 and modulo till you get 0 if you do its a prime number
		for(int i = 2; i <= (number/2); i++){
			if((number % i) == 0){
				
				return false;
			}
		}	
	//if unable to get num mod i = 0 while i is less than square root then it is prime	
	return true;
};

int main()
{
	int number;

	//exhaust stdin and check if prime
	while (cin >> number) {
	  if (isprime(number)){

	    cout << number << " is a prime number" << endl;
	  }
    }
}
