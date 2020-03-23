#pragma once
#include "Node.h"



template<typename T >
class BagList
{
private:
	Node<T>* Head;


public:
	BagList();
	~BagList();
	bool insertBeg(T newEntry);
	bool deleteitem(T & removed);


};
template<typename T>
BagList<T>::BagList()
{
	Head = NULL; 
}

template <typename T>
bool BagList<T>::insertBeg( T newEntry)  // It doesn't matter in Bag ADT where to blace the objects 
{
	Node<T>* newptr = new Node<T>(newEntry);
	 
	newptr->setNext(Head);
	Head = newptr;
	return true ;




}

template<typename T>
bool BagList<T> ::deleteitem (T & removed)
{

	Node<T>* L = Head;
	Node<T>* prev = L;
	if (! Head)
	{
		return false;
	}
	if (Head->getItem() == removed)
	{
		Head = Head->getNext();
		delete L;
		L = NULL;
		return true;
	}

	while (L && L->getitem != removed)
	{
		prev = L;
		L = L->getNext();
	}
	if (!L)
	{
		return false;
	}
	prev->setNext(L->getNext());
	delete L;
	L = NULL;
	return true;
}




template<typename T>
BagList<T>::~BagList()
{
}