#include "CSCIx229.h"
#include "objects.h"


//some colors used for material/lighting purposes
float white[] = {1,1,1,1};
float black[] = {0,0,0,1};
typedef struct {float x,y,z;} vtx;
typedef struct {int A,B,C,D;} quad;

//inspired by triangle ex13
static void Quad(vtx A, vtx B, vtx C, vtx D){
    // planar vector 0
    float dx0 = A.x-B.x;
    float dy0 = A.y-B.y;
    float dz0 = A.z-B.z;
    // planar vector 1
    float dx1 = C.x-A.x;
    float dy1 = C.y-A.y;
    float dz1 = C.z-A.z;
    // Normal
    float Nx = dy0*dz1 - dy1*dz0;
    float Ny = dz0*dx1 - dz1*dx0;
    float Nz = dx0*dy1 - dx1*dy0;
    //draw quad
    glNormal3f(Nx,Ny,Nz);
    glBegin(GL_QUADS);
    glVertex3f(A.x,A.y,A.z);
    glVertex3f(B.x,B.y,B.z);
    glVertex3f(C.x,C.y,C.z);
    glVertex3f(D.x,D.y,D.z);
    glEnd();
}

/*
 *  Draw vertex in polar coordinates and set normals
 * Taken from ex13
 */
static void Vertex(double th,double ph)
{
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z = Sin(ph);
    //for a sphere at the origin, normal = vertex
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
}

//draw a generic sphere
void Sphere(double x, double y, double z, double r){
    //save any current transformations
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glMaterialf(GL_FRONT,GL_SHININESS,1);
    glMaterialfv(GL_FRONT,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT,GL_EMISSION,black);
    //  Latitude bands
   for (int ph=-90;ph<90;ph+=15)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=15)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }
   glPopMatrix();
}

/*
* Draw the ground
* texture - which texture do we want to use (will probably change later)
* num - number of quads to use in the ground.
*/
void Ground(int texture){
    glPushMatrix();
    glColor3f(0,0.5,0);
    glBegin(GL_QUADS);
    glNormal3d(0,1,0);
    glVertex3d(-100,0,-100);
    glVertex3d(100,0,-100);
    glVertex3d(100,0,100);
    glVertex3d(-100,0,100);
    glEnd();
    glPopMatrix();
}

//tree data
const int Ntree = 52;
// tree index list
const quad tree_idx[] = 
{
    //body of tree
    {3,2,1,0}, {2,5,4,1}, {5,7,6,4}, {7,3,0,6},
    {8,9,2,3}, {9,10,5,2}, {10,11,7,5}, {11,8,3,7},
    {12,13,9,8},{13,14,10,9}, {14,15,11,10}, {15,12,8,11},
    {16,17,13,12},{17,18,14,13},{18,19,15,14},{19,16,12,15},
    {19,18,17,16}, //cap that puppy off
    //start branching off - branch 1
    {20,21,22,23}, //"base" of the branches
    {20,21,25,24},{20,24,27,23},{23,27,26,22},{22,26,25,21},
    {24,25,26,27}, //another cap
    {24,28,31,27},{27,31,30,26},{26,30,29,25},{25,29,28,24},
    {28,29,30,31}, //and another one
    {32,33,34,35}, //base of smaller branch
    {32,33,37,36},{32,36,39,35},{35,39,38,34},{34,38,37,33},
    {36,37,38,39}, //cap small boy off
    //start branching off - branch 2
    {20,21,41,40},{20,40,43,23},{23,43,42,22},{22,42,41,21},
    {40,41,45,44},{40,44,47,43},{43,47,46,42},{42,46,45,41},
    {44,45,46,47}, //cap 'er off
    //start branching off - branch 3
    {20,48,51,23},{23,51,50,22},{22,50,49,21},{21,49,48,20},
    {48,52,55,51},{51,55,54,50},{50,54,53,49},{49,53,52,48},
    {52,53,54,55} // cap it off
    // {48,50,56,57}, //small branch cross section 1
    // {48,60,61,50},{50,61,62,56},{56,62,63,57},{57,63,60,48}
};
// tree vertex coords
const vtx tree[] = 
{ 
    {0.0, 0.0, 0.0}, {0.25, 0.0, 0.0}, {0.20, 0.2, -0.05}, {0.05, 0.2, -0.05}, //0,1,2,3
    {0.25, 0.0, -0.25}, {0.2,0.2,-0.2}, {0.0, 0.0, -0.25}, {0.05, 0.2, -0.2}, //4,5,6,7
    {0.25, 0.55, -0.05}, {0.45, 0.55, -0.05}, {0.45, 0.55, -0.2}, {0.25, 0.55, -0.2}, //8,9,10,11
    {0.05, 0.7, -0.05}, {0.25, 0.8, -0.05}, {0.25, 0.8, -0.25}, {0.05, 0.7, -0.25}, //12,13,14,15
    {0.0, 0.8, 0.0},{0.25, 0.9, 0},{0.25, 0.9, -0.25},{0.0, 0.8, -0.25}, //16,17,18,19
    {0.0, 0.8, -0.14},{0.0, 0.8, -0.25},{0.2, 0.8, -0.25},{0.2, 0.8, -0.1}, //20,21,22,23
    {-0.3, 1.05, -0.45},{-0.3, 1.05, -0.5},{-0.2, 1.1,-0.5 },{-0.2, 1.1, -0.4}, //24,25,26,27
    {-0.3, 1.25, -0.45},{-0.3, 1.25, -0.5},{-0.2, 1.25, -0.5},{-0.2, 1.25, -0.4}, //28,29,30,31
    {-0.125,0.92, -0.34},{-0.125, 0.92, -0.37},{-0.125, 0.96, -0.37},{-0.125, 0.96, -0.34}, //32,33,34,35
    {-0.5,0.92, -0.34},{-0.5, 0.92, -0.37},{-0.5, 0.96, -0.37},{-0.5, 0.96, -0.34}, //36,37,38,39
    {0.25, 1.3, -0.4},{0.25, 1.3, -0.5}, {0.35, 1.3, -0.5},{0.35, 1.3, -0.4}, //40,41,42,43
    {0.05, 1.5, -0.6}, {0.05, 1.5, -0.7}, {0.15, 1.5, -0.7}, {0.15, 1.5, -0.6}, //44,45,46,47
    {0.5, 1.4, 0.2},{0.5, 1.4, 0.1},{0.6, 1.4, 0.1},{0.6, 1.4, 0.2}, //48,49,50,51
    {0.3, 1.65, 0.4},{0.3, 1.6, 0.3}, {0.35, 1.6, 0.3},{0.35,1.65,0.4} //52,53,54,55
};


