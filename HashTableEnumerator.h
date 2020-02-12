#ifndef HASH_TABLE_ENUMERATOR
#define HASH_TABLE_ENUMERATOR

#include "Enumerator.h"
#include "Exceptions.h"
#include "HashTable.h"

template <typename T>
class HashTableEnumerator : public Enumerator<T>
{
private:
	unsigned long bucket = 0;								// the current bucket during the enumeration process
	OULinkedListEnumerator<T> *chainEnumerator = NULL;		// used to move through the linked list of the current bucket
	HashTable<T>* hashTable = NULL;							// pointer to the HashTable being enumerated
	unsigned long counter_link = 0;
	// you may add additional member variables and functions here to support the operation of your code
public:
	HashTableEnumerator(HashTable<T>* hashTable);			// constructor needs a pointer to the HashTable to be enumerated
	virtual ~HashTableEnumerator();
	bool hasNext() const;									// true if there are elements that have not yet been returned via next()
	T next();												// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;											// throws ExceptionEnumerationBeyondEnd if no next item is available
};

// put implementation for HashTableEnumerator here

template<typename T>
inline HashTableEnumerator<T>::HashTableEnumerator(HashTable<T>* hashTable)
{
	if (hashTable->getSize() == 0) {
		throw new ExceptionHashTableAccess();
	}

	if (hashTable != NULL)
		this->hashTable = hashTable;

	for (unsigned long i = 0; i < this->hashTable->getBaseCapacity(); i++) {

		if (this->hashTable->table[i] != NULL) {

			chainEnumerator = new OULinkedListEnumerator<T>();

			bucket = i;
			counter_link = 0;

			*chainEnumerator = this->hashTable->table[bucket]->enumerator();

			break;
		}
	}
}

template<typename T>
inline HashTableEnumerator<T>::~HashTableEnumerator()
{
	if (chainEnumerator != NULL)
		delete chainEnumerator;

	chainEnumerator = NULL;
	hashTable = NULL;
}

template<typename T>
inline bool HashTableEnumerator<T>::hasNext() const
{
	if (chainEnumerator == NULL)
		return false;

	if (chainEnumerator->hasNext() == true) {
		return true;
	}
	else {
		for (unsigned long i = bucket; i < hashTable->getBaseCapacity(); i++) { //bucket + 1 

			if (hashTable->table[i] != NULL) {

				return true;

			}
		}

		return false;
	}

	return false;
}

template<typename T>
inline T HashTableEnumerator<T>::next()
{
	T copyObject;

	if (chainEnumerator != NULL) {

		//copyObject = new T;

		if (chainEnumerator->hasNext()) {

			copyObject = chainEnumerator->next();
			counter_link++;

		}
		else {

			copyObject = chainEnumerator->peek();
			counter_link++;

		}

		if (counter_link >= hashTable->table[bucket]->getSize()) {

			//bucket++;
			delete chainEnumerator;
			chainEnumerator = NULL;

			for (unsigned long i = bucket + 1; i < hashTable->getBaseCapacity(); i++) {

				if (hashTable->table[i] != NULL) {

					chainEnumerator = new OULinkedListEnumerator<T>();

					bucket = i;
					counter_link = 0;

					*chainEnumerator = hashTable->table[bucket]->enumerator();

					break;
				}
			}

		}

		return copyObject;
	}
	else {
		throw new ExceptionEnumerationBeyondEnd();
	}

	return copyObject;
}

template<typename T>
inline T HashTableEnumerator<T>::peek() const
{
	if (chainEnumerator == NULL) {
		throw new ExceptionEnumerationBeyondEnd();
	}

	return chainEnumerator->peek();
}

#endif // !HASH_TABLE_ENUMERATOR


