//main driver for game

#include "CSCIx229.h"


//define global variables
double dim=10;   //Dimension of orth. box
int fov = 55; //field of view
double asp = 1; //aspect ratio 
#define inputKeys 4 //number of input keys we want to process synchronously (can handle having multiple pressed at the same time).
bool keyStates[inputKeys]; //[0]: W/w, [1]: A/a, [2]: S/s, [3] D/d
double sx = 0;
double sy = 0;
double sz = 0;
int th = 0; // azimuth of viewangle
int ph = 30; // elevation of view angle
int mode = 0; // debug mode
float ambient = 0.10;
float diffuse = 0.50;
float specular = 0.0;


//helper function to handle key staetes
void keyHandler(){
    double moveAmt = 0.001;
    if(keyStates[0] && !keyStates[1] && !keyStates[2] && !keyStates[3]){
        sx += moveAmt;
    }
    if(keyStates[1] && !keyStates[0] && !keyStates[2] && !keyStates[3]){
        sz -= moveAmt;
    }
    if(keyStates[2] && !keyStates[1] && !keyStates[0] && !keyStates[3]){
        sx -= moveAmt;
    }
    if(keyStates[3] && !keyStates[1] && !keyStates[2] && !keyStates[0]){
        sz += moveAmt;
    }
}

void display(){
    //debug mode
    if(mode){
        keyHandler();
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glLoadIdentity();
        gluLookAt(Ex,Ey,Ez,0,0,0,0,Cos(ph),0);
        glShadeModel(GL_SMOOTH);
        //lighting stuff
        float Ambient[] = {ambient,ambient,ambient,1.0};
        float Diffuse[] = {diffuse,diffuse,diffuse,1.0};
        float Specular[] = {specular,specular,specular,1.0};
        //set light position
        float Position[] = {sx, sy, sz, 1.0};
        //draw light ball
        glColor3f(1,1,1);
        Sphere(Position[0],Position[1],Position[2],0.1);
        //tell opengl to normalize vectors
        glEnable(GL_NORMALIZE);
        //turn on the lights
        glEnable(GL_LIGHTING);
        //set ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHT0);
        //set ambient,diffuse,and specular components
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
        //draw stuff
        Tree(0,0,0,0);

        glDisable(GL_LIGHTING);
        //draw axes
        glColor3f(1,1,1);
        const double len = 1.5;
        glBegin(GL_LINES);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(len,0.0,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,len,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,0.0,len);
        glEnd();
        //  Label axes
        glRasterPos3d(len,0.0,0.0);
        Print("X");
        glRasterPos3d(0.0,len,0.0);
        Print("Y");
        glRasterPos3d(0.0,0.0,len);
        Print("Z");
        //do the displaying
        glFlush();
        glutSwapBuffers();
    }
    else{
        //each display step, check key states.
        keyHandler();
        //eye position
        double Ex = -2*dim*Sin(th)*Cos(ph) + sx;
        double Ey = +2*dim        *Sin(ph) + sy;
        double Ez = +2*dim*Cos(th)*Cos(ph) + sz;
        //clear window and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //enable depth buffering
        glEnable(GL_DEPTH_TEST);
        //load identity matrix
        glLoadIdentity();
        //set perspective
        gluLookAt(Ex,Ey,Ez , sx,sy,sz , 0,Cos(ph),0);
        glColor3f(1,1,1);
        //try rotating before drawing
        Tree(0,0,0,0);
        Ground(1);
        //do the displaying
        glFlush();
        glutSwapBuffers();
    }
    
}

//gets called when window is resized.
void reshape(int width, int height){
    // viewport is the entire window
    glViewport(0,0, RES*width,RES*height);
    //set aspect ratio
    asp = (height>0) ? (double)width/height : 1;
    //set projection
    Project(fov,asp,dim);    
}

void specialDown(int key, int x, int y){
    if(key == GLUT_KEY_RIGHT){
        th+=5;
    }
    else if(key == GLUT_KEY_LEFT){
        th -=5;
    }
    else if(key == GLUT_KEY_UP){
        ph += 5;
    }
    else if(key == GLUT_KEY_DOWN){
        ph -= 5;
    }
    else if(key == GLUT_KEY_PAGE_DOWN){
        dim += 0.1;
    }
    else if(key == GLUT_KEY_PAGE_UP){
        dim -= 0.1;
    }
    //keep angles +-360
    th %= 360;
    ph %= 360;
    //update projection
    Project(fov,asp,dim);
    //redisplay the scene
    glutPostRedisplay();
}
void specialUp(int key, int x, int y){
    
}
void keyDown(unsigned char key, int x, int y){
    if(key == 27){
        exit(0);
    }
    if(key == 'w' || key == 'W'){
        keyStates[0] = true;
    }
    if(key == 'a' || key == 'A'){
        keyStates[1] = true;
    }
    if(key == 's' || key == 'S'){
        keyStates[2] = true;
    }
    if(key == 'd' || key == 'D'){
        keyStates[3] = true;
    }
    if(key == 'm' || key == 'M'){
        if(mode){
            dim = 10;
        }
        else{
            dim = 2;
        }
        mode = 1 - mode;
        printf("Mode: %d", mode);
    }
    if(key == 'y'){
        sy -= 0.05;
    }
    if(key == 'Y'){
        sy += 0.05;
    }
    Project(fov,asp,dim);
    glutPostRedisplay();
}
void keyUp(unsigned char key, int x, int y){
    if(key == 'w' || key == 'W'){
        keyStates[0] = false;
    }
    if(key == 'a' || key == 'A'){
        keyStates[1] = false;
    }
    if(key == 's' || key == 'S'){
        keyStates[2] = false;
    }
    if(key == 'd' || key == 'D'){
        keyStates[3] = false;
    }
    Project(fov,asp,dim);
    glutPostRedisplay();
}

void idle(){
    glutPostRedisplay();
}
//initialize anything we need
void Init(){
    // set all key states to false a.k.a not pressed
    for(int i = 0; i < inputKeys; i++){
        keyStates[i] = false;
    }
}

int main(int argc, char* argv[]){
    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutCreateWindow("Lazy Driver | Zach Huber");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
    //These functions are the callbacks for GLUT
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //ignore key repeat (key held down)
    glutIgnoreKeyRepeat(1);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
//leave you out for now    glutIdleFunc(idle);

    //pass control to GLUT
    glutMainLoop();
    //return
    return 0;
}