#include <sys/time.h>

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

class omega {
	public:
		omega();
		float get_sec();

	private:
		long Tinit;
		long Tmark;
};

omega::omega() {
	struct timeval T;
	gettimeofday(&T, NULL);

	Tinit = 1000000*T.tv_sec + T.tv_usec;
}

float omega::get_sec() {
	struct timeval T;
	gettimeofday(&T, NULL);

	Tmark = 1000000*T.tv_sec + T.tv_usec;

	return (float)(Tmark-Tinit)/1000000.0;
}

class sudoku {
	public:
		sudoku();

		void solve();

		void read(const char *);
		void write(const char *);
		void write(const char *, const char *);

	private:
		bool solve(vector<int> cells, int c);

		bool error_check_value(bool zero_valid);
		bool error_check_uniqueness();
		bool check_row(int r, int v);
		bool check_col(int c, int v);
		bool check_square(int i, int j, int v);
		vector<int> valid_values(int i, int j);

		void display();

		int game[9][9];
};

//elapsed gets time elapsed in a string with the units
string units[2] = {"sec", "msec"};
string elapsed(float duration, int i = 0){

	if(duration < 0.1){

		duration *= 1000;
		i++;
		return elapsed(duration, i);
	}else{

		ostringstream sout;
		sout << duration << ' ' << units[i];
		return sout.str();
	}
}
sudoku::sudoku() { 
	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++)
			game[i][j] = 0;
	}
}

//starting the recursive solver by making a vector of ints for the individual cells and error checks
void sudoku::solve() {

	cout << "SOLVE" << endl;
	bool error = false;
	vector<int> cells;

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){

			if(game[i][j] == 0){

				cells.push_back(((i * 9) + j));
			}
		}
	}

	if(solve(cells, 0)){

		display();
	}

	if(error_check_value(false)){

		error = true;
	}
	if(error_check_uniqueness()){

		error = true;
	}
	if(error == true){

		exit(0);
	}
	// error check data values
	// error check uniqueness
	// exit if errors detected
}

//checks for out of bounds data and reads in the table
void sudoku::read(const char *fname) {

	bool error = false;
	int line = 1;
	cout << "READ\n";

	ifstream fin(fname);

	int i, j, v;

	while (fin >> i >> j >> v) {

		if((i > 8 || i < 0) || (j > 8 || j < 0)){

			error = true;
			cerr << "line " << line << ": " << i << ' ' << j << ' ' << v << " out-of-bounds grid index"  << endl;
		}
		game[i][j] = v;
		line++;
	}

	if(error == true){

		exit(0);
	}

	display();
	fin.close();

	if(error_check_value(true)){

		error = true;
	}
	if(error_check_uniqueness()){

		error = true;
	}
	if(error == true){

		exit(0);
	}
	

	// exit if bad grid indices

	// error check data values
	// error check uniqueness
	// exit if errors detected
}

//checks if value if out of bounds
bool sudoku::error_check_value(bool zero_valid){

	bool error = false;

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){

			if(zero_valid){

				if(game[i][j] > 10 || game[i][j] < -1){

					error = true;
					//cerr << "error value check" << endl;
				}
			}else{

				if(game[i][j] > 10 || game[i][j] < 0){

					error = true;
					//cerr << "error value check" << endl;
				}
			}
		}
	}
	return error;
}

//checks for uniqueness for rows cols and squares
bool sudoku::error_check_uniqueness(){

	bool error = false;
	int c, r, v;
	for(r = 0; r < 9; r++){
		for(c = 0; c < 9; c++){
			//for(v = 1; v < 10; v++){

			v = game[r][c];
				if(v > 0){
					if(check_row(r, v)){

						error = true;
					}
					if(check_col(c, v)){

						error = true;
					}
					if(check_square(r, c, v)){

						error = true;
					}
				}
				if(error == true){

					cerr << "cell " << r << ' ' << c << " non-uniqe value " << v << endl;
					error = false;
				}
			//}
		}
	}
	return error;
}

//helper to find if row is unique
bool sudoku::check_row(int r, int v){

	int vs = 0;
	for(int i = 0; i < 9; i++){

		if(game[r][i] == v){

			vs++;
		}
		if(vs > 1){

			return true;
		}
	}
	return false;
}

