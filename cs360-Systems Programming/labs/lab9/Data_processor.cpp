
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
#include "Sptrsort.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "err" << endl;
	return 0;
  }


  vector<person> person_list;

  int line = 1;
  string input;
  size_t iL;

  getline(cin, input);

  cout << input << endl;




  //CS307: CHECK CORRECTESS OF XML HEADER 

  while (getline(cin, input)) {
	
  person n_person;
  ++line;

	if ((iL = input.find('<')) == string::npos) {
	  continue;

    } else if (input.compare(iL,9,"<faculty>") == 0) {
	  

	} else if (input.compare(iL,10,"</faculty>") == 0) {

      //CS307: CHECK CORRECTESS OF XML DATA
	  //CODE FOR ADDING FACULTY PERSON TO DATABASE

    		n_person.type = "faculty";
   
    n_person.c1 = "CS2 Pointy Pointers";
	  n_person.c2 = "PH1 Perms and Waves";
    n_person.c3 = "CS4 Reading Core Dumps";

    n_person.name = "Anna Graham";
    n_person.categ = "Assistant Professor";
	  person_list.push_back(n_person);

	  continue;

	} else if (input.compare(iL,9,"<student>") == 0) {
	  


	} else if (input.compare(iL,10,"</student>") == 0) {

      //CS307: CHECK CORRECTESS OF XML DATA
	  //CODE FOR ADDING STUDENT PERSON TO DATABASE
    n_person.type = "student";
    
	  n_person.c1 = "CS2 Pointy Pointers";
	  n_person.gp1 = 3.5;
	  n_person.c2 = "PH1 Perms and Waves";
	  n_person.gp2 = 2.8;
      n_person.c3 = "CS4 Reading Core Dumps";
	  n_person.gp3 = 3.7;
   
    n_person.name = "Kenny Dewitt";
    n_person.categ = "Sophomore";
	  person_list.push_back(n_person);
     
	  continue;

	} 
 }

if(strcmp(argv[1], "-person") == 0){

for(unsigned int i = 0; i < person_list.size(); i++){

    person_list[i].printinfo();
}
}

else if(strcmp(argv[1], "-student") == 0){

for(unsigned int i = 0; i < person_list.size(); i++){

    if(person_list[i].type == "student"){
    
      person_list[i].printinfo();
      }
}
}
else if(strcmp(argv[1], "-faculty") == 0){

for(unsigned int i = 0; i < person_list.size(); i++){

    if(person_list[i].type == "faculty"){
    
      person_list[i].printinfo();
      }
}
}


  //EXTRACT LIST OF POINTERS TO PERSON, FACULTY OR STUDENT
  //ENTRY, SORT USING SPTRSORT AND PRINT RESULT TO STDOUT

 // NOTE: PERSON, FACULTY AND STUDENT EACH HAVE THEIR OWN
 // OVERLOADED COMPARISON OPERATOR (--> DIFFERENT SORTING)

  //RELEASE ALL DYNAMICALLY ALLOCATED MEMORY
}



