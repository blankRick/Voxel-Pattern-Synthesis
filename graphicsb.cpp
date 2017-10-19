//program for printing obj file of given voxels set "finout"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

shortVector vertices[1600000];
shortVector a[300000];


int gen(string finout) {
    char *s;
    s = (char*)malloc(50*sizeof(char));
    s[0] = '\0';
    strcpy(s,finout.c_str());
    FILE *fp;
    fp = fopen(s,"r");

    int size_a;
    fscanf(fp,"%d",&size_a);
    int i=0;
    while(size_a--) {
        fscanf(fp,"%hd",&a[i].x);
        fscanf(fp,"%hd",&a[i].y);
        fscanf(fp,"%hd",&a[i].z);
        //if(a[i].x > 0 || a[i].z > 0) continue;
        i++;
    }
    //translate
    size_a = i;
    printf("%d\n",size_a);
    fclose(fp);
    printf("ok\n");

    fp = fopen("Cube.obj","w");


    fprintf(fp,"mtllib color.mtl\n");
    for(i=0 ; i < size_a ; i++) {
        fprintf(fp, "v %d %d %d\n", a[i].x*2-1, a[i].y*2-1, a[i].z*2+1);
        fprintf(fp, "v %d %d %d\n", a[i].x*2+1, a[i].y*2-1, a[i].z*2+1);
        fprintf(fp, "v %d %d %d\n", a[i].x*2+1, a[i].y*2+1, a[i].z*2+1);
        fprintf(fp, "v %d %d %d\n", a[i].x*2-1, a[i].y*2+1, a[i].z*2+1);
        fprintf(fp, "v %d %d %d\n", a[i].x*2-1, a[i].y*2-1, a[i].z*2-1);
        fprintf(fp, "v %d %d %d\n", a[i].x*2-1, a[i].y*2+1, a[i].z*2-1);
        fprintf(fp, "v %d %d %d\n", a[i].x*2+1, a[i].y*2+1, a[i].z*2-1);
        fprintf(fp, "v %d %d %d\n", a[i].x*2+1, a[i].y*2-1, a[i].z*2-1);

        fprintf(fp, "usemtl face1\n");
        fprintf(fp, "f %d %d %d %d\n", i*8+1, i*8+2, i*8+3, i*8+4);
        fprintf(fp, "usemtl face2\n");
        fprintf(fp, "f %d %d %d %d\n", i*8+5, i*8+6, i*8+7, i*8+8);
        fprintf(fp, "usemtl face3\n");
        fprintf(fp, "f %d %d %d %d\n", i*8+1, i*8+4, i*8+6, i*8+5);
        fprintf(fp, "usemtl face4\n");
        fprintf(fp, "f %d %d %d %d\n", i*8+8, i*8+7, i*8+3, i*8+2);
        fprintf(fp, "usemtl face5\n");
        fprintf(fp, "f %d %d %d %d\n", i*8+4, i*8+3, i*8+7, i*8+6);
        fprintf(fp, "usemtl face6\n");
        fprintf(fp, "f %d %d %d %d\n", i*8+2, i*8+1, i*8+5, i*8+8);
    }
    fclose(fp);
}
