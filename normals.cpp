/*
Calculates normals at each voxels by finding
interior and exterior voxels within the bounding box of the object
given as surface voxels.

The normal is calculated by first checking neighbors at opposite faces
If found add all such normals, normalize it, print it in file and continue
to next voxel

Else move on to the opposite edge neighbors, if found following a similar
procedure as above

Else move on to the opposite vertex neighbors, if found following a similar
procedure as above

*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <bitset>
#include <queue>
#define MAXSIZE 398
#define CENTER 1

using namespace std;

//3D Array of voxel centers
//voxel[i][j][k] = "00" : interior
//voxel[i][j][k] = "01" : surface
//voxel[i][j][k] = "10" : exterior
bitset<2> voxels[MAXSIZE][MAXSIZE][MAXSIZE];

//utility class for Vector operations and storage
typedef struct Vector3 {
    float x;
    float y;
    float z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    float magnitude() const{
        return sqrt(x*x+y*y+z*z);
    }
    Vector3& operator+(const Vector3 &a) const{
        Vector3 *temp = new Vector3(this->x+a.x, this->y+a.y, this->z+a.z);
        return *temp;
    }
    Vector3& unitVector() const{
        if(magnitude()==0) {
            Vector3 *temp = new Vector3(*this);
            return *temp;
        }
        Vector3 *temp = new Vector3(x/magnitude(), y/magnitude(), z/magnitude());
        return *temp;
    }
}Vector3;

//utility class for storing linked list of coordinates
typedef struct vlist {
    int x;
    int y;
    int z;
    struct vlist *next;
    vlist() : x(0), y(0), z(0), next(NULL) {}
}vlist;

//utility class for storing a Vector with lesser space consumption
typedef struct shortVector {
    short x;
    short y;
    short z;
    shortVector() : x(0), y(0), z(0) {}
    shortVector(float x, float y, float z) : x(x), y(y), z(z) {}
    float magnitude() const{
        return sqrt(x*x+y*y+z*z);
    }
    shortVector& operator+(const shortVector &a) const{
        shortVector *temp = new shortVector(this->x+a.x, this->y+a.y, this->z+a.z);
        return *temp;
    }
    shortVector& unitVector() const{
        if(magnitude()==0) {
            shortVector *temp = new shortVector(*this);
            return *temp;
        }
        shortVector *temp = new shortVector(x/magnitude(), y/magnitude(), z/magnitude());
        return *temp;
    }
}shortVector;

//visited queue for BFS Traversal
queue<shortVector> q;

//bfs traversal of the exterior voxels (exterior to the closed surface voxels)
void bfs(int x, int y, int z) {
    if( x < 0 || x > (MAXSIZE-1) ||
        y < 0 || y > (MAXSIZE-1) ||
        z < 0 || z > (MAXSIZE-1) ) {
        return;
    }
    //checking whether it is an already visited voxel or surface voxel
    if(voxels[x][y][z][0] == 1 || voxels[x][y][z][1] == 1) {
        return;
    }
    //cout<<x<<" "<<y<<" "<<z<<endl;

    //pushing the first visited voxel into the queue
    q.push(shortVector(x,y,z));
    while(!q.empty()) {
        //cout<<q.size()<<endl;
        shortVector root = q.front();
        q.pop();
        if( root.x < 0 || root.x > (MAXSIZE -1) ||
            root.y < 0 || root.y > (MAXSIZE -1) ||
            root.z < 0 || root.z > (MAXSIZE -1) ) {
            continue;
        }
        //if the voxel is a surface voxel then ignore it
        if(voxels[(int)root.x][(int)root.y][(int)root.z][0] == 1) {
            continue;
        }
//        cout<<root.x<<" "<<root.y<<" "<<root.z<<" "<<( root.x < 0 || root.x > (MAXSIZE -1) ||
//            root.y < 0 || root.y > (MAXSIZE -1) ||
//            root.z < 0 || root.z > (MAXSIZE -1) )<<endl;

        //checking a neighbor of the current voxel
        if(!( (root.x+1) < 0 || (root.x+1) > (MAXSIZE -1) ||
                root.y < 0 || root.y > (MAXSIZE -1) ||
                root.z < 0 || root.z > (MAXSIZE -1) )) {
            //if it is a surface voxel (voxel[x][y][z] == "01") 
            // or if it is an already visited exterior voxel (voxel[x][y][z] == "10")
            //ignore it else mark it as exterior ("10") and push it in queue
            if(!(voxels[(int)root.x+1][(int)root.y][(int)root.z][0] == 1 || voxels[(int)root.x+1][(int)root.y][(int)root.z][1] == 1)) {
                q.push(shortVector(root.x+1,root.y,root.z));
                voxels[(int)root.x+1][(int)root.y][(int)root.z][1] = 1;
            }
        }
        //same comments as before
        if(!( (root.x-1) < 0 || (root.x-1) > (MAXSIZE -1) ||
                root.y < 0 || root.y > (MAXSIZE -1) ||
                root.z < 0 || root.z > (MAXSIZE -1) )) {
            if(!(voxels[(int)root.x-1][(int)root.y][(int)root.z][0] == 1 || voxels[(int)root.x-1][(int)root.y][(int)root.z][1] == 1)) {
                q.push(shortVector(root.x-1,root.y,root.z));

                voxels[(int)root.x-1][(int)root.y][(int)root.z][1] = 1;
            }
        }
        //same comments as before
        if(!( root.x < 0 || root.x > (MAXSIZE -1) ||
                (root.y+1) < 0 || (root.y+1) > (MAXSIZE -1) ||
                root.z < 0 || root.z > (MAXSIZE -1) )) {
            if(!(voxels[(int)root.x][(int)root.y+1][(int)root.z][0] == 1 || voxels[(int)root.x][(int)root.y+1][(int)root.z][1] == 1)) {
                q.push(shortVector(root.x,root.y+1,root.z));
                voxels[(int)root.x][(int)root.y+1][(int)root.z][1] = 1;
            }
        }
        //same comments as before
        if(!( root.x < 0 || root.x > (MAXSIZE -1) ||
                (root.y-1) < 0 || (root.y-1) > (MAXSIZE -1) ||
                root.z < 0 || root.z > (MAXSIZE -1) )) {
            if(!(voxels[(int)root.x][(int)root.y-1][(int)root.z][0] == 1 || voxels[(int)root.x][(int)root.y-1][(int)root.z][1] == 1)) {
                q.push(shortVector(root.x,root.y-1,root.z));
                voxels[(int)root.x][(int)root.y-1][(int)root.z][1] = 1;
            }
        }
        //same comments as before
        if(!( root.x < 0 || root.x > (MAXSIZE -1) ||
                root.y < 0 || root.y > (MAXSIZE -1) ||
                (root.z+1) < 0 || (root.z+1) > (MAXSIZE -1) )) {
            if(!(voxels[(int)root.x][(int)root.y][(int)root.z+1][0] == 1 || voxels[(int)root.x][(int)root.y][(int)root.z+1][1] == 1)) {
                q.push(shortVector(root.x,root.y,root.z+1));
                voxels[(int)root.x][(int)root.y][(int)root.z+1][1] = 1;
            }
        }
        //same comments as before
        if(!( root.x < 0 || root.x > (MAXSIZE -1) ||
                root.y < 0 || root.y > (MAXSIZE -1) ||
                (root.z-1) < 0 || (root.z-1) > (MAXSIZE -1) )) {
            if(!(voxels[(int)root.x][(int)root.y][(int)root.z-1][0] == 1 || voxels[(int)root.x][(int)root.y][(int)root.z-1][1] == 1)) {
                q.push(shortVector(root.x,root.y,root.z-1));
                voxels[(int)root.x][(int)root.y][(int)root.z-1][1] = 1;
            }
        }
        //marking the current voxel as visited that is voxel[x][y][z] = "10" (setting bit[1] to 1)
        voxels[(int)root.x][(int)root.y][(int)root.z][1] = 1;
    }
}

//v_input = "bunny64.txt"
//normals = "normals.txt"
//The driver fuction to produce all the normals at each voxel
int Normals(char* v_input, char* normals) {
    string vfilename;
    vfilename = v_input;
    int maxx=0;
    int maxy=0;
    int maxz=0;
    int minx = 1000;
    int miny = 1000;
    int minz = 1000;
    FILE *fp = fopen(vfilename.c_str(), "r");

    ofstream fnormal(normals);
    for(int i=0;i<=(MAXSIZE-1);i++) {
        for(int j=0;j<=(MAXSIZE-1);j++) {
            for(int k=0;k<=(MAXSIZE-1);k++) {
                voxels[i][j][k] = bitset<2>(string("00"));
            }
        }
    }
    //creating a queue of voxel coordinates in order
    vlist *head = new vlist();
    vlist *vs = head;
    int n=0;
    if(!feof(fp)) {
        fscanf(fp,"%d",&n);
    }
    int k=n;
    while(k--) {
        int x,y,z;
        fscanf(fp,"%d %d %d",&x,&y,&z);
        //if(x > 0 || z > 0) continue;
        vs->x = x;vs->y = y;vs->z = z;
        //finding the max and min coordinates
        if(maxx < x) {
            maxx = x;
        }
        if(maxy < y) {
            maxy = y;
        }
        if(maxz < z) {
            maxz = z;
        }
        if(minx > x) {
            minx = x;
        }
        if(miny > y) {
            miny = y;
        }
        if(minz > z) {
            minz = z;
        }
        if(k!=0) {
            //storing the voxels in order
            vs->next = new vlist();
            vs = vs->next;
        }
        voxels[x+CENTER][y+CENTER][z+CENTER][0] = 1;
    }
    fclose(fp);
cout<<"read"<<endl;

    //do a bfs traversal
    bfs(0,0,0);
    cout<<"travelled"<<endl;

    vs = head;
    float countxy = 0,countyz = 0,countzx = 0;
    cout<<(fnormal==NULL)<<endl;
    fnormal<<maxx<<" "<<maxy<<" "<<maxz<<endl;
    fnormal<<minx<<" "<<miny<<" "<<minz<<endl;

    while(vs != NULL) {
        //normal vector at vs (initialized to zero vector)
        Vector3 normal;

        //k = neighborhood size
        int k=4;
        for(int z=-k; z<=k ; z++) {
            for(int y=-k; y<=k ; y++) {
                for (int x = -k; x <= k; ++x)
                {
                    //in k-neighborhood proceed only for voxels which are siirface voxels ("01")
                    if(voxels[vs->x+x+CENTER][vs->y+y+CENTER][vs->z+z+CENTER] != bitset<2>(string("01"))) {
                        continue;
                    }
                    Vector3 testNormal;
                    //check opposite faces
                    short face_neighbor[6][3] =
                    {
                        {-1,0,0},
                        {1,0,0},
                        {0,-1,0},
                        {0,1,0},
                        {0,0,-1},
                        {0,0,1}
                    };
                    for(int i=0;i<6;i++) {
            //            cout<<"++++"<<i<<"+++++"<<endl;
            //            cout<<voxels[vs->x+x+face_neighbor[i][0]+CENTER][vs->y+y+face_neighbor[i][1]+CENTER][vs->z+z+face_neighbor[i][2]+CENTER]<<endl;
            //            cout<<voxels[vs->x+x-face_neighbor[i][0]+CENTER][vs->y+y-face_neighbor[i][1]+CENTER][vs->z+z-face_neighbor[i][2]+CENTER]<<endl;
                        if(voxels[vs->x+x+face_neighbor[i][0]+CENTER][vs->y+y+face_neighbor[i][1]+CENTER][vs->z+z+face_neighbor[i][2]+CENTER]==bitset<2>(string("10"))&&
                           voxels[vs->x+x-face_neighbor[i][0]+CENTER][vs->y+y-face_neighbor[i][1]+CENTER][vs->z+z-face_neighbor[i][2]+CENTER]==bitset<2>(string("00"))) {
                            testNormal = testNormal+Vector3(face_neighbor[i][0],face_neighbor[i][1],face_neighbor[i][2]);
                        }
                    }
                    //opposite face neighbors one of which is interior and other exterior is found
                    if((testNormal.magnitude())!=0) {
                        normal = normal + testNormal;
                        continue;
                    }

                    //check opposite edges
                    short edge_neighbor[12][3] =
                    {
                        {-1,-1,0},
                        {1,1,0},
                        {0,-1,-1},
                        {0,1,1},
                        {-1,0,-1},
                        {1,0,1},
                        {1,-1,0},
                        {-1,1,0},
                        {0,1,-1},
                        {0,-1,1},
                        {1,0,-1},
                        {-1,0,1}
                    };
                    for(int i=0;i<12;i++) {
            //            cout<<"++++"<<i<<"+++++"<<endl;
            //            cout<<voxels[vs->x+x+face_neighbor[i][0]+CENTER][vs->y+y+face_neighbor[i][1]+CENTER][vs->z+z+face_neighbor[i][2]+CENTER]<<endl;
            //            cout<<voxels[vs->x+x-face_neighbor[i][0]+CENTER][vs->y+y-face_neighbor[i][1]+CENTER][vs->z+z-face_neighbor[i][2]+CENTER]<<endl;
                        if(voxels[vs->x+x+edge_neighbor[i][0]+CENTER][vs->y+y+edge_neighbor[i][1]+CENTER][vs->z+z+edge_neighbor[i][2]+CENTER]==bitset<2>(string("10"))&&
                           voxels[vs->x+x-edge_neighbor[i][0]+CENTER][vs->y+y-edge_neighbor[i][1]+CENTER][vs->z+z-edge_neighbor[i][2]+CENTER]==bitset<2>(string("00"))) {
                            testNormal = testNormal+Vector3(edge_neighbor[i][0],edge_neighbor[i][1],edge_neighbor[i][2]);
                        }
                    }
                    //opposite edge neighbors one of which is interior and other exterior is found
                    if((testNormal.magnitude())!=0) {
                        normal = normal + testNormal;
                        continue;
                    }

                    //check opposite vertex
                    short vertex_neighbor[8][3] =
                    {
                        {-1,-1,-1},
                        {1,1,1},
                        {1,-1,-1},
                        {-1,1,1},
                        {1,1,-1},
                        {-1,-1,1},
                        {-1,1,-1},
                        {1,-1,1}
                    };
                    for(int i=0;i<8;i++) {
            //            cout<<"++++"<<i<<"+++++"<<endl;
            //            cout<<voxels[vs->x+x+face_neighbor[i][0]+CENTER][vs->y+y+face_neighbor[i][1]+CENTER][vs->z+z+face_neighbor[i][2]+CENTER]<<endl;
            //            cout<<voxels[vs->x+x-face_neighbor[i][0]+CENTER][vs->y+y-face_neighbor[i][1]+CENTER][vs->z+z-face_neighbor[i][2]+CENTER]<<endl;
                        if(voxels[vs->x+x+vertex_neighbor[i][0]+CENTER][vs->y+y+vertex_neighbor[i][1]+CENTER][vs->z+z+vertex_neighbor[i][2]+CENTER]==bitset<2>(string("10"))&&
                           voxels[vs->x+x-vertex_neighbor[i][0]+CENTER][vs->y+y-vertex_neighbor[i][1]+CENTER][vs->z+z-vertex_neighbor[i][2]+CENTER]==bitset<2>(string("00"))) {
                            testNormal = testNormal+Vector3(vertex_neighbor[i][0],vertex_neighbor[i][1],vertex_neighbor[i][2]);
                        }
                    }
                    //opposite vertex neighbors one of which is interior and other exterior is found
                    if((testNormal.magnitude())!=0) {
                        normal = normal + testNormal;
                        continue;
                    }
                    //cout<<"here"<<endl;
                }
            }
        }
        if(normal.magnitude()==0)
            normal = Vector3(1,0,0);
        //normalize the vector
        normal = normal.unitVector();

        fnormal<<normal.x<<" "<<normal.y<<" "<<normal.z;
        vs = vs->next;
        if(vs!=NULL)
            fnormal<<endl;
        //
    }
    fnormal.close();
    cout<<"normals calculated"<<endl;

    //calculate components of normal on each orthogonal plane

}