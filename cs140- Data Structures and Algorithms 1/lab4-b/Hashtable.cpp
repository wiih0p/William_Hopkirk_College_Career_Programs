
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <ctype.h>
#include <sstream>

using namespace std;

bool runstats = false;

//hashtable class
class hash_table {
	public:    
		hash_table(int);
		~hash_table();
		void insert(const string &, int);
		void set_showstats();
		const vector<int> &find(const string &);
		void insert_done();
	private:
		struct key_line{

			bool inuse();
			bool operator==(const string &) const;

			string key;
			vector<int> lines;
		};

		int hash(const string &);    
		int  nextprime(int);    
		int  qprobe(const string &, bool);
		void showload();
		void resize();    
		int num_inuse;    
		int max_inuse;
		int collisions;
		bool show_stats;
		vector<key_line> table;
};

//print out extra stats after cin is exhausted if showstats is an arg
hash_table::~hash_table(){

	if(runstats == true){

		//cout stats
	}
}

//sets showstats to true
void hash_table::set_showstats(){

	show_stats = true;
}

//prints load data
void hash_table::showload(){

	cout << "**N =" << setw(6) << table.size() << " load = " << setprecision(3) << ((double)num_inuse/(double)table.size()) << endl;
}

//calls showload if showstats true
void hash_table::insert_done(){

	if(show_stats == true){

		showload();
	}
}

//operator overload to see if key == string
bool hash_table::key_line::operator==(const string &k) const{

	if(key == k){

		return true;
	}else{

		return false;
	}
}

//check if key is inuse
bool hash_table::key_line::inuse(){

	if(key == ""){

		return false;
	}else{

		return true;
	}
}

//constructor with variable init size
hash_table::hash_table(int N) {  
	
	table.assign(nextprime(N), key_line());   
	num_inuse = 0;  
	max_inuse = N/2;
	collisions = 0;
	show_stats = false;
}

//inserts string to table with its line
void hash_table::insert(const string &key, int ln) {
	
	bool c = true;
	int index = qprobe(key, c);
	if(!table[index].inuse()){

		key_line kl;
		kl.key = key;

		vector<int>::iterator p;
		p = std::find(kl.lines.begin(), kl.lines.end(), ln);

		if(*p != ln){

			kl.lines.push_back(ln);
		}

		table[index] = kl;
		num_inuse++;
	}
	if(num_inuse >= max_inuse){

		resize();
	}

}

//finding the lines where a key occurs
const vector<int> &hash_table::find(const string &key){

	bool c = false;
	int index = qprobe(key, c);
	return (table[index].lines);
}

//nextprime with n = (n * 2) +1 instead of n
int hash_table::nextprime(int N) {
	int i = 2;
	N = (N * 2) + 1;
	while (i*i <= N) {    
		if (N%i == 0) { N++; i = 1; }
		i++;  }
	return max(3,N);
}

//qprobe with added collision check
int hash_table::qprobe(const string &key, bool c) {

	int index = hash(key);  
	int k = 0;  
	while (!table[index].inuse() && !(table[index] == key)) {   
		index += 2*(++k) - 1;    
		index = index % table.size();
		if(c == true){

			collisions++;
		}
	}
	return index;
}

//resize from hash2 handout pt2
void hash_table::resize() {

	bool c = false;
	vector<key_line> tmp_table;
	for (int i=0; i<(int)table.size(); i++) {
		if (!table[i].inuse()){      
			
			tmp_table.push_back(table[i]);
		}
	}
	int N = nextprime(2*table.size());  
	table.assign(N, key_line());
	num_inuse = 0;
	max_inuse = N/2;
	for (int i=0; i<(int)tmp_table.size(); i++) {
		key_line &key = tmp_table[i];
		table[ qprobe(key.key, c) ] = key;
		num_inuse++;  
	}
}

//hash function from hash handout2 part 1
int hash_table::hash(const string &key) {

	uint index = 0;
	const char *c = key.c_str(); 
	while (*c)

		index = ((index << 5) | (index >> 27)) + *c++;

	return index % table.size();

}

//remove punct works with transform
char remove_punctuation(char c){

	if(ispunct(c)){

		return ' ';
	}else{

		return c;
	}
}

//i cant spend any more time on this so this is what i have so far
int main(int argc, char* argv[]){

	//initing vars to keep track of lines of txt and other data
	//error check to see if enough input is in command lines and requiring a file
	vector<string> v;
	bool filecheck = false;
	string fchck;
	for(int i = 1; i < argc; i++){

		istringstream sin;
		sin.str(argv[i]);
		sin >> fchck;

		if(fchck == "-f"){

			filecheck = true;
		}
	}

	if(argc < 2 || filecheck == false){

		cerr << "usage ./Hashtable [-N number] [-showstats] -f textfile" << endl;
		return -1;
	}

	string cmd;
	int line_num = 0;
	int num = 1;

//checking for a num to init the size of the table
	for(int i = 1; i < argc; i++){

		istringstream sin;
		sin.str(argv[i]);
		sin >> cmd;

		if(cmd == "-N"){

			istringstream in;
			in.str(argv[(i + 1)]);
			in >> num;
		}
	}

	//init table and check for showstats and file
	hash_table ht(num);
	for(int i = 1; i < argc; i++){

		istringstream sin;
		sin.str(argv[i]);
		sin >> cmd;

		if(cmd == "-showstats"){

			ht.set_showstats();
			ht.insert_done();
			runstats = true;
		}
		//read in the file and get all the lines remove punctuation then stringstream the lines then add them to the table
		if(cmd == "-f"){

			string si;
			string so;
			string s;
			ifstream fin;
			istringstream in;
			fin.open(argv[(i + 1)]);
			while(!(fin.eof())){
				
				while(getline(fin, si)){
					
					v.push_back(si);
					so.resize(si.size());

					std::transform(si.begin(), si.end(), so.begin(), remove_punctuation);

					line_num++;
					for(int h = 0; h < so.size(); h++){

						in.str(so);
						in >> so;
					}

					ht.insert(so, line_num);
				}

			}


		}
	}

	string lf;
	cout << "find> ";

	while(cin >> lf){

		cout << "find> ";
		cout << v[3] << endl;
	//	if(ht.find(lf)){

	//		cout << "found" << endl; 
	}


	
	return 0;
}

