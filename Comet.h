#pragma once

#include "GameObject.h"
#include <vector>

class Comet : public GameObject
{
	unsigned int spawnCount;
public:
	Comet() : spawnCount(5) {};
	~Comet() {}

	std::vector<Comet*> explode()
	{
		std::vector<Comet*> comets;
		for (unsigned int i = 0; i < spawnCount; ++i)
		{
			comets.push_back(new Comet());
		}
		return comets;
	}
};

