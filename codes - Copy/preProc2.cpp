//driver program for pre-processing the data (finding normals)
#include "normals.cpp"
#include <string.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>

using namespace std;

int main() {

	string v_input;
	cout<<"Enter voxel input file : ";
	cin>>v_input;

    char *v_in;
    v_in = (char*) malloc(100*sizeof(char));
    strcpy(v_in, v_input.c_str());
    char *n_in;
    n_in = (char*) malloc(100*sizeof(char));
    strcpy(n_in, "normals.txt");
	Normals(v_in, n_in);
}
