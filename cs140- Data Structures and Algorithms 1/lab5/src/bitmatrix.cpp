#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "bitmatrix.hpp"
#include <string>
#include <iostream>

using namespace std;

Bitmatrix::Bitmatrix(int rows, int cols)
{
	//error check for correct rows and cols
	string tmpstr;
	try{
		if(rows <= 0){

			throw((string) "Bad rows");
		}
		if(cols <= 0){

			throw((string) "Bad cols");
		}
	}
	catch(string s){

		cout << s << endl;
	}

	for(int i = 0; i < cols; i++){

		tmpstr.push_back('0');
	}

	for(int i = 0; i < rows; i++){

		M.push_back(tmpstr);
	}
}


Bitmatrix::Bitmatrix(const string &fn)
{
	//error check file and file format
	string tmpstr;
	ifstream fin(fn);
	try{

		if(!(fin.is_open())){

			throw((string) "Can't open file");
		}
		while(getline(fin, tmpstr)){
			for(unsigned int i = 0; i < tmpstr.size(); i++){
				if((tmpstr[i] != '0') && (tmpstr[i] != '1') && (tmpstr[i] != ' ')){

					throw((string) "Bad file format");
				}
			}
			tmpstr.erase(remove(tmpstr.begin(), tmpstr.end(), ' '), tmpstr.end());

			if(tmpstr != ""){

				M.push_back(tmpstr);
			}
		}
	}
	catch(string s){

		fin.close();
		cout << s << endl;
	}

	fin.close();

}


Bitmatrix *Bitmatrix::Copy() const
{
	//just copy bitmatrix to a new bitmartix called matrix
	Bitmatrix *mat = new Bitmatrix(M.size(), M[0].size());
	mat->M = M;
	return mat;
}

bool Bitmatrix::Write(const string &fn) const
{
	//output M with rows and cols
	ofstream fout(fn);
	for(unsigned int i = 0; i < M.size(); i++){
		for(unsigned int h = 0; h < M[i].size(); h++){
			if((M[i][h] == '0') || (M[i][h] == '1')){
				fout << M[i][h];
			}else{

				fout.close();
				return false;
			}
		}

		fout << endl;
	}

	fout.close();
	return true;
}

void Bitmatrix::Print(size_t w) const
{

	//switch was better than series of ifs and checks the case and prints the correct M
	switch(w){

		case '0':
			for(unsigned int i = 0; i < M.size(); i++){
				for(unsigned int h = 0; h < M[i].size(); h++){

					cout << M[i][h];
				}
				cout << endl;
			}
			break;

		case '1':

			for(unsigned int i = 0; i < M.size(); i++){
				for(unsigned int h = 0; h < M[i].size(); h++){

					cout << M[i][h];
					if(!(h == M[i].size() - 1)){

						cout << " ";
					}
				}
				cout << endl;
			}
			break;

		default:

			size_t tmp0 = 0;
			for(unsigned int i = 0; i < M.size(); i++){
				size_t tmp1 = 0;
				for(unsigned int h = 0; h < M[i].size(); h++){

					cout << M[i][h];
					tmp1++;
					if((tmp1 == w) && (h != M[i].size() - 1)){

						cout << " ";
						tmp1 = 0;
					}
				}

				cout << endl;
				tmp0++;
				if((tmp0 == w) && (i != M.size() - 1)){

					cout << endl;
					tmp0 = 0;
				}
			}

	}
}

