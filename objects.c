#include "CSCIx229.h"
#include "objects.h"


//some colors used for material/lighting purposes
float white[] = {1,1,1,1};
float black[] = {0,0,0,1};
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
//    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
}

//draw a generic sphere
void Sphere(double x, double y, double z, double r){
    //save any current transformations
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
//    glMaterialf(GL_FRONT,GL_SHININESS,1);
//    glMaterialfv(GL_FRONT,GL_SPECULAR,white);
//    glMaterialfv(GL_FRONT,GL_EMISSION,black);
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

void Square(){
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(1,1,0);
    glVertex3f(-1,1,0);
    glVertex3f(-1,-1,0);
    glVertex3f(1,-1,0);
    glEnd();
    glPopMatrix();
}