//helper to find if col is unique
bool sudoku::check_col(int c, int v){

	int vs = 0;
	for(int i = 0; i < 9; i++){

		if(game[i][c] == v){

			vs++;
		}
		if(vs > 1){

			return true;
		}
	}
	return false;
}

//helper to find if square is unique
bool sudoku::check_square(int i, int j, int v){

	int vs = 0;
	int si;
	int sj;
	si = (i / 3) * 3;
	sj = (j / 3) * 3;
	for(int r = si; r < (si + 2); r++){
		for(int c = sj; c < (sj + 2); c++){

			if(game[r][c] == v){

				vs++;
			}

			if(vs > 1){

				return true;
			}
		}
	}
	return false;
}

void sudoku::write(const char *fname) {
	ofstream fout(fname);

	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			if (0 < game[i][j]) {
				fout << i << " "
					<< j << " "
					<< game[i][j] << "\n";
			}
		}
	}

	fout.close();
}

void sudoku::write(const char *fname, const char *addon) {
	int N1 = strlen(fname);
	int N2 = strlen(addon);

	char *n_fname = new char[N1+N2+2];

	// strip .txt suffix, then concatenate _addon.txt
	strncpy(n_fname, fname, N1-4);
	strcpy(n_fname+N1-4, "_");
	strcpy(n_fname+N1-3, addon);
	strcpy(n_fname+N1-3+N2, ".txt");

	write(n_fname);

	delete [] n_fname;
}

void sudoku::display() {
	cout << "| --------------------------- |\n";
	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			if (j == 0) 
				cout << "| " << game[i][j];
			else if (j%3 == 0) 
				cout << " | " << game[i][j];
			else
				cout << "  " << game[i][j];
		}
		cout << " |\n";
		if (i%3 == 2)
			cout << "| --------------------------- |\n";
	}
}

//recursive solver to find all values for the game
bool sudoku::solve(vector<int> cells, int c) {

	if(c == cells.size()){

		return true;
	}

	//optimization finds the cell with the least valid values and starts solving there
	vector<int> temp;
	vector<int> lowest_vv(10);
	int lowest_vv_i;
	int id;
	int jd;
	for(int h = c; h < cells.size(); h++){

		int i = cells[h] / 9;
		int j = cells[h] % 9;
		temp = valid_values(i, j);
		if(temp.size() < lowest_vv.size()){

			lowest_vv = temp;
			id = i;
			jd = j;
		}
	}
	lowest_vv_i = ((id * 9) + jd);

	//solves better without the optimization but faster with it 
	//i have it solving without the optimization to show how far i got on the solver
	int i = cells[c] / 9;
	int j = cells[c] % 9;
	vector<int> values = valid_values(i, j);

	if(values.size() == 0){

		return false;
	}

	for(int h = 0; h < values.size(); h++){

		game[i][j] = values[h];
		c++;
		if(solve(cells, c)){

			return true;
		}
	}

	game[i][j] = 0;
	return false;
}

//checks for valid values in a cell
vector<int> sudoku::valid_values(int i, int j){

	bool valid;
	vector<int> vvs;
	int val;
	val = game[i][j];

	for(int h = 1; h < 10; h++){

		game[i][j] = h;
		valid = true;

		if(check_row(i, h)){

			valid = false;
		}
		if(check_col(j, h)){

			valid = false;
		}
		if(check_square(i, j, h)){

			valid = false;
		}

		if(valid == true){

			vvs.push_back(h);
		}
		game[i][j] = val;
	}

	return vvs;

}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	if ((argc != 3) ||
			(strcmp(argv[1], "-s") != 0) ||
			strstr(argv[argc-1], ".txt") == NULL) {
		cerr << "usage: Sudoku -s game.txt\n";
		exit(0);
	}

	omega timer;
	sudoku sudoku_game;

	float T0 = 0.0;
	float T1 = 0.0;

	if (strcmp(argv[1], "-s") == 0) {
		sudoku_game.read(argv[2]);

		T0 = timer.get_sec();
		sudoku_game.solve();
		T1 = timer.get_sec();

		sudoku_game.write(argv[2], "solved");
	}

	float duration = (T1 - T0);

	//print out time taken to solve
	cout << endl;
	cout << "Time: " << elapsed(duration, 0);
	cout << " (" << setprecision(6) << duration << " sec" << ")" << endl;

	//string elapsed_time = elapsed(T1-T0);
	// print string elapsed_time using two decimal places
	// print T1-T0 using six decimal places for comparison
}
