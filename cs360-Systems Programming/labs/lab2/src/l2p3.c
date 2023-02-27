//will hopkirk
//l2pl converter
//

#include <stdio.h>
#include <stdlib.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"
#include <string.h>

/*
 * NOTICE
 * need these includes for read and open etc.
*/

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//the man pages told me i needed this
#include <unistd.h>

/**
 * struct: machine
 * purpose: storing data for machines
 * in main current machine is called: mc
*/

typedef struct {

	char *ip;
	int numnames;
	JRB names;

}machine;



int main() { 

	/*
	 * variables for various purposes
	 * in = input file for fread()
	 * input = char for read byte
	 * i = while loop check for input
	 * mc =  current machine being input
	 * n = iterator for storing chars into string one by one
	 * tmpips = temporary(tmp) IP(ip) string(s)
	 * dotcheck = used for checking when to input a short name
	 * tmpip = temporary(tmp) IP(ip) char[]
	 * machines = tree for inserting machines
	 * temp = tree for traversal
	 * arbitrary malloc size for some char *s is 50
	 * tmpnn = temporary(tmp) number(n) of names(n)
	 * also allocate for JRB machines, open file, check for file error, and malloc tmpips and input
	*/

	int in;
	char *input;
	int i = 1;
	machine *mc;
	int n;
	char *tmpips;
	int dotcheck;

	unsigned char tmpip[4];

	JRB machines, temp;

	machines = make_jrb();

	in = open("converted", O_RDONLY);
	if(in < 0){

		printf("Error reading file - make sure 'converted' is in the same directory as the executable\n");
		exit(0);
	}

	input = (char *) malloc(sizeof(char) * 1);
	tmpips = (char *) malloc(50);

	/*
	 * loop for reading in machines is split into 4 distinct sections
	 * part 1 - reads in 4 bytes and forms and stores the machines IP
	 * part 2 - reads in the 4 bytes that correspond to the number of names to add to the machine
	 * part 3 - loops for the number of names established in part 2 and reads them all accounting for short names as well
	 * part 4 - stores all mc data into a jrb of all machines in converted
	*/

	/*
	 * CHANGES FOR L2P3
	 * add buffer to read everything source
	 * read in everything
	 * make iterator it to hold my place in the source buffer
	 * change while so that i read till the iterator (where i am in source) reaches the end of source
	 * everywhere i was using read i now use memcpy to grab 1 byte from source at a time
	 * works the same as l2p2 and l2p3 but now reads everything into a char[350000] aka source[350000] = converted
	*/

	char source[350000];
	i = read(in, source, 350000);
	int it = 0;

	while(it <= i){

		/*
		 * PART 1
		 * first allocate memory for the mc (current machine) and allocat the machine's JRB of names
		 * second read all 4 bytes into the 4 slots of tmpip
		 * third use sprintf to concatonate the numbers with '.' chars between them to form IP
		 * fourth store IP in mc using strdup to not waste time with malloc
		*/

		mc = malloc(sizeof(machine));
		mc->names = make_jrb();

		int tmpnn = 0;

		memcpy(input, (source + it), 1); it++;
		tmpip[0] = *input;
		memcpy(input, (source + it), 1); it++;
		tmpip[1] = *input;
		memcpy(input, (source + it), 1); it++;		
		tmpip[2] = *input;
		memcpy(input, (source + it), 1); it++;
		tmpip[3] = *input;

		sprintf(tmpips, "%d.%d.%d.%d", tmpip[0], tmpip[1], tmpip[2], tmpip[3]);
		mc->ip = strdup(tmpips);

		/*
		 * PART 2
		 * first read in the 4 bytes to an int and add them according to place 1s 10s 100s 1000s
		 * second simply store the tmpnn (temporary number of names) into the machines number of names
		 * clarification - storing the number of names was mostly used for error checking
		*/

		memcpy(input, (source + it), 1); it++;
		tmpnn += (*input * 1000);
		memcpy(input, (source + it), 1); it++;
		tmpnn += (*input * 100);
		memcpy(input, (source + it), 1); it++;
		tmpnn += (*input * 10);
		memcpy(input, (source + it), 1); it++;
		tmpnn += *input;

		mc->numnames = tmpnn;

		/*
		 * PART 3
		 * first initializes tmp[50] for concatenating chars, name for storing name temporarily, dotcheck, and n
		 * begins loop that ends when tmpnn reaches 0
		 * performs the dotcheck if that will take everything befor the '.' (assuming it hasnt done so for the current name) and adds it as a short name
		 * if the input is null that means the name is done reading in and needs to be stored so we do it in that statement as well as reset the iterators
		 * finally in all other cases than '.' and null add the current read char to the tmp[50]
		 * note - when adding a name we have to get rid of anything in tmp so i have a small loop to take care of freeing it
		*/

		char tmp[50];
		char *name;
		dotcheck = 0;
		n = 0;

		while(tmpnn > 0){

		memcpy(input, (source + it), 1); it++;

			if(*input == '.'){

				if(dotcheck == 0){

					name = strdup(tmp);
					jrb_insert_str(mc->names, name, new_jval_v(NULL));
					mc->numnames++;
					dotcheck = 1;
				}
			}

			if(*input == 0){

				tmpnn--;
				name = strdup(tmp);
				jrb_insert_str(mc->names, name, new_jval_v(NULL));
				for(int k = 0; k < 50; k++){

					tmp[k] = '\0';
				}
				n = 0;
				dotcheck = 0;
			}
			else{
				tmp[n] = *input;
				n++;
			}
		}

		/*
		 * PART 4
		 * first make 2 important yet temporary data structures
		 * tmprbt = temporary(tmp) red-black-tree(rbt) for searching machines tree
		 * tmpdll = temporary(tmp) doubly-linked-list(dll) for storing the value of the machines tree as a list of machines rather than just one cont.
		 * cont. - this way if they are stored where multiple machines have the same name, all machines will be printed instead of the first one only (property of searching JRBs)
		 * next traverse the names JRB of the current machine
		 * if name isnt in the machines jrb it will add it to a list for that name and add the list into a JRB of all machines
		 * if name is found then append the dll for that name and continue
		 * this way machines can be grouped into dlls and when searched for under a key (name) it returns all of the machines that have that as a key
		*/

		JRB tmprbt;
		Dllist tmpdll;

		jrb_traverse(temp, mc->names){

			tmpdll = new_dllist();
			tmprbt = jrb_find_str(machines, jval_s(temp->key));
			if(tmprbt == NULL){

				dll_append(tmpdll, new_jval_v(mc));
				jrb_insert_str(machines, jval_s(temp->key), new_jval_v(tmpdll));
			}else{

				tmpdll = tmprbt->val.v;
				dll_append(tmpdll, new_jval_v(mc));
			}
		}

	}//end of big while loop

	/*
	 * IO handling
	 * search = tmprbt from part 4
	 * tmpstr = input from before but with scanf this time
	 * letemp = the same as temp from the begining (just for traversal)
	 * t = like temp for traversal but for dll
	 * tmpdll = same purpose as tmpdll from above
	 * checkend = to loop untill EOF for crtl-D
	*/

	printf("Hosts all read in\n\n");
	JRB search, letemp;
	char *tmpstr;
	Dllist t, tmpdll;
	int checkend = 0;;

	tmpstr = malloc(sizeof(char) * 50);

	/*
	 * IO handling logic
	 * print asking for name
	 * set checkend to the scanf to see if anything read
	 * as long as not EOF lookup tmpstr in machines tree
	 * then check if that key exists
	 * then if it does exist travers the machines dll in the machines JRB and give the info in format
	 * for this we also have to traverse the mc->names JRB to print all the names a machine has
	*/

	while(checkend != EOF){
		printf("Enter host name: ");

		checkend = scanf("%s", tmpstr);

		if(checkend != EOF){

			search = jrb_find_str(machines, tmpstr);
			if(search == NULL){

				printf("no key %s\n", tmpstr);
			}
			else{

				tmpdll = search->val.v;
				dll_traverse(t, tmpdll){

					mc = t->val.v;
					printf("%s: ", mc->ip);
					jrb_traverse(letemp, mc->names){

						printf("%s ", jval_s(letemp->key));
					}
					printf("\n\n");
				}
			}
		}
	}//end of IO loop

	/*
	 * FREEDOM
	 * finally free everything that the interpreter would let me without causing seg faults
	*/

	close(in);
	free(input);
	free(tmpips);
	free(mc);

	return 0;

}//end of main




