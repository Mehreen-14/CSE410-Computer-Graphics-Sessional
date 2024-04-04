#include<bits/stdc++.h>
#include <cstdlib>
#include<fstream>
#include<sstream>

#include "1905078_classes.h"

#include "bitmap_image.hpp"
using namespace std;
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


int drawaxes;
extern int recursion;
int pixels=0;
int obj_count=0;
string obj_shape;

extern Vector3D eye;
Vector3D l, r, u;
double height=500, width=500;

//double viewAngle;

const double floor_amb = 0.25;
const double floor_dif = 0.25;
const double floor_spe = 0.25;
const double floor_ref = 0.25;
const double floor_shi = 10;

extern vector<PointLight> pointLights;
extern vector<spotLight> spotLights;
extern vector<Object*> objects;

int img_count = 0;
int point_light_count = 0;
int spot_light_count = 0;


double Rad_angle(double deg){
    double angle = (double)(deg*pi)/180;
    return angle;
}


void init(){
    printf("Do your initialization here\n");

	//height=, width=pixels;
	//viewAngle = 80;

	drawaxes = 1;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

	//setup camera
    eye.x = 100;
	eye.y = 100;
	eye.z = 0;

    u.x = 0;
	u.y = 0;
	u.z = 1/sqrt(2.0);

    // l.x = -1/sqrt(2.0);
	// l.y = -1/sqrt(2.0);
	// l.z = 0;

    r.x = -1/sqrt(2.0);
	r.y = 1/sqrt(2.0);
	r.z = 0;

	l= u^r;

	glClearColor(0,0,0,0);


    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
    

}

void capture(){
	bitmap_image image(pixels, pixels);
	//cout<<height<<" "<<width<<endl;
	double plane_distance = (height / 2) / tan(Rad_angle(80/2));
	//cout<<plane_distance<<endl;
	Vector3D topleft = eye + l * plane_distance - r * (width / 2) + u * (height / 2);
	//cout<<topleft.x<<" "<<topleft.y<<" "<<topleft.z<<endl;

	double du = (double) width / pixels;
	double dv = (double) height / pixels;

	topleft = topleft + r * (du / 2) - u * (dv / 2);

	int nearest;
	double t,min_t;

	for(int i=0;i<pixels;i++){
		for(int j=0;j<pixels;j++){
			Vector3D currentPixel = topleft + r*(i*du) - u*(j*dv);
            Ray ray(eye, currentPixel - eye); 
    		Color color;

             nearest = -1;
             min_t = INFINITY;

             for(int k=0; k<objects.size(); k++)
             {
                 t = objects[k]->intersect(ray, color, 0);

                 if(t > 0 && t < min_t)
                 {
                     min_t = t;
                     nearest = k;
                 }
             }

             if(nearest != -1)
             {
				Color colorContainer;
                 min_t = objects[nearest]->intersect(ray, colorContainer, 1);
				 colorContainer.ClipColor();
                 image.set_pixel(i, j, (int) round(colorContainer.r * 255), (int) round(colorContainer.g * 255), (int) round(colorContainer.b * 255));
             }
             else
            {
                 image.set_pixel(i, j, 0, 0, 0);
            }
		}
	}
	img_count++;
	image.save_image("Output_"+to_string(img_count)+".bmp");
	cout << "Image saved as out" << img_count << ".bmp" << endl;
	
}


void clearObjects(){
	for(int i=0;i<objects.size();i++){
		delete objects[i];
	}
	objects.clear();
}

void clearLights(){
	pointLights.clear();
	spotLights.clear();
}	




