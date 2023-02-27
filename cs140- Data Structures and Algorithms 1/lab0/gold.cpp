//lab0.5

#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

int main(){

	char nugget;
	int ounces;

	while(cin >> nugget){

		if((nugget == '.') || (nugget == '-')){

		}
		else{
			
			ounces += (nugget - 64);
		}
	}

	cout << ounces << endl;

	return 0;
}

