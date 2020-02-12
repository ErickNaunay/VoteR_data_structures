#ifndef HASH_TABLE
#define HASH_TABLE

#include <cstdlib>

#include "Exceptions.h"
#include "Comparator.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"

const unsigned int SCHEDULE_SIZE = 25;							// the number of items in the size schedule
const unsigned int SCHEDULE[SCHEDULE_SIZE] = { 1, 2, 5, 11, 23, 53, 107, 223, 449, 907, 1823, 3659, 7309, 14621, 29243, 58511, 117023, 234067, 468157, 936319, 1872667, 3745283, 7490573, 14981147, 29962343 };		// the size schedule (all primes)
const unsigned int DEFAULT_SCHEDULE_INDEX = 3;					// the default position in the size schedule
const unsigned long DEFAULT_BASE_CAPACITY = SCHEDULE[DEFAULT_SCHEDULE_INDEX]; 	// the default size of the array
const float DEFAULT_MAX_LOAD_FACTOR = 0.9f;						// the default load factor used to determine when to increase the table size
const float DEFAULT_MIN_LOAD_FACTOR = 0.4f;						// the default load factor used to determine when to decrease the table size

template<typename T>
class HashTable
{
	template <typename U>
	friend class HashTableEnumerator;

private:
	Comparator<T>* comparator = NULL;						// used to determine item equality
	Hasher<T>* hasher = NULL;								// used to compute hash value
	unsigned long size = 0;									// actual number of items currently in hash table
	float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;			// the load factor used to determine when to increase the table size
	float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR;			// the load factor used to determine when to decrease the table size
	unsigned int scheduleIndex = DEFAULT_SCHEDULE_INDEX;	// the index of current location in the size schedule
	unsigned long baseCapacity = DEFAULT_BASE_CAPACITY;		// the size of the array
	unsigned long totalCapacity = baseCapacity;				// the size of the array plus chains of more than one link
	OULinkedList<T>** table = NULL;							// table will be an array of pointers to OULinkedLists of type T

	OULinkedList<T>** initializeHashTale(unsigned long newBaseCapacity);
	void copyTable(OULinkedList<T>** table, unsigned long newCapacity);
	void expandTable();
	void contractTable();
	void deleteTable();

public:

	HashTable(Comparator<T>* comparator, Hasher<T>* hasher);			// creates an empty table of DEFAULT_BASE_CAPACITY
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
		// if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
		unsigned long size,
		float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR,
		float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR);
	virtual ~HashTable();

	// if an equivalent item is not already present, insert item at proper location and return true
	// if an equivalent item is already present, leave table unchanged and return false
	bool insert(T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool replace(T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool remove(T* item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionHashTableAccess
	T find(const T* item) const;

	unsigned long getSize() const;						// returns the current number of items in the table
	unsigned long getBaseCapacity() const;				// returns the current base capacity of the table
	unsigned long getTotalCapacity() const;				// returns the current total capacity of the table
	float getLoadFactor() const;						// returns the current load factor of the table
	unsigned long getBucketNumber(const T* item) const;	// returns the bucket number for an item using its hash function mod array size
};

template<typename T>
inline OULinkedList<T>** HashTable<T>::initializeHashTale(unsigned long newBaseCapacity) { //initialize all the objects linked linst from each bucket

	OULinkedList<T>**newTable = new OULinkedList<T> *[newBaseCapacity];

	if (newTable == NULL)
		throw new ExceptionMemoryNotAvailable();

	for (unsigned long i = 0; i < newBaseCapacity; i++) { // all set to null, for memory efficence

		newTable[i] = NULL;

	}

	return  newTable;
}

template<typename T>
inline HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher) //constructor with default table size
{
	this->comparator = comparator;
	this->hasher = hasher;

	table = initializeHashTale(baseCapacity);

}

template<typename T>
inline HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher, unsigned long size, float maxLoadFactor, float minLoadFactor) //constructor with a given size, max and min loadfactor
{
	this->comparator = comparator;
	this->hasher = hasher;

	this->maxLoadFactor = maxLoadFactor;
	this->minLoadFactor = minLoadFactor;

	float temp_loadFactor = 0.0;

	for (unsigned int i = 0; i < SCHEDULE_SIZE; i++) { //get the correct size of the table for the size and max load factor

		temp_loadFactor = (float)size / (float)SCHEDULE[i];

		if (size < SCHEDULE[i] && temp_loadFactor < maxLoadFactor) { //EXCLUSIVE LOAD FACTOR 

			scheduleIndex = i;
			baseCapacity = SCHEDULE[scheduleIndex];
			totalCapacity = baseCapacity;

			table = initializeHashTale(baseCapacity);

			break;
		}
	}
}

template<typename T>
inline HashTable<T>::~HashTable() //destructor
{
	deleteTable();
}

