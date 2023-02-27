#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

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
	sort(cache.begin(), cache.end());
	bool yeet;
	yeet = binary_search(cache.begin(), cache.end(), number);
	if(yeet == true){

		return true;
	}else{

		return false;
	}
}

//magical function checks if prime if so adding it to list
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

//returns random number between 1 and 140 inclusively
int random_number(){

	return ((rand() % 140) + 1);
}

//takes numbers and is_prime then loops through numbers and adds prime numbers to primes
void extract_prime(vector<int> &nums, vector<bool> &isp, vector<int> &ps)
{
    for(int i = 0; i < nums.size(); i++){

		if(isp[i] == 1){

			ps.push_back(nums[i]);
		}
	}
}

//prints in format
void print(vector<int> ps){

  int h = 0;
  for(int i = 0; i < ps.size(); i++){

	 h++;
	 cout << setw(4) << ps[i] << " ";
	 if(h == 20){

		 cout << endl;
		 h = 0;
	 }
	  
  }
  cout << endl;
}

int main(int argc, char *argv[])
{

	//gets N checking base case if N not given
	int N;

	if(argc < 2){

		N = 140;
	}else{
		
		istringstream sin;
		sin.str(argv[1]);
		sin >> N;
	}

	srand(N);

	//create and size vectors to N
	vector<int> numbers;
	vector<bool> is_prime;
	numbers.resize(N);
	is_prime.resize(N);

	//generate numbers list and copy it to prime list
	generate(numbers.begin(), numbers.end(), random_number);
	transform(numbers.begin(), numbers.end(), is_prime.begin(), isprime());

	cout << "Sequence contains " << count(is_prime.begin(), is_prime.end(), 1) << " prime numbers" << endl;
 
	cout << "All numbers in order appearance: " << endl;

	//new vector for only prime nums
	vector<int> primes;
	extract_prime(numbers, is_prime, primes);

	print(primes);

	cout << "Unique values in numerical order: " << endl;

	//make iterator for unique and sort then find unique nums and Eradicate all non-unique nums
	vector<int>::iterator p;
	sort(primes.begin(), primes.end());
	p = unique(primes.begin(), primes.end());
	primes.erase(p, primes.end());

	print(primes);
}
