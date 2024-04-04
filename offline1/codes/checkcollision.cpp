#include <GL/freeglut_std.h>
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


#define RED glColor3f(1.0f, 0.0f, 0.0f)
#define GREEN glColor3f(0.0f, 1.0f, 0.0f)
#define BLUE glColor3f(0.0f, 0.0f, 1.0f)
#define CYAN glColor3f(0.0f, 1.0f, 1.0f)
#define MAGENTA glColor3f(1.0f, 0.0f, 1.0f)
#define YELLOW glColor3f(1.0f, 1.0f, 0.0f)

bool color = false;




bool triangle = true, octahedron = true, sphere = true, cylinder = true;
double angle;

//time, event-driven
int sim = 0; //for simulation
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

double ball_radius,ball_speed;
double ball_rotation;
double ball_turn,R = 15;

point curr;
double dist  = 0;
point ball;


double Rad_angle(double deg){
    double angle = (double)(deg*pi)/180;
    return angle;
}



point a;

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


	ball_radius = 3;
	ball_speed = 2;



	ball.x = 0;
	ball.y = 0;
	ball.z = ball_radius;

    sphere_radius = SQUARE_ROOT;
    angle = 0;

	
	//rotation of ball
	ball_rotation = 0;//forward
	ball_turn = 0;//turning


    a.x = cos(Rad_angle(ball_turn)); // ball_turn for nijer axis e ghorar jonno
    a.y = sin(Rad_angle(ball_turn));
    a.z = 0;

    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
    

}




// Function to handle collisions and update ball direction
void handleCollisions(int plane) {

    cout<<"Plane "<<plane<<"\n";;
    // Define the normals of the bounding planes (adjust these according to your scene)
    unit_vect planeNormals[] = {
        {1, 0, 0},  // X-axis plane
        {-1, 0, 0}, // Negative X-axis plane
        {0, 1, 0},  // Y-axis plane
        {0, -1, 0}  // Negative Y-axis plane
    
    };

    

   
            double dotProduct = a.x * planeNormals[plane].x + a.y * planeNormals[plane].y + a.z * planeNormals[plane].z;
            a.x = a.x - 2 * dotProduct * planeNormals[plane].x;
            a.y = a.y - 2 * dotProduct * planeNormals[plane].y;
            a.z = a.z - 2 * dotProduct * planeNormals[plane].z;
        

         

            // Update ball_turn based on the wall orientation

            if((a.x>0 && a.y>0 )){
                ball_turn = (double)(atan2(a.y,a.x) * 180/pi);
                // cout<<ball_turn<<endl;

               // ball_turn = (pi/2) - ball_turn;

            }

            else if(( a.x<0 && a.y<0)){
                ball_turn = (double)(-180 + (atan2(a.y,a.x)*180/pi));
            }

            else if((a.x>0 && a.y<0)){
                ball_turn = (double)(-atan2(a.y,a.x)*180/pi);
               // ball_turn = - ball_turn;
            }

            else if( (a.x<0 && a.y>0)){
                ball_turn = (double)( 180 - (atan2(a.y,a.x)*180/pi));
            }

            //ball_turn = 5.4;

            cout<<"ball_gur"<<ball_turn<<'\n';
            ball_turn = static_cast<int>((ball_turn + 180.0)) % 360;

            if (ball_turn < 0)
                ball_turn += 360;
            ball_turn -= 180;
            
    //}
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
			glColor3f(1,1,0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glColor3f(0,1,1);   
        
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glColor3f(1,0,1);   
        
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
    
}



void drawArrow() {
    glLineWidth(3);
    double arrowLength = 10.0;
    glBegin(GL_LINES);{
        BLUE;
        glVertex3f(0,0,0);
       // printf("%f\n",cos(Rad_angle(ball_turn)));
       // printf("%f",sin(Rad_angle(ball_turn)));

        glVertex3f(arrowLength*cos(Rad_angle(ball_turn)),arrowLength*sin(Rad_angle(ball_turn)),0);
        cout<<"Arrow "<<ball_turn<<endl;
       //glVertex3f(15,15,0);
       // glColor3f(1,1,0);
		
    } glEnd();
}


void drawSquare(double a){
    glBegin(GL_QUADS);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }glEnd();

}


void drawBall(){

	//glTranslatef(0,0,R);
	
	glTranslatef(ball.x,ball.y,ball.z); // Adjust the z-coordinate based on where you want the ball
    
    drawArrow();
    //glRotatef(ball_turn,0,1,0);	
	

    glRotatef(ball_rotation,0,0,1);
    
    
    // Draw a solid sphere as the ball with alternating white and green slices
    int numSlices = 36;
    int numStacks = 20;
    float sliceAngle = 360.0 / numSlices;

    for (int i = 0; i < numSlices; ++i) {
        if(color)
            GREEN;
        else    
            RED; 
        
        glRotatef(sliceAngle, 0, 0, 1);
        glutSolidSphere(ball_radius, numStacks, numSlices);
        color = !color;
        
    }

    

   //glRotatef(ball_rotation,-1,1,0);




	
}


