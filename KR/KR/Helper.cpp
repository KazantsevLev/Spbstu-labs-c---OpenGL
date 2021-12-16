#include "Helper.h"
#include <cmath>
#include <algorithm>

namespace Helper {

	void CreateRotateMatrix(float* mtx, float angle, Axis axis) {
		float* ptr = mtx;
		switch (axis) {
		case X:
			*(ptr++) = 1; *(ptr++) = 0;             *(ptr++) = 0;           *(ptr++) = 0;
			*(ptr++) = 0; *(ptr++) = cos(angle);    *(ptr++) = sin(angle);  *(ptr++) = 0;
			*(ptr++) = 0; *(ptr++) = -sin(angle);   *(ptr++) = cos(angle);  *(ptr++) = 0;
			*(ptr++) = 0; *(ptr++) = 0;             *(ptr++) = 0;           *(ptr++) = 1;
			break;
		case Y:
			*(ptr++) = cos(angle);  *(ptr++) = 0;   *(ptr++) = -sin(angle); *(ptr++) = 0;
			*(ptr++) = 0;           *(ptr++) = 1;   *(ptr++) = 0;           *(ptr++) = 0;
			*(ptr++) = sin(angle);  *(ptr++) = 0;   *(ptr++) = cos(angle);  *(ptr++) = 0;
			*(ptr++) = 0;           *(ptr++) = 0;   *(ptr++) = 0;           *(ptr++) = 1;
			break;
		case Z:
			*(ptr++) = cos(angle);  *(ptr++) = sin(angle);  *(ptr++) = 0;   *(ptr++) = 0;
			*(ptr++) = -sin(angle); *(ptr++) = cos(angle);  *(ptr++) = 0;   *(ptr++) = 0;
			*(ptr++) = 0;           *(ptr++) = 0;           *(ptr++) = 1;   *(ptr++) = 0;
			*(ptr++) = 0;           *(ptr++) = 0;           *(ptr++) = 0;   *(ptr++) = 1;
			break;
		}
	}

	bool CrossLines(PointF* pt1, PointF* pt2, PointF* pt3, PointF* pt4, PointF *cross_point) {

		PointF *p1 = pt1, *p2 = pt2, *p3 = pt3, *p4 = pt4;
		//сначала расставим точки по порядку, т.е. чтобы было p1.x <= p2.x
		if (p2->X < p1->X) {
			PointF* tmp = p1;
			p1 = p2;
			p2 = tmp;
		}
		//и p3.x <= p4.x
		if (p4->X < p3->X) {
			PointF* tmp = p3;
			p3 = p4;
			p4 = tmp;
		}

		//проверим существование потенциального интервала для точки пересечения отрезков
		if (p2->X < p3->X) {
			return false; //ибо у отрезков нету взаимной абсциссы
		}

		//если оба отрезка вертикальные
		if ((p1->X - p2->X == 0) && (p3->X - p4->X == 0)) {
			//если они лежат на одном X
			if (p1->X == p3->X) {
				//проверим пересекаются ли они, т.е. есть ли у них общий Y
				//для этого возьмём отрицание от случая, когда они НЕ пересекаются
				if (!((std::max(p1->Y, p2->Y) < std::min(p3->Y, p4->Y)) ||
					(std::min(p1->Y, p2->Y) > std::max(p3->Y, p4->Y)))) {
					return true;
				}
			}
			return false;
		}

		//найдём коэффициенты уравнений, содержащих отрезки
		//f1(x) = A1*x + b1 = y
		//f2(x) = A2*x + b2 = y

		double Xa, A1, A2, b1, b2, Ya;

		//если первый отрезок вертикальный
		if (p1->X - p2->X == 0) {
			//найдём Xa, Ya - точки пересечения двух прямых
			Xa = p1->X;
			A2 = (p3->Y - p4->Y) / (double)(p3->X - p4->X);
			b2 = p3->Y - A2 * p3->X;
			Ya = A2 * Xa + b2;

			if (p3->X <= Xa && p4->X >= Xa && std::min(p1->Y, p2->Y) <= Ya &&
				std::max(p1->Y, p2->Y) >= Ya) {
				return true;
			}
			return false;
		}

		//если второй отрезок вертикальный
		if (p3->X - p4->X == 0) {
			//найдём Xa, Ya - точки пересечения двух прямых
			Xa = p3->X;
			A1 = (p1->Y - p2->Y) / (double)(p1->X - p2->X);
			b1 = p1->Y - A1 * p1->X;
			Ya = A1 * Xa + b1;

			if (p1->X <= Xa && p2->X >= Xa && std::min(p3->Y, p4->Y) <= Ya &&
				std::max(p3->Y, p4->Y) >= Ya) {
				return true;
			}
			return false;
		}

		//оба отрезка невертикальные
		A1 = (p1->Y - p2->Y) / (double)(p1->X - p2->X);
		A2 = (p3->Y - p4->Y) / (double)(p3->X - p4->X);
		b1 = p1->Y - A1 * p1->X;
		b2 = p3->Y - A2 * p3->X;

		if (A1 == A2) {
			return false; //отрезки параллельны
		}

		//Xa - абсцисса точки пересечения двух прямых
		Xa = (b2 - b1) / (A1 - A2);

		if ((Xa <= std::max(p1->X, p2->X)) && (Xa >= std::min(p1->X, p2->X)) &&
			(Xa <= std::max(p3->X, p4->X)) && (Xa >= std::min(p3->X, p4->X))) {
			cross_point->X = Xa;
			cross_point->Y = A2 * Xa + b2;;
			return true; //точка Xa находится не на отрезке [p3, p4]
		}
		else {
			return false;
		}
	}
}