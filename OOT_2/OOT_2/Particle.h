#ifndef __PARTICLE__
#define __PARTICLE__

#include <SDL.h>

#include "Vec2.h"

#include <vector>

class ParticleSystem;

class Particle
{
public:
	Particle(Vec2 _p, Vec2 _v, Vec2 _a);

	void Update(int _deltaTS);
	void Draw(SDL_Renderer* _r);

	Vec2 GetPos() {return m_pos;}
	Vec2 GetVel() {return m_vel;}

private:
	Vec2 m_pos;
	Vec2 m_vel;
	Vec2 m_acc;

	ParticleSystem* m_sys;
	//Change this to a store of its key value in the hashtable
	std::vector<Particle*> GetNearParticles();

};

#endif