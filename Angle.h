#pragma once

constexpr auto PI = 3.14159265359;
#define DEG_2_RAD PI / 180.0
#define RAD_2_DEG 1.0 / DEG_2_RAD

class AngleDeg
{
	double angRad;
public:
	AngleDeg() : angRad(0.0)
	{}

	AngleDeg(double angDeg) : angRad(angDeg * DEG_2_RAD) 
	{}

	AngleDeg(const AngleDeg& angDeg)
	{
		*this = angDeg;
	}

	~AngleDeg() {}

	double getDeg() const
	{
		return angRad * RAD_2_DEG;
	}

	double getRad() const
	{
		return angRad;
	}

	double setDeg(double angDeg)
	{
		angRad = angDeg * DEG_2_RAD;
	}

	AngleDeg operator+(double da) const
	{
		return AngleDeg(angRad * RAD_2_DEG + da);
	}

	AngleDeg operator-(double da) const
	{
		return AngleDeg(angRad * RAD_2_DEG - da);
	}

	AngleDeg& operator=(const AngleDeg& angDeg)
	{
		if (this != &angDeg)
		{
			angRad = angDeg.angRad;
		}
		return *this;
	}

	AngleDeg& operator++()
	{
		angRad += 1.0 * DEG_2_RAD;
		return *this;
	}

	AngleDeg operator++(int)
	{
		AngleDeg t = *this;
		++(*this);
		return t;
	}

	AngleDeg& operator--()
	{
		angRad -= 1.0 * DEG_2_RAD;
		return *this;
	}

	AngleDeg operator--(int)
	{
		AngleDeg t = *this;
		--(*this);
		return t;
	}
};