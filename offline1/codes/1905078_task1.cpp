#include<bits/stdc++.h>

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif

int counter = 0;

#define pi (2 * acos(0.0))
#define CAMERA_MOVE 5
#define CAMERA_ROTATE 5 



struct point
{
	double x, y, z;
};

struct unit_vect
{
	double x, y, z;
};

struct point position;
struct unit_vect l, r, u;

double Rad_angle(double deg){
    double angle = (double)(deg*pi)/180;
    return angle;
}


void init(){
    printf("Do your initialization here\n");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

	//setup camera
    position.x = 60;
	position.y = 50;
	position.z = 20;

    u.x = 0;
	u.y = 0;
	u.z = 1/sqrt(2.0);

    l.x = -1/sqrt(2.0);
	l.y = -1/sqrt(2.0);
	l.z = 0;

    r.x = -1/sqrt(2.0);
	r.y = 1/sqrt(2.0);
	r.z = 0;

    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
    

}




void drawAxes()
{
	glLineWidth(3);
	glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
    
}

void specialKeyHandler(int key, int x, int y)
{
	switch (key)
	{
    case GLUT_KEY_UP:
		//move forward
		position.x += l.x * CAMERA_MOVE;
		position.y += l.y * CAMERA_MOVE;
		position.z += l.z * CAMERA_MOVE;
		break;

	case GLUT_KEY_DOWN:
		//move backward
		position.x -= l.x * CAMERA_MOVE;
		position.y -= l.y * CAMERA_MOVE;
		position.z -= l.z * CAMERA_MOVE;
		break;

	
	case GLUT_KEY_RIGHT:
		//move right
		position.x += r.x * CAMERA_MOVE;
		position.y += r.y * CAMERA_MOVE;
		position.z += r.z * CAMERA_MOVE;
		break;

	case GLUT_KEY_LEFT:
		//move left
		position.x -= r.x * CAMERA_MOVE;
		position.y -= r.y * CAMERA_MOVE;
		position.z -= r.z * CAMERA_MOVE;
		break;

	case GLUT_KEY_PAGE_UP:
		//move up
		position.x += u.x * CAMERA_MOVE;
		position.y += u.y * CAMERA_MOVE;
		position.z += u.z * CAMERA_MOVE;
		break;

	case GLUT_KEY_PAGE_DOWN:
		//move down
		position.x -= u.x * CAMERA_MOVE;
		position.y -= u.y * CAMERA_MOVE;
		position.z -= u.z * CAMERA_MOVE;
		break;
	default:
		break;
	}
}

