//lab 2 part 4

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

	//pixelcounter and vector of vectors of ints to be able to track pixels to flip
	int pixc;
	vector<vector<int> > pic;

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

	if(!(cin >> mx)){

		fprintf(stderr, "Bad PGM file -- No 255 following the rows and columns\n");
		return -4;
	}

	if(mx != 255){

		fprintf(stderr, "Bad PGM file -- No 255 following the rows and columns\n");
		return -5;
	}

	//get num of pixels
	numpix = r * c;

	//print header
	printf("P2\n%d%d\n255\n", c, r);

	//resize picture to rows and columns size
	pic.resize(r);
	for(unsigned int i = 0; i < r; i++){

		pic[i].resize(c);
	}

	//build picture vector with rows and columns
	for(unsigned int i = 0; i < r; i++){
		for(unsigned int h =0; h < c; h++){

			if(!(cin >> pix)){

				fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", pixc);
				return -6;
			}
			else if(pix < 0 || pix > 255){
				
				fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", pixc);
				return -7;
			}
			else{

				pic[i][h] = pix;
				pixc++;
			}
		}
	}

	//check after the file to see if there is junk in it
	if(cin >> err){

		fprintf(stderr, "Bad PGM file -- Extra stuff after the pixels\n");
		return -8;
	}

	//print the pgm file

	for(unsigned int i = 0; i < r; i++){
		for(unsigned int h = 0; h < 0; h--){

			if(h != c){
				printf(" ");
			}
			printf("%d", pic[i][h]);
		}
	}

	printf("\n");
	return 0;
}
