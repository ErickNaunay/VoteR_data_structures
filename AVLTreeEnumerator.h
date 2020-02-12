#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <stack>
#include "Enumerator.h"
#include "AVLTreeOrder.h"
#include "AVLTree.h"

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
private:
	AVLTreeOrder order;
	std::stack<const AVLTree<T>*> traversalStack;
	void buildTraversalStack(const AVLTree<T>* current);
public:
	AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order);
	virtual ~AVLTreeEnumerator();
	bool hasNext() const;
	T next();						// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;					// throws ExceptionEnumerationBeyondEnd if no next item is available
	AVLTreeOrder getOrder();		// returns the order of this enumerator (preorder, inorder, or postorder)
};

// put implementation here

template<typename T>
inline void AVLTreeEnumerator<T>::buildTraversalStack(const AVLTree<T>* current) 
{
	if ((current->data == NULL && current->left == NULL && current->right == NULL) || current == NULL ) //verify if avl tree is empty
		return;
	
	//diferent order, diferent strategy to built the enumerator
	if (order == AVLTreeOrder::preorder) { // the root first, followed by the left and then the right
		
		traversalStack.push(current);
		
	}
	else if (order == AVLTreeOrder::inorder) { //the left first, then the root, finally the right

		traversalStack.push(current);
			
		buildTraversalStack(current->left);

	}
	else if (order == AVLTreeOrder::postorder) { // the left tree first, then the right, an finally the root
		
		traversalStack.push(current);
		
		if (!(current->left->data == NULL && current->left->right == NULL && current->left->left == NULL))
			buildTraversalStack(current->left);
		else if (!(current->right->data == NULL && current->right->right == NULL && current->right->left == NULL))
			buildTraversalStack(current->right);
		else
			return;

	}

}

template<typename T>
inline AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order) 
{
	if (root->data == NULL && root->right == NULL && root->left == NULL) //for an empty tree and an inexisten order, throws an exception
		throw new ExceptionAVLTreeAccess();
	if (!(order == AVLTreeOrder::inorder || order == AVLTreeOrder::preorder || order == AVLTreeOrder::postorder))
		throw new ExceptionAVLTreeAccess();

	this->order = order;
	buildTraversalStack(root);

}

template<typename T>
inline AVLTreeEnumerator<T>::~AVLTreeEnumerator()
{
	while (!traversalStack.empty()) {
		traversalStack.pop();
	}
}

template<typename T>
inline bool AVLTreeEnumerator<T>::hasNext() const
{
	if (traversalStack.empty())
		return false;

	return true;
}

template<typename T>
inline T AVLTreeEnumerator<T>::next()
{
	if (traversalStack.empty())
		throw new ExceptionEnumerationBeyondEnd();

	if (order == AVLTreeOrder::preorder) {

		const AVLTree<T> *currentTree = traversalStack.top();
		traversalStack.pop();

		if (!(currentTree->right->data == NULL && currentTree->right->left == NULL && currentTree->right->right == NULL)) {
			buildTraversalStack(currentTree->right);
		}

		if (!(currentTree->left->data == NULL && currentTree->left->left == NULL && currentTree->left->right == NULL)) {
			buildTraversalStack(currentTree->left);
		}

		return *(currentTree->data);

	}
	else if (order == AVLTreeOrder::inorder) {

		const AVLTree<T> *currentTree = traversalStack.top();
		traversalStack.pop();

		buildTraversalStack(currentTree->right);

		return *(currentTree->data);

	}
	else if (order == AVLTreeOrder::postorder) {

		const AVLTree<T> *currentTree = traversalStack.top();
		traversalStack.pop();

		if (!traversalStack.empty()) {

			const AVLTree<T> *parentTree = traversalStack.top();

			if (parentTree != NULL) {

				if (parentTree->left == currentTree) {
					buildTraversalStack(parentTree->right);
				}

			}

		}

		return *(currentTree->data);

	}

	return T();
}

template<typename T>
inline T AVLTreeEnumerator<T>::peek() const
{
	if (traversalStack.empty())
		throw new ExceptionEnumerationBeyondEnd();

	const AVLTree<T> *tempTree = traversalStack.top();

	return *(tempTree->data);
}

template<typename T>
inline AVLTreeOrder AVLTreeEnumerator<T>::getOrder()
{
	return order;
}

#endif // !AVL_TREE_ENUMERATOR