void keyboardListener(unsigned char key,int x, int y){
    switch (key)
	{
	case '1':
		//rotate left
		l.x = l.x * cos(Rad_angle(CAMERA_ROTATE)) - r.x * sin(Rad_angle(CAMERA_ROTATE));
		l.y = l.y * cos(Rad_angle(CAMERA_ROTATE)) - r.y * sin(Rad_angle(CAMERA_ROTATE));
		l.z = l.z * cos(Rad_angle(CAMERA_ROTATE)) - r.z * sin(Rad_angle(CAMERA_ROTATE));

		r.x = r.x * cos(Rad_angle(CAMERA_ROTATE)) + l.x * sin(Rad_angle(CAMERA_ROTATE));
		r.y = r.y * cos(Rad_angle(CAMERA_ROTATE)) + l.y * sin(Rad_angle(CAMERA_ROTATE));
		r.z = r.z * cos(Rad_angle(CAMERA_ROTATE)) + l.z * sin(Rad_angle(CAMERA_ROTATE));
		break;

	case '2':
		//rotate right
		l.x = l.x * cos(Rad_angle(CAMERA_ROTATE)) - r.x * sin(-Rad_angle(CAMERA_ROTATE));
		l.y = l.y * cos(Rad_angle(CAMERA_ROTATE)) - r.y * sin(-Rad_angle(CAMERA_ROTATE));
		l.z = l.z * cos(Rad_angle(CAMERA_ROTATE)) - r.z * sin(-Rad_angle(CAMERA_ROTATE));

		r.x = r.x * cos(Rad_angle(CAMERA_ROTATE)) + l.x * sin(-Rad_angle(CAMERA_ROTATE));
		r.y = r.y * cos(Rad_angle(CAMERA_ROTATE)) + l.y * sin(-Rad_angle(CAMERA_ROTATE));
		r.z = r.z * cos(Rad_angle(CAMERA_ROTATE)) + l.z * sin(-Rad_angle(CAMERA_ROTATE));
		break;

	case '3':
		//rotate up
		u.x = u.x * cos(Rad_angle(CAMERA_ROTATE)) - l.x * sin(Rad_angle(CAMERA_ROTATE));
		u.y = u.y * cos(Rad_angle(CAMERA_ROTATE)) - l.y * sin(Rad_angle(CAMERA_ROTATE));
		u.z = u.z * cos(Rad_angle(CAMERA_ROTATE)) - l.z * sin(Rad_angle(CAMERA_ROTATE));

		l.x = l.x * cos(Rad_angle(CAMERA_ROTATE)) + u.x * sin(Rad_angle(CAMERA_ROTATE));
		l.y = l.y * cos(Rad_angle(CAMERA_ROTATE)) + u.y * sin(Rad_angle(CAMERA_ROTATE));
		l.z = l.z * cos(Rad_angle(CAMERA_ROTATE)) + u.z * sin(Rad_angle(CAMERA_ROTATE));
		break;

	case '4':
		//rotate down
		u.x = u.x * cos(Rad_angle(CAMERA_ROTATE)) - l.x * sin(-Rad_angle(CAMERA_ROTATE));
		u.y = u.y * cos(Rad_angle(CAMERA_ROTATE)) - l.y * sin(-Rad_angle(CAMERA_ROTATE));
		u.z = u.z * cos(Rad_angle(CAMERA_ROTATE)) - l.z * sin(-Rad_angle(CAMERA_ROTATE));

		l.x = l.x * cos(Rad_angle(CAMERA_ROTATE)) + u.x * sin(-Rad_angle(CAMERA_ROTATE));
		l.y = l.y * cos(Rad_angle(CAMERA_ROTATE)) + u.y * sin(-Rad_angle(CAMERA_ROTATE));
		l.z = l.z * cos(Rad_angle(CAMERA_ROTATE)) + u.z * sin(-Rad_angle(CAMERA_ROTATE));
		break;

	case '5':
		//tilt counterclockwise
		r.x = r.x * cos(Rad_angle(CAMERA_ROTATE)) - u.x * sin(Rad_angle(CAMERA_ROTATE));
		r.y = r.y * cos(Rad_angle(CAMERA_ROTATE)) - u.y * sin(Rad_angle(CAMERA_ROTATE));
		r.z = r.z * cos(Rad_angle(CAMERA_ROTATE)) - u.z * sin(Rad_angle(CAMERA_ROTATE));

		u.x = u.x * cos(Rad_angle(CAMERA_ROTATE)) + r.x * sin(Rad_angle(CAMERA_ROTATE));
		u.y = u.y * cos(Rad_angle(CAMERA_ROTATE)) + r.y * sin(Rad_angle(CAMERA_ROTATE));
		u.z = u.z * cos(Rad_angle(CAMERA_ROTATE)) + r.z * sin(Rad_angle(CAMERA_ROTATE));
		break;

	case '6':
		//tilt clockwise
		r.x = r.x * cos(Rad_angle(CAMERA_ROTATE)) - u.x * sin(-Rad_angle(CAMERA_ROTATE));
		r.y = r.y * cos(Rad_angle(CAMERA_ROTATE)) - u.y * sin(-Rad_angle(CAMERA_ROTATE));
		r.z = r.z * cos(Rad_angle(CAMERA_ROTATE)) - u.z * sin(-Rad_angle(CAMERA_ROTATE));

		u.x = u.x * cos(Rad_angle(CAMERA_ROTATE)) + r.x * sin(-Rad_angle(CAMERA_ROTATE));
		u.y = u.y * cos(Rad_angle(CAMERA_ROTATE)) + r.y * sin(-Rad_angle(CAMERA_ROTATE));
		u.z = u.z * cos(Rad_angle(CAMERA_ROTATE)) + r.z * sin(-Rad_angle(CAMERA_ROTATE));
		break;

        //bonus kora lagbe
        
    case 'w':
        // Move up without changing reference point
        position.z +=  CAMERA_MOVE;
        break;

    case 's':
        // Move down without changing reference point
        position.z -=  CAMERA_MOVE;
        break;
    case 'p':
		position.x = 20;
		position.y = 20;
		position.z = 20;


	default:
		break;
	}
}
void drawSquare(double a){
    glBegin(GL_QUADS);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }glEnd();

}

double _rand(){
    return (double)rand() / RAND_MAX;
}

void display(){
    //printf("Display function called for %d times\n", counter);
    
    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   //camera position, where to look at, up direction
	gluLookAt(position.x, position.y, position.z,
			  position.x + l.x, position.y + l.y, position.z + l.z,
			  u.x, u.y, u.z);
    drawAxes();

    glBegin(GL_LINES);{
        glColor3f(1.0f, 1.0f, 1.0f); // Green
        glVertex2f(-1.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);
        glVertex2f(0.0f, -1.0f);
        glVertex2f(0.0f, 1.0f);
       
    }glEnd();


    for(int i = 0; i < 8; i++){
        glPushMatrix();
        glRotatef(i * 90, 1, 0, 0);
        glTranslatef(0, 0, 20);
        glColor3f(1, 0, 0);
        // glColor3f(_rand(), _rand(), _rand()); // Red
        drawSquare(20);
        glPopMatrix();
    }
    
   


    // glFlush();
    glutSwapBuffers();

}

void idle(){

    counter++;
    glutPostRedisplay();
}

int main(int argc,char** argv){
   glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("Controlling Camera");
	init();

	glEnable(GL_DEPTH_TEST); 

	glutDisplayFunc(display); 
	glutIdleFunc(idle);	  // what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyHandler);
	
	glutMainLoop();
    return 0;

}