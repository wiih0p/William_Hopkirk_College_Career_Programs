
// include header files needed
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

class data {
  public:
    // add operator< using lastname, firstname, phone number

    friend istream & operator>>(istream &, data &);
    friend ostream & operator<<(ostream &, const data &);
	bool operator<(const data &)const;

  private:
    string firstname;
    string lastname;
    string phonenum;
};

bool data::operator<(const data &d)const {
//check if lastnames are the same if so use first names
	if(this->lastname == d.lastname){

		return (this->firstname < d.firstname);
	}else{

		return (this->lastname < d.lastname);
	}
}

istream & operator>>(istream &in, data &r) { 
  // write this to read data object data
  
	in >> r.firstname >> r.lastname >> r.phonenum;
	return in;

}

ostream & operator<<(ostream &out, const data &r) {
  // write this to write data object data
 
	//concatinate strings of fn and ln to setw correctly
		out << setw(23) << left << (r.lastname + ", " + r.firstname) << " "  << setw(12) << left << r.phonenum;

	return out;
}

//sry very busy just got engaged!
template <typename T>
int partition(std::vector<T> &A, int left, int right) {  
	// sort: order left, middle and right elements  
	int middle = (left+right)/2;  
	if (A[middle] < A[left])  swap(A[middle], A[left]);  
	if (A[right] < A[left])   swap(A[right], A[left]);  
	if (A[right] < A[middle]) swap(A[right], A[middle]);  
	if (right-left+1 <= 3)     return middle;  
	// select pivot: median-of-three  
	int pindex = middle;  T pivot = A[pindex];  
	// partition A: {<=}, {pivot}, {=>}   
	swap(A[pindex], A[right-1]);  
	int i = left;  
	int j = right-1;   
	while (1) {    
		while (A[++i] < pivot) { }    
		while (pivot < A[--j]) { }    
		if (i>=j) break;    
		swap(A[i], A[j]);  }
	pindex = i;  
	swap(A[pindex], 
			A[right-1]);  
	return pindex;
}

template <typename Tdata>
void quicksort(...) { 
  
	
}

template <typename Tdata>
void quickselect(...) { 
  // write this 
}



//template <typename Tdata>
void printlist( const vector<data>::iterator &B,  const vector<data>::iterator &E) { 
  // see Lab 1

	vector<data>::iterator begin;
	begin = B;

	for(begin; B != E; begin++){

		cout << *begin << endl;
	}

}

int main(int argc, char *argv[]) {
  // perform command-line error checking

	if(argc < 3 || argc > 4){

		cerr << "usage: Qsort -stl | -rpivot [k0 k1] file.txt" << endl;
		return -1;
	}

	//read in, sort, and print out
	ifstream fin;

  fin.open(argv[2]); 
	istringstream sin;
	string fn;
	string ln;
	string pn;

  vector<data> A;

	while (!fin.eof()){
    
		data din;
		
		fin >> din;

		A.push_back(din);
	}

  fin.close();
  
  
  if (strcmp(argv[1], "-stl") == 0) {
    std::sort(A.begin(), A.end());

  } //else 
  //if (sorting option -rpivot) {
    //int N = (int)A.size();

    //int k0 = 0;
    //int k1 = N-1;

    // if specified, update k0, k1 and apply quickselect 
    // to ensure that A[0:k0-1] <= A[k0:k1] <= A[k1+1:N-1]

    //quicksort(A, k0, k1);
  //} 

  for(int i = 0; i != A.size(); i++){

	  cout << A[i] << endl;
  }
}
