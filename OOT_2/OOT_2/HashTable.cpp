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
glm::vec2 HashTable::GetHashKey(Particle* _p)
{
	return glm::vec2(floor(_p->m_pos.x / (float)(S_WIDTH / SQUARE_BUCKET_NUM)),
									 floor((_p->m_pos.y / (float)(S_HEIGHT / SQUARE_BUCKET_NUM))));
}

void HashTable::ClearTable()
{
	for (int x = 0; x < BUCKET_W; x++)
	{
		for (int y = 0; y < BUCKET_H; y++)
		{
			if(m_table[x][y])
				m_table[x][y]->Clear(m_table[x][y]->m_root); //List Items are cleaned up in ~List()
		}
	}
}

void HashTable::UpdateTable()
{
	#pragma omp parallel
	{
		for (int x = 0; x < BUCKET_W; x++)
		{
			#pragma omp for
			for (int y = 0; y < BUCKET_H; y++)
			{
				UpdateList(m_table[x][y], glm::vec2(x,y));
			}
		}
	}
}
void HashTable::UpdateList(List* _l, glm::vec2 _key)
{
	Item* current = _l->m_root->m_next;
	Item* parent = _l->m_root;

	while(current != NULL)
	{
		if(current->m_data != NULL)
		{
			glm::vec2 currKey = GetHashKey(current->m_data);
			if(currKey != _key)
			{
				current->m_data->m_key = currKey;
				if(currKey.x >= 0 && currKey.x < BUCKET_W && currKey.y > 0 && currKey.y < BUCKET_H)
				{
					int x = (int)currKey.x;
					int y = (int)currKey.y;

					#pragma omp critical 
					{
						m_table[x][y]->Add(current->m_data,m_table[x][y]->m_root);
					}
				}
				else
				{
					//current->m_data->m_vel = -current->m_data->m_vel*0.5f;	
				}

				_l->Remove(current, parent);
				//Current changes to the parent's next as that will now
				//point to what was the last current's next
				current = parent->m_next;
			}
			else
			{
				parent = current;
				current = current->m_next;
			}
		}
		
	}
}
void HashTable::CheckCollisions()
{
	#pragma omp parallel
	{
		for (int x = 0; x < BUCKET_W; x++)
		{
			#pragma omp for
			for (int y = 0; y < BUCKET_H; y++)
			{
				CheckCollision(m_table[x][y]);
			}
		}
	}
}
void HashTable::CheckCollision(List* _l)
{
	Item* start = _l->m_root->m_next;
	if(start != NULL) //Checking if bucket is empty
	{
		Item* next = start->m_next;

		Particle* p;

		while(start != NULL)
		{
			p = start->m_data;
			while(next != NULL)
			{
				p->CheckCollision(next->m_data);
				next = next->m_next;
			}
			start = start->m_next;
		}	
	}
}