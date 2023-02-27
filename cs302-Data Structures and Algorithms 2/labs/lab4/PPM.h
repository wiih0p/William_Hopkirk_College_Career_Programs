#ifndef __PPM_H__
#define __PPM_H__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

struct RGB { 
  RGB() { R = 0; G = 0; B = 0; }

  unsigned char R, G, B;
  
};

class PPM { 
  public:
    PPM() { 
	
		p6 = "";
		maxi = 0;
		w = 0;
		h = 0;
	
	}
    ~PPM() { delete this;}

	void read(string file);
	void write(string file);

  private:
	//data members

	vector<RGB> img;
	int w;
	int h;
	int maxi;
	string p6;
};

#endif
