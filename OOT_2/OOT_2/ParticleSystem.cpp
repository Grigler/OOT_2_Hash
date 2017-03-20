#include "ParticleSystem.h"

#include <SDL.h>

#include "Particle.h"
#include "List.h"
#include "HashTable.h"

ParticleSystem::ParticleSystem()
{
	m_table = new HashTable();
}

void ParticleSystem::InitWith(unsigned int _n)
{
	Clear();

	for (unsigned int i = 0; i < _n; i++)
	{
		m_particles.push_back(CreateParticle());
		m_table->AssignHashKey(m_particles.back());
	}
}

void ParticleSystem::Clear()
{
	//Clearing references to all particles
	m_table->ClearTable();

	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		//Deleting memory at all non-null particles
		if(m_particles.at(i))
			delete m_particles.at(i);
	}
	//Clearing now-invalid references to particles
	m_particles.clear();
}

Particle* ParticleSystem::CreateParticle()
{
	//initialise physicial properties
	glm::vec2 p = glm::vec2(rand()%S_WIDTH, rand()%S_HEIGHT);
	glm::vec2 v = glm::normalize(glm::vec2(rand()%2000 - 1000, rand()%2000 - 1000)) * 15.0f;
	float r = 1.0f; //TODO - CHANGE TO A CONFIGURABLE VALUE

	return new Particle(p, v, r, m_table);
}

void ParticleSystem::Draw(SDL_Renderer* _r)
{
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		m_particles.at(i)->Draw(_r);
	}
}
void ParticleSystem::Update(float _deltaT)
{
	if(m_table)
		m_table->ClearTable();

	//Assigning hashes to all particles
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		m_table->AssignHashKey(m_particles.at(i));
	}

	//Updating all particles (includes handling collisions)
	//Pragma omp is used to do this is in parallel as there is no danger of collision
	#pragma omp parallel
	{
		#pragma omp for
		for (unsigned int i = 0; i < m_particles.size(); i++)
		{
			m_particles.at(i)->Update(_deltaT);
		}
	}
}