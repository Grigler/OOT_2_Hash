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

Item* List::Find(Particle* _p, Item* _c)
{
	if(_c != NULL)
		if(_c->m_data == _p)
			return _c;
		else
			return Find(_p, _c->m_next);
	else
		return NULL;
}
Item* List::Find(unsigned int _gIndx, Item* _c, int _cIndx)
{
	if(_c == NULL)
		return NULL;
	else
	{
		if(_cIndx == _gIndx)
			return _c;
		else
			return Find(_gIndx, _c->m_next, ++_cIndx);
	}
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
		Item* next = _c->m_next;

		if(_c->m_data != NULL) //Presuming root's data is always NULL
		{
			delete _c;
			_c = NULL;			
		}
		else
			_c->m_next = NULL;

		Clear(next);
	}
}
unsigned int List::GetLength(Item* _c)
{
	if(_c->m_next != NULL)
		return GetLength(_c->m_next) + 1;
	else
		return 0; //Returning 0 to account for root node which isn't technicall a particle
}