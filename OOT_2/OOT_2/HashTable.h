#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#define SCALE 10

#include <vector>

#include "Vec2.h"

class Particle;

class HashTable
{
public:

	void Hash(Particle* _p);
	void ClearMap();

	std::vector<Particle*> m_map[1280 / SCALE][720 / SCALE];

};

#endif