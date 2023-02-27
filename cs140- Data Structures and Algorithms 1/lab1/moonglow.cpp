//lab 1 part 2

#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

int main(){

	//for keeping scores and averages and and names
	double grd = 0;
	double fgrd = 0;
	double avg = 0;
	double favg = 0;
	string name;
	string t;
	int i = 0;

	//loop to read whole file
	while(!(cin.eof())){

		//count all the grades
		while(cin >> grd){

			fgrd += grd;
		}
		
		//clear cin for input
		if(!(cin.eof())){

			cin.clear();
		}
		
		//words are read into temporary t string then check it it is name or average and act accordingly
		if(cin >> t){

			if(t == "NAME"){

				cin >> name;
			}

			else if(t == "AVERAGE"){

				//calculate average by adding all averages then dividing by num of averages
				while(cin >> avg){

					favg += avg;
					i++;
				}

				if(i > 0){

					favg = (favg / i);
					fgrd += favg;
				}
				//reset calculating vars
				i = 0;
				favg = 0;
				avg = 0;

				//clear cin for input
				if(!(cin.eof())){

					cin.clear();
				}
			}
			else if(!(cin.eof())){

				cin.clear();
			}
		}
	}

	//output info
	cout << name << " " << fgrd << endl;



	return 0;
}

