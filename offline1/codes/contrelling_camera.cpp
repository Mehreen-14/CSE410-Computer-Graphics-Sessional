#include <GL/glut.h>
#include <cmath>

float ballRadius = 5.0f;
float ballSpeed = 0.1f;
float ballDirection = 45.0f; // in degrees

void drawBall() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the ball

    const int sectors = 36; // number of sectors
    const float sectorStep = 360.0f / sectors;

    glBegin(GL_TRIANGLE_FAN);
    {
        glVertex3f(0.0f, 0.0f, 0.0f); // Center of the ball

        for (int i = 0; i <= sectors; ++i) {
            float angle = i * sectorStep;
            float x = ballRadius * cos(angle * (M_PI / 180.0f));
            float y = ballRadius * sin(angle * (M_PI / 180.0f));
            glVertex3f(x, y, 0.0f);
        }
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move the ball based on its direction and speed
    float deltaX = ballSpeed * cos(ballDirection * (M_PI / 180.0f));
    float deltaY = ballSpeed * sin(ballDirection * (M_PI / 180.0f));
    glTranslatef(deltaX, deltaY, 0.0f);

    drawBall();

    glutSwapBuffers();
}

void keyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            // Move the ball up
            ballDirection = 90.0f;
            break;
        case 's':
            // Move the ball down
            ballDirection = 270.0f;
            break;
        case 'a':
            // Move the ball left
            ballDirection = 180.0f;
            break;
        case 'd':
            // Move the ball right
            ballDirection = 0.0f;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-50, 50, -50, 50); // Set orthographic projection
    glMatrixMode(GL_MODELVIEW);
}

void idle(){

    //counter++;
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Ball Movement on XY Plane");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutIdleFunc(idle);	  // what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardHandler);

    glutMainLoop();
    return 0;
}
