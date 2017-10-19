/*
applies pattern by raycasting the normal to intersect the faces of the
bounding box of the given voxel object.
Based on the repeated pattern applied on all the faces of the bounding box
with variable transformations, the intersection point closest to the voxel
is rounded to integer coordinates and the pattern value is checked
If it is marked solid then in the output file "tempout.txt" 1 is appended to
its coordinates else 2 is appended
*/
#include <bits/stdc++.h>

using namespace std;

//utility struct for Vector operation and storage
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
    Vector3& operator-(const Vector3 &a) const{
        Vector3 *temp = new Vector3(this->x-a.x, this->y-a.y, this->z-a.z);
        return *temp;
    }
    Vector3& operator/(const float &magnitude) const{
        Vector3 *temp = new Vector3(x/magnitude, y/magnitude, z/magnitude);
        return *temp;
    }
    Vector3& operator*(const float &magnitude) const{
        Vector3 *temp = new Vector3(x*magnitude, y*magnitude, z*magnitude);
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
    float dot(const Vector3 &a) const{
        return this->x*a.x+this->y*a.y+this->z*a.z;
    }
    Vector3& cross(const Vector3 &b) const{
        Vector3 *temp = new Vector3(this->y*b.z - this->z*b.y,
                       this->z*b.x - this->x*b.z,
                       this->x*b.y - this->y*b.x);
        return *temp;
    }
    bool operator==(const Vector3 &a) const{
        return this->x==a.x && this->y==a.y && this->z==a.z;
    }
    void print() const{
        cout<<x<<","<<y<<","<<z;
    }
}Vector3;

//storage for pattern pixels
short pattern[20][20];
//height of pattern
int height;
//width of pattern
int width;

//returns the point of intersection , which is closest to voxel v, among
//all the point of intersections of the raycasted normal from voxel v,
//with all the planes of the bounding box, if the ray intersects
Vector3 getPlanePoint(Vector3 vox_norm, Vector3 vox_pos, Vector3 maxpos, Vector3 minpos) {

    Vector3 pointz(-1,-1,-1);
    if(vox_norm.z<0) {
        float t = -(vox_pos.z)/vox_norm.z;
        pointz = Vector3(vox_pos.x+vox_norm.x*t, vox_pos.y+vox_norm.y*t, 0);
    }
    else if(vox_norm.z>0) {
        float t = (maxpos.z - vox_pos.z)/vox_norm.z;
        pointz = Vector3(vox_pos.x+vox_norm.x*t, vox_pos.y+vox_norm.y*t, -1);
    }
    Vector3 pointx(-1,-1,-1);
    if(vox_norm.x<0) {
        float t = -(vox_pos.x)/vox_norm.x;
        pointx = Vector3(0, vox_pos.y+vox_norm.y*t, vox_pos.z+vox_norm.z*t);
    }
    else if(vox_norm.x>0) {
        float t = (maxpos.x-vox_pos.x)/vox_norm.x;
        pointx = Vector3(-1, vox_pos.y+vox_norm.y*t, vox_pos.z+vox_norm.z*t);
    }
    Vector3 pointy(-1,-1,-1);
    if(vox_norm.y<0) {
        float t = -(vox_pos.y)/vox_norm.y;
        pointy = Vector3(vox_pos.x+vox_norm.x*t, 0, vox_pos.z+vox_norm.z*t);
    }
    else if(vox_norm.y>0) {
        float t = (maxpos.y-vox_pos.y)/vox_norm.y;
        pointy = Vector3(vox_pos.x+vox_norm.x*t, -1, vox_pos.z+vox_norm.z*t);
    }
    if(pointz.x <= (maxpos.x) && pointz.x >= 0 && pointz.y <= (maxpos.y) && pointz.y>= 0) {
        return pointz;
    }
    if(pointx.y <= (maxpos.y) && pointx.y >= 0 && pointx.z <= (maxpos.z) && pointx.z>= 0) {
        return pointx;
    }
    if(pointy.x <= (maxpos.x) && pointy.x >= 0 && pointy.z <= (maxpos.z) && pointy.z>= 0) {
        return pointy;
    }
    return Vector3();
}

