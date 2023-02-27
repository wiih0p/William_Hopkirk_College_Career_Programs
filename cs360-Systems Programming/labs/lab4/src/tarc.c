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

/*
 *just directory stuff
 *just holds the inodes JRB to check stuff
*/
typedef struct{

    JRB inodes;
    JRB dirnames;

}dirstuf;

/*
 *just a small function to chop the name up to just have the relative pathname
 *because we need to have both relative and full pathnames
 *comparison function for JRB gen
 *because jvals support longs but JRBs dont
*/
char *chopname(char *ogname){

    char *cringe;

    cringe = strdup(ogname);
    while(strchr(cringe, '/') != NULL){

        cringe = strchr(cringe, '/') + 1;
    }

    return cringe;
}

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
 *the recursive function that grabs all of the directories from the given dir
 *args are the absolute pathname (dirname), the inode holder (ds), and the relative path name i made earlier (cringe)
*/

void graballdirs(char *dirname, dirstuf ds, char *cringe){

    /*
     *VARLIST
     *buf - for stat struct
     *exists - also for stat
     *d - the directory to be opened
     *de - for the dirent lib
     *yeet - for building the full pathname
     *in - for file reading
     *bytes - for holding the bytes
     *nsize - holds the size of the filename
     *printname - the relative pathname is stored here
     *fullpath & relpath - self explanatory
     *tmp - for traversal
     *stupid - handles the 10 descriptor limit
    */

    struct stat buf;
    int exists;
    DIR *d;
    struct dirent *de;
    char yeet[500];
    char printname[500];
    char *fullpath;
    char *relpath;
    JRB tmp, stupid;

    stupid = make_jrb();

   d = opendir(dirname);
    if (d == NULL) {
        fprintf(stderr, "Couldn't open \"%s\"\n", dirname);
        exit(1);
  }

  for (de = readdir(d); de != NULL; de = readdir(d)) {
        sprintf(yeet, "%s/%s", dirname, de->d_name);
        exists = lstat(yeet, &buf);

    if (exists < 0) {
        fprintf(stderr, "%s not found\n", de->d_name);
        exit(1);
    }
    if(S_ISDIR(buf.st_mode) == 1 && S_ISLNK(buf.st_mode) == 0 && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0){

        fullpath = strdup(yeet);
        sprintf(printname, "%s/%s", cringe, de->d_name);
        relpath = strdup(printname);
        jrb_insert_str(ds.dirnames, fullpath, new_jval_s(relpath));
        jrb_insert_str(stupid, fullpath, new_jval_s(relpath));

    }

   }

    //close the dir
    closedir(d);
    
    //traverse the jrb thats a copy of the main jrb to get all the dirnames
    jrb_traverse(tmp, stupid){

        graballdirs(tmp->key.s, ds, tmp->val.s);
    }
        

}

void readfiles(char *dirname, dirstuf ds, char *cringe){

    /*
     *VARLIST
     *buf - for stat struct
     *exists - also for stat
     *d - the directory to be opened
     *de - for the dirent lib
     *yeet - for building the full pathname
     *in - for file reading
     *bytes - for holding the bytes
     *nsize - holds the size of the filename
     *printname - the relative pathname is stored here
    */

    struct stat buf;
    int exists;
    DIR *d;
    struct dirent *de;
    char yeet[500];
    FILE *in;
    char *bytes;
    int nsize;
    char printname[500];

    /*
     *open and check the directory
     *start loop to read the directories files
     *use dirname and de->d_name to cat the pathname
     *call lstat and check
    */

    d = opendir(dirname);
    
    if (d == NULL) {
        fprintf(stderr, "Couldn't open \"%s\"\n", dirname);
        exit(1);
  }

  for (de = readdir(d); de != NULL; de = readdir(d)) {
        sprintf(yeet, "%s/%s", dirname, de->d_name);
        exists = lstat(yeet, &buf);

    if (exists < 0) {
        fprintf(stderr, "%s not found\n", de->d_name);
        exit(1);
    } else {

        /*
         *malloc the bytes
         *check if its a file
         *check if the inode is in the JRB
         *print file info
        */

        bytes = malloc(buf.st_size);

        if(S_ISDIR(buf.st_mode) == 0 && S_ISLNK(buf.st_mode) == 0 && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0){


            sprintf(printname, "%s/%s", cringe, de->d_name);
            nsize = strlen(printname);
            fwrite(&nsize, 1, 4, stdout);
            fwrite(printname, 1, nsize, stdout);
            fwrite(&buf.st_ino, 1, 8, stdout);

            if(jrb_find_gen(ds.inodes, new_jval_l(buf.st_ino), compfn)){

            }
            else{

            jrb_insert_gen(ds.inodes, new_jval_l(buf.st_ino), new_jval_v(NULL), compfn);

            fwrite(&buf.st_mode, 1, 4, stdout);
            fwrite(&buf.st_mtime, 1, 8, stdout);
            
            in = fopen(yeet, "r");
            fread(bytes, 1, buf.st_size, in);
            fclose(in);
            fwrite(&buf.st_size, 1, 8, stdout);
            fwrite(bytes, 1, buf.st_size, stdout);
            free(bytes);
            }
        }

        
   }
  }

    //close the dir
    closedir(d);    


}

int main(int argc, char **argv){

    /*
     * variables for various uses

     * VARLIST- same as earlier vars

     * 
    */

    struct stat buf;
    int exists;
    dirstuf ds;
    char *cringe;
    JRB tmp;
    int nsize;
    char *relpath;

    /*
     *ERROR check file given
     *then make relative name
     *make JRB
     *call lstat on given pathname & error check path name
    */

    if (argc != 2) { 

        fprintf(stderr, "Usage: ./tarc [ description-file ]\n"); 
        exit(1); 
        
    }

    cringe = chopname(argv[1]);

    ds.inodes = make_jrb();
    ds.dirnames = make_jrb();

    /*
     *algorithm to print main dir
     *prints dir info and then call recursive function
    */

        relpath = strdup(cringe);
        jrb_insert_str(ds.dirnames, argv[1], new_jval_s(relpath));
            
        graballdirs(argv[1], ds, cringe);  


    jrb_traverse(tmp, ds.dirnames){

        
        /*
         *check if the dir is in the JRB
         *print out the info
         *call the file reader to go through the directory in question
        */

        exists = lstat(tmp->key.s, &buf);

        if (exists < 0) {
            fprintf(stderr, "%s not found\n", tmp->val.s);
            return -1;
        } else {

            nsize = strlen(tmp->val.s);
            fwrite(&nsize, 1, sizeof(nsize), stdout);
            fwrite(tmp->val.s, 1, nsize, stdout);
            fwrite(&buf.st_ino, 1, 8, stdout);
            
        if(jrb_find_gen(ds.inodes, new_jval_l(buf.st_ino), compfn)){
            
        }else{

            jrb_insert_gen(ds.inodes, new_jval_l(buf.st_ino), new_jval_v(NULL), compfn);

            fwrite(&buf.st_mode, 1, 4, stdout);
            fwrite(&buf.st_mtime, 1, 8, stdout);
            
            }

            

     readfiles(tmp->key.s, ds, tmp->val.s);

    }
    }
}