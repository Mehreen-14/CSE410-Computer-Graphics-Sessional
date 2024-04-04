#include<bits/stdc++.h>



using namespace std;

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif

#define pi (2 * acos(0.0))
#define CAMERA_MOVE 5
#define CAMERA_ROTATE 5 
#define SEGMENT 36
#define SQUARE_ROOT 1.0/(sqrt(2))

#define STEPS 16
#define ANGLE_CYLINDER 70.5287794
#define D2R acos(-1) / 180.0f


#define RED glColor3f(1.0f, 0.0f, 0.0f)
#define GREEN glColor3f(0.0f, 1.0f, 0.0f)
#define BLUE glColor3f(0.0f, 0.0f, 1.0f)
#define CYAN glColor3f(0.0f, 1.0f, 1.0f)
#define MAGENTA glColor3f(1.0f, 0.0f, 1.0f)
#define YELLOW glColor3f(1.0f, 1.0f, 0.0f)


int color = 0;


bool triangle = true, octahedron = true, sphere = true, cylinder = true;
double angle;

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

double sphere_radius = 0;

point t_x,t_y,t_z;


double Rad_angle(double deg){
    double angle = (double)(deg*pi)/180;
    return angle;
}








void init(){
    printf("Do your initialization here\n");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

	//setup camera
    position.x =10;
	position.y = 10;
	position.z = 2;

    u.x = 0;
	u.y = 0;
	u.z = 1/sqrt(2.0);

    l.x = -1/sqrt(2.0);
	l.y = -1/sqrt(2.0);
	l.z = 0;

    r.x = -1/sqrt(2.0);
	r.y = 1/sqrt(2.0);
	r.z = 0;

    t_x.x = 1;
    t_x.y = 0;
    t_x.z = 0;

    t_y.x = 0;
    t_y.y = 1;
    t_y.z = 0;

    t_z.x = 0;
    t_z.y = 0;
    t_z.z = 1;

    sphere_radius = 0;
    angle = 0;

    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
    

}


void drawCheckerboard(int width, int height)
{
    const float step = 2.0f;

    for (float i = -width / 2.0f; i < width / 2.0f; i++)
    {
        for (float j = -height / 2.0f; j < height / 2.0f; j++)
        {
            if ((int(i) + int(j)) % 2 == 0)
                glColor3f(1.0f, 1.0f, 1.0f); // white
            else
                glColor3f(0.0f, 0.0f, 0.0f); // black

            glBegin(GL_QUADS);
            {
                glVertex3f(i * step, j * step, 0);
                glVertex3f(i * step + step, j * step, 0);
                glVertex3f(i * step + step, j * step + step, 0);
                glVertex3f(i * step, j * step + step, 0);
            }
            glEnd();
        }
    }
}

void drawAxes()
{
	glLineWidth(3);
	glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			//glColor3f(1,1,0);
            YELLOW;
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			//glColor3f(0,1,1);   
            CYAN;
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			//glColor3f(1,0,1);   
            MAGENTA;
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
    
}


void drawSquare(double a){
    glBegin(GL_QUADS);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }glEnd();

}



bool triangleFill = true;



void drawTriangle(){
    glPushMatrix();
        double scaling_factor = 1 - sqrt(2) * sphere_radius;
        double translate_factor= 1/sqrt(3) * sphere_radius;
        glTranslatef(translate_factor, translate_factor, translate_factor);
        glScalef(scaling_factor, scaling_factor, scaling_factor);
        if (triangleFill) glBegin(GL_TRIANGLES);
        else glBegin(GL_LINE_LOOP);
        
        glVertex3f(t_x.x, t_x.y, t_x.z);
        glVertex3f(t_y.x, t_y.y, t_y.z);
        glVertex3f(t_z.x, t_z.y, t_z.z);
        
        glEnd();
    glPopMatrix();
}


void drawPyramid()
{
    for (int i = 0; i < 2; i++)
    {
        glPushMatrix();
            if(color)
                MAGENTA;
            else 
                CYAN;    
            glRotatef(i * 90, 0, 1, 0);
            drawTriangle();
        glPopMatrix();
        
        color = 1-color;
    }
}

void drawOctahedron()
{
    drawPyramid();
    glPushMatrix();
        glRotatef(180, 1, 0, 0);
        drawPyramid();
    glPopMatrix();
}