Vector3 getSpherePoint(Vector3 vox_norm, Vector3 vox_pos, Vector3 center, double radius) {
    Vector3 d = vox_pos - center;
    double b = (d.x*vox_norm.x+d.y*vox_norm.y+d.z*vox_norm.z);
    double c = d.x*d.x + d.y*d.y + d.z*d.z - radius*radius;
    if((b*b-c) < 0) {
        cout<<"vox_pos : ";vox_pos.print();cout<<endl;
        cout<<"vox_norm : ";vox_norm.print();cout<<endl;
        cout<<"vox_center : ";center.print();cout<<endl;
        cout<<"radius : "<<radius<<endl;
        exit(0);
    }
    double t = -b + sqrt(b*b - c);
    if(t <= 0) {
        cout<<"f off"<<endl;
        exit(0);
    }
    Vector3 P = d + vox_norm*t;
    Vector3 Pzx = Vector3(P.x, 0, P.z);

    double val = P.magnitude();
    if(val < (radius-0.001) || val > (radius+0.001) ) {
        cout<<val<<" "<<radius<<" "<<(val < (radius-0.001))<<(val > (radius+0.001))<<endl;
        exit(0);
    }
    double dot = Pzx.dot(Vector3(0,0,1));
    double det = Vector3(0,1,0).dot(Pzx.cross(Vector3(0,0,1)));
    double xDisp = (atan2(det, dot)) * radius;
    Vector3 Pzy = Vector3(0,0,1) * Pzx.magnitude() * ((dot > 0) ? 1 : -1) + Vector3(0,P.y,0);
    dot = Pzy.dot(Vector3(0,0,1));
    det = Vector3(1,0,0).dot(Pzy.cross(Vector3(0,0,1)));
    double yDisp = (atan2(det, dot)) * radius;
    yDisp = (yDisp < 0) ? acos(-1) - yDisp : yDisp;
    xDisp = (xDisp < 0) ? acos(-1) - xDisp : xDisp;
    return Vector3(0, yDisp, xDisp);
}

