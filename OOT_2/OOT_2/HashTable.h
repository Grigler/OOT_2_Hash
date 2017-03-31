#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#define S_WIDTH 1280
#define S_HEIGHT 720

#define SQUARE_BUCKET_NUM 40
#define BUCKET_W S_WIDTH  / SQUARE_BUCKET_NUM
#define BUCKET_H S_HEIGHT / SQUARE_BUCKET_NUM

#include <glm.hpp>

class List;
class Particle;

class HashTable
{
public:
	HashTable();
	~HashTable();

	List* m_table[BUCKET_W][BUCKET_H];

	//Hashes Particle and assigns it that key value - also inserts to table
	void AssignHashKey(Particle* _p);
	glm::vec2 GetHashKey(Particle* _p);
	void ClearTable();
	void UpdateTable();
	void CheckCollisions();

private:
	void UpdateList(List* _l, glm::vec2 _key);
	void CheckCollision(List* _l);

};

#endif