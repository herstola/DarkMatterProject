#pragma once

#include "Vector2D.h"

// DCM principle frame is relative to the game board
class Dcm2D
{
	double m[2][2];
public:
	Dcm2D(const Vector2D&);
	Dcm2D(const Dcm2D&);
	~Dcm2D();

	Dcm2D& operator=(const Dcm2D&);
	Dcm2D& operator~();
	Vector2D operator*(const Vector2D&) const;
};