void drawRegion(){
	// Draw the red box
    RED; 
    

    // Left face
    glBegin(GL_QUADS);
    {
        glVertex3f(-50.0f, -50.0f, 0.0f);  
        glVertex3f(-50.0f, 50.0f, 0.0f);   
        glVertex3f(-50.0f, 50.0f, 5.0f); 
        glVertex3f(-50.0f, -50.0f, 5.0f); 
    }
    glEnd();

    // Right face
    glBegin(GL_QUADS);
    {
        glVertex3f(50.0f, -50.0f, 0.0f);  
        glVertex3f(50.0f, 50.0f, 0.0f);   
        glVertex3f(50.0f, 50.0f, 5.0f); 
        glVertex3f(50.0f, -50.0f, 5.0f); 
    }
    glEnd();


	glBegin(GL_QUADS);
    {
        glVertex3f(50.0f, 50.0f, 0.0f);  
        glVertex3f(-50.0f, 50.0f, 0.0f); 
        
        glVertex3f(-50.0f, 50.0f, 5.0f);
        glVertex3f(50.0f, 50.0f, 5.0f);  
         
        
	}
    glEnd();



	glBegin(GL_QUADS);
    {
        glVertex3f(50.0f, -50.0f, 0.0f);   
        glVertex3f(-50.0f, -50.0f, 0.0f);
           
        glVertex3f(-50.0f, -50.0f, 5.0f);
        glVertex3f(50.0f, -50.0f, 5.0f); 
        
	}
    glEnd();



}




void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up the camera position
    gluLookAt(position.x, position.y, position.z,
              position.x + l.x, position.y + l.y, position.z + l.z,
              u.x, u.y, u.z);

    // glRotatef(angle, 0, 0, 1);

    // Draw the axes
    //drawAxes();

    // Draw the checkerboard
    drawCheckerboard(120, 320);
	drawRegion();


     // Update and handle collisions
    //
    

    // Set  the ball
	



    glPushMatrix();
    
	drawBall();
    //drawArrow();
   
    glPopMatrix();

     //
    
	

	//

    glutSwapBuffers();
}



//check collision



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

    glutPostRedisplay();
}



int plane;
void keyboardListener(unsigned char key,int x, int y){
    bool hit=false;
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
    // case 'a': 
    //     angle -= 5;
    //     break; 

    // case 'd':
    //     angle += 5;
    //     break;

    case 'm':
        ball_radius -= 1;
        if(ball_radius<=0){
            ball_radius = 3;
        }
        break;    
    case 'i':
        // Rotate the ball
        ball_rotation += 5;
        cout<<"rotation "<<ball_rotation<<endl;
        // Move the ball forward
        //ball_turn+=5;
        // dist = ball_speed * cos(ball_turn * pi / 180.0);
        dist= 2*pi*(double)ball_radius*(double)(100)/(double)(360);
        ball.x -= dist * a.x;
        cout<<"a.x "<<a.x<<endl;
        ball.y -= dist * a.y;
        cout<<"a.y "<<a.y<<endl;
        // dist=2*pi*ball_radius*(double)(100/360);
        // ball.x -= cos(Rad_angle(ball_rotation))*dist;
        // ball.y -= sin(Rad_angle(ball_rotation))*dist;
        

        // dist = 2*pi*(double)ball_radius*(double)(100)/(double)(360);
        // ball.x += cos(Rad_angle(ball_rotation)) * dist;
        // ball.y += sin(Rad_angle(ball_radius)) * dist;

        
        if((abs(ball.x)+ball_radius)>49){
            //cout<<"ki"<<'\n';
            hit = true;
        if(ball.x>0){
            //cout<<"0"<<'\n';
            ball.x=49-ball_radius;
            plane = 0;
         }
        else if(ball.x<0){
            //cout<<"1"<<'\n';
            ball.x=-49+ball_radius;
            plane = 1;
         }
        }
        else if((abs(ball.y)+ball_radius)>49){
            //cout<<"hoise"<<'\n';
            hit = true;
            if(ball.y>0){
                //cout<<"2"<<'\n';
            ball.y=49-ball_radius;
            plane = 2;
         }
        else if(ball.y<0){
           // cout<<"3"<<'\n';
            ball.y=-49+ball_radius;
            plane = 3;
         }
        }


            
        if(hit){
            cout<<"HIT"<<endl;
            handleCollisions(plane);
        }

        
        break; 

	case 'k':
        // Rotate the ball
        ball_rotation -= 5;	

        cout<<"R "<<ball_rotation<<endl; 
        dist= 2*pi*(double)ball_radius*(double)(-100)/(double)(360);
		// dist = ball_speed * cos(ball_turn * pi / 180.0);
        ball.x -= dist * a.x;
        ball.y -= dist * a.y;

        if((abs(ball.x)+ball_radius)>49){
            
            hit = true;
        if(ball.x>0){
            
            ball.x=49-ball_radius;
            plane = 0;
         }
        else if(ball.x<0){
            
            ball.x=-49+ball_radius;
            plane = 1;
         }
        }
        else if((abs(ball.y)+ball_radius)>49){
            
            hit = true;
            if(ball.y>0){
                
            ball.y=49-ball_radius;
            plane = 2;
         }
        else if(ball.y<0){
            
            ball.y=-49+ball_radius;
            plane = 3;
         }
        }

            
        if(hit){
            cout<<"HIT"<<endl;
            handleCollisions(plane);
        }


        
		break;
	case 'j':
        ball_turn += 5;  // Adjust the rotation angle as needed
        ball_turn = static_cast<int>((ball_turn + 180.0)) % 360;

        if (ball_turn < 0)
             ball_turn += 360;
        ball_turn -= 180;
        break;	
	case 'l':
		ball_turn -= 5;	
        ball_turn = static_cast<int>((ball_turn + 180.0)) % 360;

        if (ball_turn < 0)
             ball_turn += 360;
        ball_turn -= 180;
        break;
    case 'r':
		init();
		break;

    case ' ':
        sim = 1 - sim;    
	default:
		break;
	}

    glutPostRedisplay();
}




