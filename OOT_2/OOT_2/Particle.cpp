#include "Particle.h"

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
void Particle::Update(float _deltaT)
{
	m_pos += m_vel*_deltaT;
	//Maybe toggleable gravity?
}

void Particle::HandleCollisions()
{
	const int kX = m_key.x, kY = m_key.y;
	List* bucket = m_table->m_table[kX][kY];
	Item* i = bucket->m_root;
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
	float d = glm::distance(_p->m_pos, m_pos);
	if(d <= m_rad || d <= _p->m_rad)
	{
		m_vel += _p->m_vel;
	}
}