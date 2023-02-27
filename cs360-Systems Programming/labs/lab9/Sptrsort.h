#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <set>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <utility>
#include <sstream>
#include <cmath>
#include <map>

//WRITE THIS BASED ON SMART POINTER CLASS

using namespace std;

template <typename T>class sptr {  
  public:
      
    sptr(T *_ptr=NULL) { ptr = _ptr; }
        
    bool operator< (const sptr &rhs) const {
      
      return *ptr < *rhs.ptr;    
      
    }    
      
      operator T * () const { return ptr; }
  private:
    
    T *ptr;
};

template <typename T>
void sptrsort(std::vector<T *> &A) {
  //USE std::sort on SPTR VERSION OF A
  //NOTE: A is vector of pointers (T *)
  
	std::sort(A.begin(), A.end());
  
}
