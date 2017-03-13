#include "Particle.h"
#include "ParticleSystem.h"
#include "HashTable.h"

Particle::Particle(Vec2 _p, Vec2 _v, Vec2 _a)
{
	m_pos = _p;
	m_vel = _v;
	m_acc = _a;
}

void Particle::Update(int _deltaTS)
{
	std::vector<Particle*> nearParticles = GetNearParticles();

	for (unsigned int i = 0; i < nearParticles.size(); i++)
	{
		if(nearParticles[i] != this && m_pos == nearParticles[i]->GetPos())
		{
			m_vel.m_x = m_vel.m_x + nearParticles[i]->GetVel().m_x;
			m_vel.m_y = m_vel.m_y + nearParticles[i]->GetVel().m_y;
		}
	}

	m_pos = m_pos + (m_vel*(_deltaTS * 0.005f));
}

void Particle::Draw(SDL_Renderer* _r)
{
	SDL_RenderDrawPoint(_r, (int)m_pos.m_x, (int)m_pos.m_y);
}

bool Particle::IsInBounds()
{
	return m_pos.m_x > 0 && m_pos.m_x < 1280 && m_pos.m_y > 0 && m_pos.m_y < 720;
}

void Particle::Reset()
{
	m_pos = Vec2(rand()%1280, rand()%720);
}

std::vector<Particle*> Particle::GetNearParticles()
{
	const int kX = m_key.m_x, kY = m_key.m_y;

	return m_sys->m_table->m_map[kX][kY];
}