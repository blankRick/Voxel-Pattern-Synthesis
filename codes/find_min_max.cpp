/*Finds the limits of the coordinates to set the macros in 
normals.cpp and surfaceGraph.cpp*/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string>

using namespace std;

#define max_t3(x,y,z) (x>y)?(x>z)?x:z:(y>z)?y:z
#define min_t3(x,y,z) (x<y)?(x<z)?x:z:(y<z)?y:z

int* find_min_max(string infile) {
	FILE *fp = fopen(infile.c_str(), "r");
	int n, max_t=0, min_t = INT_MAX;
	fscanf(fp, "%d", &n);

	//loop through all the voxel to find min and max
	while(n--) {
		int x,y,z;
		fscanf(fp, "%d %d %d", &x, &y, &z);
		int t = max_t3(x,y,z);
		max_t = (t>max_t)?t:max_t;
		t = min_t3(x,y,z);
		min_t = (t<min_t)?t:min_t;
	}
	fclose(fp);
	fp = fopen("limits.txt", "w");
	int t;
	if(min_t < 0) {
		t = -min_t;
	}
	else {
		t=0;
	}
	t++;

	fprintf(fp, "%d\n", t);
	fprintf(fp, "%d\n", max_t + t + 1);
	fclose(fp);

	//returns limits to caller
	int *limits;
	limits = (int*) malloc(2*sizeof(int));
	limits[0] = t;
	limits[1] = max_t+t+1;
	return limits;
}