void loadData(){
	ifstream in;

	in.open("scene.txt");

	if(!in.is_open()) {
        cout << "input.is_open(): failed to open input file" << endl;
        exit(EXIT_FAILURE);
    }

	in >> recursion;
	in >> pixels;
	in >> obj_count;

	//cout << "Recursion: " << recursion << endl;
	//cout << "Pixels: " << pixels << endl;
	//cout << "Object count: " << obj_count << endl;
	
	string obj_type;
	Object* ob = NULL;

	for(int i=0;i<obj_count;i++){
		in >> obj_type;
		if(obj_type == "sphere"){
			//printf("Sphere\n");
			double center_x, center_y, center_z, radius;
			double color_r, color_g, color_b;
			double amb, dif, spe, ref, shi;
			in>>center_x>>center_y>>center_z;
			in>>radius;
			in>>color_r>>color_g>>color_b;
			in>>amb>>dif>>spe>>ref;
			in>>shi;

			

			Vector3D center(center_x, center_y, center_z);
			ob = new Sphere(center,radius);
			ob->setColor(color_r, color_g, color_b);
			ob->setCoefficients(amb, dif, spe, ref);
			ob->setShine(shi);
			objects.push_back(ob);

		}

		else if(obj_type == "triangle"){
			//printf("Triangle\n");
			double point1_x, point1_y, point1_z;
			double point2_x, point2_y, point2_z;
			double point3_x, point3_y, point3_z;
			double color_r, color_g, color_b;
			double amb, dif, spe, ref, shi;

			in>>point1_x>>point1_y>>point1_z;
			in>>point2_x>>point2_y>>point2_z;
			in>>point3_x>>point3_y>>point3_z;
			in>>color_r>>color_g>>color_b;
			in>>amb>>dif>>spe>>ref;
			in>>shi;

			
			Vector3D point1(point1_x, point1_y, point1_z);
			Vector3D point2(point2_x, point2_y, point2_z);
			Vector3D point3(point3_x, point3_y, point3_z);

			ob = new Triangle(point1, point2, point3);
			ob->setColor(color_r, color_g, color_b);
			ob->setCoefficients(amb, dif, spe, ref);
			ob->setShine(shi);
			objects.push_back(ob);

		}

		else if(obj_type == "general"){
			//cout<<"General Quadratic\n";
			double A, B, C, D, E, F, G, H, I, J;
			double color_r, color_g, color_b;
			double amb, dif, spe, ref, shi;
			double length, width, height;
			double ref_x, ref_y, ref_z;

			in>>A>>B>>C>>D>>E>>F>>G>>H>>I>>J;
			in>>ref_x>>ref_y>>ref_z>>length>>width>>height;
			in>>color_r>>color_g>>color_b;
			in>>amb>>dif>>spe>>ref;
			in>>shi;

			
			Vector3D ref_point(ref_x, ref_y, ref_z);

			ob = new GeneralQuadratic(A, B, C, D, E, F, G, H, I, J, ref_point, length, width, height);
			ob->setColor(color_r, color_g, color_b);
			ob->setCoefficients(amb, dif, spe, ref);
			ob->setShine(shi);
			objects.push_back(ob);

			
		}

		else {
			printf("Unknown object\n");
			exit(1);
		}
	}


	//cout << "Objects loaded: " << objects.size() << endl;
	in >> point_light_count;
	cout << "Point light count: " << point_light_count << endl;
	double light_x, light_y, light_z;
	double light_r, light_g, light_b;

	for(int i=0;i<point_light_count;i++){
		in >> light_x >> light_y >> light_z;
		in >> light_r >> light_g >> light_b;
		Vector3D pos(light_x, light_y, light_z);
		Color color(light_r, light_g, light_b);
		pointLights.push_back(PointLight(pos, color));
	}

	in >> spot_light_count;
	cout << "Spot light count: " << spot_light_count << endl;
	double spot_x, spot_y, spot_z;
	double spot_r, spot_g, spot_b;
	double spot_dir_x, spot_dir_y, spot_dir_z;
	double spot_ang;
	for (int  i = 0; i < spot_light_count; i++)
	{
		/* code */
		in >> spot_x >> spot_y >> spot_z;
		in >> spot_r >> spot_g >> spot_b;
		in >> spot_dir_x >> spot_dir_y >> spot_dir_z;
		in >> spot_ang;

		Vector3D pos(spot_x, spot_y, spot_z);
		Vector3D dir(spot_dir_x, spot_dir_y, spot_dir_z);
		Color color(spot_r, spot_g, spot_b);

		spotLights.push_back(spotLight(pos, dir, color,spot_ang));

	}
	
	in.close();

	ob = new Floor(1000, 20);
	ob->setCoefficients(floor_amb, floor_dif, floor_spe, floor_ref);
	ob->setShine(floor_shi);
	objects.push_back(ob);

	
}




