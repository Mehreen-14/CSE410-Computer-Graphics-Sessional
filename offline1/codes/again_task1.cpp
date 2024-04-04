#include <GL/glut.h>
#include <cmath>

bool simulationMode = false;
float ballRadius = 5.0f;
float ballSpeed = 1.0f;
float ballDirection = 0.0f; // in degrees
float rollingSpeed = 5.0f;

void drawCheckerboard() {
    const int size = 20; // size of the checkerboard
    const float step = 2.0f;

    for (float i = -size / 2.0f; i < size / 2.0f; i++) {
        for (float j = -size / 2.0f; j < size / 2.0f; j++) {
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

void drawBall() {
    const int sectors = 36; // number of sectors
    const int stacks = 18;  // number of stacks
    const float sectorStep = 360.0f / sectors;
    const float stackStep = 180.0f / stacks;

    glColor3f(1.0f, 0.0f, 0.0f); // Initial color

    for (int i = 0; i < stacks; ++i) {
        float stack1 = -90.0f + i * stackStep;
        float stack2 = -90.0f + (i + 1) * stackStep;

        for (int j = 0; j < sectors; ++j) {
            float sector1 = j * sectorStep;
            float sector2 = (j + 1) * sectorStep;

            glBegin(GL_QUADS);
            {
                glVertex3f(ballRadius * cos(stack1 * (M_PI / 180.0f)) * cos(sector1 * (M_PI / 180.0f)),
                           ballRadius * cos(stack1 * (M_PI / 180.0f)) * sin(sector1 * (M_PI / 180.0f)),
                           ballRadius * sin(stack1 * (M_PI / 180.0f)));

                glVertex3f(ballRadius * cos(stack1 * (M_PI / 180.0f)) * cos(sector2 * (M_PI / 180.0f)),
                           ballRadius * cos(stack1 * (M_PI / 180.0f)) * sin(sector2 * (M_PI / 180.0f)),
                           ballRadius * sin(stack1 * (M_PI / 180.0f)));

                glVertex3f(ballRadius * cos(stack2 * (M_PI / 180.0f)) * cos(sector2 * (M_PI / 180.0f)),
                           ballRadius * cos(stack2 * (M_PI / 180.0f)) * sin(sector2 * (M_PI / 180.0f)),
                           ballRadius * sin(stack2 * (M_PI / 180.0f)));

                glVertex3f(ballRadius * cos(stack2 * (M_PI / 180.0f)) * cos(sector1 * (M_PI / 180.0f)),
                           ballRadius * cos(stack2 * (M_PI / 180.0f)) * sin(sector1 * (M_PI / 180.0f)),
                           ballRadius * sin(stack2 * (M_PI / 180.0f)));
            }
            glEnd();

            // Switch color for alternating sectors
            if (i % 2 == 0)
                glColor3f(0.0f, 0.0f, 1.0f);
            else
                glColor3f(1.0f, 1.0f, 0.0f);
        }
    }
}

void drawArrow() {
    glColor3f(0.0f, 1.0f, 0.0f); // Green color for arrow

    glBegin(GL_LINES);
    {
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(ballRadius * cos(ballDirection * (M_PI / 180.0f)),
                   ballRadius * sin(ballDirection * (M_PI / 180.0f)),
                   0.0f);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (simulationMode) {
        ballDirection += rollingSpeed;
        if (ballDirection >= 360.0f)
            ballDirection -= 360.0f;
    }

    // Camera position, where to look at, up direction
    gluLookAt(20.0, 20.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    drawCheckerboard();
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.0f, ballRadius);
        glRotatef(ballDirection, 0.0f, 0.0f, 1.0f);
        drawBall();
        drawArrow();
    }
    glPopMatrix();

    glutSwapBuffers();
}

void keyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
        case ' ': // Space key to toggle between manual control and simulation modes
            simulationMode = !simulationMode;
            break;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("OpenGL Ball Simulation");
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(display); // Register idle function to continuously redraw
    glutKeyboardFunc(keyboardHandler);

    glutMainLoop();
    return 0;
}
