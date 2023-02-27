#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char argv[]){

	string filename;
	int startingLine = 0;
	int endingLine = 0;

	if (argc < 2 || argc > 4){
		cout << "Usage: " << argv[0] << " <filename> [start] [end]\n";
		return -1;
	}

	ifstream fin;
	fin.open(argv[1]);
	if(!fin){
		perror(argv[1]);
		return -1;
	}
	istringstream sin;
	if(argc > 2){
		sin.str(argv[2]);
		sin >> startingLine;
	}
	sin.clear();
	if(argc > 3){
		sin.str(argv[3]);
		sin >> endingLine;
	}
	string line;
	int i = 1;
	while(getline(fin, line)){
		if(i >= endingLine){
			break;
		}
		if(i >= startingLine){
			cout << line << '\n';
		}
		i++;

}
