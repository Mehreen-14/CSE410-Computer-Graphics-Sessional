#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <bits/stdc++.h>

using namespace std;


const float PI = 3.14159265358979323846;
const int numPoints = 100;
const float circleRadius = 0.5f;
float lineAngle = 0.0f;
float lineAngle2 = 0.0f;
const float lineLength = 0.6f;
float endX, endY,end_X,end_Y;


int rotAngle = 0, rotInc = 5;
double pointx = 2, pointy = 0;
vector<pair<double, double>> points;



const float redcircleRadius = 0.2f;
const float yellowcircleRadius = 0.05f;


void circle() {
    glColor3f(0.0, 0.0, 1.0);

    // Draw Circle
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numPoints; ++i) {
        float angle = 2 * PI * i / numPoints;
        float x = circleRadius * cos(angle);
        float y = circleRadius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void rotatingLine() {
    // Draw rotating line
    glColor3f(0.0, 0.0, 1.0);

    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f); // Start from center
    endX = circleRadius * cos(lineAngle);
    endY = circleRadius * sin(lineAngle);
    glVertex2f(endX, endY);
    glEnd();
}

void redCircle() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numPoints; ++i) {
        float angle = 2 * PI * i / numPoints;
        float x = endX + redcircleRadius * cos(angle); 
        float y = endY + redcircleRadius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void rotatingLine2() {
    // Draw rotating line
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_LINES);
    glVertex2f(endX, endY); // Start from center
    end_X = endX+redcircleRadius * cos(lineAngle2);
    end_Y = endY+redcircleRadius * sin(lineAngle2);
    glVertex2f(end_X, end_Y);
    glEnd();
}

void yellowCircle() {
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numPoints; ++i) {
        float angle = 2 * PI * i / numPoints;
        float x = end_X + yellowcircleRadius * cos(angle); 
        float y = end_Y + yellowcircleRadius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}




void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    circle();
    rotatingLine();
    redCircle();
    rotatingLine2();
    yellowCircle();
    
    glEnd();
    

    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

void update(int value) {
    lineAngle += 0.01f;
    lineAngle2 += 0.035f; // Increment angle for rotation
    if (lineAngle > 2 * PI)
        lineAngle -= 2 * PI;
    if (lineAngle2 > 2 * PI)
        lineAngle2 -= 2 * PI;    
    glutPostRedisplay(); // Redraw the scene
    glutTimerFunc(10, update, 0); // Call update function every 10 milliseconds 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(900, 900);
    glutCreateWindow("Sine Waves");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0); // Start the update loop
    glutMainLoop();
    return 0;
}
