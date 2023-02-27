//will hopkirk
//lab1 - famtree

#include <stdio.h>
#include <stdlib.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"
#include <string.h>

/*
 * Person struct
 * Varlist
 * name = persons name
 * sex = persons sex
 * father = persons father
 * mother  = persons mother
 * kids = list of kid persons
 * parents = number of parents known for a person 0, 1, 2
 * 
 * BOOLS
 * visited = if we have visited the person in cycle detection
 * printed = if person has been printed
 * havekids = if person has kids
 * sexset = if sex has been set for person
 * fatherset = if person father has been set
 * motherset = if person mother has been set
 * kidstart = to tell begining of kid list
 * 
*/

typedef struct{

	char *name;
	char *sex;
	char *father;
	char *mother;
	Dllist kids;
	int visited;
	int printed;
	int havekids;
	int sexset;
	int fatherset;
	int motherset;
  	int kidstart;
	int parents;

}person;


/*
 * Make People
 * mallocs space for all person vars
 * builds persons name
 * if person is not in list yet add them 
 * return person
 */
person *makePeople(person *p, IS is, JRB people, int nsize){ 

	p = malloc(sizeof(person));

	nsize = strlen(is->fields[1]);
	for(int i = 1; i < is->NF-1; i++){

		nsize += (strlen(is->fields[i+1]) + 1);
	}
	p->name = (char *) malloc(sizeof(char)*(nsize+1));
	p->father = (char *) malloc(sizeof(char)*(nsize+1));
	p->mother = (char *) malloc(sizeof(char)*(nsize+1));
	strcpy(p->name, is->fields[1]);

	nsize = strlen(is->fields[1]);
	for(int i = 1; i < is->NF-1; i++){

		p->name[nsize] = ' ';
		strcpy(p->name + nsize + 1, is->fields[i+1]);
		nsize += strlen(p->name + nsize);
	}

	if(jrb_find_str(people, p->name) == NULL){

		jrb_insert_str(people, p->name, new_jval_v((void *)p));
   
	p->visited = 0;
	p->printed = 0;
	p->havekids = 0;
	p->sexset = 0;
	p->fatherset = 0;
	p->motherset = 0;
    p->kidstart = 0;
	p->parents = 0;
 
	}else{

		p = jrb_find_str(people, p->name)->val.v;
	}

    return p;
}

/*
 * checks for cycles
 * litterally got from writeup
*/
int is_descendant(person *p){

	Dllist temp;

       if (p->visited == 1) return 0;  // I.e. we've processed this 
                                               //person before and he/she's ok 
       if (p->visited == 2) return 1;  // I.e. the graph is messed up 
       p->visited = 2;

	if(p->havekids != 0){
       dll_traverse(temp, p->kids){

         if (is_descendant(temp->val.v)) return 1;
       }
	}
       p->visited = 1;
       return 0;
}

/*
 * print graph
 * traverses the list of people and puts those with no parents on dllist
 * traverse dllist and print out people
 * if they have kids go through kid list and add the kids to the dllist
 * eventually everyone is printed in order
*/
void printGraph(person *p, JRB people) {

    JRB tmp;
	Dllist temp, temp1, temp2, toprint;
	person *tp;
	person *kid;

	toprint = new_dllist();
    
    jrb_traverse(tmp, people) {

		p = (person *) tmp->val.v;
		
		if(p->parents == 0){
			dll_append(toprint, new_jval_v(tmp->val.v));
		}
		
	}

	dll_traverse(temp2, toprint){

		p = (person *) temp2->val.v;
		if(p->printed == 0){

			    printf("%s\n", p->name);
				p->printed = 1;
	
			if(p->sexset != 0){

				if(strcmp(p->sex, "M") == 0){

					printf(" Sex: Male\n");
				}else{

					printf(" Sex: Female\n");
				}
			}else{

				printf(" Sex: Unknown\n");
			}
			if(p->fatherset != 0){

				printf(" Father: %s\n", p->father);
			}else{

				printf(" Father: Unknown\n");
			}
			if(p->motherset != 0){

				printf(" Mother: %s\n", p->mother);
			}else{

				printf(" Mother: Unknown\n");
			}
			if(p->havekids == 1){

				printf(" Children: \n");
				dll_traverse(temp, p->kids){
			
					kid = (person *) temp->val.v;
					printf("  %s\n", kid->name);
				}
			}else{

				printf(" Children: None\n");
			}
	
			printf("\n");
		}

		if(p->havekids != 0){
		dll_traverse(temp1, p->kids){

			tp = (person *) temp1->val.v;
			//printf("checking kid: %s\n", tp->name);
			tp->parents--;
			if(tp->parents == 0 && tp->printed == 0){

				//printf("adding %s to printlist\n", tp->name);
				dll_append(toprint, new_jval_v(temp1->val.v));
			}
			
		}
		}		
	}
}

