#ifndef OU_LINK_H
#define OU_LINK_H

#include <cstdlib> //used for the definition of NULL
#include "Exceptions.h"

template <typename T> class OULinkedList;
template <typename T> class OULinkedListEnumerator;

template<typename T>
class OULink
{
	//template <typename T>
	friend class OULinkedList<T>;
	//template <typename T>
	friend class OULinkedListEnumerator<T>;

private:
	T * data = NULL;
	OULink* next = NULL;

public:
	OULink(const T* item);
	virtual ~OULink();
};

template <typename T>
inline OULink<T>::OULink(const T * item) //constructor for the object 
{
	data = new T;

	if (data == NULL)
		throw new ExceptionMemoryNotAvailable();

	*data = *item;
}

template <typename T>
inline OULink<T>::~OULink() //destructor for the object, it deletes by chain
{
	if (data != NULL && next != NULL) {

		delete data;
		delete next;

		data = NULL;
		next = NULL;

	}
	else if (data != NULL) {

		delete data;

		data = NULL;
	}
}

#endif // !OU_LINK_H