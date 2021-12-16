#pragma once
#undef __STRICT_ANSI__
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include <GL/glut.h>

#include "Helper.h"

using namespace Helper;

class Octahedron
{
private:
	std::vector<Point3D> points = {
		Point3D(-1, 0, 0),
		Point3D(0, 1, 0),
		Point3D(0, 0, 1),
		Point3D(0,-1, 0),
		Point3D(1, 0, 0),
		Point3D(0, 0,-1),
		Point3D(0, 0, 0)
	};

	int faces[8][3] = {
		{ 0,	2,	1 },
		{ 0,	2,	3 },
		{ 1,	2,	4 },
		{ 3,	2,	4 },
		{ 0,	5,	1 },
		{ 0,	5,	3 },
		{ 1,	5,	4 },
		{ 3,	5,	4 }
	};

	Point3D location, rotation_point;

	int getBottomPoint();
	bool isRolling = true;
	bool isFalling = false;
	void falling();
	void rolling();

	enum State {
		ON_CUBE, START_FALLING, FALLING, ON_FLOOR
	} currentState;

public:
	Octahedron();
	~Octahedron();

	void Draw();
	void Move(Point3D delta);
	void Rotate(float angle, Axis axis);
};
