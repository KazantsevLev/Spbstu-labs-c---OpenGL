#pragma once

#include <vector>

#include <GL/glut.h>

#include "Helper.h"

using namespace Helper;

class Cube
{
public:
	Cube(double a);
	~Cube();

	void Draw();
	void Move(Point3D);

	double A;

private:
	std::vector<std::vector<Point3D>> surface;
};