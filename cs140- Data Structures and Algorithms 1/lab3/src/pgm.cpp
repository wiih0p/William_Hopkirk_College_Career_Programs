#include "pgm.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

bool Pgm::Read(const std::string &file)
{
	ifstream fin;
	string s;
	size_t i, j, r, c, v;

	fin.open(file.c_str());
	if (fin.fail()) return false;

	if (!(fin >> s)) return false;
	if (s != "P2") return false;
	if (!(fin >> c) || c <= 0) return false;
	if (!(fin >> r) || r <= 0) return false;
	if (!(fin >> i) || i != 255) return false;

	Pixels.resize(r);
	for (i = 0; i < r; i++) {
		Pixels[i].clear();
		for (j = 0; j < c; j++) {
			if (!(fin >> v) || v > 255) return false;
			Pixels[i].push_back(v);
		}
	}
	if (fin >> s) return false;
	fin.close();
	return true;
}

bool Pgm::Write(const std::string &file) const{

	//vars for building the file and ofstreaming
	ofstream fout;
	int c = 1;
	int r;
	int line = 0;
	int pixcount = 1;
	int pix;

	//open the file for output
	fout.open(file.c_str());

	//getting the rows
	r = Pixels.size();

	//getting the cols
	if(r > 0){

		c = Pixels[0].size();
	}

	//getting total number of pixels from rows and cols
	pix = r * c;

	//error check if there are no pixels there wont be a pgm file
	if(pix <= 0){

		return false;
	}

	//outputting the file header
	fout << "P2" << endl << c << " " << r << endl << "255" << endl;

	//constructing the file using the pixels vector and outputting the file
	for(int i = 0; i < r; i++){
		for(int h = 0; h < c; h++){

			fout << Pixels[i][h];
			line++;

			//checking for the newline pixcount goes to twenty then puts a new line down
			if((line < 20) && (pixcount != pix)){

				fout << " ";
			}else{

				fout << endl;
				line = 0;
			}
			pixcount++;
		}
	}

	//closing the output file
	fout.close();
	return true;
}

bool Pgm::Create(size_t r, size_t c, size_t pv){

	//error check to make sure greyscale is under pure white 255
	if(pv > 255){

		return false;
	}

	//sizing the vector for rows and constructing the columns with greyscale value
	Pixels.resize(r);
	for(size_t i = 0; i < r; i++){

		Pixels[i].clear();
		for(size_t h = 0; h < c; h++){

			Pixels[i].push_back(pv);
		}
	}

	return true;
}

bool Pgm::Clockwise(){

	//making temp vector to manipulate
	vector<vector<int> > tempv;

	//resizing the vector to the rows and building the columns
	tempv.resize(Pixels[0].size());

	for(size_t i = 0; i < tempv.size(); i++){

		tempv[i].resize(Pixels.size());

	}

	//swapping rows and columns and putting them in temp vector
	for(size_t i = 0; i < Pixels.size(); i++){
		for(size_t h = 0; h < Pixels[0].size(); h++){

			tempv[h][tempv[0].size() - i - 1] = Pixels[i][h];
		}
	}

	//set pixels to temp vector
	Pixels = tempv;

	return true;
}

bool Pgm::Cclockwise(){

	Clockwise();
	Clockwise();
	Clockwise();

	return true;
}

bool Pgm::Pad(size_t w, size_t pv){

	//error check greyscale like before
	if(pv > 255){

		return false;
	}

	//temporary rows, columns and vectors for manipulating
	size_t tempr = Pixels.size() + (2 * w);
	size_t tempc = Pixels[0].size() + (2 * w);

	vector<vector<int> > tempv;

	//sizing the vector to rows and building columns
	tempv.resize(tempr);

	for(size_t i = 0; i < tempr; i++){

		tempv[i].resize(tempc);
	}

	//constructing the pgm and filling in the areas that arent in the original rows and columns with the greyscale values
	for(size_t i = 0; i < tempr; i++){
		for(size_t h = 0; h < tempc; h++){

			if(i < w || i >= (Pixels.size() + w) || h < w || h >= (Pixels[0].size() + w)){
				tempv[i][h] = pv;
			}else{

				tempv[i][h] = Pixels[i -w][h - w];
			}
		}
	}

	//setting the pixel vector to the temporary vector for WRITE
	Pixels = tempv;

	return true;
}

bool Pgm::Panel(size_t r, size_t c){

	//checking if there is any pixels in pgm file
	if(r == 0 || c == 0){

		return false;
	}

	//same as before, creating temporary rows cols and vect for manipulating
	size_t tempr = Pixels.size() * r;
	size_t tempc = Pixels[0].size() * c;

	vector<vector<int> > tempv;

	//same as before, just resizing the vector to the rows and building the columns
	tempv.resize(tempr);

	for(size_t i = 0; i < tempr; i++){

		tempv[i].resize(tempc);
	}

	//actually also about the same as before just constructing th pgm but doing it rows*columns times
	for(size_t i = 0; i < r; i++){
		for(size_t h = 0; h < Pixels.size(); h++){
			for(size_t j = 0; j < c; j++){
				for(size_t k = 0; k < Pixels[0].size(); k++){

					tempv[h + (i * Pixels.size())][k+ (j * Pixels[0].size())] = Pixels[h][k];
				}
			}
		}
	}

	//setting pixel vector to the temp vector for WRITE
	Pixels = tempv;


	return true;
}

bool Pgm::Crop(size_t r, size_t c, size_t rows, size_t cols){

	//temp vectors just like before for manipulating
	vector<int> tempr(cols);
	vector<vector<int> > tempv(rows, tempr);

	//error check to make sure crop isnt bigger than og picture
	if((r + rows) > Pixels.size() || (c + cols) > Pixels[0].size() || c == 0 || r == 0){

		return false;
	}

	//store cropped pixels only in temp vector
	for(size_t i = 0; i < tempv.size(); i++){
		for(size_t h = 0; h < tempr.size(); h++){

			tempv[i][h] = Pixels[(r + i)][(c + h)];
		}
	}

	//set vector to temp vector
	Pixels = tempv;

	return true;
}
