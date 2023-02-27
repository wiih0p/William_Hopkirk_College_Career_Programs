//lab5
//lab reads in a bitmap file and inverts the color and reverses the image
//will hopkirk
//10/4/19
//Dr. Marz
//no TAs worked with
//no Students worked with

#include <cstdio>
#include <iostream>
#include <cstdint>

using namespace std;

//declare bitmap header structures
#pragma pack(push, 2)
struct BitmapFileHeader{

	uint16_t type;
	uint32_t size;
	uint16_t reserve_1;
	uint16_t reserve_2;
	uint32_t offset;

};

struct BitmapInfoHeader{

	uint32_t size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitcount;
	uint32_t compression;
	uint32_t imagesize;
	uint32_t x_pixels_per_meter;
	uint32_t y_pixels_per_meter;
	uint32_t color_used;
	uint32_t color_important;


};
#pragma pack(pop)

//declare pixel structure
struct Pixel{

	uint8_t b;
	uint8_t g;
	uint8_t r;
};

int main(int argc, char** argv){

	//check if correct args were given if not closes program
	if(argc != 3){
		
		return -1;
	}

	//declareing files and structures
	FILE *fin;
	FILE *fout;
	Pixel *pix;
	BitmapFileHeader bfh;
	BitmapInfoHeader bih;

	//open files for reading and writing
	fin = fopen(argv[1], "rb");
	fout = fopen(argv[2], "wb");

	//checking if args are correct format else closing the program
	if(nullptr == fin){

		perror(argv[1]);
		return -1;
	}
	if(nullptr == fout){

		perror(argv[2]);
		return -1;
	}

//checking if headers match the file headers else closing the program
	if(sizeof(BitmapFileHeader) != fread(&bfh, 1, sizeof(bfh), fin)){

		return -1;
	}
	if(sizeof(BitmapInfoHeader) != fread(&bih, 1, sizeof(bih), fin)){

		return -1;
	}

	//getting number of pixels in the bitmap file
    uint32_t numP = (bih.height * bih.width);
	
	//creating an array for the pixels
	pix = new Pixel[numP];

	//finding padding and setting a buffer
	uint32_t padding = (4 - (bih.width * sizeof(Pixel) % 4)) % 4;
    uint32_t buf = 0;

	//seeing that offset is correct for reading pixels
	fseek(fin, bfh.offset, SEEK_SET);

	//read in input file pixel by pixel
	for (uint32_t i = 0; i <  bih.height; i++){

		fread(pix + (bih.width * i), 1, (sizeof(Pixel) * bih.width), fin);
		fseek(fin, padding, SEEK_CUR);
	}

	//color inversion
	for(uint32_t i = 0; i < numP; i++){

		pix[i].b = 255 - pix[i].b;
		pix[i].g = 255 - pix[i].g;
		pix[i].r = 255 - pix[i].r;
	}

	//flipping all pixels from the right to the left
    for(uint32_t i = 0; i < bih.height; i++){

		for(uint32_t h =0; h < (bih.width / 2); h++){

			uint32_t h1 = bih.width - (h1 + 1);

			Pixel right = pix[h + (bih.width * i)];
			Pixel left = pix[h1 + (bih.width * i)];
			pix[h + (bih.width * i)] = right;
			pix[h1 + (bih.width + i)] = left;
		}
	}

	//writing headers to output file
	fwrite(&bfh, 1, sizeof(BitmapFileHeader), fout);
	fwrite(&bih, 1, sizeof(BitmapInfoHeader), fout);

	//seeing that the output file offset is correct for writing pixels
	fseek(fout, bfh.offset, SEEK_SET);

	//writing output file pixel by pixel
	for(uint32_t i = 0; i < bih.height; i++){

		fwrite(pix + (bih.width * i), 1, (sizeof(Pixel) * bih.width), fout);
		fwrite(&buf, 1, padding, fout);
	}

	//closing file and deleting array
	if(fin != nullptr){

		fclose(fin);
	}
	if(fout != nullptr){

		fclose(fout);
	}

	delete[] pix;


	return 0;
}

