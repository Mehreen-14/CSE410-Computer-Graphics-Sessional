#include <GL/glut.h>
#include <cmath>
#include <vector>

struct Ball {
    float x, y; // Position
    float speed; // Movement speed
    float direction; // Movement direction in degrees

    Ball(float startX, float startY, float startSpeed, float startDirection)
        : x(startX), y(startY), speed(startSpeed), direction(startDirection) {}
};

std::vector<Ball> balls;

void init() {
    // Initialize balls with initial positions, directions, and speeds
    balls.emplace_back(0.0f, 0.0f, 5.0f, 45.0f);
    balls.emplace_back(-20.0f, 10.0f, 8.0f, 120.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-50, 50, -50, 50);
}

void drawBall(float x, float y) {
    const int numSegments = 100;
    const float radius = 2.0f;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the balls
    glVertex2f(x, y);
    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0f * M_PI * i / numSegments;
        float dx = radius * cos(angle);
        float dy = radius * sin(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the balls
    for (const auto& ball : balls) {
        drawBall(ball.x, ball.y);
    }

    glutSwapBuffers();
}

void updateBalls() {
    // Update the positions of the balls based on their direction and speed
    for (auto& ball : balls) {
        float radians = ball.direction * M_PI / 180.0f;
        float dx = ball.speed * cos(radians);
        float dy = ball.speed * sin(radians);
        ball.x += dx;
        ball.y += dy;
    }
}

void idle() {
    updateBalls();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Multiple Moving Balls");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