//v_input.c_str() = "bunny64.txt"
//p_input.c_str() = "normals.txt"
//tempout.c_str() = "tempout.txt"
//normals.c_str() = "pattern.txt"
int applyPattern(string v_input, string p_input, string tempout, string normals) {
    FILE *vfp = fopen(v_input.c_str(),"r");
    FILE *fp = fopen(normals.c_str(), "r");
    FILE *ofp = fopen(tempout.c_str(), "w");

    //stores the bounds of the object given
    int n,maxx, maxy,maxz, minx, miny, minz;
    Vector3 minpos,maxpos;

    //sphere center
    Vector3 center;
    //sphere radius
    double radius;

    if(!feof(vfp)) {
        fscanf(vfp,"%d",&n);
        fscanf(fp,"%d %d %d",&maxx, &maxy, &maxz);
        fscanf(fp,"%d %d %d",&minx, &miny, &minz);
        maxpos = Vector3(maxx, maxy, maxz);
        minpos = Vector3(minx, miny, minz);
        maxpos = maxpos - minpos;

        //possibility of integer approximations
        radius = maxpos.magnitude()/2;
        center = maxpos / 2;
    }
    int i=n;

    //loading the pattern file
    FILE *pfp = fopen(p_input.c_str(),"r");
    if(!feof(pfp)) {
        fscanf(pfp,"%d",&height);
        fscanf(pfp,"%d",&width);
        for(int t=0;t<height;t++) {
            for(int g=0;g<width;g++) {
                if(!feof(fp))
                    fscanf(pfp,"%d",&pattern[t][g]);
            }
        }
     }
     fclose(pfp);
//
//    memset(voxels, 0, sizeof(voxels));
//    memset(visited, 0, sizeof(visited));
    int x, y, z;
    cout<<i<<endl;
    fprintf(ofp, "%d\n", i);
    while(i--) {
        float fx, fy, fz;
        fscanf(vfp, "%d %d %d", &x, &y, &z);
        //if(x > 0 || z > 0) continue;
        //voxels[x+CENTER][y+CENTER][z+CENTER] = 1;
        Vector3 vox_pos = Vector3 (x,y,z);
        vox_pos = vox_pos - minpos;
        //vox_pos.print();cout<<" ";
        fscanf(fp, "%f %f %f", &fx, &fy, &fz);
        //normals[x+CENTER][y+CENTER][z+CENTER] = Vector3(fx, fy, fz);
        Vector3 vox_norm = Vector3(fx,fy,fz);
        //vox_norm.print();cout<<" ";
        //Vector3 point = getPlanePoint(vox_norm, vox_pos, maxpos, minpos);
        Vector3 point = getSpherePoint(vox_norm, vox_pos, center, radius);
        //point.print();cout<<endl;

        //if the point of intersection lies on plane x=0
        if(point.x==0) {
            //if the repeated pattern is 1 then append 1
            //to the coordinate of the voxel else append 2
            if(pattern[(int)point.y%height][(int)point.z%width])
                fprintf(ofp,"%d %d %d 1",x,y,z);
            else
                fprintf(ofp, "%d %d %d 2", x,y,z);
        }
        //if the point of intersection lies on plane x=maxx
        /*else if(point.x == -1) {
            //if the repeated pattern is 1 then append 1
            //to the coordinate of the voxel else append 2
            if(pattern[(int)point.y%height][(int)point.z%width])
                fprintf(ofp,"%d %d %d 1",x,y,z);
            else
                fprintf(ofp, "%d %d %d 2", x,y,z);
        }
        //if the point of intersection lies on plane y=0
        else if(point.y==0) {
            //if the repeated pattern is 1 then append 1
            //to the coordinate of the voxel else append 2
            if(pattern[(int)point.x%height][(int)point.z%width])
                fprintf(ofp,"%d %d %d 1",x,y,z);
            else
                fprintf(ofp, "%d %d %d 2", x,y,z);
        }
        //if the point of intersection lies on plane y=maxy
        else if(point.y==-1) {
            //if the repeated pattern is 1 then append 1
            //to the coordinate of the voxel else append 2
            if(pattern[(int)point.x%height][width - 1 - (int)point.z%width])
                fprintf(ofp,"%d %d %d 1",x,y,z);
            else
                fprintf(ofp, "%d %d %d 2", x,y,z);
        }
        //if the point of intersection lies on plane z=0
        else if(point.z==0) {
            //if the repeated pattern is 1 then append 1
            //to the coordinate of the voxel else append 2
            if(pattern[(int)point.x%height][(int)point.y%width])
                fprintf(ofp,"%d %d %d 1",x,y,z);
            else
                fprintf(ofp, "%d %d %d 2", x,y,z);
        }
        //if the point of intersection lies on plane z=maxz
        else if(point.z==-1) {
            //if the repeated pattern is 1 then append 1
            //to the coordinate of the voxel else append 2
            if(pattern[(int)point.x%height][width - 1 - (int)point.y%width])
                fprintf(ofp,"%d %d %d 1",x,y,z);
            else
                fprintf(ofp, "%d %d %d 2", x,y,z);
        }*/
        if(i!=0) {
            fprintf(ofp, "\n" );
        }
    }
    fclose(fp);
    fclose(vfp);
    fclose(ofp);

    /*
    cout<<"Enter seed voxel"<<endl;
    cin>>seedx>>seedy>>seedz;
    visited[seedx+CENTER][seedy+CENTER][seedz+CENTER] = 1;
    CoordSystem start;
    p* head = bfs(seedx, seedy, seedz, start);
    print(head);
    */

}