template<typename T>
inline void HashTable<T>::copyTable(OULinkedList<T>** newTable, unsigned long newCapacity) //copy the content of table in newTable, with rehashing
{
	OULinkedList<T>* temp_LL = NULL;
	T *temp_Obj = new T;

	for (unsigned long i = 0; i < baseCapacity; i++) { //copy from the original table into the new one, all elements

		if (table[i] != NULL) {

			temp_LL = table[i];
			OULinkedListEnumerator<T> enumerator_LL = temp_LL->enumerator();

			for (unsigned int j = 0; j < temp_LL->getSize(); j++) { //copying if there was any collision 

				if (enumerator_LL.hasNext()) {

					*temp_Obj = enumerator_LL.next();

					unsigned long copyBucketNumber = hasher->hash(*temp_Obj) % newCapacity; //rehashing

					if (newTable[copyBucketNumber] == NULL)
						newTable[copyBucketNumber] = new OULinkedList<T>(comparator);

					if (newTable[copyBucketNumber]->insert(temp_Obj) == true) {
						if (newTable[copyBucketNumber]->getSize() > 1)
							totalCapacity++;
					}
					else {
						if (table[copyBucketNumber]->getSize() == 0) {
							delete table[copyBucketNumber];
							table[copyBucketNumber] = NULL;
						}
					}

				}
				else if (j == temp_LL->getSize() - 1) {

					*temp_Obj = enumerator_LL.peek();

					unsigned long copyBucketNumber = hasher->hash(*temp_Obj) % newCapacity; //rehashing 

					if (newTable[copyBucketNumber] == NULL)
						newTable[copyBucketNumber] = new OULinkedList<T>(comparator);

					if (newTable[copyBucketNumber]->insert(temp_Obj) == true) {
						if (newTable[copyBucketNumber]->getSize() > 1)
							totalCapacity++;
					}
					else {
						if (table[copyBucketNumber]->getSize() == 0) {
							delete table[copyBucketNumber];
							table[copyBucketNumber] = NULL;
						}
					}

				}

			}

		}
	}

	delete temp_Obj;

}

template<typename T>
inline void HashTable<T>::expandTable() //expand if the max load factor its reached 
{
	if (scheduleIndex + 1 >= SCHEDULE_SIZE)
		throw new ExceptionIndexOutOfRange();

	scheduleIndex = scheduleIndex + 1;

	unsigned long newCapacity = SCHEDULE[scheduleIndex];

	totalCapacity = newCapacity;

	OULinkedList<T>** newTable = new OULinkedList<T> *[newCapacity];

	if (newTable == NULL)
		throw new ExceptionMemoryNotAvailable();

	newTable = initializeHashTale(newCapacity);

	copyTable(newTable, newCapacity);

	deleteTable();

	table = newTable;

	baseCapacity = newCapacity;

}

template<typename T>
inline void HashTable<T>::contractTable() //expand if the min load factor its reached 
{
	if (scheduleIndex - 1 >= 0)
	{
		scheduleIndex = scheduleIndex - 1;

		unsigned long newCapacity = SCHEDULE[scheduleIndex];

		totalCapacity = newCapacity;

		OULinkedList<T>** newTable = new OULinkedList<T> *[newCapacity];

		if (newTable == NULL)
			throw new ExceptionMemoryNotAvailable();

		newTable = initializeHashTale(newCapacity);

		copyTable(newTable, newCapacity);

		deleteTable();

		table = newTable;

		baseCapacity = newCapacity;
	}
}

template<typename T>
inline void HashTable<T>::deleteTable()
{
	for (unsigned long i = 0; i < baseCapacity; i++) {

		if (table[i] != NULL) {

			delete table[i];

		}
	}

	delete[] table;
}

template<typename T>
inline bool HashTable<T>::insert(T * item) //ARREGLAR, crear linked  list antes de insertar
{
	unsigned long bucket = getBucketNumber(item);
	bool insertion = false;

	if (table[bucket] == NULL)
		table[bucket] = new OULinkedList<T>(comparator);

	if (table[bucket]->insert(item) == true) {

		insertion = true;
		size++;

		if (table[bucket]->getSize() > 1)
			totalCapacity++;

		if (getLoadFactor() > maxLoadFactor)
			expandTable();
	}
	else {
		if (table[bucket]->getSize() == 0) {
			delete table[bucket];
			table[bucket] = NULL;
		}
	}

	return insertion;
}

template<typename T>
inline bool HashTable<T>::replace(T * item)
{
	unsigned long bucket = getBucketNumber(item);

	if (table[bucket] != NULL) {

		if (table[bucket]->replace(item) == false) {

			return false;
		}
		else {

			return true;

		}

	}

	return false;
}

template<typename T>
inline bool HashTable<T>::remove(T * item)
{
	unsigned long bucket = getBucketNumber(item);

	if (table[bucket] != NULL) {

		if (table[bucket]->remove(item) == false) {
			return false;
		}
		else {

			size = size - 1;

			if (table[bucket]->getSize() == 0) {
				delete table[bucket];
				table[bucket] = NULL;
			}
			else if (table[bucket]->getSize() > 0) {
				totalCapacity--;
			}

			if (getLoadFactor() < minLoadFactor && baseCapacity > DEFAULT_BASE_CAPACITY)
				contractTable();

			return true;
		}

	}
	else
		return false;

	return false;
}

template<typename T>
inline T HashTable<T>::find(const T * item) const
{

	unsigned long bucket = getBucketNumber(item);

	T temp_Obj;

	if (table[bucket] != NULL) {

		try {

			temp_Obj = table[bucket]->find(item);

		}
		catch (ExceptionLinkedListAccess *e) {

			delete e;
			throw new ExceptionHashTableAccess();
		}
	}
	else {
		throw new ExceptionHashTableAccess();
	}

	return temp_Obj;
}

template<typename T>
inline unsigned long HashTable<T>::getSize() const
{
	return size;
}

template<typename T>
inline unsigned long HashTable<T>::getBaseCapacity() const
{
	return baseCapacity;
}

template<typename T>
inline unsigned long HashTable<T>::getTotalCapacity() const
{
	return totalCapacity;
}

template<typename T>
inline float HashTable<T>::getLoadFactor() const
{
	return (float)getSize() / (float)getTotalCapacity();
}

template<typename T>
inline unsigned long HashTable<T>::getBucketNumber(const T * item) const
{
	return hasher->hash(*item) % getBaseCapacity();
}

#endif // !HASH_TABLE_H