/*
Generates the Surface Graph as a voxel and the neighboring voxels connected to it 
which are of the same type (solid or empty).
The initial component number of each voxel is set to 1 if solid else 2.
Each voxel is given an id , basically the index number in which it occurs in the 
input file.
for each voxel v in input file "tempout.txt", 
    if it is empty then just it's neighbors in
        26-neighborhood which are also empty are printed in output file "graphout.txt"
    else a bfs is done, for only the voxels that are connected to voxel v, (connected by a path
        of only solid voxels), and their component number is set to the global variable
        "compNum".
        after the bfs compNum is incremented
        the neighbors of the voxel v are printed in output file

*/

#include <iostream>
#include <map>
#include <deque>
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 398
#define CENTER 1

using namespace std;

//3D array to store the voxels
int voxels[MAXSIZE][MAXSIZE][MAXSIZE];
//global variable of number of components
int compNum = 3;
//indexMap having index of each voxel
map<string, int> indexMap;


typedef struct shortVector {
    short x;
    short y;
    short z;
    shortVector() : x(0), y(0), z(0) {}
    shortVector(float x, float y, float z) : x(x), y(y), z(z) {}
}shortVector;

queue<shortVector> q;
//bfs on only the voxels that are connected to (x,y,z) voxel by a path of
//solid voxels only
void bfs(int x, int y, int z) {
    if( (x+CENTER) < 0 || (x+CENTER) > (MAXSIZE-1) ||
        (y+CENTER) < 0 || (y+CENTER) > (MAXSIZE-1) ||
        (z+CENTER) < 0 || (z+CENTER) > (MAXSIZE-1) ) {
        return;
    }
    //cout<<x<<" "<<y<<" "<<z<<endl;
    q.push(shortVector(x,y,z));
    voxels[x+CENTER][y+CENTER][z+CENTER] = compNum;
    while(!q.empty()) {
        //cout<<q.size()<<endl;
        shortVector root = q.front();
        q.pop();

        for(int i=-1;i<2;i++) {
            for(int j=-1;j<2;j++) {
                for(int k=-1;k<2;k++) {

                    if((root.x+i+CENTER) < 0 || (root.x+i+CENTER) > (MAXSIZE-1) ||
                        (root.y+j+CENTER) < 0 || (root.y+j+CENTER) > (MAXSIZE-1) ||
                        (root.z+k+CENTER) < 0 || (root.z+k+CENTER) > (MAXSIZE-1)) {

                        continue;
                    }
                    if(voxels[root.x+i+CENTER][root.y+j+CENTER][root.z+k+CENTER] == 1) {
                        voxels[root.x+i+CENTER][root.y+j+CENTER][root.z+k+CENTER] = compNum;
                        q.push(shortVector(root.x+i,root.y+j,root.z+k));
                    }
                }
            }
        }
    }
    compNum++;
}
//tempout = "tempout.txt"
//graphout = "graphout.txt"
int surfaceGraph(string tempout, string graphout) {
    FILE *fp = fopen(tempout.c_str(), "r");

    int n ;
    fscanf(fp, "%d", &n);

    int b=n;
    //sets indices for each voxel
    while(b--) {
        int x,y,z,t;
        fscanf(fp, "%d %d %d %d", &x,&y,&z, &t);
        voxels[x+CENTER][y+CENTER][z+CENTER] = t;
        stringstream keystream;
        keystream<<(x)<<" "<<(y)<<" "<<(z);
        string key = keystream.str();
        indexMap.insert(make_pair<string,int>(key,n-1-b));
    }
    fclose(fp);


    fp = fopen(tempout.c_str(), "r");
    FILE *ofp = fopen(graphout.c_str(), "w");

    fscanf(fp, "%d", &n);
    fprintf(ofp, "%d\n", n);

    b=n;
    //assign component number to each voxel
    while(b--) {
        int x,y,z,t;
        fscanf(fp, "%d %d %d %d", &x,&y,&z, &t);
        if(voxels[x+CENTER][y+CENTER][z+CENTER] == 1) {
            bfs(x,y,z);
        }
        short neighborNum = 0;
        int neighbors[26];
        for(int i=-1;i<2;i++) {
            for(int j=-1;j<2;j++) {
                for(int k=-1;k<2;k++) {

                    if((x+i+CENTER) < 0 || (x+i+CENTER) > (MAXSIZE-1) ||
                        (y+j+CENTER) < 0 || (y+j+CENTER) > (MAXSIZE-1) ||
                        (z+k+CENTER) < 0 || (z+k+CENTER) > (MAXSIZE-1)) {
                            //cout<<"here"<<endl;
                            if(x==-36&&y==-1&&z==-17) return 0;
                        continue;
                    }
                    //if it is the voxel itself then continue
                    if(i==0 && j==0 && k==0) continue;
                    //if it is a neighboring voxel regardless of solid or empty
                    //it(index of the voxel) is added as a neighbor of v
                    if(voxels[x+i+CENTER][y+j+CENTER][z+k+CENTER] != 0) {
                        stringstream keystream;
                        keystream<<(x+i)<<" "<<(y+j)<<" "<<(z+k);
                        string key = keystream.str();
                        neighbors[neighborNum++] = (indexMap[key]);
                    }
                }
            }
        }
        stringstream keystream;
        keystream<<(x)<<" "<<(y)<<" "<<(z);
        string key = keystream.str();
        if(indexMap[key]!=(n-1-b)) {cout<<"break "<<x<<" "<<y<<" "<<z<<endl;exit(0);}
        //fprint the voxel, component number, number of neighbors, and indices of neighbors
        fprintf(ofp, "%d %d %d %d %d\n", x,y,z, voxels[x+CENTER][y+CENTER][z+CENTER], neighborNum);
        for(int i=0;i<neighborNum ; i++) {
            fprintf(ofp, "%d\n", neighbors[i]);
        }
    }
    //fprint number of components of solid voxels +3
    fprintf(ofp, "%d", compNum);
    fclose(fp);
    fclose(ofp);
}