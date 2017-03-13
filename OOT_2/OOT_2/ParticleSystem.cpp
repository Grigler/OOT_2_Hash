#include "ParticleSystem.h"

#include "Particle.h"
#include "HashTable.h"

void ParticleSystem::InitWithCount(int _pNum, int _w, int _h)
{
	m_particles.clear();

	if(_pNum >= 0)
	{
		for (int i = 0; i < _pNum; i++)
		{
			Vec2 pos; pos.m_x = rand()%_w; pos.m_y = rand()%_h;
			Vec2 vel; vel.m_x = rand()%3 - 1;			vel.m_y = rand()%3 - 1;
			Vec2 acc; acc.m_x = 0;								acc.m_y = 0;
			Particle* p = new Particle(pos, vel, acc);
			p->m_sys = this;
			m_particles.push_back(p);
		}
	}

	m_table = new HashTable;
}

void ParticleSystem::HashParticles()
{
	for(size_t i = 0; i < m_particles.size(); i++)
	{
		Particle* p = m_particles.at(i);
		if(p != NULL)
		{
			if(p->IsInBounds())
				m_table->Hash(p);
			else
				p->Reset();
		}
	}
}

void ParticleSystem::Update(int _deltaTS)
{
	for(unsigned int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i]->Update(_deltaTS);
	}
}

void ParticleSystem::Draw(SDL_Renderer* _r)
{
	for(unsigned int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i]->Draw(_r);
	}
}