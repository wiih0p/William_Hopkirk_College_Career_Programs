//lab9 pt 2
//will Hopkirk

//C++
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <sstream>

//C
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//Using Claras Template
typedef unsigned int uint;

class ShapeShifter 
{
	struct shape 
	{
		//Constructors
		shape();
		shape(const string&);

		//Variables
		vector< vector<bool> > grid;
		uint width, height;
		string text;
	};

	public:
	//Constructors
	ShapeShifter();
	ShapeShifter(const vector<string>&);

	//Dealing with shapes
	void add_shape(const string&);
	bool apply_shape(uint, uint, uint);

	//Solving functions
	bool all_one();
	bool find_solution(uint = 0);
	void print_solution();

	private:
	//All shapes to be inserted into the grid
	vector<shape> shapes;

	//Game Grid
	vector< vector<bool> > grid;

	//(x, y) points for each shape when the solution is found.
	deque< pair<uint, uint> > solution;

	//Game grid size
	uint width, height;
};

ShapeShifter::shape::shape() {

	/* Leave Empty */
}

ShapeShifter::shape::shape(const string& line) {

	/*
	 * Takes "line" and makes a grid out of it.
	 *
	 * - Split the string by space (probably via istringstream).
	 *   Put each row into a temporary vector<string>.
	 *
	 * - Go through each char in each row and set "grid" up.
	 *
	 * - Set width and height
	 *
	 * - Store the original string ("line") in "text".
	 */
	istringstream sin;
	string s;
	vector<string> tmp;
	sin.str(line);

	while(sin >> s){

		tmp.push_back(s);
	}

	height = tmp.size();
	width = 0;
	for(uint i = 0; i < tmp.size(); i++){

		if(width < tmp[i].size()){

			width = tmp[i].size();
		}
	}

	for(uint i = 0; i < height; i++){

		vector<bool> tmp;
		grid.push_back(tmp);
		for(uint h = 0; h < width; h++){

			grid[i].push_back(0);
		}
	}

	for(uint i = 0; i < height; i++){
		for(uint h = 0; h < width; h++){

			if(tmp[i][h] == '1'){

				grid[i][h] = 1;
			}else{

				grid[i][h] = 0;
			}
		}
	}

	text = line;
}

ShapeShifter::ShapeShifter() {

	/* Leave Empty */
}

ShapeShifter::ShapeShifter(const vector<string>& rows) {

	/*
	 * Similar to the "shape" constructor. Except you're given a vector of
	 * rows already. So you can skip the splitting part.
	 */
	
	height = rows.size();
	width = rows[0].size();

	for(uint i = 0; i < height; i++){

		vector<bool> temp;
		grid.push_back(temp);
		for(uint h = 0; h < width; h++){

			grid[i].push_back(0);
		}
	}

	for(uint i = 0; i < rows.size(); i++){
		for(uint h = 0; h < rows[i].size(); h++){

			if(rows[i][h] == '1'){

				grid[i][h] = 1;
			}else{

				grid[i][h] = 0;
			}
		}
	}
}

void ShapeShifter::add_shape(const string& line) {
	/*
	 * Should just be one line. Push back a shape made with the string "line".
	 */
	shape tmp(line);
	shapes.push_back(tmp);
}

bool ShapeShifter::apply_shape(uint shape_id, uint x, uint y) {

	/*
	 * Insert the shape into the game grid.
	 *
	 * - Check if the shape can fit in the grid when shifted by "x" and "y".
	 * - Invert all parts of the grid where the shape would be red at
	 */
	shape tmp = shapes[shape_id];
	uint shpx = 0;
	uint shpy = 0;

	if(((tmp.width + x) > width) || ((tmp.height + y) > height)){

		return false;
	}

	for(uint i = y; i < tmp.height + y; i++){
		for(uint h = x; h < tmp.width + x; h++){

			if(tmp.grid[shpy][shpx] == 1){

				if(grid[i][h] == 1){

					grid[i][h] = 0;
			}else{

					grid[i][h] = 1;
				}
			}
			shpx++;
		}
		shpx = 0;
		shpy++;
	}

	return true;
}

bool ShapeShifter::all_one() {

	/*
	 * Check if all cells in the grid are "1". If so, return true. Otherwise,
	 * return false.
	 */
	for(uint i = 0; i < height; i++){

		for(uint h = 0; h < width; h++){

			if(grid[i][h] == 0){

				return false;
			}
		}
	}
	return true;
}

bool ShapeShifter::find_solution(uint shape_id) {

	/*
	 * The recursive function.
	 *
	 * - With "shape_id", go through every possible position and try to insert
	 *   shape[shape_id] in.
	 *
	 * - If inserted and it isn't the last shape, call:
	 *     find_solution(shape_id + 1)
	 *
     * - If inserted and it is the last shape, call "all_one". If true then
	 *   return true. Have the other calls to the function also handle this
	 *   return.
	 *
	 * - If you looped through all possible positions and it didn't work, undo
	 *   all changes done to the class involving this shape and return false.
	 */
	if(shape_id == shapes.size()){
		if(all_one()){

			return true;
		}else{

			return false;
		}
	}else{
		for(uint i = 0; i < height; i++){
			for(uint h = 0; h < width; h++){
				if(apply_shape(shape_id, h, i)){

					solution.push_back(make_pair(h, i));
					if(find_solution(shape_id + 1)){
						return true;
					}else{
					
						apply_shape(shape_id, h, i);
						solution.pop_back();
					}
				}
			}
		}
	}

	return false;
}

void ShapeShifter::print_solution() {

	/*
	 * Goes through "solution" list and prints out the coordinates of each
	 * shape, along with the original string used to make up the shape.
	 */
	for(uint i = 0; i < solution.size(); i++)
	{
		cout << shapes[i].text << " " << solution[i].second << " " << solution[i].first << endl;
	}

}

int main(int argc, char** argv) {
	
	/*
	 * - Create a vector of strings holding all argv after argv[0]
	 *
	 * - Create your ShapeShifter object.
	 *
	 * - Create all shapes.
	 *
	 * - Find a solution.
	 *
	 * - Print the result.
	 */

	vector<string> rows;
	string line;

	for(int i = 1; i < argc; i++){

		string temp = argv[i];
		rows.push_back(temp);
	}

	ShapeShifter game(rows);

	while(getline(cin, line)){

		game.add_shape(line);
	}

	if(game.find_solution(0)){

		game.print_solution();
	}else{

		cout << "No solution\n";
	}

	return 0;
}
