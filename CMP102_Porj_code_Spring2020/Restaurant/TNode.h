#pragma once

template <typename T>
class TNode
{
private:
	T data;
	TNode* left;
	TNode* right;
public:
	TNode(T val);
	void setdata(T d);
	T getdata();
	void setright(TNode* p);
	TNode*& getright();
	//returns a pointer by reference
	//Reason for that is explained in file BSTree.cpp as a note written
	//inside the body of function BSTree::rec_insertBST

	void setleft(TNode* p);
	TNode*& getleft();



};
template<typename T>
TNode<T>::TNode(T val)
{
	data = val;
	left = right = nullptr;
}

template<typename T>
void TNode<T>::setdata(T d)
{
	data = d;
}



template<typename T>
T TNode<T>::getdata()
{
	return data;
}



template<typename T>
void TNode<T>::setleft(TNode<T>* p)
{
	left = p;
}


template<typename T>
TNode<T>*& TNode<T>::getleft()
{
	return left;
}


template<typename T>
void TNode<T>::setright(TNode<T>* p)
{
	right = p;
}

template <typename T>
TNode<T>*& TNode<T>::getright()
{
	return right;
}
