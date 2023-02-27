//will Hopkirk
//lab3 "fakemake"

#include <stdio.h>
#include <stdlib.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv){

    /*
     * variables for various uses

     * VARheaders-
     * F or flag = flags
     * O = .o files
     * C or tmpc = .c files
     * L or lib = libraries
     * E = executable files

     * VARLIST-
     * is = inputstruct for reading file info
	 * tmp = temp dllist for traversal
     * xstr = string for holding and constructing whatever x corresponds to in VARheaders
     * xbuf = buf for stat reading for whatever x corresponds to in VARheaders
     * xexists = for checking if file exists again corresponding to VARheaders
     * xtime = highest modtimes for x
     * recomped = bool to check if a file was recompiled
     * excheck = bool to check if another E file was read
	 * Xlines = dllists for storing different data

     * Finally we allocate some memory for our lines and strs
    */

    IS is;
    Dllist tmp;
    char *estr;
	char *flagstr;
    char *ostr;
    char *tmpcstr;
    char *gccstr;
    char *libstr;
    struct stat obuf;
    struct stat cbuf;
    struct stat hbuf;
    struct stat ebuf;
    int oexists;
    int cexists;
    int hexists;
    int eexists;
    long htime = 0;
    long otime = 0;
    int recomped = 0;
    int excheck = 0;

    Dllist Clines;
    Dllist Hlines;
    Dllist Llines;
    Dllist Flines;

    Clines = new_dllist();
    Hlines = new_dllist();
    Llines = new_dllist();
    Flines = new_dllist();

    flagstr = malloc(50);
    ostr = malloc(50);
    gccstr = malloc(500);
    libstr = malloc(100);

    /*
     * check file openning and set IS to either default (fmakefile) or whatever input we got
     * after this we have our description file
    */

    if (argc > 2) { 
        
        fprintf(stderr, "Usage: ./fakemake [ description-file ]\n"); 
        return 1; 
        
    }else if(argc == 1){

        is = new_inputstruct("fmakefile");
    }else{

        is = new_inputstruct(argv[1]);
        
        if (is == NULL) {
        
            perror(argv[1]);
            return 1;
        }
    }

    /* 
     * input loop
     * loops through the description file and grabs info based on key chars
     * inputs the strings into various dllists
     * also check executable count to make sure there is one AND ONLY ONE ._.
     * after this we now have all of our dlls full of all the info from the description file
    */

    while(get_line(is) >= 0){

        if(strcmp(is->fields[0], "C") == 0){

            for (int i = 1; i < is->NF; i++) {
                dll_append(Clines, new_jval_s(strdup(is->fields[i])));
            }
        }

        if(strcmp(is->fields[0], "H") == 0){

            for (int i = 1; i < is->NF; i++) {
                dll_append(Hlines, new_jval_s(strdup(is->fields[i])));
            }
        }

        if(strcmp(is->fields[0], "L") == 0){

            for (int i = 1; i < is->NF; i++) {
                dll_append(Llines, new_jval_s(strdup(is->fields[i])));
            }
        }

        if(strcmp(is->fields[0], "F") == 0){

            for (int i = 1; i < is->NF; i++) {
                dll_append(Flines, new_jval_s(strdup(is->fields[i])));
            }
        }

        if(strcmp(is->fields[0], "E") == 0){

            estr = strdup(is->fields[1]);
            excheck++;

            if(excheck > 1){

                fprintf(stderr, "fmakefile (%d) cannot have more than one E line\n", is->line);
                return 1;
            }
        }
    }

    if(excheck == 0){

        fprintf(stderr, "No executable specified\n");
        return 1;
    }
    
    /*
     * preparing the calls 
     * make strings for flags and libs by catting them together seperated by a space
     * traverse headers and get the highest mod time for later also chaeck if a header is real
    */ 

    dll_traverse(tmp, Flines){

        strcat(flagstr, " ");
        strcat(flagstr, tmp->val.s);
    }
    
    dll_traverse(tmp, Llines){

        strcat(libstr, " ");
        strcat(libstr, tmp->val.s);
    }

    dll_traverse(tmp, Hlines){

        hexists = stat(tmp->val.s, &hbuf);
        if(hexists < 0){

            fprintf(stderr, "%s doesnt exist.\n", tmp->val.s);
            return 1;
        }

        if(hbuf.st_mtime > htime){

            htime = hbuf.st_mtime;
        }
    }

    /*
     * C lines traversal and the .c calls
     * in the traversal loop we start by making .o strings to go with our .c ones
     * then we call stat on .os and .cs so we can check the times
     * quick check to make sure the c file exists
     * if one of three provided conditions is true (from the writeup) then we recompile
     * cat together the ostr for the gcc call later
     * finally we grab the highest modtime for the .o files called otime 
    */

    dll_traverse(tmp, Clines){

        tmpcstr = strdup(tmp->val.s);
        tmpcstr[strlen(tmpcstr) - 1] = 'o';

        oexists = stat(tmpcstr, &obuf);
        cexists = stat(tmp->val.s, &cbuf);
        if(cexists < 0){

            fprintf(stderr, "fmakefile: %s: No such file or directory\n", tmp->val.s);
            return 1;
        }

        if((oexists < 0) || (obuf.st_mtime < cbuf.st_mtime) || (obuf.st_mtime < htime)){

            sprintf(gccstr, "gcc -c%s %s", flagstr, tmp->val.s);
            printf("%s\n", gccstr);
            if(system(gccstr) != 0){

                fprintf(stderr, "Command failed.  Exiting\n");
                return 1;
            }
            
            recomped = 1;
        }
        
        strcat(ostr, " ");
        strcat(ostr, tmpcstr);

        if(obuf.st_mtime > otime){

            otime = obuf.st_mtime;
        }
    
    }

    /*
     * final call
     * we make a stat call on the executable
     * then we check 3 provided conditions to determine if we need to recompile
     * finally we either make the recompiliation call or state that it is up to date
    */

    eexists = stat(estr, &ebuf);

    if((eexists < 0) || (ebuf.st_mtime < otime) || (recomped == 1)){

        sprintf(gccstr, "gcc -o %s%s%s%s", estr, flagstr, ostr, libstr);
        printf("%s\n", gccstr);
        if(system(gccstr) != 0){

            fprintf(stderr, "Command failed.  Fakemake exiting\n");
            return 1;
        }

        recomped = 1;
    }
    else{

        printf("%s up to date\n", estr);
    }

    return 0;
}
