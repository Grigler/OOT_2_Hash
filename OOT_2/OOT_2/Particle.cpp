#include "Particle.h"

//Used for debugging remember to remove
#include <iostream>

#include "List.h"
#include "HashTable.h"

Particle::Particle(glm::vec2 _p, glm::vec2 _v, float _r, HashTable* _t)
{
	m_pos = _p;
	m_vel = _v;
	m_rad = _r;
	m_table = _t;
}

void Particle::Draw(SDL_Renderer* _r)
{
	//Look into drawing a sphere (may be easier to draw something else)

	//For now a simple render pixel
	SDL_RenderDrawPoint(_r, m_pos.x, m_pos.y);
}
//Presuming already hashed
void Particle::Update(float _deltaT)
{
	//Attempted Forward-Euler and 1st langrangian, F.E. seemed more stable
	m_pos += m_vel*_deltaT;
	HandleCollisions();
	
}

void Particle::HandleCollisions()
{
	const int kX = m_key.x, kY = m_key.y;
	List* bucket = m_table->m_table[kX][kY];
	Item* i = bucket->m_root;

	//std::cout << "(" << kX << ", " << kY << ") : " << bucket->GetLength(bucket->m_root) << std::endl;

	while(i != NULL)
	{
		if(i->m_data != NULL && i->m_data != this)
		{
			CheckCollision(i->m_data);
		}

		i = i->m_next;
	}
}
void Particle::CheckCollision(Particle* _p)
{
	
	glm::vec2 colDir = _p->m_pos - m_pos;
	float c = colDir.x*colDir.x + colDir.y*colDir.y;

	//No square root for a massive optimisation

	if(c <= (m_rad + _p->m_rad)*(m_rad + _p->m_rad)) //Tested to be faster than pow
	{
		float a = glm::dot(m_pos, colDir);
		float b = glm::dot(_p->m_pos, colDir);

		m_vel += (b-a)*colDir;
	}
}