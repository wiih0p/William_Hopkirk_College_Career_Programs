
#include <cstdio>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct peep{

	vector<int> lines;
};

int main(){

	vector<int> yeet;

	int num_inuse = 5;

	yeet.push_back(3);
	yeet.push_back(4);
	yeet.push_back(6);

	peep peebles;
	peebles.lines.push_back(5);

	cout << peebles.lines[0] << endl;



	cout << "**N =" << setw(6) << yeet.size() << " load = " << setprecision(3) << ((double)num_inuse/(double)yeet.size()) << endl;

	return 0;
}

