#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>


//readme
//
//
//i barely just finnished the begining parts of the lab on the last day due i couldnt really work on crypto till those were done so here we are
//
//
using namespace std;

#include "PPM.h"
#include "Rnumgen.h"

const char ETX = 0x3;

struct pixel {
  
	int r;
	int g;
	int b;
};

//void set_pixel_list(...) {
//  1write this
//}

//void encode(PPM &i) {
  
//}

//void decode(PPM &i) {
//  write this
//}

int main(int argc, char *argv[]) {
  
	if(argc != 3){

		cerr << "incorrect cmd line args" << endl;
		return -1;
	}

  PPM image;
  string fname = argv[2];

  image.read(fname);
  
  return 0;

/*  declare pixel_list

  set_pixel_list(pixel_list);

  if (mode = ENCODE) {
    encode(img, pixel_list);
	img.write(fname)
  }
  else
  if (mode = DECODE) 
    decode(img, pixel_list);

	*/
}
