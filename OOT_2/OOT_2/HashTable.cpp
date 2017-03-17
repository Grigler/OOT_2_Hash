#include "HashTable.h"

#include "List.h"
#include "Particle.h"

//Lowest level include for memset() - can be removed with include of most stdlib stuff
#include <cstring>

HashTable::HashTable()
{
	//Filling m_table with NULL pointers
	for (int x = 0; x < BUCKET_W; x++)
	{
		for (int y = 0; y < BUCKET_H; y++)
		{
			m_table[x][y] = new List();
		}
	}
}

HashTable::~HashTable()
{
	for (unsigned int x = 0; x < BUCKET_W; x++)
	{
		for (unsigned int y = 0; y < BUCKET_H; y++)
		{
			if(m_table[x][y])
				delete m_table[x][y]; //List Items are cleaned up in ~List()
		}
	}
}

void HashTable::AssignHashKey(Particle* _p)
{
	//Hash values for x and y
	int hX = 0, hY = 0;

	//Simply hashing position to bucket
	hX = floor(_p->m_pos.x / (float)(S_WIDTH / SQUARE_BUCKET_NUM));
	hY = floor((_p->m_pos.y / (float)(S_HEIGHT / SQUARE_BUCKET_NUM)));

	if(hX >= 0 && hX < BUCKET_W && hY >= 0 && hY < BUCKET_H)
	{
		//Assigning key to particle
		_p->m_key = glm::vec2(hX,hY);
	
		//Checking if the list has been created yet (may be better to pre-process creation)
		if(m_table[hX][hY] == NULL)
			m_table[hX][hY] = new List();

		//Adding particle to first position in Bucket
		m_table[hX][hY]->Add(_p, m_table[hX][hY]->m_root);
	}
	
}
void HashTable::ClearTable()
{
	for (unsigned int x = 0; x < BUCKET_W; x++)
	{
		for (unsigned int y = 0; y < BUCKET_H; y++)
		{
			if(m_table[x][y])
				m_table[x][y]->Clear(m_table[x][y]->m_root); //List Items are cleaned up in ~List()
		}
	}
}