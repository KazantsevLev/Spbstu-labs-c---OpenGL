#include <GL/glut.h>

#include "Loader.h"

int current_step = 0;
int morph_steps = 200;

GLfloat light_ambient[] = { 0.9f, 0.9f, 0.9f };
GLfloat light_diffuse[] = { 0.9f, 0.9f, 0.9f };
GLfloat light_specular[] = { 0.7f, 0.7f, 0.7f };
GLfloat light_position[] = { 0.0, 0.0, 15.0 };

const GLfloat mat_ambient[] = { 0.4f, 0.45f, 0.45f };
const GLfloat mat_diffuse[] = { 0.3f, 0.35f, 0.4f };
const GLfloat mat_specular[] = { 0.95f, 0.95f, 0.7f };
const GLfloat mat_shininess[] = { 0.9f * 128 };

objl::Mesh current_mesh;
objl::Mesh mesh1;
objl::Mesh mesh2;

void get_current_mesh() {
	for (int i = 0; i < mesh1.Vertices.size(); i++) {
		current_mesh.Vertices[i].Normal.X = mesh1.Vertices[i].Normal.X +
			(mesh2.Vertices[i].Normal.X - mesh1.Vertices[i].Normal.X) / morph_steps * current_step;
		current_mesh.Vertices[i].Normal.Y = mesh1.Vertices[i].Normal.Y +
			(mesh2.Vertices[i].Normal.Y - mesh1.Vertices[i].Normal.Y) / morph_steps * current_step;
		current_mesh.Vertices[i].Normal.Z = mesh1.Vertices[i].Normal.Z +
			(mesh2.Vertices[i].Normal.Z - mesh1.Vertices[i].Normal.Z) / morph_steps * current_step;

		current_mesh.Vertices[i].Position.X = mesh1.Vertices[i].Position.X +
			(mesh2.Vertices[i].Position.X - mesh1.Vertices[i].Position.X) / morph_steps * current_step;
		current_mesh.Vertices[i].Position.Y = mesh1.Vertices[i].Position.Y +
			(mesh2.Vertices[i].Position.Y - mesh1.Vertices[i].Position.Y) / morph_steps * current_step;
		current_mesh.Vertices[i].Position.Z = mesh1.Vertices[i].Position.Z +
			(mesh2.Vertices[i].Position.Z - mesh1.Vertices[i].Position.Z) / morph_steps * current_step;
	}
}

static void idle(void) {
	glutPostRedisplay();
}

static void resize(int width, int height) {
	const float ar = (float)width / (float)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar * 0.7, ar * 0.7, -1.0, 1.0, 1.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(6, 6, 10, 2, 0, 0, 0, 0, 1);

	if (current_step <= morph_steps) {
		get_current_mesh();
		current_step++;
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	for (int i = 0; i < mesh1.Indices.size(); i += 3) {
		glBegin(GL_TRIANGLES);

		glNormal3d(current_mesh.Vertices[current_mesh.Indices[i]].Normal.X, current_mesh.Vertices[current_mesh.Indices[i]].Normal.Y, current_mesh.Vertices[current_mesh.Indices[i]].Normal.Z);
		glVertex3d(current_mesh.Vertices[current_mesh.Indices[i]].Position.X, current_mesh.Vertices[current_mesh.Indices[i]].Position.Y, current_mesh.Vertices[current_mesh.Indices[i]].Position.Z);

		glNormal3d(current_mesh.Vertices[current_mesh.Indices[i + 1]].Normal.X, current_mesh.Vertices[current_mesh.Indices[i + 1]].Normal.Y, current_mesh.Vertices[current_mesh.Indices[i + 1]].Normal.Z);
		glVertex3d(current_mesh.Vertices[current_mesh.Indices[i + 1]].Position.X, current_mesh.Vertices[current_mesh.Indices[i + 1]].Position.Y, current_mesh.Vertices[current_mesh.Indices[i + 1]].Position.Z);

		glNormal3d(current_mesh.Vertices[current_mesh.Indices[i + 2]].Normal.X, current_mesh.Vertices[current_mesh.Indices[i + 2]].Normal.Y, current_mesh.Vertices[current_mesh.Indices[i + 2]].Normal.Z);
		glVertex3d(current_mesh.Vertices[current_mesh.Indices[i + 2]].Position.X, current_mesh.Vertices[current_mesh.Indices[i + 2]].Position.Y, current_mesh.Vertices[current_mesh.Indices[i + 2]].Position.Z);
		glEnd();
	}

	glutSwapBuffers();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);

	glutCreateWindow("Лабораторная работа №3");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glClearColor(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	objl::Loader Loader;

	bool loadout = Loader.LoadFile("objects.obj");

	if (loadout) {
		current_mesh = Loader.LoadedMeshes[1];
		mesh1 = Loader.LoadedMeshes[0];
		mesh2 = Loader.LoadedMeshes[1];
	}

	glutMainLoop();

	return EXIT_SUCCESS;
}
