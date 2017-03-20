#ifndef __LIST__
#define __LIST__

class Particle;

struct Item
{
	Particle* m_data;
	Item* m_next;

	Item(Particle* _p, Item* _n)
	{
		m_data = _p;
		m_next = _n;
	}
};

class List
{
public:
	List();
	~List();

	Item* m_root;

	//Retuning first Item instance with _p as data - _c is entry point
	Item* Find(Particle* _p, Item* _c);
	//Returning Item with 'index' in list (ignoring root) of _indx - returns 0 if not found
	Item* Find(unsigned int _gIndx, Item* _c, int _cIndx);
	//Adds new item to list with _p as data and argument as parent
	void Add(Particle* _p, Item* _parent);
	//Removes _c with _l being the parent of _c, for re-allocation of m_next
	void Remove(Item* _c, Item* _parent);
	//Search for and Removes _p with _c as entry point - Returns removal success
	bool SearchAndRemove(Particle* _p, Item* _c, Item* _l);
	//Completely deletes all items except the root
	void Clear(Item* _c);
	//Returns number of particles from _c (not including _c)
	unsigned int GetLength(Item* _c);

};

#endif