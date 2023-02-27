//lab 2 part 3

#include <cstdio>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

int main(){

	//vars for various inputs for pgm
	int r;
	int c;
	int pix;
	int numpix;

	//vars to check pgm specifics
	string p2;
	int mx;

	//string to check if theres extra stuff
	string err;


	//checking for error cases
	cin >> p2;
	if(!(p2 == "P2")){

		fprintf(stderr, "Bad PGM file -- first word is not P2\n");
		return -1;
	}

	if(!(cin >> r) || (r <= 0)){

		fprintf(stderr, "Bad PGM file -- No row specification\n");
		return -2;
	}
	if(!(cin >> c) || (c <= 0)){

		fprintf(stderr, "Bad PGM file -- No column specification\n");
		return -3;
	}

	//making pixel array to check each pixel by number
	numpix = (r * c);
	int pixel[numpix];

	if(!(cin >> mx)){

		fprintf(stderr, "Bad PGM file -- No 255 following the rows and columns\n");
		return -4;
	}

	if(mx != 255){

		fprintf(stderr, "Bad PGM file -- No 255 following the rows and columns\n");
		return -5;
	}				    

	//check each pixel for error
	for(unsigned int i = 0; i < numpix; i++){

		cin >> pix;

		if((!(pix > 0)) || (!(pix < 255))){

			fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", i);
			return -6;
		}
		else{

			pixel[i] = pix;
		}
	}

	//check after the file to see if there is junk in it
	if(cin >> err){

		fprintf(stderr, "Bad PGM file -- Extra stuff after the pixels\n");
		return -7;
	}

	//print the pgm file
	printf("P2\n%d%d\n255\n", c, r);
	for(unsigned int i = 0; i < numpix; i++){

		printf("%d\n", pixel[i]);
	}

	return 0;
}