bool Bitmatrix::PGM(const string &fn, int p, int border) const
{

	//most of this is from lab and piazza tbh
	//building pgm and outputting it into the file
	ofstream fout(fn);

	if(!(fout.is_open()) || (p == 0)){

		return false;
	}

	int r = border + ((p + border) * M.size());
	int c = border + ((p + border) * M[0].size());


	vector <int> tmpv;
	vector <vector<int> > mat;

	mat.resize(r, tmpv);
	for(unsigned int i = 0; i < mat.size(); i++){

		mat[i].resize(c, 0);
	}


	int mi = 0;
	int mh = 0;


	for(int i = border; i < r; i += (p + border)){
		for(int h = border; h <  c; h += (p + border)){
			for(int rr = 0; rr != p; rr++){
				for(int cc = 0; cc != p; cc++){

					int a = i + rr;
					int b = h + cc;
					if(M[mi][mh] == '1'){

						mat[a][b] = 100;
					}
					else{

						mat[a][b] = 255;
					}
				}
			}
			mh++;
		}
		mi++;
		mh = 0;
	}

	//i feel like this is messing it up but its 3 am and i have some of it done
	fout << "P2" << endl << mat[0].size() << " " << mat.size() << endl << "255" << endl;
	for(unsigned int i = 0; i < mat.size(); i++){
		for(unsigned int h = 0; h < mat[i].size(); h++){

			fout << mat[i][h] << " ";
		}
	}

	return true;

}

int Bitmatrix::Rows() const
{
	return (M.size());
}

int Bitmatrix::Cols() const
{
	return (M[0].size());
}

char Bitmatrix::Val(int row, int col) const
{
	//setting up rows and cols for main vect
	unsigned int r = row;
	unsigned int c = col;

	if((r < M.size()) && (c < M[0].size())){

		return(M[row][col]);
	}else{

		return 'x';
	}
}

bool Bitmatrix::Set(int row, int col, char val)
{
	//setting rows and cols of vector to val
	if(val == '0' || val == '1'){

		M[row][col] = val;
	}else{

		return false;
	}
	return true;
}

bool Bitmatrix::Swap_Rows(int r1, int r2)
{
	//swapping r1 and r2
	if((r1 >= 0) && (r2 >= 0) && (r1 < M.size()) && (r2 < M.size())){

		string tmpstr = M[r1];
		M[r1] = M[r2];
		M[r2] = tmpstr;
		return true;
	}else{

		return false;
	}

}

bool Bitmatrix::R1_Plus_Equals_R2(int r1, int r2)
{
	//setting r1 to (r1 + r2)
	if((r1 >= 0) && (r2 >= 0) && (r1 < M.size()) && (r2 < M.size())){
		if(M[r1].size() == M[r2].size()){
			for(unsigned int i = 0; i < M[r1].size(); i++){

				int tmp1;
				int tmp2;
				tmp1 = M[r1][i] - '0';
				tmp2 = M[r2][i] - '0';
				tmp1 += tmp2;
				if(tmp1 == 2){

					tmp1 = 0;
				}
				M[r1][i] = tmp1 + '0';
			}
			return true;
		}
	}
	return false;

}


Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2)
{

	//check for matching dimensions
	if((a1->Rows() * a1->Cols()) != (a2->Rows() * a2->Cols())){

		return NULL;
	}

	//using copy make a new bitmatrix according to input
	Bitmatrix *mat = a1->Copy();
	for(int i = 0; i < a1->Rows(); i++){
		for(int h = 0; h < a2->Cols(); h++){

			//store bits int matrix as chars
			int o = a1->Val(i, h) - '0';
			int t = a2->Val(i, h) - '0';
			char tmp = ((o + t) % 2) + '0';
			mat->Set(i, h, tmp);
		}
	}

	return mat;

}

Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2)
{
	//for keeping track of the total for multiplying
	int tot;
	//check dimensions like before
	if(a1->Cols() != a2->Rows()){

		return NULL;
	}

	//allocating space for new bm with correct ros and cols
	Bitmatrix *mat = new Bitmatrix(a1->Rows(), a2->Cols());
	for(int i = 0; i < a1->Rows(); i++){
		for(int h = 0; h < a2->Cols(); h++){

			//multiply matices and return new matix
			tot = 0;
			for(int j = 0; j < a2->Rows(); j++){

				tot += (a1->Val(i, j) - '0') * (a2->Val(j, h) - '0');
			}
			tot = (tot % 2) + '0';
			mat->Set(i, h, tot);
		}
	}

	return mat;
	

}

Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const vector <int> &rows)
{

	//allocating space for new bm with rows and cols	
	Bitmatrix *mat = new Bitmatrix(rows.size(), a1->Cols());
	for(int i = 0; i < rows.size(); i++){
		//error check if specific row exists
		if(rows[i] >= a1->Rows()){
			
			return NULL;
		}else{

			//setting submatrix
			for(int h = 0; h < a1->Cols(); h++){

				mat->Set(i, h, a1->Val(rows[i], h));
			}
		}
	}

	return mat;

}

