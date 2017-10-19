/*
Change some voxles which were empty, to solid so that, the final Graph of
solid voxels becomes connected
Algorithm followed to connect the solid voxels in the graph obtained from graphout.txt,
is a modified version of Djikstra's Algorithm
Instead of setting the distance of a single voxel (source voxel s) to 0
Set the distance value of all the solid voxels connected to s, by path of solid voxels
, to 0.
Now carry out the Djikstra's algorithm on all the voxels (including solid and empty both)
and create a Tree, with root node as s.
After the algorithm is completed and the Tree is do a inorder traversal from root (s),
if in a path a solid voxel is obtained, after a empty voxel, convert the empty voxel to
solid.
Output the voxels that finally remain solid to "finout.txt".

*/

#include <bits/stdc++.h>
#define MAX 280000

using namespace std;
//utility struct to store a vertex of the graph in input file
typedef struct vertex {
    shortVector pos;
    int neighborNum;
    int compNum;
    int neighbors[27];

    vertex() : pos(), compNum(-2), neighborNum(0) {}
    vertex(shortVector pos, int compNum) : pos(pos), compNum(compNum), neighborNum(0) {}

    void insertLast(int index) {
        neighbors[neighborNum++] = index;
    }
    void deleteLast() {
        neighborNum--;
    }
}vertex;
//storing the graph ininput file in this variable
vertex graph[MAX];
//distance array to be used in Djikstra's algorithm
vector<int> dist(MAX,INT_MAX);

//utility struct to store the nodes along with
//parent child relationship obtained from Djikstra's algorithm
typedef struct treeNode {
    int compNum;
    shortVector pos;
    int childNum;
    int parent;
    int children[27];

    treeNode()  {pos = shortVector();childNum=0;compNum=1;parent=-1;}
    treeNode(shortVector pos, int compNum) {this->pos = pos;childNum=0;this->compNum=compNum;parent=-1;}

    void setChild(int index) {
        int prev = this->compNum;
        children[childNum++] = index;
        this->compNum = compNum;
    }
    void deleteInd(int index) {
        int i;
        for(i=0;i<childNum;i++) {
            if(children[i]==index) break;
        }
        childNum--;
        for(int j=i;j<childNum;j++) {
            children[j]=children[j+1];
        }
    }
    void setParent(int index) {
        parent = index;
    }
}treeNode;

treeNode mst[MAX];
int n;
typedef pair<int,int> iPair;
priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

//function which does Djikstra's algorithm using
//priority queue and creates the Shortest Path Tree as it proceeds
void shortestPath()
{
    //checking whether mst and graph are same intially
    for(int i=0;i<n;i++) {
        if(mst[i].compNum != graph[i].compNum) {
            cout<<"fatal error (duplicates): exitting..."<<endl;
            exit(0);
        }
        if(mst[i].pos.x != graph[i].pos.x
           ||mst[i].pos.y != graph[i].pos.y
           ||mst[i].pos.z != graph[i].pos.z) {
            cout<<"fatal error (duplicates): exitting..."<<endl;
            exit(0);
        }
    }
    int vert=0;
    while (!pq.empty())
    {

        int u = pq.top().second;
        vert++;
        pq.pop();

        // 'i' is used to get all adjacent vertices of a vertex

        for (int i = 0; i < graph[u].neighborNum; ++i)
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = graph[u].neighbors[i];

            if (dist[v] > (dist[u] + 1))
            {
                // Updating distance of v
                if(u >=n || v >= n) {
                    cout<<"fatal error : vertex index more than number of vertices"<<endl;
                    exit(0);
                }
                dist[v] = dist[u] + 1;
                pq.push(make_pair<int,int>(dist[v],v));
                mst[u].setChild(v);
                if(mst[v].parent!=-1)
                    mst[mst[v].parent].deleteInd(v);
                mst[v].setParent(u);
            }
        }
    }

    for(int i=0;i<n;i++) {
        if(mst[i].compNum != graph[i].compNum) {
            cout<<"fatal error (duplicates): exitting..."<<endl;
            exit(0);
        }
        if(mst[i].pos.x != graph[i].pos.x
           ||mst[i].pos.y != graph[i].pos.y
           ||mst[i].pos.z != graph[i].pos.z) {
            cout<<"fatal error (duplicates): exitting..."<<endl;
            exit(0);
        }
    }
    cout<<"+++Shortest Path created+++"<<endl;
}

