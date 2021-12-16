#include "Octahedron.h"

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 0.1f };
const GLfloat mat_diffuse[] = { 0.5f, 0.5f, 0.5f };
const GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f };
const GLfloat high_shininess[] = { 0.9f * 128 };

Octahedron::Octahedron() {
	Rotate(M_PI_4, Z);
	Point3D p = points[getBottomPoint()];
	Move(Point3D(0, -0.75, -p.z));
}

Octahedron::~Octahedron() {
}

void Octahedron::Draw() {
	falling();
	rolling();

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);

	for (int i = 0; i < 8; i++) {
		glBegin(GL_POLYGON);

		GLfloat Qx, Qy, Qz, Px, Py, Pz;
		Qx = points[faces[i][0]].x - points[faces[i][1]].x;
		Qy = points[faces[i][0]].y - points[faces[i][1]].y;
		Qz = points[faces[i][0]].z - points[faces[i][1]].z;
		Px = points[faces[i][2]].x - points[faces[i][1]].x;
		Py = points[faces[i][2]].y - points[faces[i][1]].y;
		Pz = points[faces[i][2]].z - points[faces[i][1]].z;

		GLfloat fNormalX = Py * Qz - Pz * Qy;
		GLfloat fNormalY = Pz * Qx - Px * Qz;
		GLfloat fNormalZ = Px * Qy - Py * Qx;

		glNormal3d(fNormalX, fNormalY, fNormalZ);

		glTexCoord2d(0.5f, 0.0f);
		glVertex3d(points[faces[i][0]].x, points[faces[i][0]].y, points[faces[i][0]].z);

		glTexCoord2d(0.0f, 1.0f);
		glVertex3d(points[faces[i][1]].x, points[faces[i][1]].y, points[faces[i][1]].z);

		glTexCoord2d(1.0f, 1.0f);
		glVertex3d(points[faces[i][2]].x, points[faces[i][2]].y, points[faces[i][2]].z);

		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

}

int Octahedron::getBottomPoint() {
	double minZ = points[0].z;
	int bottom_point_idx = 0;
	for (int i = 1; i < points.size(); i++)
		if (points[i].z < minZ) {
			bottom_point_idx = i;
			minZ = points[i].z;
		}
	return bottom_point_idx;
}

void Octahedron::rolling() {
	if (!isRolling || currentState == START_FALLING || currentState == FALLING)
		return;


	if (currentState == ON_FLOOR) {
		double a = 10;
	}

	Point3D saved = Point3D(rotation_point);
	double z = location.z;
	Move(rotation_point.inverse());
	if (currentState == ON_CUBE) {
		Rotate(-0.0025, X);	
		double k = (z - points[6].z) * 0.9;
		Rotate(-k, X);
	}
	else {
		Rotate(-0.0055, X);
	}
	Move(saved);

	int new_rotation_point = getBottomPoint();
	if (rotation_point == points[new_rotation_point])
		return;

	if (currentState == ON_CUBE && location.y > 5) {
		isRolling = false;
		isFalling = true;
		currentState = START_FALLING;

		PointF p1(rotation_point.x, rotation_point.y), p2(points[new_rotation_point].x, points[new_rotation_point].y),
			p3(-5, 5), p4(5, 5), cross_point;

		bool isCrossed = CrossLines(&p1, &p2, &p3, &p4, &cross_point);
		if (!isCrossed)
			return;

		rotation_point = Point3D(cross_point.X, cross_point.Y, 0);
		return;
	}

	if (currentState == ON_FLOOR && location.y > 9) {
		isRolling = false;
	}

	rotation_point = points[new_rotation_point];
	if (currentState == ON_CUBE)
		Move(Point3D(0, 0, -rotation_point.z));
	else if (currentState == ON_FLOOR) {
		Move(Point3D(0, 0, -(10 + rotation_point.z)));
	}
}

void Octahedron::falling() {
	if (!isFalling)
		return;
	if (currentState == START_FALLING) {

		Point3D saved = Point3D(rotation_point);
		Move(rotation_point.inverse());
		Rotate(-0.0045f, X);
		Move(saved);

		bool isOver = true;
		for (int i = 1; i < points.size(); i++)
			if (points[i].y < 4.99) {
				isOver = false;
				break;
			}

		if (isOver)
			currentState = FALLING;
	}
	else if (currentState == FALLING) {
		Move(Point3D(0, 0, -0.05f));

		int bottom_point = getBottomPoint();
		if (points[bottom_point].z < -10) {
			rotation_point = Point3D(points[bottom_point]);
			isRolling = true;
			isFalling = false;
			currentState = ON_FLOOR;
		}
	}
}

void Octahedron::Move(Point3D offset) {
	for (int i = 0; i<7; i++) {
		points[i] += offset;
	}
	location = points[6];
}

void Octahedron::Rotate(float angle, Axis axis) {
	float newX, newY, newZ;
	float matrix[4][4];
	CreateRotateMatrix(&matrix[0][0], angle, axis);

	for (int i = 0; i<7; i++) {
		newX = matrix[0][0] * points[i].x + matrix[1][0] * points[i].y + matrix[2][0] * points[i].z + matrix[3][0];
		newY = matrix[0][1] * points[i].x + matrix[1][1] * points[i].y + matrix[2][1] * points[i].z + matrix[3][1];
		newZ = matrix[0][2] * points[i].x + matrix[1][2] * points[i].y + matrix[2][2] * points[i].z + matrix[3][2];
		points[i].x = newX; points[i].y = newY; points[i].z = newZ;
	}
	location = points[6];
}
