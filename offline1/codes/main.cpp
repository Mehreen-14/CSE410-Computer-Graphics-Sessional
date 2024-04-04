#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include<bits/stdc++.h>

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif

GLfloat xRotation, yRotation,zRotation;
GLdouble  radius = 1;

void display(void);
void reshape(int x,int y);
void idle(void){
    xRotation += 0.01;
    zRotation += 0.01;
    display();
}

int main(int argc,char **argv){
    glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("Controlling Camera");
    xRotation = 43;
    yRotation = 50;

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0; 
	  
}

void display(void){
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0,0.0,-5.0);
    glColor3f(0.9,0.3,0.2);  

    glRotatef(xRotation,1.0,0.0,0.0);
    glRotatef(yRotation,0.0,1.0,0.0);
    glRotatef(zRotation,0.0,0.0,1.0);

    glScalef(1.0,1.0,1.0); 
    glutWireSphere(radius,20,20);

    glFlush();
}

void reshape(int x,int y){
      if(y==0||x==0)
      return;

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(39.0,(GLdouble)x/(GLdouble)y,0.6,21.0);
      glMatrixMode(GL_MODELVIEW);
      glViewport(0,0,x,y); 
}