#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

class isprime {
  public:
    isprime();
    bool operator()(int);

  private:
    void magic(int x);
	bool is_prime(int x);
	vector<int> cache;
};

//adds 2 to cache to start
isprime::isprime(){

	cache.push_back(2);

}

//calls magical function then tries to find() number in cache if not in cache return false if in cache return true
bool isprime::operator()(int number){

	magic(number);
	vector<int>::iterator p;
	p = find(cache.begin(), cache.end(), number);
	if(p != cache.end()){

		return true;
	}else{

		return false;
	}
}

//magical function first checks if given num is in cache (if already in cache exit function without adding to cache) if num isnt in cache and is prime then num is added to cache
void isprime::magic(int x){

	vector<int>::iterator p;
	p = find(cache.begin(), cache.end(), x);
	if(p != cache.end()){

		return;
	}
	if(is_prime(x)){

		cache.push_back(x);
	}

}

//same function as Prime1.cpp
bool isprime::is_prime(int number){

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

}

int main()
{
	isprime pcheck;
	int number;

	while (cin >> number) {
	  if (pcheck(number)){

	    cout << number << " is a prime number" << endl;
	  }
	}
}
