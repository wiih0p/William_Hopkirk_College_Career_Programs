/*
 *  * INSTRUCTOR-GIVEN TEMPLATE
 *   * COSC 140: Lab 6 - Candy Crush
 *    *
 *     * Description:
 *      *     Implements a 1D variation of Candy Crush where you crush candy and all
 *       *     adjacent candies of the same kind.
 *        *
 *         * Author:
 *          *     will
 *           */
#include <iostream>
#include <cstdio>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include "candyCrush.hpp"

using namespace std;

// ----------------------------------------------------------------------------
// CandyCrush Class Methods                                                {{{1
// ----------------------------------------------------------------------------

/*
 * candyCrush(const string &)                                              {{{2
 *
 * Sets up the candyCrush instance via a file at "inputFile". This file will
 * specify the rules of the game, as well as points given for a specific number
 * of candies being broken in a row.
 */

//all of this is pretty much commented for me so i didnt add much
candyCrush::candyCrush(const string &inputFile) {

	//all of this was tedious error checking from the gradescripts big oof
	int listcnt = 1;
	int seed;
	string flavor;
	string tmp;
	istringstream sin;
	int percent = 0;
	ifstream fin;

	fin.open(inputFile);
	if(!fin.is_open()){

		cerr << inputFile <<": No such file or directory\n";
		exit(0);
	}

	getline(fin, tmp);
	sin.str(tmp);
	if(!(sin >> seed)){

		sin.clear();
		sin.str(tmp);
		string er;
		sin >> er;
		cerr << "line 1: Bad seed for the random number generator\n\terroneous intput was: " << er << "\n";
		exit(0);
	}

	if(!(sin >> rowLength)){

		sin.clear();
		sin.str(tmp);
		string er, er0;
		sin >> er >> er0;
		cerr << "line 1: Bad seed for the random number generator\n\terroneous intput was: " << er0 <<"\n";
		exit(0);

	}

	listcnt++;
	srand(seed);
	if(rowLength < 1 || rowLength > 100){

		cerr << "line 1: The row length you entered, " << rowLength << ", must be from 1-100\n";
		exit(0);
	}
	sin.clear();

	getline(fin, tmp);
	sin.str(tmp);
	while(sin >> flavor){

		flavors.push_back(flavor);
	}

	sin.clear();
	listcnt++;

	while(getline(fin, tmp)){

		int tmp1, tmp2;

		sin.str(tmp);

		if(!(sin >> tmp1)){

			sin.clear();
			sin.str(tmp);
			string er;
			sin >> er;
			cerr << "line " << listcnt << ": Probability and points for this sequesnce must be non-negative integers\n\terroneous input was: " << er << "\n";
			exit(0);
		}
		if(!(sin >> tmp2)){

			sin.clear();
			sin.str(tmp);
			string er, er0;
			sin >> er >> er0;
			cerr << "line " << listcnt << ": Probability and points for this sequesnce must be non-negative integers\n\terroneous input was: " << er0 << "\n";

			exit(0);
		}
		if(tmp1 < 0 || tmp2 > 100){

			cerr << "Line " << listcnt << ": The probability you entered, " << tmp1 << ", must be 0-100\n";
			exit(0);
		}

		percent += tmp1;

		if(percent > 100){

			 cerr << "Line " << listcnt << ": The cumulative probability exceeds 100. It must be exactly 100 when all probabilities have been entered\n";
			exit(0);
		}

		probabilities.push_back(percent);

		if(tmp2 < 0){

			cerr << "Line " << listcnt << ": points, " << tmp2 << ", is negative. It must be non-negative\n";
			exit(0);
		}

		points.push_back(tmp2);
		sin.clear();
		listcnt++;

	}

	fin.close();

	if(points.size() != rowLength){

		cerr << "You entered " << points.size() << " pairs of probabilities and points. You must enter the same number of pairs as the rowLength, which is " << rowLength << "\n";
		exit(0);
	}

	if(percent != 100){

		cerr << "The probabilities you entered must sum to 100 but their sum was " << percent << "\n";
		exit(0);
	}

	fill();

}


/*
 * getRowLength                                                            {{{2
 *
 * Gets the length of amount of candy in the row...
 */

int candyCrush::getRowLength() const {

	//yup
	return rowLength;
}

/*
 * getScore                                                                {{{2
 *
 * Gets the current score in-game.
 */

int candyCrush::getScore() const {

	//yup
	return score;
}

/*
 * printCandy                                                              {{{2
 * 
 * Prints candy to stdout.
 */

void candyCrush::printCandy() const {

	//using iterator print the candy in proper format
	list<string>::const_iterator i;
	int cnt = 0;

	for(i = candy.begin(); i != candy.end(); i++){

		printf("%-10s", i->c_str());
		cnt++;
		if(cnt == 8){

			printf("\n");
			cnt = 0;
		}
	}
	printf("\n");

}

/*
 * play                                                                    {{{2
 *
 * Make a play in the game. The candy destroyed is determined by "choice".
 */

int candyCrush::play(int choice) {

	//inspired by clara notes :)
	string flavor;
	list<string>::iterator l = candy.begin();

	int cscore = 0;

	for(int i = 0; i < choice; i++){

		l++;
	}

	list<string>::iterator r = l;
	flavor = *l;

	while(*l == flavor && l != candy.begin()){

		l--;
	}
	if(*l != flavor){

		l++;
	}
	while(*r == flavor && r != candy.end()){

		r++;
	}

	candy.erase(l, r);
	cscore = rowLength - candy.size() - 1;
	fill();
	score += points[cscore];

	return points[cscore];
}

void candyCrush::fill(){

	//fills the flavors list
	while(candy.size() != rowLength){

		string flavor;
		int prob = 0;
		int time = 0;

		flavor = flavors[rand() % flavors.size()];
		prob = rand() % 100;

		for(int i = 0; i < probabilities.size(); i++){
			if(prob < probabilities[i]){

				time = i + 1;
				break;
			}
		}

		candy.resize((candy.size() + time), flavor);
		if(candy.size() > rowLength){

			candy.resize(rowLength);
		}
	}
}
