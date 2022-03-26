#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include <gl/glut.h>

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

struct complex {
    double real;
    double img;
};

double complex_absolute(complex x) {
    return sqrt(x.real * x.real + x.img * x.img);
}

complex multiply(complex x, complex y) {
    complex result;
    result.real = (x.real * y.real - x.img * y.img);
    result.img = (x.real * y.img + y.real * x.img);

    return result;
}

int inJc(complex& x, int iter_no, int abs_max, complex c)
{
    complex z0, z1;

    z0 = x;
    for (int i = 1; i < iter_no; i++)
    {
        z1.real = multiply(z0, z0).real + c.real;
        z1.img = multiply(z0, z0).img + c.img;

        if (z1.real == z0.real && z1.img == z0.img || complex_absolute(z1) > abs_max)
        {
            return -1;
        }
        z0 = z1;
    }

    return 0;
}

//Julia-Fatou 1
void Display1() {
    complex c = { -0.12375, 0.056805 };
    double ratio = 0.01;
    int iteration_number = 30;
    int max_abs = 10000000;
    double xmin = -0.8;
    double xmax = 0.8;
    double ymin = -0.4;
    double ymax = 0.4;

    glColor3f(1.0, 0.1, 0.1);

    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_POINTS);
    for (double x = xmin; x <= xmax; x += ratio)
        for (double y = ymin; y <= ymax; y += ratio) {
            complex z = { x, y };
            int res = inJc(z, iteration_number, max_abs, c);
            if (res == 0) {
                glVertex3d(x, y, 0);
            }
        }
    glEnd();

    glPopMatrix();
}

//Julia-Fatou 2
void Display2() {
    complex c = { -0.012, 0.74 };
    double ratio = 0.01;
    int iteration_number = 30;
    int max_abs = 10000000;
    double xmin = -1;
    double xmax = 1;
    double ymin = -1;
    double ymax = 1;

    glColor3f(1.0, 0.1, 0.1);

    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_POINTS);
    for (double x = xmin; x <= xmax; x += ratio)
        for (double y = ymin; y <= ymax; y += ratio) {
            complex z = { x, y };
            int res = inJc(z, iteration_number, max_abs, c);
            if (res == 0) {
                glVertex3d(x, y, 0);
            }
        }
    glEnd();

    glPopMatrix();
}

int inMn(complex& x, int iter_no, int abs_max)
{
    complex z0, z1;

    z0 = { 0, 0 };
    for (int i = 1; i < iter_no; i++)
    {
        z1.real = multiply(z0, z0).real + x.real;
        z1.img = multiply(z0, z0).img + x.img;

        if (z1.real == z0.real && z1.img == z0.img)
        {
            return 0;
        }
        else if (complex_absolute(z1) > abs_max) {
            return i;
        }
        z0 = z1;
    }

    return 0;
}

//Mandelbrot
void Display3() {
    double ratio = 0.005; //sau 0.005
    int iteration_number = 10;
    int max_abs = 2;
    double xmin = -2;
    double xmax = 2;
    double ymin = -2;
    double ymax = 2;

    glColor3f(1.0, 0.1, 0.1);
    glPushMatrix();
    glLoadIdentity();

    glTranslated((xmin + xmax) * fmax(fabs(xmax), fabs(xmin)) / (xmin - xmax),
        (ymin + ymax) * fmax(fabs(ymax), fabs(ymin)) / (ymin - ymax), 0);
    glScaled(fmax(fabs(xmax), fabs(xmin)) / (xmax - xmin),
        fmax(fabs(ymax), fabs(ymin)) / (ymax - ymin), 1);
    glBegin(GL_POINTS);
    for (double x = xmin; x <= xmax; x += ratio)
        for (double y = ymin; y <= ymax; y += ratio) {
            complex z = { x, y };
            int res = inMn(z, iteration_number, max_abs);
            if (res == 0) {
                glColor3f(1.0, 0.1, 0.1),
                glVertex3d(x, y, 0);
            }
            else {
                glColor3f(0, 1.0 / res, 0);
                glVertex3d(x, y, 0);
            }
        }
    glEnd();
    glPopMatrix();
}

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    switch (prevKey) {
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        Display1();
        break;
    case '2':
        glClear(GL_COLOR_BUFFER_BIT);
        Display2();
        break;
    case '3':
        glClear(GL_COLOR_BUFFER_BIT);
        Display3();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}

