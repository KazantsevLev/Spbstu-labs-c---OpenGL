#pragma once

namespace Helper {

	typedef struct Point3D {
		Point3D() {	}
		Point3D(double _x, double _y, double _z) :
			x(_x), y(_y), z(_z) {}

		Point3D(const Point3D &p) :
			x(p.x), y(p.y), z(p.z){}

		Point3D inverse() {
			return Point3D(-x, -y, -z);
		}

		bool operator== (Point3D p) {
			return x == p.x && y == p.y && z == p.z;
		}

		void operator += (Point3D p) {
			x += p.x; y += p.y; z += p.z;
		}

		double x, y, z;
	};

	struct PointF {
		float X;
		float Y;

		PointF(float x, float y) {
			X = x;
			Y = y;
		}

		PointF() {
			X = 0;
			Y = 0;
		}
	};

	enum Axis {
		X, Y, Z
	};

	void CreateRotateMatrix(float* mtx, float angle, Axis axis);
	bool CrossLines(PointF* pt1, PointF* pt2, PointF* pt3, PointF* pt4, PointF *cross_point);
}