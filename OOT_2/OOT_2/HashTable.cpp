#include "HashTable.h"

#include "List.h"
#include "Particle.h"

//Lowest level include for memset() - can be removed with include of most stdlib stuff
#include <cstring>

HashTable::HashTable()
{
	//Filling m_table with NULL pointers
	memset(m_table, NULL, sizeof(m_table));
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
	int hX = 0, hY = 0;

	//hash function
	
	//TODO - Write the function

	//Assigning key to particle
	_p->m_key = glm::vec2(hX,hY);
	
	//Checking if the list has been created yet (may be better to pre-process creation)
	if(m_table[hX][hY] == NULL)
		m_table[hX][hY] = new List();
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