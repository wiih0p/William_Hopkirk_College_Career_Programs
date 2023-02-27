//lab4
//lab adds subtracts and multiplies 2 numbers only using logical operators
//will hopkirk
//9/20/19
//Dr. Marz
//no TAs worked with
//no Students worked with

#include <cstdio>
#include <iostream>

using namespace std;

void add(bool left, bool right, bool cin, bool &cout, bool &sum);
int twos(int value);
int add(int left, int right);
int sub(int left, int right);
int mul(int left, int right);

int main(){

	int l, r;
	printf("Enter two values: ");
	scanf("%d %d", &l, &r);
	printf("%d + %d = %d\n", l, r, add(l, r));
	printf("%d - %d = %d\n", l, r, sub(l, r));
	printf("%d * %d = %d\n", l, r, mul(l, r));

	return 0;
}

void add(bool left, bool right, bool cin, bool &cout, bool &sum){

	//adds the bits by xoring them together then carrying them in then carrying out the new digit
	sum = left ^ right ^ cin;
	cout = (right & left) | (right & cin) | (left & cin);
}

int twos(int value){

	//finds the twos compliment by adding the notted value with one
	int tc = add(~value, 1);

	return tc;
}

int add(int left, int right){

	int i;
	int tot = 0;
	bool sum = false;
	bool cin = false;
	bool cout = false;

	//loop to add the two number together bit by bit
	for(i = 0; i < 32; i++){

		//add one bit then set the new carryin to new carryout
		add(((left >> i) & 1) == 1, ((right >> i) & 1) == 1, cin, cout, sum);
		cin = cout;

		//check if bit is 1 then Lshift total accordingly to construct the final value
		if(sum){

			tot |= (1 << i);
		 }
	 }
	return tot;
}

int sub(int left, int right){

	//add left with the twos compliment of the right basically always adding left to opposite of right
	int tot = add(left, twos(right));

	return tot;
}

int mul(int left, int right){

	unsigned int l;
	unsigned int r;
	int tot = 0;
	bool neg = false;

	//check sign bit to see if number is negative by shifting 31 places for each number
	if(((right >> 31) & 1) == 1){

		r = twos(right);
		neg = !neg;
	}else{

		r = right;
	}

	if(((left >> 31) & 1) == 1){

		l = twos(left);
		neg = !neg;
	}else{
		
		l = left;
	}

	//loop to keep adding until the numbers are multiplied correctly
	while(r != 0){

		if(r & 1){
			
			tot = add(tot, l);
		}
		r = (r >> 1);
		l = (l << 1);
	}

	//get twos compliment if neg is true to make total negative
	if(neg){

		tot = twos(tot);
	}

	return tot;
	
}

