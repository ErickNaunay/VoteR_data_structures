#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "HashTable.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)

template <typename T>
long long binarySearch(const T& item, const TemplatedArray<T>& array, const Comparator<T>& comparator) { //binary search

	long long mid = 0; //mid index
	long long low = 0;
	long long high = array.getSize() - 1;
	try {
		while (high >= low)
		{
			mid = (low + high) / 2;

			if (comparator.compare(array[(unsigned)mid], item) == -1) { //compare the mid index with the item
				low = mid + 1;
			}
			else if (comparator.compare(array[(unsigned)mid], item) == 1) {
				high = mid - 1;
			}
			else if (comparator.compare(array[(unsigned)mid], item) == 0) { //if its equal, (element found), there it loop to find the first element 

				if (mid > 0 && (unsigned)mid < array.getSize() - 1) {

					long long left = mid - 1;
					//left validation for repeated values

					while (true) {
						if (comparator.compare(array[(unsigned)left], array[(unsigned)mid]) == 0) {
							left--;

							if (left < 0) {
								left = 0;
								break;
							}
						}
						else {
							left++;
							break;
						}
					}
					return left; //return the first element founded in the array
				}
				else {
					return mid;
				}
			}
		}
	}
	catch (Exception *e) {
		throw new ExceptionIndexOutOfRange();
		delete e;
	}

	return -(mid + 1);
}

template<typename T>
unsigned long linearSearch(const T& item, const TemplatedArray<T>& array, const Comparator<T>& comparator) { //linear search 

	try {

		unsigned long counterDataFound = 0;

		for (unsigned long i = 0; i < array.getSize(); i++) { //loop for all the array searching for the item

			if (comparator.compare(item, array[i]) == 0) {
				printDataLinearSearch(array[i]);
				counterDataFound++;

			}
		}
		return counterDataFound;
	}
	catch (Exception *e) {
		delete e;
		throw new ExceptionIndexOutOfRange();
	}
}

template<typename T>
void printDataLinearSearch(T objectToPrint) {
	std::cout << objectToPrint << std::endl;
}

template<typename T>
unsigned long  searchHashTable(const T key, const HashTable<T> &hashTable) {

	T *findKey = new T;
	*findKey = key;

	try {

		printKeyHashTable(hashTable.find(findKey));

		return 1;
	}
	catch (ExceptionHashTableAccess *e) {
		
		delete e;

		return 0;
	}
}

template<typename T>
void printKeyHashTable(T objectToPrint) {
	std::cout << objectToPrint << std::endl;
}
#endif 