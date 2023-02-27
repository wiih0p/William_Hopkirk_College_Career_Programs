//lab7
//
//will hopkirk
//

#include "code_processor.hpp"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>

using namespace std;

bool Code_Processor::New_Prize(const string &id, const string &description, int points, int quantity){

	//error check
	if(Prizes.find(id) != Prizes.end() || points <= 0 || quantity <= 0){

		return false;
	}

	//make prize and insert it into prizes map
	Prize *tmpprize = new Prize;
	tmpprize->id =id;
	tmpprize->description = description;
	tmpprize->points = points;
	tmpprize->quantity = quantity;
	Prizes.insert(make_pair(id, tmpprize));

	return true;
}

bool Code_Processor::New_User(const std::string &username, const std::string &realname, int starting_points){


	//error check
	if(Names.find(username) != Names.end() || starting_points < 0){

		return false;
	}

	//same as prizes but for users
	User *tmpuser = new User;
	tmpuser->username = username;
	tmpuser->realname = realname;
	tmpuser->points = starting_points;

	Names.insert(make_pair(username, tmpuser));

	return true;

}

bool Code_Processor::Delete_User(const string &username){

	//error check
	if(Names.find(username) == Names.end()){

		return false;
	}

	//create iterators
	set <string>::const_iterator i;
	map <string, User *>::iterator h;
	//delete all the user's phone numbers
	User *tmpuser = Names.find(username)->second;
	for(i = tmpuser->phone_numbers.begin(); i != tmpuser->phone_numbers.end(); i++){

		h = Phones.find(*i);
		Phones.erase(h);
	}

	//remove user from map
	map <string, User *>::iterator j;
	j = Names.find(username);
	User *tmpname = j->second;
	Names.erase(j);
	delete tmpname;

	return true;
}

bool Code_Processor::Add_Phone(const std::string &username, const std::string &phone){

	//error check
	if(Names.find(username) == Names.end() || Phones.find(phone) != Phones.end()){

		return false;
	}

	//add number to user and phones map
	Names.find(username)->second->phone_numbers.insert(phone);
	Phones.insert(make_pair(phone, Names.find(username)->second));

	return true;
}

bool Code_Processor::Remove_Phone(const std::string &username, const std::string &phone){

	//error check
	if(Names.find(username) == Names.end() || Phones.find(phone) == Phones.end() || Phones.find(phone)->second->username != username){

		return false;
	}

	//removing phone from phone map like delete user
	set <string>::iterator i;
	map <string, User *>::iterator h;
	User *tmpuser = Phones.find(phone)->second;
	i = tmpuser->phone_numbers.find(phone);
	h = Phones.find(*i);
	Phones.erase(h);
	tmpuser->phone_numbers.erase(i);

	return true;
}

string Code_Processor::Show_Phones(const std::string &username) const{

	//error check
	if(Names.find(username) == Names.end()){

		return "BAD USER";
	}

	//make a return string and add users phone nums
	string phn = "";
	set <string>::iterator i;
	User *tmpuser = Names.find(username)->second;

	for(i = tmpuser->phone_numbers.begin(); i != tmpuser->phone_numbers.end(); i++){

		phn += *i;
		phn += "\n";
	}

	return phn;
}

int Code_Processor::Enter_Code(const std::string &username, const std::string &code){

	//error check
	if(Codes.find(code) != Codes.end() || Names.find(username) == Names.end()){
		
		return -1;
	}

	//add points to user score through temp reference hashing
	User *tmpuser = Names.find(username)->second;
	size_t i;
	unsigned int h;

	h = 5381;

	for(i = 0; i <code.size(); i++){

		h = (h << 5) + h + code[i];
	}

	if(h % 17 == 0){

		Codes.insert(code);
		tmpuser->points += 10;
		return 10;
	}else if(h % 13 == 0){

		Codes.insert(code);
		tmpuser->points += 3;
		return 3;
	}else{

		return 0;
	}
}

int Code_Processor::Text_Code(const std::string &phone, const std::string &code){

	//error check
	if(Phones.find(phone) == Phones.end()){

		return -1;
	}

	//get user and use enter code
	int val;
	User *tmpuser = Phones.find(phone)->second;

	val = Enter_Code(tmpuser->username, code);
	
	return val;
}

bool Code_Processor::Mark_Code_Used(const std::string &code){

	//add code through hashing if possible
	size_t i;
	unsigned int h;

	h = 5381;

	for(i = 0; i < code.size(); i++){

		h = (h << 5) + h + code[i];
	}
	if(Codes.find(code) != Codes.end()){

		return false;
	}else if(h % 17 == 0 || h % 13 == 0){

		Codes.insert(code);
		return true;
	}else{

		return false;
	}
}

int Code_Processor::Balance(const std::string &username) const{

	//return points to specific user
	map <string, User *>::const_iterator i;
	i = Names.find(username);
	if(i == Names.end()){

		return -1;
	}else{

		return i->second->points;
	}
}

bool Code_Processor::Redeem_Prize(const std::string &username, const std::string &prize){

	//use iterators to increment points and quantity
	map <string, User *>::const_iterator i;
	map <string, Prize *>::iterator h;
	i = Names.find(username);
	h = Prizes.find(prize);

	//error check
	if(i == Names.end() || Prizes.find(prize) == Prizes.end() || i->second->points < h->second->points){
		
		return false;
	}

	i->second->points -= h->second->points;
	h->second->quantity -= 1;

		                
	if(h->second->quantity == 0){

		Prize *tmpprize = h->second;
		Prizes.erase(h);
		delete tmpprize;
	}

	return true;
}

Code_Processor::~Code_Processor(){

	//deconstruct new users and prizes
	map <string, User *>::iterator i;
	map <string, Prize *>::iterator h;

	for(i = Names.begin(); i != Names.end(); i++){

		User *tmpuser = i->second;
		delete tmpuser;
	}
	for(h = Prizes.begin(); h != Prizes.end(); h++){

		Prize *tmpprize = h->second;
		delete tmpprize;
	}
}


bool Code_Processor::Write(const std::string &filename) const{

	//open file and check that its open
	ofstream fout;
	fout.open(filename.c_str());
	if(!(fout.is_open())){

		return false;
	}

	//set iterators
	map <string, User *>::const_iterator i;
	map <string, Prize *>::const_iterator h;
	set <string>::iterator j;
	set <string>::iterator k;

	//going through map to add users and their phones
	for(i = Names.begin(); i != Names.end(); i++){

		fout << "ADD_USER " << i->second->username << " " << i->second->points << " " << i->second->realname << endl;
		User *tmpuser = Names.find(i->second->username)->second;
		for(k = tmpuser->phone_numbers.begin(); k != tmpuser->phone_numbers.end(); k++){

			fout << "ADD_PHONE " << i->second->username << " " << *k << endl;
		}
	}

	//call prize for all prizes and mark_used_code for all codes
	for(h = Prizes.begin(); h != Prizes.end(); h++){

		fout << "PRIZE " << h->second->id << " " << h->second->points << " " << h->second->quantity << " " << h->second->description << endl;
	}

	for(j = Codes.begin(); j != Codes.end(); j++){

		fout << "MARK_USED " << *j << endl;
	}

	//close out file
	fout.close();

	return true;

}


