#ifndef __PARTICLE_SYSTEM__
#define __PARTICLE_SYSTEM__

#include <SDL.h>

#include <vector>

class Particle;
class HashTable;

class ParticleSystem
{
public:
	std::vector<Particle*> m_particles;
	HashTable* m_table;

	void InitWithCount(int _pNum, int _w, int _h);

	void HashParticles();
	void Update(int _deltaTS);
	void Draw(SDL_Renderer* _r);

private:



};

#endif