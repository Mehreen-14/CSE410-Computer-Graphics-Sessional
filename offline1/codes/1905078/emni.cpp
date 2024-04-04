#include <GL/glut.h>

GLfloat size = 1.0f; // Size of the square

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 0.1f, 100.0f); // Perspective projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0); // Camera position
}

void drawCube() {
    glPushMatrix();
    // Draw the front face of the cube (square)
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);
    glEnd();

    // Translate and rotate to draw the remaining faces
    glTranslatef(0.0f, 0.0f, -2.0f * size); // Translate to the back face
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glBegin(GL_QUADS); // Draw the back face
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);
    glEnd();

    // Rotate and draw the remaining faces
    for (int i = 0; i < 4; ++i) {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate around the y-axis
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glBegin(GL_QUADS); // Draw the remaining faces
        glVertex3f(-size, -size, size);
        glVertex3f(size, -size, size);
        glVertex3f(size, size, size);
        glVertex3f(-size, size, size);
        glEnd();
    }

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glLoadIdentity();
    drawCube();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("OpenGL Cube");

    init();
    glEnable(GL_DEPTH_TEST); // Enable depth testing for proper 3D rendering

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
