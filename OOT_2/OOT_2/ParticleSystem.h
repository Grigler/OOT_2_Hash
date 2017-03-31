#ifndef __PARTICLE_SYSTEM__
#define __PARTICLE_SYSTEM__

#include <vector>

class Particle;
class HashTable;
struct SDL_Renderer;

class ParticleSystem
{
public:
	ParticleSystem();

	//Intialises with a specific number of particles
	void InitWith(size_t _n);

	void Clear();

	std::vector<Particle*> m_particles;
	HashTable* m_table;

	void Draw(SDL_Renderer* _r);
	void Update(float _deltaT);

private:

	Particle* CreateParticle();

};


#endif