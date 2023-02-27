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

#include "Person.h"

void person::printinfo(){

	cout << endl;
	cout << "Name: " << this->name << endl;
	cout << "Category: " << this->categ << endl;
	
	if(this->type == "student"){

		cout << "Course: " << this->c1 << " " << this->gp1 << endl;
    cout << "Course: " << this->c2 << " " << this->gp2 << endl;
    cout << "Course: " << this->c3 << " " << this->gp3 << endl;
  }
  else{
  
		cout << "Course: " << this->c1 << endl;
    cout << "Course: " << this->c2 << endl;
    cout << "Course: " << this->c3 << endl;
    }
}