//canopy index list
const int Ncnpy = 25;
const quad cnpy_idx[] = {
    {0,1,2,3}, //base
    {0,3,4,0},{1,0,5,1},{2,1,6,2},{3,2,7,3}, //triangles off of base
    {0,4,5,0},{1,5,6,1},{2,6,7,2},{3,7,4,3}, //more triangles, make a quad shape
    {4,8,9,5},{5,9,10,6},{6,10,11,7},{7,11,8,4}, //quads off the formed quad (middle of the canopy)
    {8,12,9,8},{9,13,10,9},{10,14,11,10},{11,15,8,11}, // MORE TRIANGLES!!!!
    {8,15,12,8},{9,12,13,9},{10,13,14,10},{11,14,15,11}, //WE NEED MORE TRIANGLES!!!!!
    {12,15,16,12},{15,14,16,15},{14,13,16,14},{13,12,16,13}//cap this sucker off
};
//canopy vertex list
const vtx cnpy[] = {
    {0.42, 0, 0.42},{0.42, 0, -0.42},{-0.42, 0, -0.42},{-0.42, 0, 0.42}, //0,1,2,3
    {0.0, 0.2, 0.83},{0.83, 0.2, 0.0},{0.0, 0.2, -0.83},{-0.83, 0.2, 0.0}, //4,5,6,7
    {0.0, 0.4, 0.83},{0.83, 0.4, 0.0},{0.0, 0.4, -0.83},{-0.83, 0.4, 0.0}, //8,9,10,11
    {0.42, 0.6, 0.42},{0.42, 0.6, -0.42},{-0.42, 0.6, -0.42},{-0.42, 0.6, 0.42}, //12,13,14,15
    {0.0,0.7,0.0} // 16
};
/*
Draw Canopy at (x,y,z), scaled by (sx,sy,sz)
c decides color of canopy.
1: Dark Green, 2: Lighter Green
Will be re-used for bushes 
*/
void Canopy(double x, double y, double z, double sx, double sy, double sz, int c){
    //set the color
    float color[4];
    if(c == 1){
        color[0] = 0.0235;color[1] = 0.349;color[2] = 0.0;color[3] = 1.0;
    }
    if(c == 2){
        color[0] = 0.2431;color[1] = 0.4824;color[2] = 0.1647;color[3] = 1.0;
    }
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,1);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    glColor4fv(color);
    glPushMatrix();
    //translate, scale
    glTranslated(x,y,z);
    glScaled(sx,sy,sz);
    for(int i = 0; i < Ncnpy; i++){
        Quad(cnpy[cnpy_idx[i].A],cnpy[cnpy_idx[i].B],cnpy[cnpy_idx[i].C],cnpy[cnpy_idx[i].D]);
    }
    glPopMatrix();
}
/*
Draw Tree at (x,y,z) rotated about y by rotation.
Canopy determines if the tree has green or not
*/
void Tree(double x, double y, double z, double rotation, bool canopy){
    //set up some colors we need
    float brown[] = {0.5430, 0.2610, 0.0742, 1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,1);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,brown);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);   
    glColor4fv(brown);
    glPushMatrix();
    //translate, rotate
    glTranslated(x,y,z);
    glRotated(rotation,0,1,0);
    //draw all the vectors
    for(int i = 0; i < Ntree; i++){
        Quad(tree[tree_idx[i].A],tree[tree_idx[i].B],tree[tree_idx[i].C],tree[tree_idx[i].D]);
    }
    if(canopy){
        Canopy(-0.25,1.2,-0.45,0.8,0.8,0.8,2);
        glPushMatrix();
        glRotated(-5,0,0,1);
        Canopy(0.325,1.6,0.35,1,1,1,1);
        glPopMatrix();
        glPushMatrix();
        glRotated(90,0,0,1);
        Canopy(0.94,0.5,-0.345,0.3,0.5,0.5,2);
        glPopMatrix();
    }

    glPopMatrix();
}
/*
Draw a FarmHouse at x,y,z rotated about y axis by rotation
*/
void FarmHouse(double x, double y, double z, double rotation){

}
/*
Draw a Fence at x,y,z rotated about y axis by rotation
*/
void Fence(double x, double y, double z, double rotation){

}

void Car(double x, double y, double z){

}