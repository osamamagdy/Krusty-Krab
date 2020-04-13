#pragma once
#include "TNode.h"
#include <iostream>
using namespace std;
template<typename T>

class BSTree
{
private:
	TNode<T>* root;


	static void rec_insertBST(TNode<T>*& subRoot, T key); // A recursive function to insert a new value to an empty subtree
	//We traverse this subtree and go to other subtree according to features of BST untill we reach the last leave


	//////// 3 different methods to traverse a Tree/////////
	static void rec_inorder_traverse(TNode<T>* subRoot);

	static void rec_preorder_traverse(TNode<T>* subRoot);

	static void rec_postorder_traverse(TNode* subRoot);

	/////////////////////////////////////////////////////////



	static TNode* rec_find_max(TNode<T>* subRoot); //////////////// The Maximum value in tree

	static TNode* rec_find_min(TNode<T>* subRoot); //////////////// The Minimum value in tree

	static int rec_max_depth(TNode* subRoot);  ///////// The max depth of a subtree (its Height)

	static TNode<T>* rec_search(T val, TNode* subRoot); ////////Searching for a certain value in a tree

	static void rec_destroy_tree(TNode*& subRoot);   /////// for the destructor


	static bool rec_delete_val(TNode<T>* subRoot, T val); ///////////Deleting a Node with certain value (returns false if not found)


public:  /// Usually they just use the Utility functions but with root passed in the first call

	BSTree<T>(void);

	void insertBST(T key); // not const

	void inorder_traverse() const;

	void preorder_traverse() const;

	void postorder_traverse() const;

	TNode<T>* find_max() const; // return a pointer to the node that hold the maximum value in binary search tree.

	TNode<T>* find_min() const; // return a pointer to the node that hold the minimum value in binary search tree.

	void destroy_tree(); // not const

	int max_depth() const;

	TNode* search(T val) const;

	bool delete_val(T val);

	~BSTree<T>(void);

};

// Utility Functions - Recursive Functions --------------------------
// Insert a key in a binary search tree.
// [Note]: subRoot is passed by ref because 
//         the new node may be inserted in the subRoot (if subRoot is empty nullptr)
template<typename T>
void BSTree<T>::rec_insertBST(TNode<T>*& subRoot, T key)
{
	if (subRoot == nullptr)
		subRoot = new TNode<T>(key); // this line is why we pass subRoot by reference (subRoot = something)
	else if (subRoot->getdata() < key)
	{

		//recursively call insert for the right subtree
		rec_insertBST(subRoot->getright(), key);

	}
	else
	{
		// recursive call on left
		rec_insertBST(subRoot->getleft(), key);
	}
}

//* print the tree with inorder traversal.
template<typename T>
void BSTree<T>::rec_inorder_traverse(TNode<T>* subRoot)
{
	if (subRoot == nullptr) return;
	rec_inorder_traverse(subRoot->getleft());
	cout << subRoot->getdata() << " ";   /////// This will cause a runtime error if we deal with classes
	rec_inorder_traverse(subRoot->getright());
}

//* print the tree with preorder traversal.
template<typename T>
void BSTree<T>::rec_preorder_traverse(TNode<T>* subRoot)
{
	if (subRoot == nullptr) return;
	cout << subRoot->getdata() << "  ";    /////// This will cause a runtime error if we deal with classes
	rec_preorder_traverse(subRoot->getleft());
	rec_preorder_traverse(subRoot->getright());
}

//* print the tree with postorder traversal.
template<typename T>
void BSTree<T>::rec_postorder_traverse(TNode<T>* subRoot)
{
	if (subRoot == nullptr) return;
	rec_postorder_traverse(subRoot->getleft());
	rec_postorder_traverse(subRoot->getright());
	cout << subRoot->getdata() << "  "; /////// This will cause a runtime error if we deal with classes
}


//* Deletes all the nodes in the tree.
// [Note]: subRoot is passed by ref because 
//         after deleting subRoot node we will make it point to nullptr (change subRoot)
template<typename T>
void BSTree<T>::rec_destroy_tree(TNode<T>*& subRoot)
{
	if (subRoot != nullptr)
	{
		// recursive call on left
		rec_destroy_tree(subRoot->getleft());

		// recursive call on right
		rec_destroy_tree(subRoot->getright());

		delete subRoot;
		subRoot = nullptr; // this line is why we pass subRoot by reference (subRoot = something)
	}
}




//* return a pointer to the node that hold the maximum value in binary search tree of the passed subRoot.
template <typename T>
TNode<T>* BSTree<T>::rec_find_max(TNode<T>* subRoot)
{
	if (subRoot->getright() == nullptr)
	{
		return subRoot;
	}
	return rec_find_max(subRoot->getright());
}

