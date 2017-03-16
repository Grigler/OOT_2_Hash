#include "List.h"

#include "Particle.h"

List::List()
{
	m_root = new Item(NULL, NULL);
}
List::~List()
{
	//Clearing up to the root of the List
	Clear(m_root);
	//Force-deleting the root
	delete m_root;
}

Item* List::Find(Particle* _p, Item* _i)
{
	if(_i != NULL)
		if(_i->m_data == _p)
			return _i;
		else
			return Find(_p, _i->m_next);
	else
		return NULL;
}
void List::Add(Particle* _p, Item* _parent)
{
	Item* i = new Item(_p, _parent->m_next);
	_parent->m_next = i;
}
void List::Remove(Item* _c, Item* _parent)
{
	_parent->m_next = _c->m_next;
	delete _c;
}
bool List::SearchAndRemove(Particle* _p, Item* _c, Item* _l)
{
	if(_c != NULL)
	{
		if(_c->m_data == _p)
		{
			_l->m_next = _c->m_next;
			delete _c;
			return true;
		}
		else
			return SearchAndRemove(_p, _c->m_next, _c);
	}
	else
		return false;
}
void List::Clear(Item* _c)
{
	if(_c != NULL)
	{
		if(_c->m_data != NULL) //Presuming root's data is always NULL
		{
			Item* next = _c->m_next;
			delete _c;
			Clear(next);
		}
	}
}
unsigned int GetLength(Item* _c)
{
	if(_c->m_next != NULL)
		return GetLength(_c->m_next) + 1;
	else
		return 0; //Returning 0 to account for root node which isn't technicall a particle
}