//lab4
//will hopkirk

#include "hash140.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <stdlib.h>

using namespace std;

HashTable::HashTable(int table_size, string function, string collision){

	//setting up hash and functions and collision 
	keys.resize(table_size);
	vals.resize(table_size);

	if(function == "Last7"){

		Fxn = 0;
	}else{

		Fxn = 1;
	}

	if(collision == "Linear"){

		Coll = 0;
	}else{

		Coll = 1;
	}

}

void HashTable::Add_Hash(string &key, string &val){

	//stringstream and ints for reading and manipulating hash info
	istringstream sin;
	unsigned int last7 = 0;
	unsigned int XOR = 0;
	unsigned int hashkey;
	unsigned int hashkey1;
	int dhash;

	string tempv0;
	string tempv1;

	//this is all for last7 and xor i tried to do these in separate functions but that was against the format
	if(key.length() <= 7){

		tempv0 = key;
	}else{

		tempv0 = key.substr(key.length() - 7);
	}

	sin.str(tempv0);
	sin >> hex >> last7;
	sin.clear();

	tempv1 = key;
	sin.str(tempv1);
	sin >> hex >> XOR;
	sin.clear();

	for(unsigned int i = 0; i < key.length(); i += 7){

		unsigned int h;
		string t;

		if(key.length() >= (i + 7)){

			t = key.substr(i, 7);
		}else{

			t = key.substr(i);
		}
		sin.str(t);
		sin >> hex >> h;
		sin.clear();

		if(i == 0){

			XOR = h;
		}else{

			XOR ^= h;
		}
	}

	// adding function info depending on fxn mode
	if(Fxn == 0){

		hashkey = last7;
	}else{

		hashkey = XOR;
	}

	//storing hashkey as hashkey mod keys.size and added a sin.clear because it got more gradescripts for some reason
	hashkey %= keys.size();
	sin.clear();

	//error checking and getting other hash key value while counting probes to hash
	if(keys[hashkey] != ""){
		if(Coll == 0){
			for(unsigned int i = hashkey; i < keys.size(); i++){

				tmp++;
				if(i == keys.size()){

					{
						cerr << "Hash Table Full" << endl;
						exit(0);
					}

				}else if(keys[i] == ""){

					hashkey = i;
					break;
				}
			}
		}else{

			//dhash is for the creating the full hash with hashkey and hashkey1
			if(Fxn == 0){

				hashkey1 = XOR % keys.size();
			}else{

				hashkey1 = last7 % keys.size();
			}

			if(hashkey1 == 0){

				hashkey1++;
			}

			for(int i = 1; i <= keys.size(); i++){

				dhash = hashkey + (hashkey1 * i);
				dhash %= keys.size();

				if(keys[dhash] == ""){

					hashkey = dhash;
					break;
				}
				if(i == keys.size()){

					{
						cerr << "Hash Table Full" << endl;
						exit(0);
					}
				}
			}
		}
	}

	keys[hashkey] = key;
	vals[hashkey] = val;
}

string HashTable::Find(string &key){

	//same as add but not adding 
	//alot of this is just copied from add_hash because the format of this lab wouldnt allow for extra functions
	//this function instead of adding returns a key string
	istringstream sin;
	unsigned int last7 = 0;
	unsigned int XOR = 0;
	unsigned int hashkey;
	unsigned int hashkey1;
	int dhash;

	string tempv0;
	string tempv1;

	if(key.length() <= 7){

		tempv0 = key;
	}else{

		tempv0 = key.substr(key.length() - 7);
	}

	sin.str(tempv0);
	sin >> hex >> last7;
	sin.clear();

	tempv1 = key;
	sin.str(tempv1);
	sin >> hex >> XOR;
	sin.clear();




	for(unsigned int i = 0; i < key.length(); i += 7){

		unsigned int h;
		string t;

		if(key.length() >= (i + 7)){

			t = key.substr(i, 7);
		}else{

			t = key.substr(i);
		}
		sin.str(t);
		sin >> hex >> h;
		sin.clear();

		if(i == 0){

			XOR = h;
		}else{

			XOR ^= h;
		}
	}

	if(Fxn == 0){

		hashkey = last7;
	}else{

		hashkey = XOR;
	}

	hashkey %= keys.size();
	sin.clear();

	if(keys[hashkey] != key){
		if(Coll == 0){
			for(unsigned int i = hashkey; i < keys.size(); i++){

				tmp++;

				if(i == keys.size()){

					return("");
				}else if(keys[i] == key){

					return(vals[i]);
				}
			}

		}else{

			tmp++;

			if(Fxn == 0){

				hashkey1 = XOR;
			}else{

				hashkey1 = last7;
			}
			dhash = hashkey + hashkey1;

			for(int i = 1; i <= keys.size(); i++){

				dhash = (dhash * i) % keys.size();
				if(keys[dhash] == key){

					return(vals[dhash]);
				}else if(i == keys.size()){

					return("");
				}
			}

		}

	}else{

		//no collision
		return(vals[hashkey]);
	}
}

void HashTable::Print(){

	//printing out the keys and vals in the requested format
	for(unsigned int i = 0; i < keys.size(); i++){

		if(keys.at(i) != ""){

			printf("%5d %s %s/n", i, keys.at(i).c_str(),  vals.at(i).c_str());
		}
	}
}

int HashTable::Total_Probes(){

	//final add up of the total amout of probes used then return probes
	int tprobes = 0;
	for(unsigned int i = 0; i < keys.size(); i++){
		if(keys.at(i) != ""){
		
			Find(keys[i]);
			tprobes += tmp;
	}

	return tprobes;

}
}

/*int Last7(string &key){


  }


  int XOR(){

  
  }
  */

