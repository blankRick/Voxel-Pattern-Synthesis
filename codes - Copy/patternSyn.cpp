//driver program to run the algorithm for synthesizing pattern
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "applyPattern.cpp"
#include "removeDup.cpp"
#include "surfaceGraph.cpp"
#include "connectSamples.cpp"
#include "graphicsb.cpp"

using namespace std;

int main() {
	string v_input, p_input;
	cout<<"Enter voxel input file : ";
	cin>>v_input;
	cout<<"Enter pattern input file : ";
	cin>>p_input;
	string normals("normals.txt");
	string tempout("tempout.txt");
	string graphout("graphout.txt");
	string finout("finout.txt");

    cout<<"applying Pattern..."<<endl;
	applyPattern(v_input, p_input, tempout, normals);
	//return 0;
	cout<<"removing Duplicates..."<<endl;
	removeDup(tempout);
	cout<<"creating Surface Graph..."<<endl;
	surfaceGraph(tempout, graphout);
	cout<<"connecting Samples..."<<endl;
	connectSamples(graphout,finout);
	cout<<"generating the obj file..."<<endl;
	gen(finout);
	cout<<"done"<<endl;
}