//* return a pointer to the node that hold the minimum value in binary search tree of the passed subRoot.
template <typename T>
TNode<T>* BSTree<T>::rec_find_min(TNode<T>* subRoot)
{
	if (subRoot->getleft() == nullptr)
	{
		return subRoot;
	}
	return rec_find_min(subRoot->getleft());
}

//* return the depth of binary search tree of the passed subRoot.
template <typename T>
int BSTree<T>::rec_max_depth(TNode<T>* subRoot)
{
	if (!subRoot)
		return 0;
	int Ldepth = 1 + rec_max_depth(subRoot->getleft());
	int Rdepth = 1 + rec_max_depth(subRoot->getright());
	if (Ldepth > Rdepth) return Ldepth;
	return Rdepth;

}

//* returns a pointer to the Node of the passed value and NULL if not found
template <typename T>
TNode<T>* BSTree<T>::rec_search(T val, TNode<T>* subRoot)
{
	if (!subRoot) return nullptr;
	if (val == subRoot->getdata())
		return subRoot;
	if (val < subRoot->getdata())
		return rec_search(val, subRoot->getleft());
	if (val > subRoot->getdata())
		return rec_search(val, subRoot->getright());



}

///////////////Need to edit this acordind to the type of data we search for or by function overriden or we can make operator overloading for the used class itself when needed

template <typename T>
bool BSTree<T> ::rec_delete_val(TNode<T>* subRoot, T val)
{
	if (!subRoot)  ////////Reached the leave without finding the node or empty tree
		return false;


	if (val > subRoot->getdata()) /// The value is greater so we need to go for bigger values of the tree ----> the right child
		return rec_delete_val(subRoot->getright(), val);
	else if (val < subRoot->getdata()) ///////////// The value is smaller so we need to go for smaller values of the tree ---------> the left child
		return rec_delete_val(subRoot->getleft(), val);
	else
	{
		if (!subRoot->getleft()) ///---------> it has one child at most (maybe none) , it is the right child so we delete it and substitute with the right child
		{
			TNode<T>* temp = subRoot->getright();
			delete subRoot;
			subRoot = temp;
			return true;
		}

		else if (!subRoot->getright()) ///---------> it has one child at most (maybe none) , it is the left child so we delete it and substitute with the left child
		{
			TNode<T>* temp = subRoot->getleft();
			delete subRoot;
			subRoot = temp;
			return true;
		}

		else ////////////----------> it has two childs and maybe both of them are subTrees
		{
			TNode<T>* Largest = rec_find_max(subRoot->getleft()); /// we search for the largest val in the leftsubtree (we may search for the min in rightsubtree)
			
			subRoot->setdata(Largest->getdata());  /// We exchange the data it self (the same as deleting the node) but we need to delete the largest so we decrement thecount of nodes

			return rec_delete_val(subRoot->getleft(), Largest->getdata()); //////////As Largest is copy of the pointer to the the largest we want to delete the original and make it nullptr to actually delete the prev node 
			
		}


	}
}










template<typename T>
BSTree<T>::BSTree()  ////////////Constructor
{
	root = nullptr;
}

template<typename T>
BSTree<T>::~BSTree() ////////// Destructor
{
	destroy_tree();
}






// Public Functions ---------------------------------

template <typename T>
void BSTree<T>::insertBST(T key) // not const
{
	// not const because rec_insertBST takes the root by reference, so may change it
	rec_insertBST(root, key);
}


template <typename T>
void BSTree<T>::inorder_traverse() const
{
	rec_inorder_traverse(root);
}


template<typename T>
void BSTree<T>::preorder_traverse() const
{
	rec_preorder_traverse(root);
}

template<typename T>
void BSTree<T>::postorder_traverse() const
{
	rec_postorder_traverse(root);
}

template <typename T>
void BSTree<T>::destroy_tree() // not const
{
	// not const because rec_destroy_tree takes the root by reference, so may change it
	rec_destroy_tree(root);
}


template <typename T>
TNode<T>* BSTree<T>::find_max() const // return a pointer to the node that hold the maximum value in binary search tree.
{

	return rec_find_max(root);
}

template <typename T>
TNode<T>* BSTree<T>::find_min() const // return a pointer to the node that hold the minimum value in binary search tree.
{
	return rec_find_min(root);
}

template<typename T>
int BSTree<T>::max_depth() const
{

	return rec_max_depth(root);
}

template<typename T>
TNode<T>* BSTree<T>::search(T val) const
{
	return rec_search(val, root);
}


template<typename T>
bool BSTree<T> ::delete_val(T val)
{
	return  rec_delete_val(root, val);
}

