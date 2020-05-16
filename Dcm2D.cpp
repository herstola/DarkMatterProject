#include "Dcm2D.h"
#include <math.h>

Dcm2D::Dcm2D(const Vector2D& vBoard)
{
	// Relative to gameboard
	Vector2D xBoard = Vector2D(1, 0);
	double ang = acos((vBoard * xBoard) / vBoard.getMagnitude());
	m[0][0] = cos(ang);
	m[0][1] = -sin(ang);
	m[1][0] = sin(ang);
	m[1][1] = cos(ang);
}

Dcm2D::Dcm2D(const Dcm2D& d)
{
	*this = d;
}

Dcm2D::~Dcm2D() {}

Dcm2D& Dcm2D::operator=(const Dcm2D& d)
{
	if (this != &d)
	{
		m[0][0] = d.m[0][0];
		m[0][1] = d.m[0][1];
		m[1][0] = d.m[1][0];
		m[1][1] = d.m[1][1];
	}
	return *this;
}

Dcm2D& Dcm2D::operator~()
{
	m[0][1] = m[1][0];
	return *this;
}

Vector2D Dcm2D::operator*(const Vector2D& v) const
{
	int x = v[0] * m[0][0] + v[1] * m[0][1];
	int y = v[0] * m[1][0] + v[1] * m[1][1];
	return Vector2D(x, y);
}