#pragma once

class Vector2D
{
	double v[2];
public:
	Vector2D() : v{ 0.0, 0.0 } {}
	Vector2D(double x, double y) : v{ x, y } {}
	Vector2D(const Vector2D& v) { *this = v; }
	~Vector2D() {}
	
	Vector2D getUnitVector() const;
	double getMagnitude() const;
	Vector2D& operator++(); // Prefix increment
	Vector2D operator++(int); // Postfix increment
	Vector2D& operator--(); // Prefix increment
	Vector2D operator--(int); // Postfix increment
	Vector2D& operator+=(const Vector2D&);
	Vector2D operator+(const Vector2D&) const;
	Vector2D operator-(const Vector2D&) const;
	Vector2D& operator=(const Vector2D&);
	Vector2D operator*(double) const;
	double operator*(const Vector2D&) const;
	double operator[](unsigned int) const;
	double& operator[](unsigned int);
};