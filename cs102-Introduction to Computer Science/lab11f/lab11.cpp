//Will Hopkirk
//Dr. Marz
//4/16/2019
//Create manual vector
//cite: Alex Baldwin

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;

class doublevector {
	static constexpr double DOUBLE_DEFAULT_VALUE = -5.55;
	double *mValues;
	int mNumValues;
	public:
	doublevector();
	~doublevector();
	void resize(int new_size, double initial_value=DOUBLE_DEFAULT_VALUE);
	void push_back(double value);
	double &at(int index);
	const double &at(int index) const;
	unsigned long size() const;
};
void print_all(const doublevector &v)
{
	if (v.size() == 0) {
		cout << "Vector is empty.\n";
	}
	else {
		unsigned long i;
		for (i = 0;i < v.size();i++) {
			cout << "[" << setfill('0') << right << setw(3) << i
				<< "] = " << fixed << setprecision(4) << v.at(i)
				<< '\n';
		}
	}
}
int main()
{
	doublevector v;
	do {
		string command;
		cout << "Enter a command ('quit' to quit): ";
		if (!(cin >> command) || command == "quit") {
			break;
		}
		else if (command == "push_back") {
			double value;
			cin >> value;
			v.push_back(value);
			cout << "Pushed back "
				<< fixed << setprecision(4)
				<< value << '\n';
		}
		else if (command == "resize") {
			string line;
			int new_size;
			double initial;
			cin >> new_size;
			getline(cin, line);
			istringstream sin(line);

			if (sin >> initial)
				v.resize(new_size, initial);
			else
				v.resize(new_size);
		}
		else if (command == "size") {
			cout << v.size() << '\n';
		}
		else if (command == "print") {
			print_all(v);
		}
		else if (command == "get") {
			int index;
			cin >> index;
			try {
				cout << "Value at " << index << " = "
					<< setprecision(4) << fixed
					<< v.at(index) << '\n';;
			}
			catch(out_of_range &err) {
				cout << err.what() << '\n';
			}
		}
		else if (command == "set") {
			double d;
			int index;
			cin >> index >> d;
			try {
				v.at(index) = d;
				cout << "SET: " << index << " = "
					<< setprecision(4) << fixed
					<< v.at(index) << '\n';;
			}
			catch(out_of_range &err) {
				cout << err.what() << '\n';
			}
		}
		else if (command == "clear") {
			v.resize(0);
		}
		else {
			cout << "Invalid command '" << command << "'\n";
		}
	} while(true);
	cout << endl;
	return 0;
}
//Write your class members below here.
//ok

//member function to construct a doublevector size 0
doublevector::doublevector(){
	mNumValues = 0;
	mValues = nullptr;
}

//member function to deconstruct doublevector
doublevector::~doublevector(){
	if(mNumValues != 0){
		delete[] mValues;
	}
}

//member function to resize the doublevec to new_size
void doublevector::resize(int new_size, double initial_value){

	//if the size is less than 0 do nothing
	if(new_size < 0){		
		return;
	}
	//check if new size is less than size copy size from temporary pointer x
	else if(new_size < mNumValues){

		if(new_size == 0){
			mNumValues = 0;
			mValues = nullptr;
			delete mValues;
		}
		else{
			double *x = new double [new_size];
			for(int i = 0; i < mNumValues; i++){
				x[i] = mValues[i];
			}
			delete [] mValues;
			mValues = x;
			mNumValues = new_size;
		}
	}
	//if new size is larger than size create temporary memory x and copy to new size
		else if (new_size > mNumValues){

			double *x = new double [new_size];
			for (int i = 0; i < mNumValues; i++){
				x[i] = mValues[i];
			}
			delete [] mValues;
			mValues = x;

			for (int i = mNumValues; i < new_size; i++){
				mValues[i] = initial_value;
			}
			mNumValues = new_size;
		}
	}

	//member function to increase vector by 1
	void doublevector::push_back(double value){

		resize(mNumValues + 1, value);
	}

	//member function to return a value from a given index from the vector
	double& doublevector::at(int index){

		if (index >= mNumValues) {
			ostringstream sout;
			sout << "Invalid index #" << index;
			throw out_of_range(sout.str());
		}
		else{
			return mValues[index];
		}
	}


	//member function to return a read-only value from the index of the vector
	const double& doublevector::at(int index) const{

		if (index >= mNumValues) {
			ostringstream sout;
			sout << "Invalid index #" << index;
			throw out_of_range(sout.str());
		}
		else{
			return mValues[index];
		}
	}

	//member function to typecast mNumVals to an unsigned long
	unsigned long doublevector::size() const{

		return (unsigned long)mNumValues;
	}





