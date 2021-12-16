#include <GL/glut.h>

#include <sstream>
#include <string>

#include "Octahedron.h"
#include "Cube.h"

GLuint  texture1, texture2, texture3;

Octahedron octahedron;
Cube cube(10);

struct texture {
	int W;
	int H;
	unsigned char *Image;
}get_texture;

GLfloat global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

GLfloat light_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 0.3f };
GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 0.3f };
GLfloat light_position[] = { 0.0, 0.0, 15.0 };

const GLfloat mat_ambient[] = { 0.3, 	0.3, 	0.3 };
const GLfloat mat_diffuse[] = { 0.2, 	0.2, 	0.2 };
const GLfloat mat_specular[] = { 0.1,	0.1, 	0.1 };
const GLfloat high_shininess[] = { 0.1f * 128 };


int LoadTexture(char *FileName, GLuint  *texture_id) {
	FILE *F;
	if ((F = fopen(FileName, "rb")) == NULL)
		return 0;
	fseek(F, 18, SEEK_SET);
	fread(&(get_texture.W), 2, 1, F);
	fseek(F, 2, SEEK_CUR);
	fread(&(get_texture.H), 2, 1, F);

	if ((get_texture.Image = (unsigned char *)malloc(sizeof(unsigned char) * 3 * get_texture.W * get_texture.H)) == NULL) {
		fclose(F);
		return 0;
	}
	fseek(F, 30, SEEK_CUR);
	fread(get_texture.Image, 3, get_texture.W * get_texture.H, F);

	glGenTextures(1, texture_id);
	glBindTexture(GL_TEXTURE_2D, *texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, get_texture.W, get_texture.H, 0, GL_RGB, GL_UNSIGNED_BYTE, get_texture.Image);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, get_texture.W, get_texture.H, GL_RGB, GL_UNSIGNED_BYTE, get_texture.Image);
	free(get_texture.Image);
	fclose(F);

	return 1;
}

static void idle(void) {
	glutPostRedisplay();
}

static void resize(int width, int height) {
	const float ar = (float)width / (float)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar * 0.7, ar * 0.7, -1.0, 1.0, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void drawPlane(void) {
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);


	for (int i = -30; i<=30; i+=5) {
		for (int j = -30; j<=30; j+=5) {

			glBegin(GL_POLYGON);

			glNormal3d(i, j, -10);
			glTexCoord2d(0, 0);
			glVertex3d(i, j, -10);

			glNormal3d(i + 5, j, -10);
			glTexCoord2d(1, 0);
			glVertex3d(i + 5, j, -10);

			glNormal3d(i + 5, j + 5, -10);
			glTexCoord2d(1, 1);
			glVertex3d(i + 5, j + 5, -10);

			glNormal3d(i, j + 5, -10);
			glTexCoord2d(0, 1);
			glVertex3d(i, j + 5, -10);

			glEnd();
		}
	}

	glEnd();

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glDisable(GL_TEXTURE_2D);
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLoadIdentity();

	gluLookAt(8, 12, 7, 0, 0, 0, 0, 0, 1);

	glBindTexture(GL_TEXTURE_2D, texture1);
	drawPlane();

	glBindTexture(GL_TEXTURE_2D, texture2);
	cube.Draw();

	glBindTexture(GL_TEXTURE_2D, texture3);
	octahedron.Draw();

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);

	glutCreateWindow("Курсовой проект. Вариант 32");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glClearColor(0.95, 0.95, 0.95, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	LoadTexture((char *)"texture1.bmp", &texture1);
	LoadTexture((char *)"texture2.bmp", &texture2);
	LoadTexture((char *)"texture3.bmp", &texture3);

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
