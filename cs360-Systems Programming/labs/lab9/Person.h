#ifndef __PERSON_H__ 
#define __PERSON_H__

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

using namespace std;

enum type_enum { UNKNOWN=-1, FACULTY=0,  ASST_PROF=1, ASSOC_PROF=2, FULL_PROF=3,
                 STUDENT=10, FRESHMAN=11, SOPHOMORE=12, JUNIOR=13, SENIOR=14 };

class person {

	public:

	string name;
	string type;
	string categ;

	string c1;
	string c2;
	string c3;

	float gp1;
	float gp2;
	float gp3;
 
   void printinfo();
};
//class faculty DEFINITION -- derived class

//class student DEFINITION -- derived class

#endif

