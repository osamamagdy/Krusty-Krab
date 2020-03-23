
#ifndef __NODE_H_
#define __NODE_H_
#include <iostream>
using namespace std;

template < typename T>
class Node
{
private :
	int priority; // An integer that represents the priority of the node in priority queue
	T item; // A data item
	Node<T>* next; // Pointer to next node
public :
	Node();
	Node( const T & r_Item, int Priority =0 );	//passing by const ref.
	Node( const T & r_Item, Node<T>* nextNodePtr, int Priority = 0);
	void setItem( const T & r_Item, int Priority = 0);
	void setpriority(int Priority) ;
	void setNext(Node<T>* nextNodePtr);
	T getItem() const ;
	int getpriority() const;
	Node<T>* getNext() const ;
}; // end Node


template < typename T>
Node<T>::Node() 
{
	next = nullptr;
} 

template < typename T>
Node<T>::Node( const T& r_Item,int Priority)
{
	priority = Priority;
	item = r_Item;
	next = nullptr;
} 

template < typename T>
Node<T>::Node( const T& r_Item, Node<T>* nextNodePtr, int Priority)
{
	priority = Priority;
	item = r_Item;
	next = nextNodePtr;
}
template < typename T>
void Node<T>::setItem( const T& r_Item, int Priority)
{
	priority = Priority;
	item = r_Item;
} 

template<typename T>
void Node<T> ::setpriority(int Priority)
{
 priority = Priority;
}

template < typename T>
void Node<T>::setNext(Node<T>* nextNodePtr)
{
	next = nextNodePtr;
} 

template < typename T>
T Node<T>::getItem() const
{
	return item;
} 

template<typename T>
int Node<T> ::getpriority() const
{
	return priority ;
}


template < typename T>
Node<T>* Node<T>::getNext() const
{
	return next;
} 

#endif