void idle(){

    //counter++;
   // handleCollisions();
    //updateBallPosition();
    glutPostRedisplay();
}


int minimum_time()
{
    double v = 10;
    double b1,b2,b3,b4;
    int time=INT_MAX;
    b1=50-ball.x;
    b2=50-ball.y;
    b3=50+ball.x;
    b4=50+ball.y;
    time=min(time,(int)(b1/v));
    time=min(time,(int)(b2/v));
    time=min(time,(int)(b3/v));
    time=min(time,(int)(b4/v));
    return time;
}
void timer(int millisec)
{
    bool hit = false;
    if (sim==1) //e
    {
         // Rotate the ball
        ball_rotation += 5;
        cout<<"rotation "<<ball_rotation<<endl;
        // Move the ball forward
        //ball_turn+=5;
        // dist = ball_speed * cos(ball_turn * pi / 180.0);
        dist= 2*pi*(double)ball_radius*(double)(100)/(double)(360);
        ball.x -= dist * a.x;
        cout<<"a.x "<<a.x<<endl;
        ball.y -= dist * a.y;
        cout<<"a.y "<<a.y<<endl;
        // dist=2*pi*ball_radius*(double)(100/360);
        // ball.x -= cos(Rad_angle(ball_rotation))*dist;
        // ball.y -= sin(Rad_angle(ball_rotation))*dist;
        

        // dist = 2*pi*(double)ball_radius*(double)(100)/(double)(360);
        // ball.x += cos(Rad_angle(ball_rotation)) * dist;
        // ball.y += sin(Rad_angle(ball_radius)) * dist;

        
        if((abs(ball.x)+ball_radius)>49){
            
            hit = true;
        if(ball.x>0){
            
            ball.x=49-ball_radius;
            plane = 0;
         }
        else if(ball.x<0){
            
            ball.x=-49+ball_radius;
            plane = 1;
         }
        }
        else if((abs(ball.y)+ball_radius)>49){
            
            hit = true;
            if(ball.y>0){
               
            ball.y=49-ball_radius;
            plane = 2;
         }
        else if(ball.y<0){
            
            ball.y=-49+ball_radius;
            plane = 3;
         }
        }


            
        if(hit){
            cout<<"HIT"<<endl;
            handleCollisions(plane);
        }

        int dt=minimum_time();
        glutPostRedisplay();
        glutTimerFunc(dt, timer, 0);
    }
    else //t
    {
        glutPostRedisplay();
        glutTimerFunc(5, timer, 0);
    }
}

int main(int argc,char** argv){
   glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("TASK-2");
	init();

	glEnable(GL_DEPTH_TEST); 

	glutDisplayFunc(display); 

    glutTimerFunc(10, timer, 35);

	glutIdleFunc(idle);	  

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyHandler);
	
	glutMainLoop();
    return 0;

}