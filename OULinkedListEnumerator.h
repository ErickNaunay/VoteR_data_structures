#ifndef OU_LINKED_LIST_ENUMERATOR_H
#define OU_LINKED_LIST_ENUMERATOR_H

#include <cstdlib> //used for the definition of NULL

#include "Enumerator.h"
#include "OULink.h"

template<typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
private:
	OULink<T>* current; //pointer to an object of the linked list
public:
	OULinkedListEnumerator(OULink<T>* first); //constructor
	OULinkedListEnumerator();
	virtual ~OULinkedListEnumerator();
	bool hasNext() const; //if there is a next elemet 
	T next(); //return an element and advance to the next position
	T peek() const; //return an element without advancing to the next position
};

template<typename T>
inline OULinkedListEnumerator<T>::OULinkedListEnumerator(OULink<T>* first) //contructor, set the pointer to a position in the oinked list
{

	current = first;

}

template<typename T>
inline OULinkedListEnumerator<T>::OULinkedListEnumerator() //default constructor
{

}

template<typename T>
inline OULinkedListEnumerator<T>::~OULinkedListEnumerator()
{

}

template<typename T>
inline bool OULinkedListEnumerator<T>::hasNext() const //return true if there is an next object 
{
	if (current != NULL && current->next != NULL)
	{
		return true;
	}

	return false;
}

template<typename T>
inline T OULinkedListEnumerator<T>::next()
{
	if (current == NULL)
		throw new ExceptionEnumerationBeyondEnd();

	T copyData = *(current->data);
	current = current->next;

	return copyData;
}

template<typename T>
inline T OULinkedListEnumerator<T>::peek() const
{
	if (current == NULL)
		throw new ExceptionEnumerationBeyondEnd();

	return *(current->data);

}

#endif // !OU_LINKED_LIST_ENUMERATOR_H