void drawAxes(){
	if(drawaxes==1)
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
    
}

void specialKeyHandler(int key, int x, int y)
{
	switch (key)
	{
    case GLUT_KEY_UP:
		//move forward
		eye.x += l.x * CAMERA_MOVE;
		eye.y += l.y * CAMERA_MOVE;
		eye.z += l.z * CAMERA_MOVE;
		break;

	case GLUT_KEY_DOWN:
		//move backward
		eye.x -= l.x * CAMERA_MOVE;
		eye.y -= l.y * CAMERA_MOVE;
		eye.z -= l.z * CAMERA_MOVE;
		break;

	
	case GLUT_KEY_RIGHT:
		//move right
		eye.x += r.x * CAMERA_MOVE;
		eye.y += r.y * CAMERA_MOVE;
		eye.z += r.z * CAMERA_MOVE;
		break;

	case GLUT_KEY_LEFT:
		//move left
		eye.x -= r.x * CAMERA_MOVE;
		eye.y -= r.y * CAMERA_MOVE;
		eye.z -= r.z * CAMERA_MOVE;
		break;

	case GLUT_KEY_PAGE_UP:
		//move up
		eye.x += u.x * CAMERA_MOVE;
		eye.y += u.y * CAMERA_MOVE;
		eye.z += u.z * CAMERA_MOVE;
		break;

	case GLUT_KEY_PAGE_DOWN:
		//move down
		eye.x -= u.x * CAMERA_MOVE;
		eye.y -= u.y * CAMERA_MOVE;
		eye.z -= u.z * CAMERA_MOVE;
		break;
	default:
		break;
	}
}

void keyboardListener(unsigned char key,int x, int y){
    switch (key)
	{
	case '0':
		capture();
		break;
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

        
        
    case 'w':
        // Move up without changing reference point
        eye.z +=  CAMERA_MOVE;
        break;

    case 's':
        // Move down without changing reference point
        eye.z -=  CAMERA_MOVE;
        break;
    case 'p':
		eye.x = 20;
		eye.y = 20;
		eye.z = 20;


	default:
		break;
	}
}

void MouseListener(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		
		
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		
	}
}

double _rand(){
    return (double)rand() / RAND_MAX;
}

void display(){
    //printf("Display function called for %d times\n", counter);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   //camera eye, where to look at, up direction
	gluLookAt(eye.x, eye.y, eye.z,
			  eye.x + l.x, eye.y + l.y, eye.z + l.z,
			  u.x, u.y, u.z);
			  
	glMatrixMode(GL_MODELVIEW);
			  
    drawAxes();



    for(int i=0;i<point_light_count;i++){
		pointLights[i].draw();
	}

	for(int i=0;i<spot_light_count;i++){
		spotLights[i].draw();
	}

	for(int i=0;i<objects.size();i++){
		objects[i]->draw();
	}
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

	glutCreateWindow("Ray Tracing");
	init();

	glEnable(GL_DEPTH_TEST); 

	glutDisplayFunc(display); 
	glutIdleFunc(idle);	  // what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyHandler);
	glutMouseFunc(MouseListener);


	if((atexit(clearObjects) != 0)||(atexit(clearLights) != 0)){
		cout << "Failed to register clearObjects" << endl;
		exit(EXIT_FAILURE);
	}
	
	loadData();
	glutMainLoop();
    return 0;

}