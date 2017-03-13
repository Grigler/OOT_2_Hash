#include "Particle.h"
#include "ParticleSystem.h"

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
		if(m_pos == nearParticles[i]->GetPos())
		{
			m_vel.m_x = m_vel.m_x + nearParticles[i]->GetVel().m_x;
			m_vel.m_y = m_vel.m_y + nearParticles[i]->GetVel().m_y;
		}
	}

	m_pos = m_pos + (m_vel*(_deltaTS * 0.05f));
}

void Particle::Draw(SDL_Renderer* _r)
{
	SDL_RenderDrawPoint(_r, (int)m_pos.m_x, (int)m_pos.m_y);
}

std::vector<Particle*> Particle::GetNearParticles()
{
	std::vector<Particle*> retVec;

	//Fill retVec with all particles in relevent bucket that aren't this particle

	return retVec;
}