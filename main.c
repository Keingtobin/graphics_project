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
int th = 0;
int ph = 30;

//helper function to handle key staetes
void keyHandler(){
    double moveAmt = 0.01;
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
    Sphere(sx,sy,sz,1);
    Ground(1);
    //do the displaying
    glFlush();
    glutSwapBuffers();
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