#ifndef TEMPLATED_ARRAY_H
#define TEMPLATED_ARRAY_H

#include <cstdlib>											//used to define NULL
#include "Exceptions.h"

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;			// capacity used in no arg constructor

template <typename T>
class TemplatedArray {

private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;	// maximum capacity, in items
	unsigned long size = 0;								// actual number of items currently in array
	T* data = NULL;										// pointer to array of any type
	void doubleCapacity();								//double the capacity of the array when its needed
	void halveCapacity();								//halve the capacitty of the array when its needed 

public:
	TemplatedArray();									// constructs array with default capacity
	TemplatedArray(const TemplatedArray<T> &copy);		//copy constructor 
	TemplatedArray(unsigned long capacity);				// constructs array with specified capacity

	void add(const T* item);							// adds item, increments size, doubles capacity as necessary
	void addAt(const T* item, unsigned long index);		// adds item at index, shifts following, doubles capacity as necessary
	void replaceAt(const T* item, unsigned long index);	// replaces item at index, otherwise unchanged
	void removeAt(unsigned long index);					// removes item at index, shifts following back
	void clear();										// leaves the array empty
	T get(unsigned long index) const;					// returns (copy of) item at index
	unsigned long getCapacity() const;					// returns the current capacity of the array
	unsigned long getSize() const;						// returns the current number of items in the array
	T operator[](unsigned long index) const;			// returns (copy of) item at index

	unsigned long count(T *key);						//count the number of repetitons of an item 

	virtual ~TemplatedArray();							// frees array space as object is deleted

};

template<typename T>
inline void TemplatedArray<T>::doubleCapacity()
{
	T *newRecordArray = new T[2 * getCapacity()]; //creates a new pointer (nvraRecord) of arrays, with double of previous size 

	if (newRecordArray == NULL) {
		throw new ExceptionMemoryNotAvailable();
	}

	for (unsigned long i = 0; i < getSize(); i++) { //copy all the data that was in the first array to the new one

		newRecordArray[i] = data[i];
	}

	this->capacity *= 2; //sets the new value of capacity

	delete[] data; //deallocate the memory of the first array

	data = newRecordArray; //sets the new direction in heap of the array
}

template<typename T>
inline void TemplatedArray<T>::halveCapacity()
{
	T *newRecordArray = new T[getCapacity() / 2]; //creates a new pointer (nvraRecord) of arrays, with half of previous size 

	if (newRecordArray == NULL) {
		throw new ExceptionMemoryNotAvailable();
	}

	for (unsigned long i = 0; i < getSize(); i++) { //copy all the data that was in the first array to the new one

		newRecordArray[i] = data[i];
	}

	this->capacity = capacity / 2; //sets the new value of capacity

	delete[] data; //deallocate the momory of the first array

	data = newRecordArray; //sets the new direction in heap of the array
}

template<typename T>
inline TemplatedArray<T>::TemplatedArray()
{
	data = new T[capacity];

	if (data == NULL)
		throw new ExceptionMemoryNotAvailable();

}

template<typename T>
inline TemplatedArray<T>::TemplatedArray(const TemplatedArray<T>& copy)
{
	this->size = 0;

	data = new T[copy.getCapacity()];

	if (data == NULL)
		throw new ExceptionMemoryNotAvailable();

	this->capacity = copy.getCapacity();
	this->size = copy.getSize();

	for (unsigned long i = 0; i < size; i++) {
		this->data[i] = copy.get(i);
	}
}

template<typename T>
inline TemplatedArray<T>::TemplatedArray(unsigned long capacity)
{
	data = new T[capacity];

	if (data == NULL)
		throw new ExceptionMemoryNotAvailable();

	this->capacity = capacity;
}

template<typename T>
inline void TemplatedArray<T>::add(const T * item)
{
	if (getSize() >= getCapacity()) { //if the size its equal to the capacity, then the size its doubled 
		doubleCapacity();
	}

	if (getSize() >= getCapacity())
		throw new ExceptionIndexOutOfRange();

	data[size] = *item;

	size++;
}

template<typename T>
inline void TemplatedArray<T>::addAt(const T * item, unsigned long index)
{
	if (index >= getSize() || index < 0) //expection handling 
		throw new ExceptionIndexOutOfRange();

	//poniter that points to an object into the array
	T *temp = new T;
	*temp = data[getSize() - 1]; //pointer to the last element

	add(temp);				//move the last element +1 position

	if (index == getSize() - 2) {	//if the index its the last in the object
		replaceAt(item, index);
	}
	else {					//any position in the array
		for (unsigned long i = getSize() - 2; i >= index; i--) {  // move one position to the right when the item its inserted in ind position (usage of a pointer because all the method need a pointer in the parameter)
			if (i != index) {
				*temp = data[i - 1];
				replaceAt(temp, i);
			}
			else { // if every element has moved to the right we add the record in the index position 
				replaceAt(item, index);
			}
		}
	}
	delete temp;
}

template<typename T>
inline void TemplatedArray<T>::replaceAt(const T* item, unsigned long index)
{
	if (index >= getSize() || index < 0) //exception handling 
		throw new ExceptionIndexOutOfRange();

	data[index] = *item;
}

template<typename T>
inline void TemplatedArray<T>::removeAt(unsigned long index)
{
	if (index >= getSize() || index < 0) //exception handling 
		throw new ExceptionIndexOutOfRange();

	//we need one pointer that points to the object that is gpoing to be removed an the elements that are goinf to be moved to the left 
	T *temp = new T;

	if (index == getSize() - 1)
	{
		--size;
	}
	else {
		for (unsigned long i = index; i < size - 1; i++) { //iterate to move the elements one position to the left to cover the space that was remove
			*temp = data[i + 1];
			replaceAt(temp, i);
		}
		--size;
	}

	if (size <= (capacity / 2) && capacity > DEFAULT_ARRAY_CAPACITY) { //halving if the size its in a domain diferent than the capacity

		halveCapacity();

	}

	delete temp;
}

template<typename T>
inline void TemplatedArray<T>::clear()
{
	delete[] data;
	capacity = DEFAULT_ARRAY_CAPACITY;
	size = 0;

	data = new T[capacity];

}

template<typename T>
inline T TemplatedArray<T>::get(unsigned long index) const
{
	if (index >= getSize() || index < 0) //excpetion handling 
		throw new ExceptionIndexOutOfRange();

	T temp = data[index];

	return temp;
}

template<typename T>
inline unsigned long TemplatedArray<T>::getCapacity() const
{
	return capacity;
}

template<typename T>
inline unsigned long TemplatedArray<T>::getSize() const
{
	return size;
}

template<typename T>
inline T TemplatedArray<T>::operator[](unsigned long index) const
{
	if (index >= getSize() || index < 0)
		throw new ExceptionIndexOutOfRange();
	T temp = data[index];

	return temp;
}

template<typename T>
inline unsigned long TemplatedArray<T>::count(T *key) //fuction that counts the repetition of a specific value in the array
{
	unsigned long count = 0;

	for (unsigned long i = 0; i < getSize(); i++) {
		if (get(i) == *key)
			count++;
	}

	return count;
}

template<typename T>
inline TemplatedArray<T>::~TemplatedArray()
{
	delete[] data;
}

#endif // !TEMPLATED_ARRAY_H