/*
 * main code
 * if PERSON make the person
 * if SEX set the sex and check for errors
 * if FATHER_OF make person for the child set sex and add to child list same for MOTHER_OF
 * if FATHER or MOTHER make person for the parent set sex of the parent and add this person to child list of the parent
 * then go through and check for cycles and call the print
*/
int main() { 

	IS is;
	JRB people, tmp;
	people = make_jrb();
	int nsize;
	char *tname;

	person *p;
	person *tp;

	is = new_inputstruct(NULL);

	while(get_line(is) >= 0){

		if(is->NF != 0){

			if(strcmp(is->fields[0], "PERSON") == 0){
				
				tp = makePeople(p, is, people, nsize);
				tname = tp->name;

			}
			if(strcmp(is->fields[0], "SEX") == 0){

				p = jrb_find_str(people, tname)->val.v;

				if(p->sexset == 0){

					p->sex = (char *) malloc(1);

					strcpy(p->sex, is->fields[1]);

					p->sexset = 1;
				}else{

					if(strcmp(p->sex, "M") == 0){

						if(strcmp(is->fields[1], "F") == 0){

							fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
							return 1;
						}
					}
					else if(strcmp(p->sex, "F") == 0){

						if(strcmp(is->fields[1], "M") == 0){

							fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
							return 1;
						}
						
					}
				}

			}

			if(strcmp(is->fields[0], "FATHER_OF") == 0){

				tp = makePeople(p, is, people, nsize);

				char *sex = "M";

				p = jrb_find_str(people, tname)->val.v;

        		if(p->sexset == 0){

					p->sex = (char *) malloc(1);

					strcpy(p->sex, sex);

					p->sexset = 1;
				}else{

					if(strcmp(p->sex, "F") == 0){

						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						return 1;
					}
					
				}
        
				p->havekids = 1;

				if(p->kidstart == 0){

					p->kids = new_dllist();
				}

				dll_append(p->kids, new_jval_v((void *) tp));

				if(tp->fatherset == 0){

					strcpy(tp->father, tname);
					tp->fatherset = 1;
					tp->parents++;
				}else{

					fprintf(stderr, "Bad input -- child with two fathers on line %d\n", is->line);
					return -1;
				}

				p->kidstart++;
			}
			
			if(strcmp(is->fields[0], "MOTHER_OF") == 0){

				tp = makePeople(p, is, people, nsize);

				char *sex = "F";

				p = jrb_find_str(people, tname)->val.v;

        		if(p->sexset == 0){

					p->sex = (char *) malloc(1);

					strcpy(p->sex, sex);

					p->sexset = 1;
				}else{

					if(strcmp(p->sex, "M") == 0){

						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						return 1;
					}
				}

				p->havekids = 1;

				if(p->kidstart == 0){

					p->kids = new_dllist();
				}

				dll_append(p->kids, new_jval_v((void *) tp));

				if(tp->motherset == 0){

					strcpy(tp->mother, tname);
					tp->motherset = 1;
					tp->parents++;
				}else{

					fprintf(stderr, "Bad input -- child with two mothers on line %d\n", is->line);
					return -1;
				}

				p->kidstart++;
			}
			
			
			if(strcmp(is->fields[0], "FATHER") == 0){

				tp = makePeople(p, is, people, nsize);

				char *sex = "M";

				p = jrb_find_str(people, tname)->val.v;

				if(tp->sexset == 0){

					tp->sex = (char *) malloc(1);

					strcpy(tp->sex, sex);

					tp->sexset = 1;
				}else{

					if(strcmp(tp->sex, "F") == 0){

						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						return 1;
					}
				}
        
				tp->havekids = 1;

				if(tp->kidstart == 0){

					tp->kids = new_dllist();
				}

				dll_append(tp->kids, new_jval_v((void *) p));

				if(p->fatherset == 0){

					strcpy(p->father, tp->name);
					p->fatherset = 1;
					p->parents++;
				}else{

					fprintf(stderr, "Bad input -- child with two fathers on line %d\n", is->line);
					return -1;
				}

				tp->kidstart++;

			}
      
      	if(strcmp(is->fields[0], "MOTHER") == 0){

				tp = makePeople(p, is, people, nsize);

				char *sex = "F";

				p = jrb_find_str(people, tname)->val.v;

        		if(tp->sexset == 0){

					tp->sex = (char *) malloc(1);

					strcpy(tp->sex, sex);

					tp->sexset = 1;
				}else{

					if(strcmp(tp->sex, "M") == 0){

						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						return 1;
					}

				}
        
				tp->havekids = 1;

				if(tp->kidstart == 0){

					tp->kids = new_dllist();
				}

				dll_append(tp->kids, new_jval_v((void *) p));

				if(p->motherset == 0){

					strcpy(p->mother, tp->name);
					p->motherset = 1;
					p->parents++;
				}else{

					fprintf(stderr, "Bad input -- child with two mothers on line %d\n", is->line);
					return -1;
				}

				tp->kidstart++;

			}
		}
	}


	jrb_traverse(tmp, people){

		if(is_descendant(tmp->val.v)){

			fprintf(stderr, "Bad input -- cycle in specification\n");
			exit(0);
		}
	}


	printGraph(p, people);
	jettison_inputstruct(is);
	return 0;

}

