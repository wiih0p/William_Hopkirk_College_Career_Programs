//lab1c


#include <cstdio>
#include <iostream>

using namespace std;

//class definition
class stats{
	
	public:
		stats();
		void push(int);
		void print() const;

	private:	
		int n;
		int sum;
		int min;
		int max;
		int tmp;
};

int main(){

	//setting temp var and instantiating/constructing obj
	int temp;
	stats stat;
	stats();

	//looping while taking data
	while(cin){

		//calling push function
		cin >> temp;
		stat.push(temp);
	}
	
	//calling print function
	stat.print();


	return 0;
}

//constructor
stats::stats(){

	n = -1;
	sum = 0;
	min = 0;
	max = 0;
	tmp = 0;
}

//same as Prog1b just in a function
void stats::push(int temp){

	n++;
	tmp = temp;
	sum = (sum + temp);
	if(min == 0){

		min = temp;
	}
	if(min > temp){

		min = temp;
	}
	if(max < temp){

		max = temp;
	}
	
}

//same print as Prog1b just in a function
void stats::print() const{

	cout << "N   = " << n << endl;
	cout << "sum = " << (sum - tmp) << endl;
	cout << "min = " << min << endl;
	cout << "max = " << max << endl;
}

