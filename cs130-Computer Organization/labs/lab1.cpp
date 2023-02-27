//lab1
//lab takes in 4 command line arguments as c-style string and converts them to various data types
//will hopkirk
//8/30/19
//Dr. Marz
//no TAs worked with
//no Students worked with
#include <cstdio>
#include <iostream>

int main (int argc, char** argv){

	//if statement to check if there are enough arguments in the command line
	if (argc < 5){
		printf("Usage: [./lab1 value1 value2 value3 value4] - not enough arguments were given\n");
		return -1;
	}								    

	//declaring the variables for the various data types
	int int32;
	unsigned long int64;
	float fp32;
	double fp64;

	//takes the command line arguments and converts them accordingly (arg1 to 32bit int, arg 2 to 32 bit float etc.)
	sscanf(argv[1], "%d", &int32);
	sscanf(argv[3], "%ld", &int64);
	sscanf(argv[2], "%f", &fp32);
	sscanf(argv[4], "%lf", &fp64);

	//prints the converted values to the console while setting the widths and percisions
	printf("%-8d\n", int32);
	printf("%7.3f\n", fp32);
	printf("%05ld\n", int64);
	printf("%-6.5lf\n", fp64);
	

	return 0;
}
