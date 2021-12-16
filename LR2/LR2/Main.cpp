#undef __STRICT_ANSI__
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include <GL/glut.h>

#include <sstream>
#include <string>

#define surface_grid_step M_PI / 35

typedef struct Point3D {
	Point3D() {	}
	Point3D(double _x, double _y, double _z) {
		x = _x, y = _y; z = _z;
	}
	double x, y, z;
};

std::vector<std::vector<Point3D>> tor_vertices;

GLuint  texture1;
struct texture {
	int W;
	int H;
	unsigned char *Image;
}get_texture;

bool r = true, g = true, b = true;

GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 0.3f };
GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 0.3f };
GLfloat light_position[] = { 0.0, 0.0, 15.0 };

const GLfloat cone_ambient[] = { 0.0215,	0.1745, 	0.0215 };
const GLfloat cone_diffuse[] = { 0.07568, 	0.61424, 	0.07568, 0.55 };
const GLfloat cone_specular[] = { 0.633, 	0.727811, 	0.633 };
const GLfloat cone_shininess[] = { 0.6f * 128.0 };

const GLfloat sphere_ambient[] = { 0.25, 	0.20725, 	0.20725 };
const GLfloat sphere_diffuse[] = { 0.829, 	0.829, 	0.829 };
const GLfloat sphere_specular[] = { 0.628281, 	0.555802, 	0.366065 };
const GLfloat sphere_shininess[] = { 0.9f * 128.0 };

const GLfloat tor_ambient[] = { 0.75, 	0.75, 	0.75 };
const GLfloat tor_diffuse[] = { 0.75, 	0.75, 	0.75 };
const GLfloat tor_specular[] = { 0.75, 	0.75, 	0.75 };
const GLfloat tor_shininess[] = { 0.1f * 128.0 };

double angle = 0;

int LoadTexture(char *FileName)
{
	FILE *F;
	if ((F = fopen(FileName, "rb")) == NULL)
		return 0;
	fseek(F, 18, SEEK_SET);
	fread(&(get_texture.W), 2, 1, F);
	fseek(F, 2, SEEK_CUR);
	fread(&(get_texture.H), 2, 1, F);

	printf("%d x %d\n", get_texture.W, get_texture.H);

	if ((get_texture.Image = (unsigned char *)malloc(sizeof(unsigned char) * 3 * get_texture.W * get_texture.H)) == NULL)
	{
		fclose(F);
		return 0;
	}
	fseek(F, 30, SEEK_CUR);
	fread(get_texture.Image, 3, get_texture.W * get_texture.H, F);

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, get_texture.W, get_texture.H, 0, GL_RGB, GL_UNSIGNED_BYTE, get_texture.Image);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, get_texture.W, get_texture.H, GL_RGB, GL_UNSIGNED_BYTE, get_texture.Image);
	free(get_texture.Image);
	fclose(F);

	return 1;
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_UP:
		light_position[1] -= 0.1;
		break;
	case GLUT_KEY_DOWN:
		light_position[1] += 0.1;
		break;
	case GLUT_KEY_LEFT:
		light_position[0] += 0.1;
		break;
	case GLUT_KEY_RIGHT:
		light_position[0] -= 0.1;
		break;
	}
	glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		r = !r;
		light_ambient[0] = 0.5 * r;
		break;
	case 'g':
		g = !g;
		light_ambient[1] = 0.5 * g;
		break;
	case 'b':
		b = !b;
		light_ambient[2] = 0.5 * b;
		break;
	case 45:
		for (int i = 0; i < 3; i++)
			light_ambient[i] -= 0.05;
		break;
	case 61:
		for (int i = 0; i < 3; i++)
			light_ambient[i] += 0.05;
		break;
	}

	glutPostRedisplay();
}

void idle(void)
{
	glutPostRedisplay();
}

void resize(int width, int height)
{
	const float ar = (float)width / (float)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar * 0.7, ar * 0.7, -1.0, 1.0, 2.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void torCreate() {
	double r1 = 1.5;
	double r2 = 1;

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

void drawAxes(void) {
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

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

	glDisable(GL_COLOR_MATERIAL);
}

void drawTor() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tor_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tor_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tor_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, tor_shininess);

	for (int i = 0; i<tor_vertices.size(); i++) {
		for (int j = 0; j<tor_vertices[i].size(); j++) {

			int k = i + 1 < tor_vertices.size() ? i + 1 : 0;
			int m = j + 1 < tor_vertices[i].size() ? j + 1 : 0;

			glBegin(GL_QUADS);

			GLfloat Qx, Qy, Qz, Px, Py, Pz;
			Qx = tor_vertices[i][j].x - tor_vertices[k][j].x;
			Qy = tor_vertices[i][j].y - tor_vertices[k][j].y;
			Qz = tor_vertices[i][j].z - tor_vertices[k][j].z;
			Px = tor_vertices[k][m].x - tor_vertices[k][j].x;
			Py = tor_vertices[k][m].y - tor_vertices[k][j].y;
			Pz = tor_vertices[k][m].z - tor_vertices[k][j].z;

			GLfloat fNormalX = Py * Qz - Pz * Qy;
			GLfloat fNormalY = Pz * Qx - Px * Qz;
			GLfloat fNormalZ = Px * Qy - Py * Qx;

			glNormal3d(fNormalX, fNormalY, fNormalZ);


			glTexCoord2d(i * (1.0 / tor_vertices.size()), j * (1.0 / tor_vertices[i].size()));
			glVertex3d(tor_vertices[i][j].x, tor_vertices[i][j].y, tor_vertices[i][j].z);

			glTexCoord2d(k * (1.0 / tor_vertices.size()), j * (1.0 / tor_vertices[i].size()));
			glVertex3d(tor_vertices[k][j].x, tor_vertices[k][j].y, tor_vertices[k][j].z);

			glTexCoord2d(k * (1.0 / tor_vertices.size()), m * (1.0 / tor_vertices[i].size()));
			glVertex3d(tor_vertices[k][m].x, tor_vertices[k][m].y, tor_vertices[k][m].z);


			glTexCoord2d(i * (1.0 / tor_vertices.size()), m * (1.0 / tor_vertices[i].size()));
			glVertex3d(tor_vertices[i][m].x, tor_vertices[i][m].y, tor_vertices[i][m].z);

			glEnd();
		}
	}

	glDisable(GL_TEXTURE_2D);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLoadIdentity();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 0, 1);


	drawAxes();
	
	glPushMatrix();
	angle--;
	angle = angle > 0 ? angle : 360;
	glTranslatef(2, -3, 0);
	glRotatef(angle, 0, 0, 1);
	drawTor();
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sphere_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sphere_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sphere_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, sphere_shininess);

	glPushMatrix();
	glTranslatef(-2, 2, 0);
	glutSolidSphere(2, 25, 25);
	glPopMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cone_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cone_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cone_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, cone_shininess);
	glPushMatrix();
	glTranslatef(2, 2, 0);
	glutSolidCone(2, 5, 25, 25);
	glPopMatrix();
	glDisable(GL_BLEND);

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);

	glutCreateWindow("Компьютерная графика. Лабораторная работа №2");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutSpecialFunc(pressKey);
	glutIdleFunc(idle);

	glClearColor(0.95, 0.95, 0.95, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	
	torCreate();
	LoadTexture((char *)"texture.bmp");
	
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glShadeModel(GL_SMOOTH);


	glutMainLoop();

	return EXIT_SUCCESS;
}
