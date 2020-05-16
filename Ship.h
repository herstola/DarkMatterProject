#pragma once

#include "GameObject.h"
#include "Shot.h"
#include "SDL.h"
#include <vector>
#include "Dcm2D.h"

class Ship : public GameObject
{
	double axialAcceleration;
	double offAxisAcceleration;
	bool axiallyAccelerating;
	bool turningLeft;
	bool turningRight;

public:
	Ship() :
		axialAcceleration(0.0),
		offAxisAcceleration(0.0),
		axiallyAccelerating(false),
		turningLeft(false),
		turningRight(false)
	{}

	Ship(const Ship& ship)
	{
		*this = ship;
	}

	~Ship() {}

	Shot* fire()
	{
		Shot* shot = new Shot();
		shot->overrideHeading(readHeading());
		shot->overridePosition(readPosition());
		shot->setSpeed(getSpeed() + 50);
		return shot;
	}

	double getAxialAccel() const
	{
		return axialAcceleration;
	}

	void setAxialAccel(double a)
	{
		axialAcceleration = a;
	}

	double getOffAxisAccel() const
	{
		return offAxisAcceleration;
	}

	void setOffAxisAccel(double a)
	{
		offAxisAcceleration = a;
	}

	void accelAxially(bool value)
	{
		axiallyAccelerating = value;
	}


	void accelLeft(bool value)
	{
		turningLeft = value;
	}

	void accelRight(bool value)
	{
		turningRight = value;
	}

	void propagate(double dt)
	{
		Vector2D position = readPosition();
		Vector2D velocity = getVelocity();

		if (axiallyAccelerating || turningLeft || turningRight)
		{
			Vector2D uv = velocity.getUnitVector();
			Dcm2D dcm2Body = Dcm2D(uv);

			// Build acceleration command in noninertial body frame
			double fwdCmd = axialAcceleration;
			double leftCmd = -offAxisAcceleration;
			double rightCmd = offAxisAcceleration;
			Vector2D accelCmd;

			if (axiallyAccelerating)
			{
				accelCmd[0] += axialAcceleration;
			}

			if (turningLeft)
			{
				accelCmd[1] -= offAxisAcceleration;
			}

			if (turningRight)
			{
				accelCmd[1] += offAxisAcceleration;
			}

			// Acceleration in Game board frame
			Vector2D acceleration = ~dcm2Body * accelCmd;

			// Update position
			position += acceleration * 0.5 * dt * dt + velocity * dt;

			// Update velocity
			velocity += acceleration * dt;

			// Set speed
			//setSpeed(velocity.getMagnitude());

			//set
		}
		else
		{
			// Update position
			position += velocity * dt;

			overridePosition(position);
		}
	}

	Ship& operator()(const Ship& s)
	{
		if (this != &s)
		{
			axialAcceleration = s.axialAcceleration;
			offAxisAcceleration = s.offAxisAcceleration;
			axiallyAccelerating = s.axiallyAccelerating;
			turningLeft = s.turningLeft;
			turningRight = s.turningRight;
		}
	}
};

