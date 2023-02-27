//lab3
//lab manipulates individual bits in a number
//will hopkirk
//8/6/19
//Dr. Marz
//no TAs worked with
//Students worked with: Alex Baldwin

#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

class Bitset{
	unsigned long m_value;
public:
	Bitset();
	Bitset(unsigned long initial_value);
	bool test(int bit) const;
	void set(int bit);
	void clear(int bit);
	void reset();
	void print() const;
};

int main(){

	Bitset mybs { 0b00011100111UL };

	mybs.print();
	printf("\n");

	mybs.set(44);
	mybs.set(45);
	mybs.clear(1);
	mybs.set(13);
	mybs.clear(14);
	mybs.print();
	printf("\n");

	mybs.reset();
	mybs.print();
	printf("\n");


	return 0;
}

//construtor does absolutly nothing
Bitset::Bitset(){
	
}

//constructor sets m_value to the given value
Bitset::Bitset(unsigned long initial_value){

	m_value = initial_value;

}

//checks if given bit is one or zero and checks out of range value, returns true for 1 and false for 0 or out of range
bool Bitset::test(int bit) const{

	bool test = false;

	if(bit >= 64){

		return test;
	}else{
		
		//Rshift by bit amount and & by one to see if bit is one or zero then sets to the boolean test
		test = ((m_value >> bit) & 1) == 1;
		return test;
	}

}

//sets m_value by oring the value to Lshifted unsigned long
void Bitset::set(int bit){

	if(bit < 64){

		m_value |= (1UL << bit);
	}
}

//and m_value to a notted unsigned long Lshifted by bit
void Bitset::clear(int bit){

	if(bit < 64){

		m_value &= ~(1UL << bit);
	}

}

//clears bitset to a 0 value unsigned long
void Bitset::reset(){

	m_value = 0UL;
}

//for loop to construct the long value then prints it adding "_" every 4 spaces
void Bitset::print() const{

	string value;

	for(int i = 63; i >= 0; i--){

		if(test(i)){

			value += '1';
		}else{
			
			value += '0';
		}
		if(i != 0 && i % 4 == 0){

			value += '_';
		}
	}
	
	printf(value.c_str());

}
