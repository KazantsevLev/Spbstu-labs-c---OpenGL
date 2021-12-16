#include "Cube.h"

const GLfloat cube_ambient[] = { 0.9f, 0.9f, 0.9f };
const GLfloat cube_diffuse[] = { 0.9f, 0.9f, 0.9f };
const GLfloat cube_specular[] = { 0.1f, 0.1f, 0.1f };
const GLfloat cube_shininess[] = { 0.1f * 128.0 };

Cube::Cube(double a) {
	A = a;

	std::vector<Point3D> parallel1;
	parallel1.push_back(Point3D(-A / 2, -A / 2, -A / 2));
	parallel1.push_back(Point3D(-A / 2, A / 2, -A / 2));
	parallel1.push_back(Point3D(A / 2, A / 2, -A / 2));
	parallel1.push_back(Point3D(A / 2, -A / 2, -A / 2));
	surface.push_back(parallel1);

	std::vector<Point3D> parallel2;
	parallel2.push_back(Point3D(-A / 2, -A / 2, A / 2));
	parallel2.push_back(Point3D(-A / 2, A / 2, A / 2));
	parallel2.push_back(Point3D(A / 2, A / 2, A / 2));
	parallel2.push_back(Point3D(A / 2, -A / 2, A / 2));
	surface.push_back(parallel2);

	Move(Point3D(0, 0, -A / 2));
}


Cube::~Cube() {
}

void Cube::Draw() {

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cube_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cube_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cube_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, cube_shininess);

	glBegin(GL_POLYGON);

	GLfloat Qx, Qy, Qz, Px, Py, Pz;
	Qx = surface[0][0].x - surface[0][1].x;
	Qy = surface[0][0].y - surface[0][1].y;
	Qz = surface[0][0].z - surface[0][1].z;
	Px = surface[0][2].x - surface[0][1].x;
	Py = surface[0][2].y - surface[0][1].y;
	Pz = surface[0][2].z - surface[0][1].z;

	GLfloat fNormalX = Py * Qz - Pz * Qy;
	GLfloat fNormalY = Pz * Qx - Px * Qz;
	GLfloat fNormalZ = Px * Qy - Py * Qx;

	glNormal3d(fNormalX, fNormalY, fNormalZ);

	glTexCoord2d(0, 0);
	glVertex3d(surface[0][0].x, surface[0][0].y, surface[0][0].z);

	glTexCoord2d(1, 0);
	glVertex3d(surface[0][1].x, surface[0][1].y, surface[0][1].z);

	glTexCoord2d(1, 1);
	glVertex3d(surface[0][2].x, surface[0][2].y, surface[0][2].z);

	glTexCoord2d(0, 1);
	glVertex3d(surface[0][3].x, surface[0][3].y, surface[0][3].z);

	glEnd();

	glBegin(GL_POLYGON);

	Qx = surface[1][3].x - surface[1][2].x;
	Qy = surface[1][3].y - surface[1][2].y;
	Qz = surface[1][3].z - surface[1][2].z;
	Px = surface[1][1].x - surface[1][2].x;
	Py = surface[1][1].y - surface[1][2].y;
	Pz = surface[1][1].z - surface[1][2].z;

	fNormalX = Py * Qz - Pz * Qy;
	fNormalY = Pz * Qx - Px * Qz;
	fNormalZ = Px * Qy - Py * Qx;

	glNormal3d(fNormalX, fNormalY, fNormalZ);

	glTexCoord2d(0, 0);
	glVertex3d(surface[1][3].x, surface[1][3].y, surface[1][3].z);

	glTexCoord2d(1, 0);
	glVertex3d(surface[1][2].x, surface[1][2].y, surface[1][2].z);

	glTexCoord2d(1, 1);
	glVertex3d(surface[1][1].x, surface[1][1].y, surface[1][1].z);

	glTexCoord2d(0, 1);
	glVertex3d(surface[1][0].x, surface[1][0].y, surface[1][0].z);

	glEnd();

	for (int i = 0; i<surface.size(); i++) {
		for (int j = 0; j<surface[i].size(); j++) {

			int k = i + 1 < surface.size() ? i + 1 : 0;
			int m = j + 1 < surface[i].size() ? j + 1 : 0;

			glBegin(GL_POLYGON);

			Qx = surface[i][j].x - surface[k][j].x;
			Qy = surface[i][j].y - surface[k][j].y;
			Qz = surface[i][j].z - surface[k][j].z;
			Px = surface[i][m].x - surface[k][j].x;
			Py = surface[i][m].y - surface[k][j].y;
			Pz = surface[i][m].z - surface[k][j].z;

			fNormalX = Py * Qz - Pz * Qy;
			fNormalY = Pz * Qx - Px * Qz;
			fNormalZ = Px * Qy - Py * Qx;

			glNormal3d(fNormalX, fNormalY, fNormalZ);

			glTexCoord2d(0, 0);
			glVertex3d(surface[i][j].x, surface[i][j].y, surface[i][j].z);

			glTexCoord2d(1, 0);
			glVertex3d(surface[k][j].x, surface[k][j].y, surface[k][j].z);

			glTexCoord2d(1, 1);
			glVertex3d(surface[k][m].x, surface[k][m].y, surface[k][m].z);

			glTexCoord2d(0, 1);
			glVertex3d(surface[i][m].x, surface[i][m].y, surface[i][m].z);

			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void Cube::Move(Point3D delta) {
	for (int i = 0; i < surface.size(); i++) {
		for (int j = 0; j < surface[i].size(); j++) {
			surface[i][j].x += delta.x;
			surface[i][j].y += delta.y;
			surface[i][j].z += delta.z;
		}
	}
}