Bitmatrix *Inverse(const Bitmatrix *m)
{

	//checking dimensions
	if(m->Rows() != m->Cols()){

		return NULL;
	}

	//allocating space and copying new bitmatrix
	Bitmatrix *inv = new Bitmatrix(m->Rows(), m->Cols());
	Bitmatrix *copy = m->Copy();

	for(int i = 0; i < inv->Rows(); i++){
		for(int h = 0; h < inv->Rows(); h++){

			//setting entries to one for identity mat
			if(i == h){

				inv->Set(i, h, '1');
			}
		}
	}

	//if not a 1 in [i][i] swap with the row above it or delete new bm if unable to invert
	for(int i = 0; i < copy->Rows(); i++){
		if(copy->Val(i, i) != '1'){
			for(int h = copy->Rows(); h >= i; h++){
				if(h == i){
					
					delete copy;
					return NULL;
				}else if(copy->Val(h, i) == '1'){

					copy->Swap_Rows(h, i);
					inv->Swap_Rows(h, i);
					break;
				}
			}
		}
		//after swap for all '1' set row to h + i
		for(int h = copy->Rows(); h > i; h--){
			if(copy->Val(h, i) == '1'){

				copy->R1_Plus_Equals_R2(h, i);
				inv->R1_Plus_Equals_R2(h, i);
			}
		}
	}

	//starting from the bottom iterate up and set row to i + h
	for(int i = copy->Rows() - 1; i >= 0; i--){
		for(int h = copy->Rows(); h > i; h--){
			if(copy->Val(i, h) == '1'){

				copy->R1_Plus_Equals_R2(h, i);
				inv->R1_Plus_Equals_R2(h, i);
			}
		}
	}

	//deallocate memory used for copy mat and return the inv matrix
	delete copy;
	return inv;
}



BM_Hash::BM_Hash(int size)
{
	Table.resize(size);
}

bool BM_Hash::Store(const string &key, Bitmatrix *bm)
{
	//storing the key and bitmatrix
	if(bm == NULL){

		return false;
	}
	size_t i;
	unsigned int h; 

	h = 5381;
	for(i = 0; i < key.size(); i++){

		h = (h << 5) + h + key[i];
	}

	int idx = h % Table.size();

	if(Table[idx].size() != 0){
		for(unsigned int i = 0; i < Table[idx].size(); i++){
			if(Table[idx][i].key == key){

				return false;
			}
		}
	}
	//entry for key and bm
	HTE entry;
	entry.key = key;
	entry.bm = bm;
	Table[idx].push_back(entry);
	return true;

}

Bitmatrix *BM_Hash::Recall(const string &key) const
{
	//return the bitmatrix 

	size_t i;
	unsigned int h;

	h = 5381;
	for(i = 0; i < key.size(); i++){

		h = (h << 5) + h + key[i];
	}

	int idx = h % Table.size();

	for(unsigned int i = 0; i < Table[idx].size(); i++){
		if(Table[idx][i].key == key){

			Bitmatrix *mat = Table[idx][i].bm;
			return mat;
		}
	}

	return NULL;

}

vector <HTE> BM_Hash::All() const
{
	//fix the table size
	vector <HTE> rv;

	for(unsigned int i = 0; i < Table.size(); i++){
		for(unsigned int h = 0; h < Table[i].size(); h++){

			rv.push_back(Table[i][h]);
		}
	}
	return rv;
}
