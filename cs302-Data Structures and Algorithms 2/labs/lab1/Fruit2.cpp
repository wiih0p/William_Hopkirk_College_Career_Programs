// list all headers needed 

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

//fruit struct
struct fruit {

	bool operator<(const fruit &)const ;
  
	string name;
	float quan;
	float cost;
};

struct node {
  
	node();
	~node();
	node *next;
	fruit element;
};

node::~node(){

	delete this;
}

//overload for < operator compares the names
bool fruit::operator<(const fruit &fr)const {

	return (this->name < fr.name);
}
int main(int argc, char *argv[])
{

	//error checking for cmd line args
	if (argc != 3){

		cerr << "incorrect num of args" << endl;
		return -1;
	}

	if ((strcmp(argv[1], "-inorder") == 0) || (strcmp(argv[1], "-sortall") == 0) || (strcmp(argv[1], "-sortone") == 0)){

	}else{

		cerr << "incorrect cmd" << endl;
		return -1;
	}


	//open file and instantiate tmp variables
	ifstream fin;
	fin.open(argv[2]);

	istringstream sin;
	string nm;
	float q;
	float c;

	//fruit vector
	vector<fruit> l;

	int ct = 0;

	//reading file and adding to vector
	while(!fin.eof()){

		fruit f;
		string line;
		while(getline(fin, line)){

			sin.str(line);
			sin >> nm;
			sin >> q;
			sin >> c;
			sin.clear();

			f.name = nm;
			f.quan = q;
			f.cost = c;

			l.push_back(f);
		}

	}

}
	//sorting for sortall and sortone
/*	if((strcmp(argv[1], "-sortall") == 0)){
		
		stable_sort(l.begin(), l.end());
	}
	if((strcmp(argv[1], "-sortone") == 0)){

		stable_sort(l.begin(), l.end());

		for(int i = 0; i < l.size(); i++){

			if(l[i].name == l[i + 1].name){

				l[i + 1].quan = (l[i + 1].quan + l[i].quan);
				l.erase(l.begin() + i);
				i--;
			}
		}
	}








*/
	
//int main(int argc, char *argv[])
//{
//  prog -inorder|sortall|sortone file.txt
//  set program mode from command line arg

//  declare linked_list<fruit>

//  open file
//  while (reading more data)
//    store data INORDER | SORTALL | SORTONE 

//  pretty-print linked list content to stdout

//  free linked list memory
//}
