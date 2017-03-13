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
	~Particle() 
	{
		getchar();
	}

	void Update(int _deltaTS);
	void Draw(SDL_Renderer* _r);

	bool IsInBounds();
	void Reset();
	
	Vec2 GetPos() {return m_pos;}
	Vec2 GetVel() {return m_vel;}

	Vec2 m_key;

	ParticleSystem* m_sys;

private:
	Vec2 m_pos;
	Vec2 m_vel;
	Vec2 m_acc;

	
	//Change this to a store of its key value in the hashtable
	std::vector<Particle*> GetNearParticles();

};

#endif