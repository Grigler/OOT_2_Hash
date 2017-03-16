#include "ParticleSystem.h"

#include <SDL.h>

#include "Particle.h"
#include "HashTable.h"

ParticleSystem::ParticleSystem()
{
	m_table = new HashTable();
}

void ParticleSystem::InitWith(unsigned int _n)
{
	if(m_table)
		m_table->ClearTable();
	else
		m_table = new HashTable();

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
	glm::vec2 v = glm::normalize(glm::vec2(rand()%1000, rand()%1000));;
	float r = 0.5f; //TODO - CHANGE TO A CONFIGURABLE VALUE

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

	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		m_particles.at(i)->Update(_deltaT);
	}
}