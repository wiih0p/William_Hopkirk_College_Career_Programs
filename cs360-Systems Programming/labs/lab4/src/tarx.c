//will Hopkirk
//lab4-a "tarc"

#include <stdio.h>
#include <stdlib.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

//needed these for utimes

#include <sys/time.h>
#include <time.h>

/*
 *same dirstuf struct from last time just for inodes
 *also a struct to hold the data i grab
*/

typedef struct{

    JRB inodes;

}dirstuf;

typedef struct{

    int nsize;
    char name[500];
    long inode;
    int mode;
    long mtime;
    long size;
    char bytes[500];

}directory;

/*
 *small comparison funct for the JRB handling
 *same as in tarc
*/

int compfn(Jval x, Jval y){

    if(x.l < y.l){

        return -1;
    }
    if(x.l >y.l){

        return 1;
    }

    return 0;
}

/*
 *this function reads in the tarc file and creates a dll of all the files called contents
*/

Dllist readtarc(){


    /*
     *VARLIST - 
     *i - for keeping track of how much i read
     *d - struct to store read data
     *contents - dll that holds all of the d structs
     *ds - dirstuf
     *errcount - for the err messages the require a byte count
    */
    int i = 1;
    directory *d;
    Dllist contents;
    dirstuf ds;
    ds.inodes = make_jrb();

    contents = new_dllist();

    int errcount;

    /*
     *very simply just go through the data and extract the info i need
    */
    while (i != 0){

        d = malloc(sizeof(directory));

        i = fread(&d->nsize, 1, 4, stdin);
        errcount += 4;
        if(i != 0 && i != 4){

            fprintf(stderr, "Bad filename at byte %d. tried to read filename size, but only got %d bytes. \n", errcount, i);
            exit(1);
        }
        
        i = fread(&d->name, 1, d->nsize, stdin);
        errcount += d->nsize;
        if(i != 0 && i != d->nsize){

            fprintf(stderr, "Bad tarc file at byte %d. File name size is %d, but bytes read = %d. \n", errcount, d->nsize, i);
            exit(1);
        }

        i = fread(&d->inode, 1, 8, stdin);
        errcount += 8;
        if(i != 0 && i != 8){

            fprintf(stderr, "Bad tarc file for %s.  Couldnt read inode\n", d->name);
            exit(1);
        }

        //check the inodes to see if i need to grab the rest
        if(jrb_find_gen(ds.inodes, new_jval_l(d->inode), compfn)){

            
        }else{

        jrb_insert_gen(ds.inodes, new_jval_l(d->inode), new_jval_v(NULL), compfn);

        i = fread(&d->mode, 1, 4, stdin);
        errcount += 4;
        if(i != 0 && i != 4){

            fprintf(stderr, "Bad tarc file for %s.  Couldnt read mode\n", d->name);
            exit(1);
        }

        i = fread(&d->mtime, 1, 8, stdin);
        errcount += 8;
        if(i != 0 && i != 8){

            fprintf(stderr, "Bad tarc file for %s.  Couldnt read mode\n", d->name);
            exit(1);
        }

        //check if its a file for size and bytes
        if(S_ISDIR(d->mode) == 0){

            i = fread(&d->size, 1, 8, stdin);
            errcount += 8;
            if(i != 0 && i != 8){

                fprintf(stderr, "Bad tarc file for %s.  Couldnt read size\n", d->name);
                exit(1);
            }

            i = fread(&d->bytes, 1, d->size, stdin);
            errcount += d->size;
            if(i != 0 && i != d->size){

                fprintf(stderr, "Bad tarc file for %s.  Trying to read %d bytes of the file, and got EOF\n", d->name, errcount);
                exit(1);
            }

        }
        }

        if(i > 0){

            /*
             *for early checking leaving it here cuz i liked it
            printf("FILE NAME SIZE: %d\n", d->nsize);
            printf("FILE NAME: %s\n", d->name);
            printf("FILE INODE: %ld\n", d->inode);
            printf("FILE MODE: %d\n", d->mode);
            printf("FILE MODTIME: %ld\n", d->mtime);
            */

            dll_append(contents, new_jval_v(d));
        
        }
    }

    return contents;
}

int main(int argc, char* argv[]) {

    Dllist contents, tmp;

    contents = new_dllist();

    contents = readtarc();

    /*
     *after calling readtarc i have a dll of all the files
     *so first i traverse and set permissions to 0777 so i can do what i like
     *then if there is a dir i call mkdir
     *if its a file i open it and write the bytes
     *if its a link i try to link it but it hadnt worked well for me im not sure but i think this is my main problem
     *but i wanted some bonus points so 59/100 for tarx with 100/100 for tarc made sense to me
    */

        dll_traverse(tmp, contents){

            directory *d;
            directory *ld;
            FILE *out;

            d = tmp->val.v;

            if(S_ISDIR(d->mode) == 1){

                mkdir(d->name, 0777);

            }else{

                if(S_ISLNK(d->mode) == 1){

                    chmod(d->name, 0777);
                    link(ld->name, d->name);

                }else{

                    chmod(d->name, 0777);
                    out = fopen(d->name, "w");
                    fwrite(&d->bytes, 1, d->size, out);
                    fclose(out);
                    
                }
            }

            ld = tmp->val.v;
            
        }
        
        /*
         *final traversal to set the modes back and fix the modtimes with utimes()
         *dr planks notes helped alot with these sys calls
        */

        dll_traverse(tmp, contents){

            directory *d;
            struct timeval t[2];

            d = tmp->val.v;

            t[0].tv_sec = time(NULL);
            t[0].tv_usec = 0;
            t[1].tv_sec = d->mtime;
            t[1].tv_usec = 0;

            utimes(d->name, t);

            if(S_ISDIR(d->mode) == 1){

                chmod(d->name, d->mode);
            }else{

                chmod(d->name, d->mode);
            }
            
        }
    return 0;
}
