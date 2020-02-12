#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"

#include "Comparator.h"
#include "TemplatedArray.h"

#include "AVLTree.h"
#include "AVLTreeEnumerator.h"
#include "AVLTreeOrder.h"

template<typename T>
class Sorter
{
private:

	static void QuickSort(TemplatedArray<T>& array, const Comparator<T>& comparatorSort, long long left, long long right); //quick sort method 

public:
	static void sort(TemplatedArray<T> &array, const Comparator<T> &comparator); //sort method
	static void mergeArrays(TemplatedArray<T> &array, TemplatedArray<T> &array_1, TemplatedArray<T> &array_2, const Comparator<T> &comparator); //merge of two arrays sorted 
	static void mergeTrees(AVLTree<T> *& data, AVLTree<T> *&temp_1, AVLTree<T>  *&temp_2, const Comparator<T>& comparator); //merge of two arrays sorted 

};

template<typename T>
inline void Sorter<T>::QuickSort(TemplatedArray<T>& array, const Comparator<T>& comparatorSort, long long left, long long right) //methpd that has as parameters the array, comparator, begining of the array and the end of it
{
	if (left < right) { //if the left index its less than the right index 

		long long pivotPoint = (left + right) / 2;
		long long i = left - 1;
		long long j = right + 1;

		try {

			do {

				do {

					i = i + 1;

				} while (i < (signed)array.getSize() && (comparatorSort.compare(array[(unsigned)i], array[(unsigned)pivotPoint]) <= 0));
				do {

					j = j - 1;

				} while (j >= i && (comparatorSort.compare(array[(unsigned)j], array[(unsigned)pivotPoint]) >= 0));

				if (i < j) {
					// swap items at i and j
					T temp1 = array[(unsigned)i];
					T temp2 = array[(unsigned)j];
					array.replaceAt(&temp2, (unsigned)i);
					array.replaceAt(&temp1, (unsigned)j);
				}

			} while (i < j);

			if (i < pivotPoint) {
				j = i;
			}

			// swap items at left and j
			T temp1 = array[(unsigned)pivotPoint];
			T temp2 = array[(unsigned)j];

			array.replaceAt(&temp2, (unsigned)pivotPoint);
			array.replaceAt(&temp1, (unsigned)j);

			// Recursive Steps
			if (j > 0) {
				QuickSort(array, comparatorSort, left, j - 1);
			}
			QuickSort(array, comparatorSort, j + 1, right);

		}
		catch (Exception *e) {
			//cout << "Exception accessing data from the array for the sorting algorithm" << endl;
			delete e;
			throw new ExceptionIndexOutOfRange();
		}
	}
}

template<typename T>
inline void Sorter<T>::sort(TemplatedArray<T>& array, const Comparator<T>& comparator)
{
	try {
		QuickSort(array, comparator, 0, array.getSize() - 1);
	}
	catch (Exception *e) {
		delete e;
		throw new ExceptionIndexOutOfRange();
	}
}

template<typename T>
inline void Sorter<T>::mergeArrays(TemplatedArray<T>& array, TemplatedArray<T> &array_1, TemplatedArray<T> &array_2, const Comparator<T>& comparator)
{
	array.clear();

	unsigned long i = 0; //index to the first element of the first templated array
	unsigned long j = 0; //index to the fist element of the second templated array

	try {

		T *temporal = new T; //creates a new object for the swapping 

		while (i < array_1.getSize() && j < array_2.getSize()) { //if both indexs are not in the final of the arrays, there makes a comparison to copy in the main array

			if (comparator.compare(array_1[i], array_2[j]) == -1) { //if its less, then the item for 1 its copies an the index i increase
				*temporal = array_1[i];

				array.add(temporal);
				i++;
			}
			else if (comparator.compare(array_1[i], array_2[j]) == 1) { //if its more, then the item for 2 its copies an the index j increase
				*temporal = array_2[j];
				array.add(temporal);
				j++;
			}
			else if (comparator.compare(array_1[i], array_2[j]) == 0) { //if both are equal, the item of the second its copied to the main array, because its the more recent one 
				*temporal = array_2[j];
				array.add(temporal);
				j++;
				i++;
			}
		}

		//if one of the index reaches the end of the array, then the elements of the other array are copied to the main array

		while (i < array_1.getSize())
		{
			*temporal = array_1[i];
			array.add(temporal);
			i++;
		}
		while (j < array_2.getSize())
		{
			*temporal = array_2[j];
			array.add(temporal);
			j++;
		}

		delete temporal;
	}
	catch (Exception *e) {

		delete e;
		throw new ExceptionIndexOutOfRange();

	}
}

template<typename T>
inline void Sorter<T>::mergeTrees(AVLTree<T> *& data , AVLTree<T> *&temp_1, AVLTree<T>  *&temp_2, const Comparator<T>& comparator) //FIX !!!
{
	AVLTreeOrder order = AVLTreeOrder::inorder;

	AVLTreeEnumerator<T> *enum_temp1 = new AVLTreeEnumerator<T>(temp_1, order);
	AVLTreeEnumerator<T> *enum_temp2 = new AVLTreeEnumerator<T>(temp_2, order);

	try {

		T *temporal = new T; //creates a new object for the swapping 

		while (enum_temp1->hasNext() && enum_temp2->hasNext()) { //if both indexs are not in the final of the arrays, there makes a comparison to copy in the main array

			if (comparator.compare(enum_temp1->peek(), enum_temp2->peek()) == -1) { //if its less, then the item for 1 its copies an the index i increase
				
				*temporal = enum_temp1->next();

				if (data->insert(temporal) == false) {
					throw new Exception();
				}
				//i++;
			}
			else if (comparator.compare(enum_temp1->peek(), enum_temp2->peek()) == 1) { //if its more, then the item for 2 its copies an the index j increase
				
				*temporal = enum_temp2->next();

				if (data->insert(temporal) == false) {
					throw new Exception();
				}
				//j++;
			}
			else if (comparator.compare(enum_temp1->peek(), enum_temp2->peek()) == 0) { //if both are equal, the item of the second its copied to the main array, because its the more recent one 
				
				*temporal = enum_temp2->next();

				if (data->insert(temporal) == false) {
					throw new Exception();
				}

				*temporal = enum_temp1->next();

			}
		}

		//if one of the index reaches the end of the tree, then the elements of the other array are copied to the main array

		while (enum_temp1->hasNext())
		{
			*temporal = enum_temp1->next();

			if (data->insert(temporal) == false) {
				throw new Exception();
			}
		}
		while (enum_temp2->hasNext())
		{
			*temporal = enum_temp2->next();

			if (data->insert(temporal) == false) {
				throw new Exception();
			}
		}

		delete temporal;
	}
	catch (ExceptionEnumerationBeyondEnd *e) {

		delete e;
		throw new ExceptionEnumerationBeyondEnd();

	}

	delete enum_temp1;
	delete enum_temp2;

}


#endif // !SORT_H
