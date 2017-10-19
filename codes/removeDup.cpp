//Removes duplicate voxels from tempout.txt

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <stdio.h>
typedef struct shortVector_r {
    short x;
    short y;
    short z;
    short t;
    shortVector_r(){}
    shortVector_r(int x, int y, int z, int t):x(x), y(y), z(z), t(t){}
}shortVector_r;
using namespace std;
int removeDup(string tempout) {

    FILE *fp = fopen(tempout.c_str(),"r");
    map<string, int> indexMap;
    int n;
    fscanf(fp, "%d", &n);
    int actN=0;
    vector< shortVector_r > finVert;
    int i=n;
    while(i--) {
        int x,y,z,t;
        fscanf(fp,"%d %d %d %d", &x, &y, &z, &t);
        stringstream ss;
        ss<<x<<" "<<y<<" "<<z;
        string key = ss.str();
        bool isnew = indexMap.insert(make_pair<string, int>(key,n-1-i)).second;
        if (isnew) {
            finVert.push_back(shortVector_r(x,y,z,t));
            actN++;
        }
        else {
            // cout<<x<<","<<y<<","<<z<<","<<t<<endl;
        }
    }
    fclose(fp);
    fp = fopen(tempout.c_str(),"w");
    fprintf(fp,"%d\n", actN);
    for(int i=0;i<actN;i++) {
        fprintf(fp, "%d %d %d %d",finVert[i].x, finVert[i].y, finVert[i].z, finVert[i].t);
        if(i!=(actN-1)) fprintf(fp,"\n");
    }
    fclose(fp);
}