//Once the tree has been created
bool convEmptySolid(int totalComp) {
    int leastNode[totalComp];
    for(int i=0; i<totalComp ; i++) {
        leastNode[i] = -1;
    }
    //find the index of the voxel with compNum = j, with the least distance
    //from the sources (voxels of the starting component in ShortestPath())
    //and store it in leastNode[j]
    for(int i=0; i<n ;i++) {
        if(mst[i].compNum < 3) continue;
        int j = mst[i].compNum;
        if(j>=totalComp){printf("up %d %d\n",j, i);exit(0);}
        if(dist[leastNode[j]] > dist[i]) {
            leastNode[j] = i;
        }
    }
    //from the least distanced node for a particular compNum
    //from the sources traverse to its parent, until one of the sources
    //If a parent's compNum < 3, then it is a empty voxel and hence it is
    //turned solid by assigning a compNum >= 3
    for(int i=0 ; i<totalComp ; i++) {
        if(leastNode[i]==-1) continue;
        int j = mst[leastNode[i]].parent;
        if(j==-1) continue;
        while(mst[j].parent != -1) {
            if(mst[j].compNum < 3) {
//                cout<<"here"<<endl;
                mst[j].compNum = totalComp+1;
            }
            j = mst[j].parent;
        }
//        cout<<(mst[j].compNum)<<endl;
    }
    printf("Connected all components...\n");
}

//graphout.c_str() = "graphout.txt"
//finout.c_str() = "finout.txt"
int connectSamples(string graphout, string finout) {
    FILE *fp = fopen(graphout.c_str(), "r");

    fscanf(fp, "%d", &n);
    int i=n;
    //intializing the vertices in graph and mst and distances
    //if compNum==3 then dist is made 0, and it is pushed into
    //priority queue
    while(i--) {
        int x,y,z,cn,nn;
        fscanf(fp, "%d %d %d %d %d", &x, &y, &z, &cn, &nn);
        graph[n-1-i] = vertex(shortVector(x,y,z), cn);
        mst[n-1-i] = treeNode(shortVector(x,y,z), cn);
        for(int j=0;j<nn;j++) {
            int ind;
            fscanf(fp, "%d", &ind);
            graph[n-1-i].insertLast(ind);
        }
        if(cn!=3) {
            continue;
        }
        dist[n-1-i] = 0;
        pq.push(make_pair<int,int>(0,n-1-i));
    }

    int totalComp;
    fscanf(fp, "%d", &totalComp);
    printf("Number of components (excluding empty voxels) %d\n", totalComp-3);

    fclose(fp);
//int j = mst[95485].compNum;
//        printf("up %d\n",j);
//
    //shortest Path tree is calculated
    cout<<"Finding path..."<<endl;
    shortestPath();
    //Empty voxels are coverted to solid
    cout<<"Converting Empty to solid"<<endl;
    // convEmptySolid(totalComp);
    fp = fopen(finout.c_str(), "w");

    //to store the number of voxels finally printed
    int fin_Count = 0;
    for(i=0;i<n;i++) {
        //if >=3 then it is a solid voxel hence increment and store it in output file
        if(mst[i].compNum >=3) {
            fin_Count++;
            fprintf(fp, "%d %d %d\n", mst[i].pos.x,mst[i].pos.y,mst[i].pos.z);
        }
    }
    fclose(fp);

    //print the output file along with number of voxels
    fp = fopen(finout.c_str(), "w");
    fprintf(fp,"%d\n",fin_Count);
    for(i=0;i<n;i++) {
        if(mst[i].compNum >=3) {
            fprintf(fp, "%d %d %d", mst[i].pos.x,mst[i].pos.y,mst[i].pos.z);
            fin_Count--;

            if(fin_Count!=0)
                fprintf(fp, "\n");
        }
    }
    fclose(fp);
}

