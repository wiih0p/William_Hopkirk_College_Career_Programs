#ifndef __RNUMGEN_H__
#define __RNUMGEN_H__

#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <numeric>
#include <algorithm>


//from handout
using namespace std;

class rnumgen {  
	public:    
		rnumgen(int seedvalue, std::vector<int> &v);    
		int rand();  
	private:    
		std::vector<float> F;
};

rnumgen::rnumgen(int seedvalue, vector<int> &v) {  
	srand(seedvalue);  F.resize(v.size());  
	partial_sum(v.begin(), v.end(), F.begin());  
	transform(F.begin(), F.end(), F.begin(),  bind2nd(divides<float>(), F.back()));
}

int rnumgen::rand() {  
	const double randmax = RAND_MAX+1.0;  
	const double p = (double)std::rand()/randmax;  

	return upper_bound(F.begin(), F.end(), p) - F.begin();
}

#endif
