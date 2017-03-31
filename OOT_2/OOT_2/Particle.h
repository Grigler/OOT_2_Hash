#ifndef __PARTICLE__
#define __PARTICLE__

#include <SDL.h>
#include <glm.hpp>

class HashTable;

class Particle
{
public:

	Particle(glm::vec2 _p, glm::vec2 _v, float _r, HashTable* _t);


	//Physical Properties
	glm::vec2 m_pos;
	glm::vec2 m_vel;
	float m_rad;

	glm::vec2 m_key;
	HashTable* m_table;

	void Draw(SDL_Renderer* _r);
	void Update(float _deltaT);

	void HandleCollisions();

	//Does simple sphere intersection test and changes velocity
	void CheckCollision(Particle* _p);

};

#endif