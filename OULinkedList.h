#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST

#include <cstdlib>

#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"

// OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
// order (based on the comparator provided to it when the list is created) and that only contains unique
// items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {

private:
	Comparator<T>* comparator = NULL;				// used to determine list order and item equality
	unsigned long size = 0;							// actual number of items currently in list
	OULink<T>* first = NULL;						// pointer to first link in list
	OULink<T>* last = NULL;							// pointer to last link in list
public:
	OULinkedList(Comparator<T>* comparator);		// creates empty linked list with comparator
	virtual ~OULinkedList();						// deletes all links and their data items

													// if an equivalent item is not already present, insert item in order and return true
													// if an equivalent item is already present, leave list unchanged and return false
	bool insert(const T* item);

	// if item is greater than item at last, append item at end and return true
	// if item is less than or equal to item at last, leave list unchanged and return false
	bool append(const T* item);  // now const

								 // if an equivalent item is already present, replace item and return true
								 // if an equivalent item is not already present, leave list unchanged and return false
	bool replace(const T* item);  // now const

								  // if an equivalent item is already present, remove item and return true
								  // if an equivalent item is not already present, leave list unchanged and return false
	bool remove(const T* item);  // now const

								 // if any items are present, return a copy of the first item
								 // if no items are present, throw new ExceptionLinkedListAccess
	T get() const;

	// if an equivalent item is present, return a copy of the first such item
	// if an equivalent item is not present, throw a new ExceptionLinkedListAccess
	T find(const T* item) const;

	unsigned long getSize() const;					// returns the current number of items in the list

	OULinkedListEnumerator<T> enumerator() const;	// create an enumerator for this linked list
};

template<typename T>
inline OULinkedList<T>::OULinkedList(Comparator<T>* comparator)
{
	this->comparator = comparator;
}

template<typename T>
inline bool OULinkedList<T>::insert(const T * item)
{
	OULink<T> *currentItem = first;
	OULink<T> *predItem = NULL;

	OULink<T> *itemLink = new OULink<T>(item);

	if (itemLink == NULL)
		throw new ExceptionMemoryNotAvailable();

	if (first == NULL && last == NULL) { //if the list is empty

		first = itemLink;
		last = itemLink;

		++size;

		return true;

	}
	else { //insert the item when the item is less than the object in the list 

		while (currentItem != NULL) {

			if (comparator->compare(*(currentItem->data), *item) == 0) {

				delete itemLink;

				return false;

			}
			else if (comparator->compare(*(currentItem->data), *item) == -1) {

				if (currentItem == last) {

					last->next = itemLink;
					last = itemLink;

					++size;

					return true;
				}
				else {

					predItem = currentItem;
					currentItem = currentItem->next;

				}

			}
			else if (comparator->compare(*(currentItem->data), *item) == 1) {

				if (currentItem == first) {
					itemLink->next = first;
					first = itemLink;

					++size;

					return true;

				}
				else {

					itemLink->next = currentItem;
					predItem->next = itemLink;

					++size;

					return true;

				}
			}
		}
	}

	return false;
}

template<typename T>
inline bool OULinkedList<T>::append(const T* item)
{
	OULink<T> *itemLink = new OULink<T>(item);

	if (itemLink == NULL)
		throw new ExceptionMemoryNotAvailable();

	if (first == NULL) {

		first = itemLink;
		last = itemLink;

		++size;

		return true;
	}
	else {

		if (comparator->compare(*(last->data), *(itemLink->data)) == -1) {
			last->next = itemLink;
			last = itemLink;

			++size;

			return true;

		}
	}

	delete itemLink;

	return false;
}

template<typename T>
inline bool OULinkedList<T>::replace(const T * item)
{

	OULink<T> *predItem = NULL; //predecesor item
	OULink<T> *currentItem = first;

	OULink<T> *itemLink = new OULink<T>(item);

	if (itemLink == NULL)
		throw new ExceptionMemoryNotAvailable();

	while (currentItem != NULL) { //linear search until the item it is found, otherwise there is no change 

		if (comparator->compare(*(currentItem->data), *item) == 0) { //if the item it is found 

			if (currentItem == first) { //take care of if is the first element

				itemLink->next = first->next;

				first = itemLink;

				currentItem->next = NULL;

				if (currentItem == last) //if there is one element (first and last pointer points to the same object)
					last = itemLink;

				delete currentItem;

				return true;

			}
			else { //if the element is other 

				itemLink->next = currentItem->next;


				predItem->next = itemLink;

				if (currentItem == last)
					last = itemLink;

				currentItem->next = NULL;

				delete currentItem;

				return true;
			}

		}

		predItem = currentItem;
		currentItem = currentItem->next;
	}

	delete itemLink;
	return false;
}

template<typename T>
inline bool OULinkedList<T>::remove(const T * item) //look if just one element deletes not the hole list
{
	OULink<T> *currentItem = first;
	OULink<T> *predItem = NULL;

	while (currentItem != NULL) { //linear serach until the item it is found 

		if (comparator->compare(*(currentItem->data), *item) == 0) {

			if (currentItem == first) { //take car if th eitem its the first 
				first = currentItem->next;
				currentItem->next = NULL;

				if (currentItem == last) { //if there is just one element
					last = NULL;
				}

				--size;

			}
			else if (currentItem == last) { //if it is the last 
				predItem->next = NULL;
				last = predItem;

				--size;
			}
			else { //any element in the middle of the list

				predItem->next = currentItem->next;
				currentItem->next = NULL;

				--size;
			}

			delete currentItem;
			return true;

		}

		predItem = currentItem;
		currentItem = currentItem->next;
	}

	//predItem = NULL;
	currentItem = NULL;

	return false;
}

template<typename T>
inline T OULinkedList<T>::get() const
{
	if (first == NULL)
		throw new ExceptionLinkedListAccess();

	return *(first->data);
}

template<typename T>
inline T OULinkedList<T>::find(const T * item) const
{

	OULink<T> *currentItem = first;
	T copyData;

	bool found = false;

	if (first == NULL)
		throw new ExceptionLinkedListAccess();

	while (currentItem != NULL) { //linear search to find an item 

		if (comparator->compare(*(currentItem->data), *item) == 0) { //use of the comparator to verify 

			found = true;

			copyData = *(currentItem->data);

			return copyData;
		}

		currentItem = currentItem->next;
	}

	if (found == false)
		throw new ExceptionLinkedListAccess();

	return *(currentItem->data);
}

template<typename T>
inline unsigned long OULinkedList<T>::getSize() const
{
	return size;
}

template<typename T>
inline OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const
{
	return OULinkedListEnumerator<T>(first);
}

template<typename T>
inline OULinkedList<T>::~OULinkedList() //delete the pointer to the first element
{
	if (first != NULL && last != NULL)
	{
		delete first;
	}

}

#endif // !OU_LINKED_LIST_H
