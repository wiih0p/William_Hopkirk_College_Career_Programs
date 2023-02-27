//lab1b
//will Hopkirk

#include <cstdio>
#include <iostream>

using namespace std;

int main(){

	//setting vars
	int n = -1;
	int tmp;
	int sum = 0;
	int min = 0;
	int max = 0;

	//loop while taking input
	while(cin){

		//add to n and sum and check for min max
		n++;
		cin >> tmp;
		sum = (sum + tmp);
		if(min == 0){

			min = tmp;
		}
		if(min > tmp){

			min = tmp;
		}
		if(max < tmp){

			max = tmp;
		}
	}

	//print data in format provided
	//sum kept adding the last value twice so i subtracted the last read value from the total in print
	cout << "N   = " << n << endl;
	cout << "sum = " << (sum - tmp) << endl;
	cout << "min = " << min << endl;
	cout << "max = " << max << endl;

	return 0;
}

