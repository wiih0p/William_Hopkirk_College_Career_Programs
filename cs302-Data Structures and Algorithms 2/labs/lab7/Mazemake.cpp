#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "dset.h"

struct cell {

	cell() {i = 0; j = 0;}
	cell(int x, int y) { i = x; j = y;}
	int i;
	int j;

};

//void swap(cell &c1, cell &c2) { ... }

int main(int argc, char *argv[]) {

	if (argc != 4) {

		printf ("error");

		return 0;
	}


	int Nrow = atoi(argv[1]);

	int Ncol = atoi(argv[2]);

	// Grid wall:     (i0,j0) | (i1,j1)
	// Horizontal wall: (i,j) | (i+1,j)
	// Vertical wall:   (i,j) | (i,j+1)


	int N = ((Ncol - 1) * Nrow) + ((Nrow - 1) * Ncol);

	cell wall[N][2];

	int k = 0;

	for(int i = 0; i < Nrow; i++){

		for(int j = 0; j < Ncol; j++){

			wall[k][0] = cell(i, j);
			wall[k][1] = cell(i + 1, j);
			k++;
		}
	}

	/*
	for(int i = 0; i < Nrow; i++){

		for(int j = 0; j < Ncol; j++){

			wall[k][0] = cell(i, j);
			wall[k][1] = cell(i, j + 1);
			k++;
		}
	}
*/
	//randperm it
	FILE * tmpf;
	tmpf = fopen(argv[3], "w");

	fprintf(tmpf, "MAZE %d %d \n", Nrow, Ncol);

	for(int i = 0; i < Nrow; i++){
		for(int j = 0; j < Ncol; j++){

			fprintf (tmpf, "%d ", wall[i][j].i);
			fprintf (tmpf, " ");
			fprintf (tmpf, "%d ", wall[i][j].j);
			fprintf (tmpf, " ");
			fprintf (tmpf, "%d ", wall[i + 1][j + 1].i);
			fprintf (tmpf, " ");
			fprintf (tmpf, "%d \n", wall[i + 1][j + 1].j);

		}

	}

	/*
	   populate wall[][] with interior grid walls
	   randomly perturb list order: swap based

	   open output file for writing (argv[3])

	   write MAZE, Nrow, Ncol header 

	   for (k=0; k<N; k++) {
if: pair of cells given by wall[k][] are not
connected (belong to different disjoint sets),
merge them

else: write wall location to file as two pairs 
of cell indices: i0 j0 i1 j1

if: all cells belong to the same set, break
}

write any remaining walls to file

close output file
*/

fclose(tmpf);
}
