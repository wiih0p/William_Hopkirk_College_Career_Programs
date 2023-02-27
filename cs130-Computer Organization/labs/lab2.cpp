//lab2
//lab takes decimal value as unsigned int and converts to string in base 2, base 10, and base 16
//will hopkirk
//8/6/19
//Dr. Marz
//no TAs worked with
//no Students worked with

#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

string base2(unsigned int value);
string base10(unsigned int value);
string base16(unsigned int value);

int main(){

	unsigned int value;

	printf("Enter a decimal number: ");
	scanf("%u", &value);

	printf("Base  2 = 0b%s\n", base2(value).c_str());
	printf("Base 10 = %s\n", base10(value).c_str());
	printf("Base 16 = 0x%s\n", base16(value).c_str());

	return 0;

}

string base2(unsigned int value){
	
	//declare string to return
	string binary;

	//loop to concatenate each bit to the binary string
	do{
		binary += ((value % 2) + 48);
		value = (value / 2);

	}while(value > 0);

	return binary;
}

string base10(unsigned int value){

	//declare string to return
	string decimal;
	
	//loop to concatenate each bit to decimal string
	do{

		decimal += ((value % 10) + 48);
		value = (value / 10);
	}while(value > 0);
	
	return decimal; 
}
string base16(unsigned int value){
	
	//declare string to return
	string hex;

	//loop to concatenate each bit to hex string
	do{
		//checks to see if number is greater than 9 and if so replaces the value with abcdef for 10 11 12 13 14 and 15
		if ((value % 16) > 9){

			hex += ((value % 16) + 87);
		}else{

			hex += ((value % 16) + 48);
		}

		value = (value / 16);

	}while(value > 0);

	return hex;

}

