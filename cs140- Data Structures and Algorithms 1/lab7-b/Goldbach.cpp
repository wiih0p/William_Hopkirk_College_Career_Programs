#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

//class definition
class prime_partition {
	public:
		// constructor
		prime_partition();
	// operator(int)
	void operator()(int);

	private:
	void create_pset();
	// compute_partitions
	bool isprime(int number);
	void compute_partitions(vector<int> &, const int &, int = 0);
	// print_partitions
	void print_partitions();

	set<int> pset;
	int pset_min, pset_max;
	int max_terms;
	// miscl member data

	vector< vector<int> > partitions;
};

//same isprime function from lab3
bool prime_partition::isprime(int number){

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

//create list of prime numbers
void prime_partition::create_pset(){

	pset.clear();
	
	for(int i = 2; i < 2000; i++){

		if(isprime(i)){

			pset.insert(i);
		}
	}

}

//just calls create pset
prime_partition::prime_partition(){

	create_pset();
}

//sets max terms and sets up recursion
void prime_partition::operator()(int num){

	if(num < 2 || num >= 2000){

		return;
	}
	if(num % 2 == 0){

		max_terms = 2;
	}else{

		max_terms = 3;
	}

	partitions.clear();

	vector<int> tmp;

	compute_partitions(tmp, num, 0);
	print_partitions();
}

//prints out partitions 2d vector
void prime_partition::print_partitions(){

	for(int i = 0; i < partitions.size(); i++){
		for(int h = 0; h < partitions[i].size(); h++){

			cout << partitions[i][h] << endl;
		}
	}

}

//recursive solver to find primes that add to target value
void prime_partition::compute_partitions(vector<int> &numbers, const int &target, int sum){

	if(sum == target){
		if(numbers.size() < max_terms){

			//a new min sequence size was found
			max_terms = numbers.size();
			partitions.clear();
			partitions.push_back(numbers);
			return;
		}
	}
	if(sum > target || numbers.size() > max_terms){

		return;
	}
	
	set<int>::iterator start = pset.begin();
	set<int>::iterator end;

	if(numbers.size() == 0){

		end = upper_bound(pset.begin(), pset.end(), target);
	}else{

		end = upper_bound(pset.begin(), pset.end(), numbers[numbers.size()]);
	}

	set<int>::iterator ii;

	for(ii = start; ii != end; ii++){

		numbers.push_back(*ii);
		compute_partitions(numbers, target, (sum + *ii));
		numbers.pop_back();
	}
}

int main(int argc, char *argv[])
{
	prime_partition goldbach;

	int number;
	while (1) {
		cout << "number = ";
		cin >> number;
		if (cin.eof())
			break;

		goldbach(number);
	}

	cout << "\n";
}
