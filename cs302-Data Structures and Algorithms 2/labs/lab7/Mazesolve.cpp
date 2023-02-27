#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

struct cell {

	cell() {i = 0; j = 0;}
	cell(int x, int y) { i = x; j = y;}
	int i;
	int j;

};

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf ("error");
			return 0;
	} 

	FILE * in;
	FILE * out;

	in = fopen("maze.txt", "r");
	out = fopen(argv[2], "w");
	

	int Nrow;
	int Ncol;

	fscanf(in, "MAZE %d %d", &Nrow, &Ncol);

	printf ("%d %d", Nrow, Ncol);
	//determine Nrow,Ncol from input file

		//create array of walls for each grid cell
		//initialize to have boundary walls set and
		//interior walls unset

	int N = ((Ncol - 1) * Nrow) + ((Nrow - 1) * Ncol);
	cell wall[N][2];

	//read input file, set interior walls

		
		fprintf(out, "PATH %d %d \n", Nrow, Ncol);

    int k = 0;
		for(int i = 0; i < Nrow; i++){
		for(int j = 0; j < Ncol; j++){

			wall[k][0] = cell(i, j);
			wall[k][1] = cell(i + 1, j);
			k++;
		}
	}
 
 for(int i = 0; i < Nrow; i++){
		for(int j = 0; j < Ncol; j++){

			fprintf (out, "%d ", wall[i][j].i);
			fprintf (out, " ");
			fprintf (out, "%d \n", wall[i][j].j);
		}
 }
		//write cells on path to file

		
		fclose(in);
		fclose(out);
}
