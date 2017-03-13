#include "HashTable.h"

#include "Particle.h"

void HashTable::Hash(Particle* _p)
{
	//Simple spatial hash function that sets a key based upon its pos and a scale
	int x = _p->GetPos().m_x / SCALE - 1;
	int y = _p->GetPos().m_y / SCALE - 1;
	m_map[x][y].push_back(_p);
	_p->m_key = Vec2(x,y);
}
void HashTable::ClearMap()
{
	for (int x = 0; x < 1280/SCALE; x++)
	{
		for (int y = 0; y < 720/SCALE; y++)
		{
			m_map[x][y].clear();
		}
	}
}