vector<vector<point>> UnitPositiveX(int subdivision)
{
   

    // compute the number of Vertices per row, 2^n + 1
    int points_per_row = (int)pow(2, subdivision) + 1;
    vector<vector<point>> Vertices(points_per_row);
    float a1;           // longitudinal angle (Y-axis)
    float a2;           // latitudinal angle (Z-axis)
    float n1[3];        // normal of longitudinal plane rotating  (Y-axis)
    float n2[3];        // normal of latitudinal plane rotating (Z-axis)
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < points_per_row; ++i)
    {
        a2 = D2R * (45.0f - 90.0f * i / (points_per_row - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < points_per_row; ++j)
        {
            /* normal for longitudinal plane
             if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
             therefore, it is rotating (0,0,-1) vector by longitude angle a1*/
            a1 = D2R * (-45.0f + 90.0f * j / (points_per_row - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            //direction vector of n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            
            Vertices[i].push_back({v[0], v[1], v[2]});
        }
    }

    return Vertices;
}

void drawSphere()
{
    vector<vector<point>> Vertices = UnitPositiveX(3);
    int points_per_row = (int)pow(2, 3) + 1;
    
    glScalef(sphere_radius, sphere_radius, sphere_radius); 
    for (int i = 0; i < points_per_row - 1; i++)
    {
        glBegin(GL_QUADS);
        for (int j = 0; j < points_per_row - 1; j++)
        {
            glVertex3f(Vertices[i][j].x, Vertices[i][j].y, Vertices[i][j].z);
            glVertex3f(Vertices[i][j + 1].x,Vertices[i][j + 1].y,Vertices[i][j + 1].z);
            glVertex3f(Vertices[i + 1][j + 1].x,Vertices[i + 1][j + 1].y,Vertices[i + 1][j + 1].z);
            glVertex3f(Vertices[i + 1][j].x,Vertices[i + 1][j].y,Vertices[i + 1][j].z);
        }
        glEnd();
    }
}

void drawAllSphere(){
    double dist = 1 - sqrt(2) * sphere_radius;
    double translate_factor[4][3] = {{dist, 0, 0}, {0, 0, -dist}, {-dist, 0, 0}, {0, 0, dist}};

    for (int i = 0; i < 4; i++) {
        if (i%2 != 0) RED;
        else BLUE;
        glPushMatrix();
            glTranslatef(translate_factor[i][0], translate_factor[i][1], translate_factor[i][2]);
            glRotatef(i * 90, 0, 1, 0);
            drawSphere();
        glPopMatrix();
    }

    GREEN;  

    glPushMatrix();
        glTranslatef(0, dist, 0);
        glRotatef(90, 0, 0, 1);
        drawSphere();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -dist, 0);
        glRotatef(-90, 0, 0, 1);
        drawSphere();
    glPopMatrix();

}


void drawCylinder(float angle, float radius, float height) {
    const int no_segments = 100;
    const float segment_angle = angle * 3.1415f / 180.0f;

    float angle_div2 = angle/2;

    glPushMatrix();
        glTranslatef(0.0f, -height/2, 0.0f);
        glRotatef(angle_div2, 0.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= no_segments; ++i) {
            float theta = i * segment_angle / no_segments;

            float x = radius*cos(theta);
            float z = radius*sin(theta);

            glVertex3f(x, 0.0f, z);
            glVertex3f(x, height*1.0f, z);
        }
        glEnd();
    glPopMatrix();
}

void drawAllCylinder(){
    double radius = sphere_radius; 
    double height = sqrt(2) - 2*radius;
    double t = (1 - sqrt(2) * radius)/2;

    GREEN;
    
    // 8 cylinder segments that are parallel to XY and YZ plane
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
            glRotatef(i * 180, 1, 0, 0);
            for (int j = 0; j < 4; j++) {
                glPushMatrix();
                    glRotatef(j * 90, 0, 1, 0);
                    glPushMatrix();
                        glTranslatef(t, t, 0);
                        glRotatef(45, 0, 0, 1);
                        drawCylinder(ANGLE_CYLINDER, radius, height);
                    glPopMatrix();
                glPopMatrix();
            }
        glPopMatrix();
    }   
    
    // 4 cylinder segments that are parallel to XZ plane
    for (int j = 0; j < 4; j++) {
        glPushMatrix();
            glRotatef(j * 90, 0, 1, 0);
            glPushMatrix();
                glRotatef(90, 1, 0, 0);
                glPushMatrix();
                    glTranslatef(t, t, 0);
                    glRotatef(45, 0, 0, 1);
                    drawCylinder(ANGLE_CYLINDER, radius, height);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    }

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

    case ',': 
        sphere_radius += SQUARE_ROOT/16.0;
        if(sphere_radius > SQUARE_ROOT) sphere_radius = SQUARE_ROOT;
        break;
    
    case '.': 
        sphere_radius -= SQUARE_ROOT/16.0;
        if(sphere_radius < 0) sphere_radius = 0;
        break;    
    case 'a': 
        angle -= 5;
        break; 

    case 'd':
        angle += 5;
        break;
       
   
	default:
		break;
	}
}



double _rand(){
    return (double)rand() / RAND_MAX;
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-50, 50, -50, 50); // Set orthographic projection
    glMatrixMode(GL_MODELVIEW);
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
    glRotatef(angle,0,0,1);          
    drawAxes();
    if(octahedron)
        drawOctahedron();
    if(sphere)
        drawAllSphere();
     if(cylinder)
         drawAllCylinder();    


	//glColor3f(1, 1, 1);

    //drawCheckerboard(120, 320);


    /*glBegin(GL_LINES);{
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
    }*/

	
    //drawBall();
    
   


    // glFlush();
    glutSwapBuffers();

}

void idle(){

    //counter++;
    glutPostRedisplay();
}

int main(int argc,char** argv){
   glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("MAGIC!!");
	init();

	glEnable(GL_DEPTH_TEST); 

	glutDisplayFunc(display); 

	glutIdleFunc(idle);

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyHandler);
	
	glutMainLoop();
    return 0;

}
