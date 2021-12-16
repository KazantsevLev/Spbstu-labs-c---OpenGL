#undef __STRICT_ANSI__
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <GL/glut.h>

#define surface_grid_step M_PI / 25

int h, w, task = 1;

typedef struct Point3D {
	Point3D() {	}
	Point3D(double _x, double _y, double _z) {
		x = _x, y = _y; z = _z;
	}
	double x, y, z;
};

std::vector<std::vector<Point3D>> tor_vertices;
std::vector<std::vector<Point3D>> cube_vertices;


static void resize(int width, int height)
{
	h = height;
	w = width;

	const float ar = (float)width / (float)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar * 0.7, ar * 0.7, -1.0, 1.0, 2.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void drawString(
	float x,
	float y,
	float z,
	void *font,
	char *string) {

	char *c;
	glColor3f(0, 0, 0);
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void torCreate() {
	double r1 = 1;
	double r2 = 0.5;

	for (double phi = -M_PI; phi <= M_PI; phi += surface_grid_step) {
		std::vector<Point3D> parallel;
		for (double theta = -M_PI; theta <= M_PI; theta += surface_grid_step) {
			Point3D p;
			p.x = (r1 + r2 * std::cos(phi)) * std::cos(theta);
			p.y = (r1 + r2 * std::cos(phi)) * std::sin(theta);
			p.z = r2 * std::sin(phi);
			parallel.push_back(p);
		}
		tor_vertices.push_back(parallel);
	}
}

void cubeCreate() {
	double A = 2;

	std::vector<Point3D> parallel1;
	parallel1.push_back(Point3D(-A / 2, -A / 2, -A / 2));
	parallel1.push_back(Point3D(-A / 2, A / 2, -A / 2));
	parallel1.push_back(Point3D(A / 2, A / 2, -A / 2));
	parallel1.push_back(Point3D(A / 2, -A / 2, -A / 2));
	cube_vertices.push_back(parallel1);

	std::vector<Point3D> parallel2;
	parallel2.push_back(Point3D(-A / 2, -A / 2, A / 2));
	parallel2.push_back(Point3D(-A / 2, A / 2, A / 2));
	parallel2.push_back(Point3D(A / 2, A / 2, A / 2));
	parallel2.push_back(Point3D(A / 2, -A / 2, A / 2));
	cube_vertices.push_back(parallel2);
}

void drawAxes(void) {
	glLineWidth(2);

	glBegin(GL_LINES);

	glColor3f(1, 0, 0); glVertex3f(-1, 0, 0); glVertex3f(5, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, -1, 0); glVertex3f(0, 5, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, -1); glVertex3f(0, 0, 5);
	glEnd();

	glEnd();

	glColor3f(1, 0, 0);
	glPushMatrix();	
	glTranslatef(5, 0, 0);	
	glRotatef(90, 0, 1, 0);
	glutSolidCone(0.1, 0.5, 25, 25);
	glPopMatrix();

	glColor3f(0, 1, 0);
	glPushMatrix();	
	glTranslatef(0, 5, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.5, 25, 25);
	glPopMatrix();

	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslatef(0, 0, 5);
	glRotatef(-90, 0, 0, 1);
	glutSolidCone(0.1, 0.5, 25, 25);
	glPopMatrix();
}

void drawTor() {
	glColor3f(0.5, 0.5, 1);
	glLineWidth(1);
	for (int i = 0; i<tor_vertices.size(); i++) {
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j<tor_vertices[i].size() - 1; j++) {
			glVertex3d(tor_vertices[i][j].x, tor_vertices[i][j].y, tor_vertices[i][j].z);
		}
		glEnd();
	}
	for (int i = 0; i<tor_vertices.size(); i++) {
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j<tor_vertices[i].size() - 1; j++) {
			glVertex3d(tor_vertices[j][i].x, tor_vertices[j][i].y, tor_vertices[j][i].z);
		}
		glEnd();
	}
}

void drawCube() {
	glColor3f(1, 0.5, 0.5);
	glLineWidth(3);

	for (size_t i = 0; i<cube_vertices.size(); i++) {
		glBegin(GL_LINE_LOOP);
		for (size_t j = 0; j<cube_vertices[i].size(); j++) {
			glVertex3d(cube_vertices[i][j].x, cube_vertices[i][j].y, cube_vertices[i][j].z);
		}
		glEnd();
	}

	for (size_t j = 0; j < cube_vertices[0].size(); j++) {
		glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < cube_vertices.size(); i++) {
			glVertex3d(cube_vertices[i][j].x, cube_vertices[i][j].y, cube_vertices[i][j].z);
		}
		glEnd();
	}
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	
	setOrthographicProjection();

	char s[50];
	sprintf_s(s, "Task %d", task);
	drawString(5, 30, 0, GLUT_BITMAP_HELVETICA_18, s);	

	restorePerspectiveProjection();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 0, 1);
	
	drawAxes();

	switch (task)
	{
	case 1:
		drawTor();
		glColor3f(1, 0, 0);
		glutWireSphere(1.5, 25, 25);
		break;
	case 2:
		glPushMatrix();
		glTranslatef(0, 3, 0);
		drawTor();
		glPopMatrix();
		glColor3f(1, 0, 0);
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glutWireSphere(1.5, 25, 25);
		glPopMatrix();

		break;
	case 3:

		drawCube();

		glColor3f(0, 1, 0);
		glLineWidth(1);

		glPushMatrix();
		glTranslatef(0, 0, 1);
		glutWireCone(1, 2, 25, 25);
		glPopMatrix();

		break;
	case 4:
		glPushMatrix();
		glTranslatef(0, 0, 3);
		drawCube();
		glPopMatrix();

		glColor3f(0, 1, 0);
		glLineWidth(1);

		glutWireCone(1, 2, 25, 25);
		break;
	default:
		break;
	}




	//glRotatef(dangleX, 1, 0, 0);
	//glRotatef(dangleY, 0, 1, 0);
	//glRotatef(dangleZ, 0, 0, 1);
	//glutWireDodecahedron();
	//glPopMatrix();


	glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 32:
		task++;
		if (task > 4)
			task = 1;
		break;
	}

	glutPostRedisplay();
}

static void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Компьютерная графика. Лабораторная работа №1");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glClearColor(0.95, 0.95, 0.95, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_COLOR_MATERIAL);

	torCreate();
	cubeCreate();

	glutMainLoop();

	return 0;
}
