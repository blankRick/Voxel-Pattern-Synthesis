//driver program for pre-processing the data (finding limits and changing macros in
//surfaeGraph.cpp and normals.cpp
#include "find_min_max.cpp"
#include <vector>
#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

int main() {

	string v_input;
	cout<<"Enter voxel input file : ";
	cin>>v_input;
	int * limits = find_min_max(v_input);

	cout<<"min : "<<limits[0]<<"\nmax : "<<limits[1]<<endl;

	ifstream infile("normals.cpp");
	vector<string> lines;
	string line;
	while(getline(infile, line)) {
		if(line.find("#define MAXSIZE")!= string::npos) {
			stringstream ss;
			ss<<"#define MAXSIZE "<<limits[1];
			lines.push_back(ss.str());
			continue;
		}
		if(line.find("#define CENTER")!= string::npos) {
			stringstream ss;
			ss<<"#define CENTER "<<limits[0];
			lines.push_back(ss.str());
			continue;
		}
		lines.push_back(line);
	}
	infile.close();

	FILE *fp = fopen("normals.cpp", "w");
	for(vector<string>::iterator it = lines.begin(); it != lines.end() ;) {
		fprintf(fp, "%s", it->c_str());
		it++;
		if(it != lines.end()) {
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
    cout<<"normals.cpp changed"<<endl;

	infile.open("surfaceGraph.cpp");
	lines = vector<string>();
	while(getline(infile, line)) {
		if(line.find("#define MAXSIZE")!= string::npos) {
			stringstream ss;
			ss<<"#define MAXSIZE "<<limits[1];
			lines.push_back(ss.str());
			continue;
		}
		if(line.find("#define CENTER")!= string::npos) {
			stringstream ss;
			ss<<"#define CENTER "<<limits[0];
			lines.push_back(ss.str());
			continue;
		}
		lines.push_back(line);
	}
	infile.close();
	fp = fopen("surfaceGraph.cpp", "w");
	for(vector<string>::iterator it = lines.begin(); it != lines.end() ;) {
		fprintf(fp, "%s", it->c_str());
		it++;
		if(it != lines.end()) {
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
	cout<<"surfaceGraph.cpp changed"<<endl;
}
