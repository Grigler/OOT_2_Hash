#include "ParticleSystem.h"

#include <SDL.h>

#include "Particle.h"
#include "List.h"
#include "HashTable.h"

ParticleSystem::ParticleSystem()
{
	m_table = new HashTable();
}

void ParticleSystem::InitWith(size_t _n)
{
	Clear();
	m_particles.resize(_n);

	for (size_t i = 0; i < _n; i++)
	{
		m_particles.at(i) = CreateParticle();
		m_table->AssignHashKey(m_particles.at(i));
	}
}

void ParticleSystem::Clear()
{
	//Clearing references to all particles
	m_table->ClearTable();
	
	for (size_t i = 0; i < m_particles.size(); i++)
	{
		//Deleting memory at all non-null particles
		delete m_particles.at(i);
	}
	//Clearing now-invalid references to particles
	m_particles.clear();
}

Particle* ParticleSystem::CreateParticle()
{
	//initialise physicial properties
	glm::vec2 p = glm::vec2(rand()%S_WIDTH, rand()%S_HEIGHT);
	glm::vec2 v = glm::normalize(glm::vec2(rand()%2000 - 1000, rand()%2000 - 1000)) * 30.0f;
	float r = 1.0f;

	return new Particle(p, v, r, m_table);
}

void ParticleSystem::Draw(SDL_Renderer* _r)
{
	for (size_t i = 0; i < m_particles.size(); i++)
	{
		m_particles.at(i)->Draw(_r);
	}
}
/*
void ParticleSystem::Update(float _deltaT)
{
	if(m_table)
		m_table->ClearTable();
	
	//Assigning hashes to all particles
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_table->AssignHashKey(m_particles.at(i));
	}

	//Updating all particles (includes handling collisions)
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles.at(i)->Update(_deltaT);
	}
}
*/

void ParticleSystem::Update(float _deltaT)
{
	
	//Re-hashes particles and moves between buckets as need be (parallelised)
	m_table->UpdateTable();
	//Checks particle collisions on a per-bucket basis (parallelised)
	m_table->CheckCollisions();

	#pragma omp parallel
	{
		//Updating positions with second langrangian method
		#pragma omp for
		for (int i = 0; i < m_particles.size(); i++) //int must be used as iterator with omp for
		{
			//Update position
			m_particles[i]->m_pos += m_particles[i]->m_vel * _deltaT;
			//arbritary dampening of velocity
			//m_particles[i]->m_vel += -m_particles[i]->m_vel/15.0f * _deltaT;
		}
	}
}