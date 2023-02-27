#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

//class definitions
class name_t {
  public:
    //constructor(s)
	
	name_t();
	name_t(const string &, const string &);

    bool operator<(const name_t &)const;
    void print(int W=0)const;

  private:
    string name;
};

class scores_t {
  public:
    //constructor(s)

	scores_t();

    void insert(int);
    void insert_done();
    void print();
	float givememean() const;

  private:
    vector<int> scores;
    float mean;
};

class namescores_t{

	public:

	namescores_t();
	namescores_t(const name_t &, const scores_t &);

	bool operator<(const namescores_t &) const;
	
	void print_name(int = 0);
	void print_scores();

    private:

	name_t n;
	scores_t s;
};

//nothin
namescores_t::namescores_t(){

	//nothin
}
//sets name_t and scores_t
namescores_t::namescores_t(const name_t &nm, const scores_t & sc){

	n = nm;
	s = sc;
}

//print names and scores
void namescores_t::print_name(int w){

	n.print(w);
}

void namescores_t::print_scores(){

	s.print();
}

//checks for both mean cases and returns comparisons if means are the same goes to names
bool namescores_t::operator<(const namescores_t & ns) const{

	float mean1 = ns.s.givememean();

	if(mean1 == this->s.givememean()){

		return (ns.n < this->n);
	}else{

		return (mean1 > this->s.givememean());
	}
	
}

//returns mean
float scores_t::givememean() const{

	return mean;
}
//blank constructor
name_t::name_t(){

	name = "";
}
//constructor takes first and last names and concatonates them into 1 name
name_t::name_t(const string &fn, const string &ln){

	name = (ln + ", " + fn + ' ');
}

//checks if one name is lexicographically less than another
bool name_t::operator<(const name_t &n) const{

	return (this->name < n.name);
}

//prints in format
void name_t::print(int w) const{

	cout << setw(w + 3) << setfill('.') << left << name;
}

//score constructor
scores_t::scores_t(){

	mean = 0.0;
}

//inserts score into the scores vector
void scores_t::insert(int n){

	scores.push_back(n);
}

//gets the mean value
void scores_t::insert_done(){

	float num = 0;
	float n = 0;

	if(!scores.empty()){

		for(int i = 0; i < scores.size(); i++){

			num = num + scores[i];
			n++;
		}

		mean = (num / n);
	}
}

//prints scores
void scores_t::print(){

	cout << right << setfill(' ');

	for(int i = 0; i < scores.size(); i++){

		cout << ' ' << setw(2) << scores[i];
	}

	cout << " : " << setprecision(1) << fixed << mean << endl;
}

int main(int argc, char** argv) {

	//get width from cmd line
	int W = atoi(argv[1]);
	int K = atoi(argv[2]);

	//vector init
	vector<namescores_t> NS;
 
	//open file and read in lines
	string line;
	ifstream fin;
	fin.open(argv[3]);
	while(!(fin.eof())){
		
		while(getline(fin, line)){

			//get the first and lastname & put them into a name_t
		istringstream sin;
		string firstname;
		string lastname;
		int scr;
		sin.str(line);
		sin >> firstname;
		sin >> lastname;
		

		name_t n(firstname, lastname);
		scores_t s;
		
		//insert the scores
		while(sin >> scr){

			s.insert(scr);
		}
		s.insert_done();

		//push back namescores into the vector
		namescores_t yeet(n, s);
		NS.push_back(yeet);
	}
	}

	//close the file, loop thru the map, print the contents
	fin.close();

	//max heap creation and printing
	make_heap(NS.begin(), NS.end());
	for(int i = 0; i < K; i++){

		NS[0].print_name(W);
		NS[0].print_scores();

		pop_heap(NS.begin(), NS.end());
		NS.pop_back();

	}

}

  /*open filename.txt
  map<name_t,scores_t> NS;

  while (more data in file) {
    add first and lastname to name_t object

    while (more scores) {
      insert score into scores_t object
    have insert_done calculate mean score

    create and insert (name_t,scores_t) pair into NS map
  }

  close filename.txt

  while (NP map iterator loop) {
    print name
	print scores
  }

  */


