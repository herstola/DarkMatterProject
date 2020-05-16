#include "Vector2D.h"
#include <math.h>

Vector2D Vector2D::getUnitVector() const
{
	double mag = getMagnitude();
	double ux = v[0] / mag;
	double uy = v[1] / mag;
	return Vector2D(ux, uy);
}

double Vector2D::getMagnitude() const
{
	double prod1 = v[0] * v[0];
	double prod2 = v[1] * v[1];
	double sum = prod1 + prod2;
	return sqrt(sum);
}

Vector2D& Vector2D::operator++()
{
	v[0] += 1.0;
	v[1] += 1.0;
	return *this;
}

Vector2D Vector2D::operator++(int)
{
	Vector2D t = *this;
	++(*this);
	return t;
}

Vector2D& Vector2D::operator--()
{
	v[0] -= 1.0;
	v[1] -= 1.0;
	return *this;
}

Vector2D Vector2D::operator--(int)
{
	Vector2D t = *this;
	--(*this);
	return t;
}

Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	Vector2D t = *this;
	this->v[0] += v.v[0];
	this->v[1] += v.v[1];
	return t;
}

Vector2D Vector2D::operator+(const Vector2D& v) const
{
	double x = this->v[0] + v[0];
	double y = this->v[1] + v[1];
	return Vector2D(x, y);
}

Vector2D Vector2D::operator-(const Vector2D& v) const
{
	double x = this->v[0] - v[0];
	double y = this->v[1] - v[1];
	return Vector2D(x, y);
}

Vector2D& Vector2D::operator=(const Vector2D& v)
{
	this->v[0] = v.v[0];
	this->v[1] = v.v[1];
	return *this;
}

Vector2D Vector2D::operator*(double m) const
{
	double x = m * v[0];
	double y = m * v[1];
	return Vector2D(x, y);
}

double Vector2D::operator*(const Vector2D& v) const
{
	return this->v[0] * v.v[0] + this->v[1] * v.v[1];
}

double Vector2D::operator[](unsigned int idx) const
{
	if (idx > 2) idx = 1;
	return v[idx];
}

double& Vector2D::operator[](unsigned int idx)
{
	if (idx > 2) idx = 1;
	return v[idx];
}
