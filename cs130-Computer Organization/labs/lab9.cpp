//lab9
//lab takes a virtual adress and simulating an MMU returns an actual adress
//will hopkirk
//11/26/19
//Dr. Marz
//no TAs worked with
//worked with Nick Mize

#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv){

	//check if enough args to start program
	if(argc != 2){

		return -1;
	}

	//init file as well as bools for checking if the user is inputing correct figures
	//also init string and cstyle string to check if the user quits
	char stringquit[4];
	string stringq;
	bool nonhex = false;
	bool pgfault = false;
	bool loopdone = false;
	FILE *fin;

	//init variables for all parts of the virtual and physical adresses
	uint32_t va;
	uint64_t vpn1;
	uint64_t vpn0;
	uint64_t po;
	uint32_t mode;
	uint32_t satp;
	uint64_t ppn;
	uint32_t pgadress1;
	uint32_t pgadress0;
	uint64_t pa = 0;

	//open mmu file for reading
	fin = fopen(argv[1], "rb");

	//check if file is correct for mmu
	if(nullptr == fin){

		perror(argv[1]);
		return -1;
	}

	//search file for satp and get mode
	fseek(fin, -4, SEEK_END);
	fread(&satp, 4, 1, fin);
	mode = satp >> 31;
	
	//loop for mmu simulation
	while(!loopdone){

		//prompt user and check response for errors or if they type quit
		printf("Enter virtual adress (or type 'quit' to exit): ");
		if(scanf("%x", &va) != 1){

			scanf("%s", stringquit);
			stringq = stringquit;
			if(stringq == "quit"){

				loopdone = true;
			}
			else{


				printf("enter a hexidecimal number \n");
				nonhex = true;
			}
		}
		
		
		//get vpn1 and 0 and po from virtual adress
		vpn1 = va >> 22;
		vpn0 = (va & 0x3FF000) >> 12;
		po = va & 0xFFF;

		//simultate mmu if mode is one else physical = virtual
		if (mode == 1){

			//get ppn and read page adress
			ppn = (satp & 0x3FFFFF) << 12;
			fseek(fin, ppn, SEEK_SET);
			fseek(fin, (vpn1 * 4), SEEK_CUR);
			fread(&pgadress1, 4, 1, fin);

			//formats page adress from vpn1, 0 and pgoffset into physical adress form
			if ((pgadress1 & 1) == 1){

				if((pgadress1 & 0xE) == 1){

					vpn0 = vpn0 << 12;
					pa = pa | ((pgadress1 & 0x3FF00000) << 2) | vpn0 | po;
				}
				else{

					ppn = pgadress1 & 0xFFFFFC00;
					ppn = ppn << 2;
					fseek(fin, ppn, SEEK_SET);
					fseek(fin, (vpn0 * 4), SEEK_CUR);
					fread(&pgadress0, 4, 1, fin);

					if(((pgadress0 & 1) == 1) && ((pgadress0 & 0xE) != 0)){

						pa = pa | ((pgadress0 & 0xFFFFFC00) << 2) | po;
					}

					//pagefault if pageadress0 contains a 1
					else{

						pgfault = true;

					}
				}
			}
			
		//pagefault if pageadress1 contains a 1
		else{

			pgfault = true;
		}
		}
		//otherwise if mode isnt one pa = va
		else{

			pa = va;
		}
		//pagefault case for pageadresses
		if(pgfault && !loopdone && !nonhex){

			printf("Page fault!\n");
		}
		//formats output to 0x----- format
		else if(!loopdone && !nonhex){

			printf("0x%09lx\n", pa);
		}
	}
	
	//reset bools and physical adress for next use
	nonhex = false;
	pgfault = false;
	pa = 0;
						
	return 0;
}

