#ifndef AVL_TREE
#define AVL_TREE

#include "Comparator.h"
#include <cstdlib>

template <typename T>
class AVLTree {

	template <typename U>
	friend class AVLTreeEnumerator;

private:
	Comparator<T>* comparator = NULL;				// used to determine tree order and item equality
	unsigned long size = 0;							// actual number of items currently in tree
	T* data = NULL;									// pointer to data at this node
	int diff = 0;									// height of right minus height of left
	AVLTree<T>* left = NULL;						// pointer to left subtree
	AVLTree<T>* right = NULL;						// pointer to right subtree
	void zig();										// right rotation
	void zag();										// left rotation
	void zigzag();									// left rotation on left subtree, followed by right rotation
	void zagzig();									// right rotation on right subtree, followed by left rotation
	void rebalance();								// check for and rebalance this node, if needed

public:
	
	AVLTree(Comparator<T>* comparator);				// creates empty linked tree with comparator
	virtual ~AVLTree();								// deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave tree unchanged and return false
	bool insert(const T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool replace(const T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool remove(const T* item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionLinkedtreeAccess
	T find(const T* item) const;

	unsigned long getSize() const;				// returns the current number of items in the tree
};

// add public functions here

template<typename T>
inline AVLTree<T>::AVLTree(Comparator<T>* comparator) //tree constructor
{
	this->comparator = comparator;
}

template<typename T>
inline AVLTree<T>::~AVLTree() //tree destructor (recursive deletion)
{
	if (data != NULL) {
		
		delete data;
	}

	data = NULL;

	if (right != NULL) {
		delete right;
	}

	right = NULL;

	if (left != NULL) {
		delete left;
	}
	
	right = NULL;

}

template<typename T>
inline bool AVLTree<T>::insert(const T * item) //insert method using recursion
{
	if (data == NULL && left == NULL && right == NULL) { // if the correct position it is reached the item is inserted, insert data in an empty tree

		data = new T;
		*data = *item;

		left = new AVLTree<T>(comparator);
		right = new AVLTree<T>(comparator);

		diff = 0;
		size++;

		return true;

	}

	if (comparator->compare(*data, *item) == 0) { // if there is an item equal, return false, also returning false recursively 
		return false;
	}

	int olddiff;

	if (comparator->compare(*data, *item) == 1) { //if the item is less than the node(tree)

		if (left->data == NULL && left->left == NULL && left->right == NULL) { //if the tree is empty, insert 

			if (left->insert(item)) {
				
				diff--;
			}
			else
				return false;

		}
		else { //if is not empty, keep track of the old difference for the correct rebalance 

			olddiff = left->diff;
			
			if (left->insert(item)) {

				if (olddiff != left->diff && left->diff != 0)
					diff--;

			}
			else
				return false;
		}

		size++;
	}
	else if (comparator->compare(*data,*item) == -1) { //if the item is greater than the node(tree)

		if (right->data == NULL && right->left == NULL && right->right == NULL) { //if the tree is empty, insert 

			if (right->insert(item)) {

				diff++;
			}
			else
				return false;

		}
		else { //if is not empty, keep track of the old difference for the correct rebalance 

			olddiff = right->diff;

			if (right->insert(item)) {
				
				if (olddiff != right->diff && right->diff != 0)
					diff++;

			}
			else
				return false;

		}

		size++;

	}

	rebalance(); //rebalance each tree that needs it 

	return true;
}

template<typename T>
inline bool AVLTree<T>::replace(const T * item) //with recursion 
{
	if (data == NULL && left == NULL && right == NULL) //if a empty tree it is reached (no item found for replace)
		return false;

	if (comparator->compare(*data, *item) == 0) { //if an item equal it is found, the replace action it is done
		
		*data = *item;

		return true;

	}
	else if (comparator->compare(*data, *item) == 1) { // search to the left tree if the root is greater than the root
		
		if (left->replace(item) == false) {
		
			return false;
		
		}
	}
	else if (comparator->compare(*data, *item) == -1) { //search in the right tree if the item is greater than the root

		if (right->replace(item) == false) {
			
			return false;

		}
	}

	return true;
}

template<typename T>
inline bool AVLTree<T>::remove(const T * item) //method done with recursion strategy
{
	if (data == NULL && right == NULL && left == NULL && diff == 0) { //if an empty tree it is reached, the function return false
		
		return false;

	}

	int olddiff;

	if (comparator->compare(*data, *item) == 1) { //if the tree is greater than than the item, search for the left subtree
	
		olddiff = left->diff;

		if (left->remove(item)) { //remove recursively, keep track of the difference, comparing the old difference and the new one

			if ((left->data == NULL && left->left == NULL && left->right == NULL && left->diff == 0) || (left->diff != olddiff && left->diff == 0)) {
				diff++;
			}
		
		}
		else
			return false;

	}
	else if (comparator->compare(*data, *item) == -1) { //if the item is greater than the tree, search for the rigth subtree

		olddiff = right->diff;

		if (right->remove(item)) { //remove recursively, keep track of the difference, comparing the old difference and the new one
			
			if ((right->data == NULL && right->left == NULL && right->right == NULL && right->diff == 0) || (right->diff != olddiff && right->diff == 0)) {
				diff--;
			}
			
		}
		else
			return false;

	}
	else { //if the item it is reached, remove the tree

		if (right->data == NULL && right->left == NULL && right->right == NULL && right->diff == 0) { // if the tree has an empty right child (also handle leaf trees)

			AVLTree<T> *oldLeftTree = left;
			delete right;
			delete data;

			if (!(left->data == NULL && left->left == NULL && left->right == NULL && left->diff == 0)) { //if the left tree is not empty, replace the data from the left tree to the root reached
				data = new T;
				*data = *(left->data);

				left = left->left;
				right = left->right;
			}
			else { // if the left tree is empty (the tree reached is a leaf)

				data = NULL;
				left = NULL;
				right = NULL;

			}
			//set to null the old tree (original left)
			oldLeftTree->data = NULL;
			oldLeftTree->right = NULL;
			oldLeftTree->left = NULL;

			delete oldLeftTree; //delete old tree

			diff = 0;


		}
		else if (left->data == NULL && left->left == NULL && left->right == NULL && left->diff == 0) { //same proccess as above, but handle if the tree has an empty left subtree

			AVLTree<T> *oldRightTree = right;

			delete left;
			delete data;

			if (!(right->data == NULL && right->left == NULL && right->right == NULL && right->diff == 0)) {
				data = new T;
				*data = *(right->data);

				left = right->left;
				right = right->right;
			}

			oldRightTree->data = NULL;
			oldRightTree->right = NULL;
			oldRightTree->left = NULL;

			delete oldRightTree;

			diff = 0;
	
		}
		else { //if the tree reached for removal, has both, left and right subtress

			AVLTree<T> *succTree = right;

			while (!(succTree->left->data == NULL && succTree->left->left == NULL && succTree->left->right == NULL && succTree->left->diff == 0)) { //get the inorder succersor for the tree removal (left most right)

				succTree = succTree->left;

			}

			//when the tree succesor has been located, then replace the data from tree removal with the sucessor
			delete data;
			data = new T;
			*data = *(succTree->data);

			olddiff = right->diff;

			if (right->remove(succTree->data)) { //remove the sucessor using recursion (it might have a right subtree or it can be a leaf)

				if ((right->data == NULL && right->left == NULL && right->right == NULL && right->diff == 0) || (right->diff != olddiff && right->diff == 0)) {

					diff--;

				}

			}
			else
				return false;

		}	
	}

	size--;

	rebalance();

	return true;
}

template<typename T>
inline T AVLTree<T>::find(const T * item) const //find method usign recursion 
{
	T temp_obj;

	if (data == NULL && left == NULL && right == NULL) //if an empty tree it is reached, trhow an exception
		throw new ExceptionAVLTreeAccess();

	if (comparator->compare(*data, *item) == 0) { //if an equal item it is found, return the data from the tree

		return *data;

	}
	else if (comparator->compare(*data, *item) == 1) { //search recursively right if the tree if the item is greater
		
		try {

			temp_obj = left->find(item);
		
		}
		catch (ExceptionAVLTreeAccess *e) {
			
			throw new ExceptionAVLTreeAccess();
			delete e;

		}

	}
	else if (comparator->compare(*data, *item) == -1) { //search recursively right if the tree if the item is lower

		try {

			temp_obj = right->find(item);

		}
		catch (ExceptionAVLTreeAccess *e) {

			throw new ExceptionAVLTreeAccess();
			delete e;

		}

	}

	return temp_obj;
}

template<typename T>
inline unsigned long AVLTree<T>::getSize() const
{
	return size;
}

// add additional private functions here

template<typename T>
inline void AVLTree<T>::zig()
{
	if (left == NULL) {  // no left, no way to zig
		return;
	}
	// keep track of diff of node and left for changes
	int gdiff = diff;
	int pdiff = left->diff;

	// modify the tree
	AVLTree<T>* olnr = left;  // olnr is "old left, new right"
	left = olnr->left;
	olnr->left = olnr->right;
	olnr->right = right;
	right = olnr;

	// note that the modifications kept the node itself in place, so we need to swap its data with its olnr's
	T* tempData = data;
	data = olnr->data;
	olnr->data = tempData;

	// update the diff fields for node and new right
	if (pdiff < 0) {  // if imbalance was outside left heavy (left-left violation)
		diff = gdiff + 2;
		right->diff = gdiff - pdiff + 1;
	}
	else {  // otherwise imbalance was inside left heavy (left-right violation)
		diff = pdiff + 1;
		right->diff = gdiff + 1;
	}
}

template<typename T>
inline void AVLTree<T>::zag()
{
	if (right == NULL) {  // no right, no way to zig
		return;
	}
	// keep track of diff of node and right for changes
	int gdiff = diff;
	int pdiff = right->diff;

	// modify the tree
	AVLTree<T>* ornl = right;  // ornl is "old right, new left"
	right = ornl->right;
	ornl->right = ornl->left;
	ornl->left = left;
	left = ornl;

	// note that the modifications kept the node itself in place, so we need to swap its data with its ornl's
	T* tempData = data;
	data = ornl->data;
	ornl->data = tempData;

	// update the diff fields for node and new left
	if (pdiff > 0) {  // if imbalance was outside right heavy (right-right violation)
		diff = gdiff - 2;
		left->diff = gdiff - pdiff - 1;
	}
	else {  // otherwise imbalance was inside right heavy (right-left violation)
		diff = pdiff - 1;
		left->diff = gdiff - 1;
	}
}

template<typename T>
inline void AVLTree<T>::zigzag() 
{
	left->zag();
	zig();
}

template<typename T>
inline void AVLTree<T>::zagzig()
{	
	right->zig();
	zag();
}

template<typename T>
inline void AVLTree<T>::rebalance() //taking kare of the difference of the right or left subtrees
{
	if (diff >= -1 && diff <= 1)
		return;

	if (diff < 0 && left->diff <= 0)
		zig();
	else if (diff < 0 && left->diff > 0)
		zigzag();
	else if (diff > 0 && right->diff >= 0)
		zag();
	else if (diff > 0 && right->diff < 0)
		zagzig();

}

#endif // !AVL_TREE