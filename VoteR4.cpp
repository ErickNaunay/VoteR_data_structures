// VoteR.cpp : Defines the entry point for the console application.
//
#include "Exceptions.h"
#include "Comparator.h"
#include "Enumerator.h"
#include "Hasher.h"
#include "NvraHasher.h"
#include "HashTable.h"
#include "HashTableEnumerator.h"
#include "AVLTree.h"
#include "AVLTreeEnumerator.h"
#include "TemplatedArray.h"
#include "Sorter.h"
#include "Search.h"

#include "NvraComparator.h"
#include "NvraRecord.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// prototypes of the funtions, first 

bool loadData(unsigned long &linesCtr, unsigned long &recordCtr, AVLTree<NvraRecord> *&AVLTree_data, TemplatedArray<NvraRecord> *&nvraArray, HashTable<NvraRecord> *& hashTableData); //function that load the data into the ifstream
bool readData(ifstream &data, unsigned long &linesCtr, unsigned long &recordCtr, AVLTree<NvraRecord> *&AVLTree_data); //read the data with the ifstream to the array of data
void loadDataArray(AVLTree<NvraRecord> *& AVLTree_data, TemplatedArray<NvraRecord> *& nvraArray); //method that copies the information from the linked list into the array
void loadDataHashTable(AVLTree<NvraRecord> *& AVLTree_data, HashTable<NvraRecord> *& hashTableData);

void mainMenu(unsigned long linesCtr, unsigned long recordCtr, AVLTree<NvraRecord> *& AVLTree_data, TemplatedArray<NvraRecord> *&nvraDataAray, unsigned int &sortColumn, HashTable<NvraRecord> *&hashTableData); //fuction that manage the input from user 

void outputMenu(TemplatedArray<NvraRecord> &nvraData, unsigned long recordCtr, unsigned long linesCtr); // (o) selection, output the information into the console or in a file

void sortMenu(TemplatedArray<NvraRecord> &nvraData, unsigned int &sortColumn); //sort menu function (select column and sort it)

void searchMenu(TemplatedArray<NvraRecord> &nvraData, const HashTable<NvraRecord> &hashTableData, unsigned int &sortColumn);  //search menu function (select column and make sure if its an sort column or unsorted to make a binary search or not)
void printDataFound(TemplatedArray<NvraRecord> &nvraData, Comparator<NvraRecord> &nvraComparator, unsigned long indexFirstElement); //print the objects found by the search 
void findTallie(unsigned long counterSearch); //tallie for the search found

void mergeMenu(unsigned long &linesCtr, unsigned long &recordCtr, AVLTree<NvraRecord> *& AVLTree_data, TemplatedArray<NvraRecord> *&nvraArray, HashTable<NvraRecord> *& hashTableData, unsigned int &sortColumn); //merge menu, aks for a file name and merge it (uses load data method)

void purgeMenu(unsigned long &linesCtr, unsigned long &recordCtr, AVLTree<NvraRecord> *& AVLTree_data, TemplatedArray<NvraRecord> *&nvraArray, HashTable<NvraRecord> *&hashTableData, unsigned int &sortColumn); //purge menu, akes for a file with data that are going to be purged
void purgeData(AVLTree<NvraRecord> *& AVLTree_data, AVLTree<NvraRecord> *& purgeData); //purge data from avltree data with purgedata avltree

void hashTableMenu(HashTable<NvraRecord> *&hashTableData, unsigned long recordCtr, unsigned long linesCtr); //menu that output the data inside the hash table data structure

void avlTreeMenu(AVLTree<NvraRecord> *&AVLTree_data, AVLTreeOrder order, unsigned long recordCtr, unsigned long linesCtr); //menu that output the data from the avltree structure, uses for the tree option (preorder, inorder, postorder)

void printFileArray(ofstream &outputFile, TemplatedArray<NvraRecord> &nvraData, unsigned long &linesCtr, unsigned long &recordCtr); //suport function to print the data into a file 
void printDataTemplatedArray(const TemplatedArray<NvraRecord> &nvraData); // print the data from the array
void printTalliesArray(unsigned long &linesCtr, unsigned long &recordCtr, TemplatedArray<NvraRecord> &nvraData); //print the tallie for the output

void printDataHashTable(HashTable<NvraRecord> *&hashTableData); //print data from the hash table
void printFileHashTable(ofstream &outputFile, HashTable<NvraRecord> *&hashTableData, unsigned long & linesCtr, unsigned long & recordCtr); //print the data into a file 
void printInfoHashTable(HashTable<NvraRecord> *&hashTableData); //print the information about the data structure 
void printTalliesHashTable(unsigned long &linesCtr, unsigned long &recordCtr, HashTable<NvraRecord> *&hashTableData); // print the tallie of the data 

void printDataAVLTree(AVLTree<NvraRecord> *& AVLTree_data, AVLTreeOrder order); //print the data of the avl tree
void printFileAVLTree(ofstream &outputfile_tree, AVLTree<NvraRecord> *& AVLTree_data, AVLTreeOrder order, unsigned long &linesCtr, unsigned long &recordCtr); //
void printTalliesAVLTree(unsigned long & linesCtr, unsigned long & recordCtr, AVLTree<NvraRecord> *& AVLTree_data);

void quitMenu(bool &screen); //quit opction, finish the program

void printInvalidId(long id, unsigned long line); //print error message for invalid id 
void printInvalidData(unsigned long line); //print error message for invalid value 

int fromString(string value); //convert string into int

//main program

const unsigned int DEFAUL_SORT_COLUMN = 0; //sort by nvra ID number

int main()
{
	//array that will contain all the data of the nvra records
	//TemplatedArray<NvraRecord> *nvraData = new TemplatedArray<NvraRecord>();

	unsigned long linesCtr = 0; // total lines reads
	unsigned long recordCtr = 0; //valid record reads
	unsigned int sortColumn = DEFAUL_SORT_COLUMN; //which column has been sorted

	NvraComparator *nvraComparator = new NvraComparator(sortColumn);

	AVLTree<NvraRecord> *nvraAVLTree = new AVLTree<NvraRecord>(nvraComparator);

	TemplatedArray<NvraRecord> *nvraTemplatedArray = NULL;
	HashTable<NvraRecord> *hashTable = NULL;

	if (loadData(linesCtr, recordCtr, nvraAVLTree, nvraTemplatedArray, hashTable) == true) //first loop to read the data from diferent files 
	{
		mainMenu(linesCtr, recordCtr, nvraAVLTree, nvraTemplatedArray, sortColumn, hashTable); //user interaction loop
	}

	delete nvraAVLTree;
	nvraAVLTree = NULL;
	
	if (nvraTemplatedArray != NULL) {
		delete nvraTemplatedArray;
	}

	nvraTemplatedArray = NULL;

	if (hashTable != NULL) {
		delete hashTable;
	}

	hashTable = NULL;

	return 0;
}

//implementations of the functions 

bool loadData(unsigned long & linesCtr, unsigned long & recordCtr, AVLTree<NvraRecord>*& AVLTree_data, TemplatedArray<NvraRecord>*& nvraArray, HashTable<NvraRecord>*& hashTableData)
{
	try {

		ifstream data;

		string name;
		int filesCtr = 0;

		while (true) { //load a file into the input stream

			cout << "Enter data file name: ";
			getline(cin, name);

			if (name == "" && filesCtr == 0) //if there was not name enter and no files loaded, the program exits
			{
				return false;
			}
			/* receive more than one file
			else if (name == "" && filesCtr > 0) {

			loadDataArray(nvraData, nvraArray);
			return true;
			}
			*/

			data.open(name, ios_base::in); //open the file

			if (!data.is_open()) //if the file does not exists
			{
				cout << "File is not available." << endl;
			}
			else //if the file its correctly opened
			{

				if (readData(data, linesCtr, recordCtr, AVLTree_data) == true) { //read the data into the linked list 
					
					filesCtr++;

					loadDataArray(AVLTree_data, nvraArray); //just admit one file 
					loadDataHashTable(AVLTree_data, hashTableData);
					
					data.close();

					break;

				}
				else { //verify if there was at leat one valid record 
					cout << "No valid records found." << endl;
				}

				data.close();
			}

		}

		return true;
	}
	catch (ExceptionIndexOutOfRange *e) {

		cout << "Error loading (merging )the data from the files.";
		delete e;

	}
	return false;
}

bool readData(ifstream & data, unsigned long & linesCtr, unsigned long & recordCtr, AVLTree<NvraRecord>*& AVLTree_data)
{

	string temp_data = "";

	size_t startPos = 0; //start position
	size_t endPos = 0; //end position

	try {

		int temp_value = 0;
		unsigned long fileLines = 0;
		bool invalidData = false;

		NvraRecord *nvraRecord = NULL;

		while (getline(data, temp_data)) {

			if (fileLines > 0) // do not care the first line fo headers (names of the columns)
			{
	
				try {

					nvraRecord = new NvraRecord;

					for (int i = 0; i < 21; i++) { //obtain the value fo each column
						
						temp_value = 0;

						if (invalidData == false) {

							if (i < 20) { //get the position for each columns (next coma)
								endPos = temp_data.find(',', startPos);
							}

							temp_value = fromString(temp_data.substr(startPos, endPos - startPos));

							if ( temp_value < 0) { //validates if the int data is higher than zero

								invalidData = true;
								printInvalidData(fileLines);

							}
							else {

								nvraRecord->addNum(temp_value);	// if there was no invalid data and unique record, the data ois storaged

								if (i == 2) { // storage the data from the string columns (same process)

									startPos = endPos + 1;
									endPos = temp_data.find(',', startPos);

									nvraRecord->addString(temp_data.substr(startPos, endPos - startPos));

								}
								else if (i == 9) { // storage the data from the string columns

									for (int i = 0; i < 2; i++)
									{
										startPos = endPos + 1;
										endPos = temp_data.find(',', startPos);

										nvraRecord->addString(temp_data.substr(startPos, endPos - startPos));
									}
								}
								startPos = endPos + 1;
							}
						}
					}

					if (invalidData == false) { // if the record has VALID ID AND VALID VALUES storage in the object that has array of records

						recordCtr++;

						try {

							if (AVLTree_data->insert(nvraRecord) == false) {

								printInvalidId(nvraRecord->getNum(0), fileLines);

							}

						}
						catch (ExceptionHashTableAccess *e) {

							cout << "ERROR in the load data method (reading data from the file and inserting into the AVL tree)." << endl;
							delete e;
						}

					}

					delete nvraRecord;

				}
				catch (ExceptionMemoryNotAvailable * e) {
					cout << "Error when a new object tries to be added in the AVL tree" << endl;
					delete e;
				}

				temp_data = "";
				linesCtr++;
			}
			startPos = 0;
			endPos = 0;
			fileLines++;
			invalidData = false;
		}

		if (AVLTree_data->getSize() > 0)
			return true;

		return false;
	}
	catch (ExceptionMemoryNotAvailable *e) {
		cout << "Error creating objects in the heap, the pointer is set to NULL (no memory for the object)." << endl;
		delete e;
	}
	catch (ExceptionIndexOutOfRange *e) {
		cout << "Error adding, removing or counting from the templated arrays: nvraData or uniqueId's." << endl;
		delete e;
	}

	return false;

}

void loadDataArray(AVLTree<NvraRecord>*& AVLTree_data, TemplatedArray<NvraRecord>*& nvraArray)
{

	if (nvraArray != NULL) {

		delete nvraArray;

	}

	nvraArray = new TemplatedArray<NvraRecord>(AVLTree_data->getSize());

	if (AVLTree_data->getSize() <= 0)
		return;

	AVLTreeOrder order = AVLTreeOrder::inorder;

	NvraRecord *temporalRecord = new NvraRecord;

	try {

		AVLTreeEnumerator<NvraRecord> *enumeratorTree = new AVLTreeEnumerator<NvraRecord>(AVLTree_data, order);

		for (unsigned long i = 0; i < AVLTree_data->getSize(); i++) { //loop that go over all the list copying all the objects


			if (enumeratorTree->hasNext()) {
				*temporalRecord = enumeratorTree->next();

				nvraArray->add(temporalRecord);
			}
			else if (i == AVLTree_data->getSize() - 1) {
				*temporalRecord = enumeratorTree->peek();

				nvraArray->add(temporalRecord);
			}

		}
		
		delete enumeratorTree;

	}
	catch (ExceptionEnumerationBeyondEnd *e) {

		cout << "ERROR in the method load AVL Tree data into the array." << endl;
		delete e;

	}
	catch (ExceptionAVLTreeAccess *e) {

		delete e;

	}

	delete temporalRecord;

}

void loadDataHashTable(AVLTree<NvraRecord>*& AVLTree_data, HashTable<NvraRecord>*& hashTableData)
{
	if (hashTableData != NULL) {

		delete hashTableData;

	}

	try {

	NvraComparator *nvraComparator = new NvraComparator(DEFAUL_SORT_COLUMN);
	NvraHasher *hasher = new NvraHasher;

	hashTableData = new HashTable<NvraRecord>(nvraComparator, hasher, AVLTree_data->getSize());

	if (AVLTree_data->getSize() <= 0)
		return;

	AVLTreeOrder order = AVLTreeOrder::inorder;

	AVLTreeEnumerator<NvraRecord> *enumeratorTree = new AVLTreeEnumerator<NvraRecord>(AVLTree_data, order);

	NvraRecord *temporalRecord = new NvraRecord;

	
		for (unsigned long i = 0; i < AVLTree_data->getSize(); i++) { //loop that go over all the list copying all the objects


			if (enumeratorTree->hasNext()) {
				*temporalRecord = enumeratorTree->next();

				if (hashTableData->insert(temporalRecord) == false)
					cout << "ITEM ALREADY ALLOCATED IN THE HASH TABLE" << endl;
			}
			else if (i == AVLTree_data->getSize() - 1) {
				*temporalRecord = enumeratorTree->peek();

				if (hashTableData->insert(temporalRecord) == false)
					cout << "ITEM ALREADY ALLOCATED IN THE HASH TABLE" << endl;
			}

		}

		delete temporalRecord;
		delete enumeratorTree;

	}
	catch (ExceptionEnumerationBeyondEnd *e) {

		cout << "ERROR in the method load AVL Tree data into the hashTable." << endl;
		delete e;

	}
	catch (ExceptionAVLTreeAccess *e) {

		//cout << "ERROR in the constructor of avl tree enumerator." << endl;
		delete e;

	}



}

void mainMenu(unsigned long linesCtr, unsigned long recordCtr, AVLTree<NvraRecord>*& AVLTree_data, TemplatedArray<NvraRecord>*& nvraDataArray, unsigned int & sortColumn, HashTable<NvraRecord>*& hashTableData)
{

	string opSelected;
	bool screen = true;
	string optionsMenu[10] = {"o", "s", "f", "m", "p", "h", "pre", "in", "post", "q"};
	try {

		while (screen) //manipulation menu loop 
		{

			cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (h)ash table, (pre)order, (in)order, (post)order, or (q)uit: ";
			getline(cin, opSelected);
			cin.clear();

			if (opSelected == optionsMenu[0]) {

				outputMenu(*nvraDataArray, recordCtr, linesCtr);
			}
			else if (opSelected == optionsMenu[1]) {

				sortMenu(*nvraDataArray, sortColumn);

			}
			else if (opSelected == optionsMenu[2]) {

				searchMenu(*nvraDataArray, *hashTableData, sortColumn);

			}
			else if (opSelected == optionsMenu[3]) {

				mergeMenu(linesCtr, recordCtr, AVLTree_data, nvraDataArray, hashTableData, sortColumn);
			}
			else if (opSelected == optionsMenu[4]) {

				purgeMenu(linesCtr, recordCtr, AVLTree_data, nvraDataArray, hashTableData, sortColumn);

			}
			else if (opSelected == optionsMenu[5]) {

				hashTableMenu(hashTableData, recordCtr, linesCtr);

			}
			else if (opSelected == optionsMenu[6]) {

				AVLTreeOrder preorder = AVLTreeOrder::preorder;

				avlTreeMenu(AVLTree_data, preorder, recordCtr, linesCtr);

			}
			else if (opSelected == optionsMenu[7]) {

				AVLTreeOrder inorder = AVLTreeOrder::inorder;

				avlTreeMenu(AVLTree_data, inorder, recordCtr, linesCtr);

			}
			else if (opSelected == optionsMenu[8]) {

				AVLTreeOrder postorder = AVLTreeOrder::postorder;

				avlTreeMenu(AVLTree_data, postorder, recordCtr, linesCtr);

			}
			else if (opSelected == optionsMenu[9]) {

				quitMenu(screen);

			}

		}
	}
	catch (Exception *e) { //exceptions handling 
		cout << "Error in fuctions of main menu." << endl;
		delete e;
	}

}

void outputMenu(TemplatedArray<NvraRecord> &nvraData, unsigned long recordCtr, unsigned long linesCtr) { //first option 'o' in menu
	try {
		ofstream fileOutput; //create a reference file output file 
		string outputFileName = "";

		while (true) { //loop to get the name of the file where the data its going to be written or just standar output

			cout << "Enter output file name: ";
			getline(cin, outputFileName);
			cin.clear();

			if (outputFileName == "") { //if no nome its written, just print the standar output (console)

				printDataTemplatedArray(nvraData);
				printTalliesArray(linesCtr, recordCtr, nvraData);

				break;
			}
			else {

				fileOutput.open(outputFileName, ios_base::out | ios_base::trunc); //open a file with the user's name

				if (fileOutput.is_open()) { //verify if it was correctly opened 
											//cout << "Succesfully create the OUTPUT" << endl;
					printFileArray(fileOutput, nvraData, linesCtr, recordCtr); //write in the file the data

					printDataTemplatedArray(nvraData); //print also in the standar output 
					printTalliesArray(linesCtr, recordCtr, nvraData);

					//fileOutput.close();

					break;
				}
				else { // handling if the file was not able to be created

					fileOutput.clear();

					cout << "File is not available." << endl;
				}
			}
		}

		fileOutput.close();

	}
	catch (Exception *e) {
		cout << "Error in output file( or console) menu";
		delete e;
	}

}

void sortMenu(TemplatedArray<NvraRecord> &nvraData, unsigned int &sortColumn) {

	try { //loop to ask a value to sort the data by that value (column)

		int column;
		string opc;
		cout << "Enter sort field(0 - 23) : ";
		getline(cin, opc);

		column = fromString(opc);

		cin.clear();


		if (column >= 0 && column <= 23) { // verify if the value its in the range

			sortColumn = (unsigned)column; //set the column sort variable 

			Comparator<NvraRecord> *comparator; //creation of a new comparator  with the new column that is goin to be sorted

			NvraComparator *comparatorNvra = new NvraComparator(sortColumn);

			comparator = comparatorNvra;

			Sorter<NvraRecord>::sort(nvraData, *comparator); //sort by the new comparator

			comparator = NULL;
			//delete comparatorNvra;
		}

	}
	catch (ExceptionIndexOutOfRange *e) {

		cout << "Index out of Bounds in the sort menu fuction (quicksort method)" << endl;
		delete e;
	}

}

void searchMenu(TemplatedArray<NvraRecord> &nvraData, const HashTable<NvraRecord> &hashTableData, unsigned int &sortColumn) {

	int menuOption;
	string opc;

	cout << "Enter search field (0-23):";
	getline(cin, opc);

	menuOption = fromString(opc);

	cin.clear();

	try { //search an item (value)  in an specific column

		if (menuOption < 24 && menuOption >= 0) { //verify if the option its in the range 

			unsigned int searchOpt = (unsigned)menuOption;

			int keyInt = 0;
			string keyString = "";

			Comparator<NvraRecord> *comparator; //cretae a comparator with the columns selected

			NvraComparator *comparatorNvra = new NvraComparator(searchOpt);

			comparator = comparatorNvra;

			if (searchOpt != 3 && searchOpt != 11 && searchOpt != 12) { // verify if the column selected was numerical

				cout << "Enter non-negative field value:";
				cin >> keyInt;

				if (!cin) {
					cin.clear();
					cin.ignore(999, '\n');
					return;
				}
				else {
					cin.ignore();
				}

				if (keyInt < 0) {
					keyInt *= (-1);
				}

				NvraRecord *keyObject = new NvraRecord; //create an object that contains the key value to find in the column selected

				try {

					if (searchOpt < 3)
						keyObject->setNum(keyInt, searchOpt);
					else if (searchOpt > 3 && searchOpt < 11)
						keyObject->setNum(keyInt, searchOpt - 1);
					else if (searchOpt > 12 && searchOpt < 24)
						keyObject->setNum(keyInt, searchOpt - 3);
				}
				catch (ExceptionIndexOutOfRange *e) {

					cout << "Error setting values in NvraRecord arrays" << endl;
					delete e;
				}

				if (sortColumn == searchOpt) { // if the column selected was a column sorted, the search its going to be done by binary search

					long long indexKeyFound = binarySearch<NvraRecord>(*keyObject, nvraData, *comparator);

					if (indexKeyFound >= 0)
					{
						printDataFound(nvraData, *comparator, (unsigned)indexKeyFound);
					}
					else {
						findTallie(0);
					}
				}
				else { //if it not, linear search
					findTallie(linearSearch<NvraRecord>(*keyObject, nvraData, *comparator));
				}
				delete keyObject;
			}
			else { //if the columsn selected for search was string value

				cout << "Enter exact text on which to search: ";
				getline(cin, keyString);
				cin.clear();

				if (keyString == "")
					return;

				NvraRecord *keyObject = new NvraRecord; //cretae na object with the key string value to search int he column specified

				try {

					if (searchOpt == 3)
						keyObject->setString(keyString, 0);
					if (searchOpt == 11)
						keyObject->setString(keyString, 1);
					if (searchOpt == 12)
						keyObject->setString(keyString, 2);
				}
				catch (ExceptionIndexOutOfRange *e) {

					cout << "Error setting values in NvraRecord arrays" << endl;
					delete e;

				}

				if (sortColumn == 0) {

					findTallie(searchHashTable<NvraRecord>(*keyObject, hashTableData));

				}
				else if (sortColumn == searchOpt) { //if the columns was previously sorted, its going to make an bianry search

					long long indexKeyFound = binarySearch<NvraRecord>(*keyObject, nvraData, *comparator);

					if (indexKeyFound >= 0)
					{
						printDataFound(nvraData, *comparator, (unsigned)indexKeyFound);
					}
					else {

						findTallie(0);

					}
				}
				else { // if its not, linary search 
					findTallie(linearSearch<NvraRecord>(*keyObject, nvraData, *comparator));
				}

				delete keyObject;
			}

			comparator = NULL;
			//delete comparatorNvra;

		}
	}
	catch (ExceptionIndexOutOfRange *e) {
		cout << "Error index out of bound on the sorting algorithms" << endl;
		delete e;
	}
}

void printDataFound(TemplatedArray<NvraRecord> &nvraData, Comparator<NvraRecord> &nvraComparator, unsigned long indexFirstElement) {

	try { //print all the data found by the search, if there is data duplicate (ONLY WITH BINARY SEARCH)
		unsigned long searchCount = 0;

		do {
			if (nvraComparator.compare(nvraData[indexFirstElement], nvraData[indexFirstElement + searchCount]) == 0) { //compare the element with the next element (if it was searched by the binary search)
				cout << nvraData.get(indexFirstElement + searchCount) << endl;

				searchCount++;

				if (indexFirstElement + searchCount > nvraData.getSize() - 1)
					break;
			}
			else {
				break;
			}

		} while (true);

		findTallie(searchCount); //print the tallie fo the data found 
	}
	catch (ExceptionIndexOutOfRange *e) {
		cout << "Error getting the objects from the templated array, index out of bounds." << endl;
		delete e;
	}
}

void findTallie(unsigned long counterSearch) { //tallie for data search and found 
	cout << "NVRA records found: " << counterSearch << "." << endl;
}

void mergeMenu(unsigned long &linesCtr, unsigned long &recordCtr, AVLTree<NvraRecord> *& AVLTree_data, TemplatedArray<NvraRecord> *&nvraArray, HashTable<NvraRecord> *& hashTableData, unsigned int &sortColumn) {

	try {

		ifstream data;

		string name;

		while (true) { //load a file into the input stream

			cout << "Enter data file name: ";
			getline(cin, name);

			if (name == "") //if there was not name enter and no files loaded, the program exits
			{
				break;
			}

			data.open(name, ios_base::in); //open the file

			if (!data.is_open()) //if the file does not exists
			{
				cout << "File is not available." << endl;
			}
			else //if the file its correctly opened
			{
				NvraComparator *nvraComparator = new NvraComparator(DEFAUL_SORT_COLUMN);

				AVLTree<NvraRecord> *newFile_AVLTree = new AVLTree<NvraRecord>(nvraComparator);
				AVLTree<NvraRecord> *merge_AVLTree = new AVLTree<NvraRecord>(nvraComparator);

				if (readData(data, linesCtr, recordCtr, newFile_AVLTree) == true) { //read the data into the linked list 

					Sorter<NvraRecord>::mergeTrees(merge_AVLTree, AVLTree_data, newFile_AVLTree, *nvraComparator); //FIX THIS!!!!!!!!!!!!!

					delete newFile_AVLTree;
					delete AVLTree_data;

					AVLTree_data = merge_AVLTree;

					loadDataArray(AVLTree_data, nvraArray);

					loadDataHashTable(AVLTree_data, hashTableData);

					sortColumn = 0;

				}

				data.close();

				break;
			}

		}

	}
	catch (ExceptionIndexOutOfRange *e) {

		cout << "Error loading (merging )the data from the files.";
		delete e;

	}

}

void purgeMenu(unsigned long & linesCtr, unsigned long & recordCtr, AVLTree<NvraRecord>*& AVLTree_data, TemplatedArray<NvraRecord> *&nvraArray, HashTable<NvraRecord>*& hashTableData, unsigned int & sortColumn) {
	try {

		ifstream data;

		string name;

		while (true) { //load a file into the input stream

			cout << "Enter data file name: ";
			getline(cin, name);

			if (name == "") //if there was not name enter and no files loaded, the program exits
			{
				break;
			}

			data.open(name, ios_base::in); //open the file

			if (!data.is_open()) //if the file does not exists
			{
				data.clear();
				cout << "File is not available." << endl;

			}
			else //if the file its correctly opened
			{
				NvraComparator *comparator = new NvraComparator(DEFAUL_SORT_COLUMN);
				
				AVLTree<NvraRecord> *purge_data = new AVLTree<NvraRecord>(comparator);

				if (readData(data, linesCtr, recordCtr, purge_data) == true){ //read the data into the linked list 

					purgeData(AVLTree_data, purge_data);

					loadDataArray(AVLTree_data, nvraArray);

					loadDataHashTable(AVLTree_data, hashTableData);

					sortColumn = DEFAUL_SORT_COLUMN;

				}

				data.close();

				break;
			}

		}

	}
	catch (ExceptionIndexOutOfRange *e) {

		cout << "Error loading (merging )the data from the files.";
		delete e;

	}

}

void purgeData(AVLTree<NvraRecord> *& AVLTree_data, AVLTree<NvraRecord> *& purgeData) {

	try {

		AVLTreeOrder order = AVLTreeOrder::inorder;

		AVLTreeEnumerator<NvraRecord> *enumTree = new AVLTreeEnumerator<NvraRecord>(purgeData, order);

		NvraRecord *temp_obj = new NvraRecord;

		while (enumTree->hasNext()) {

			*temp_obj = enumTree->next();

			if (AVLTree_data->remove(temp_obj) == false) { //FIX THIS! EXCEPTIONS REMOVE()
				//objects purged 
			}

		}

		delete temp_obj;
	}
	catch (ExceptionAVLTreeAccess *e) {
		
		delete e;
		cout << "Trying to purge from an empty tree" << endl;
	}

}

void hashTableMenu(HashTable<NvraRecord> *&hashTableData, unsigned long recordCtr, unsigned long linesCtr)
{
	try {

		ofstream fileOutput; //create a reference file output file 

		string outputFileName = "";

		while (true) { //loop to get the name of the file where the data its going to be written or just standar output

			cout << "Enter output file name: ";
			getline(cin, outputFileName);
			cin.clear();

			if (outputFileName == "") { //if no nome its written, just print the standar output (console)

				try {

					printDataHashTable(hashTableData);

				}
				catch (ExceptionHashTableAccess *e) {

					//cout << "Hash table size 0. Error creating the Enumerator for an empty hash table." << endl;

					delete e;
				}

				printInfoHashTable(hashTableData);
				printTalliesHashTable(linesCtr, recordCtr, hashTableData);

				break;
			}
			else {

				fileOutput.open(outputFileName, ios_base::out | ios_base::trunc); //open a file with the user's name

				if (fileOutput.is_open()) { //verify if it was correctly opened 

					try {

						printFileHashTable(fileOutput, hashTableData, linesCtr, recordCtr); //write in the file the data

						printDataHashTable(hashTableData); //print also in the standar output 

					}
					catch (ExceptionHashTableAccess *e) {

						//cout << "Hash table size 0. Error creating the Enumerator for an empty hash table." << endl;

						delete e;
					}
					printInfoHashTable(hashTableData);
					printTalliesHashTable(linesCtr, recordCtr, hashTableData);
					//fileOutput.close();
					break;
				}
				else { // handling if the file was not able to be created

					fileOutput.clear();
					cout << "File is not available." << endl;
				}



			}
		}

		fileOutput.close();
	}
	catch (Exception *e) {
		cout << "Error in output file( or console) menu";
		delete e;
	}

}

void avlTreeMenu(AVLTree<NvraRecord> *&AVLTree_data, AVLTreeOrder order, unsigned long recordCtr, unsigned long linesCtr) {

	try {

		ofstream fileOutput; //create a reference file output file 

		string outputFileName = "";

		while (true) { //loop to get the name of the file where the data its going to be written or just standar output

			cout << "Enter output file name: ";
			getline(cin, outputFileName);
			cin.clear();

			if (outputFileName == "") { //if no nome its written, just print the standar output (console)

				try {

					printDataAVLTree(AVLTree_data, order);

				}
				catch (ExceptionAVLTreeAccess *e) {

					//cout << "TREE size 0. Error creating the Enumerator for an empty hash table." << endl;

					delete e;
				}

				printTalliesAVLTree(linesCtr, recordCtr, AVLTree_data);

				break;
			}
			else {

				fileOutput.open(outputFileName, ios_base::out | ios_base::trunc); //open a file with the user's name

				if (fileOutput.is_open()) { //verify if it was correctly opened 

					try {

						printFileAVLTree(fileOutput, AVLTree_data, order, linesCtr, recordCtr); //write in the file the data

						printDataAVLTree(AVLTree_data, order); //print also in the standar output 

					}
					catch (ExceptionAVLTreeAccess *e) {

						//cout << "Tree size 0. Error creating the Enumerator for an empty hash table." << endl;

						delete e;
					}

					printTalliesAVLTree(linesCtr, recordCtr, AVLTree_data);

					//fileOutput.close();
					break;
				}
				else { // handling if the file was not able to be created

					fileOutput.clear();
					cout << "File is not available." << endl;
				}

			}
		}

		fileOutput.close();

	}
	catch (ExceptionEnumerationBeyondEnd *e) {

		cout << "Error in the output of the avl tree, enumerator error";
		delete e;

	}

}

void printDataTemplatedArray(const TemplatedArray<NvraRecord> &nvraData) {

	try {
		//printing all the recods (NvraArray)
		for (unsigned long i = 0; i < nvraData.getSize(); i++) {
			cout << nvraData.get(i) << endl;
		}
	}
	catch (Exception* e) {
		cout << "Error when the object tries to call the overload cout operator and get the values of the records";
		delete e;
	}

}

void printFileArray(ofstream &outputfile_array, TemplatedArray<NvraRecord> &nvraData, unsigned long &linesCtr, unsigned long &recordCtr)
{
	try {
		//printing all the recods (NvraArray) in a file output
		for (unsigned long i = 0; i < nvraData.getSize(); i++) {
			outputfile_array << nvraData.get(i) << endl;
		}
		outputfile_array << "Data lines read: " << linesCtr << "; Valid NVRA records read: " << recordCtr << "; NVRA records in memory: " << nvraData.getSize() << endl; //tallie 
	}
	catch (Exception* e) {
		cout << "Error whe the object tries to call the overload cout operator and get the values of the records";
		delete e;
	}

	outputfile_array.close();
}

void printTalliesArray(unsigned long &linesCtr, unsigned long &recordCtr, TemplatedArray<NvraRecord> &nvraData) {
	cout << "Data lines read: " << linesCtr << "; Valid NVRA records read: " << recordCtr << "; NVRA records in memory: " << nvraData.getSize() << endl;
}

void printDataHashTable(HashTable<NvraRecord>*& hashTableData)
{

	try {
		
		NvraRecord *item = new NvraRecord;

		HashTableEnumerator<NvraRecord> *hashTableEnumerator = new HashTableEnumerator<NvraRecord>(hashTableData);

		unsigned long current_bucket = 0;
		unsigned long previous_bucket = -1;

		for (unsigned long i = 0; i < hashTableData->getSize(); i++) { //iterate the hash table with the enumerator, getting the objects for the correct output 

			if (hashTableEnumerator->hasNext()) {

				*item = hashTableEnumerator->next();
				current_bucket = hashTableData->getBucketNumber(item);

				if (current_bucket != previous_bucket) {

					if (i != 0)
						cout << endl;

					cout << current_bucket << ": " << *item << endl;
				}
				else { //handle the overflow in the linked lists
					cout << "OVERFLOW: " << *item << endl;
				}

				previous_bucket = current_bucket;
			}
			else if (i == hashTableData->getSize() - 1) { //if the last item it is reached

				*item = hashTableEnumerator->peek();
				current_bucket = hashTableData->getBucketNumber(item);

				if (current_bucket != previous_bucket) {

					if (i != 0)
						cout << endl;

					cout << current_bucket << ": " << *item << endl;
				}
				else {
					cout << "OVERFLOW: " << *item << endl;
				}
			}

		}

		delete hashTableEnumerator;
		delete item;
	} //exception handling
	catch (ExceptionEnumerationBeyondEnd *e) {

		cout << "ERROR iterating into the data for printing (enumerator error). Standar Output." << endl;
		delete e;

	}
	catch (ExceptionHashTableAccess *e) {

		delete e;
		throw new ExceptionHashTableAccess();
		
	}
}

void printFileHashTable(ofstream &outputfile_ht, HashTable<NvraRecord>*& hashTableData, unsigned long & linesCtr, unsigned long & recordCtr) //same proccess as above, the output is not the standar cout, it is used an ouput stream
{

	try {
		
		NvraRecord *item = new NvraRecord;

		HashTableEnumerator<NvraRecord> *hashTableEnumerator = new HashTableEnumerator<NvraRecord>(hashTableData);

		unsigned long current_bucket = 0;
		unsigned long previous_bucket = -1;

		for (unsigned long i = 0; i < hashTableData->getSize(); i++) {

			if (hashTableEnumerator->hasNext()) {

				*item = hashTableEnumerator->next();
				current_bucket = hashTableData->getBucketNumber(item);

				if (current_bucket != previous_bucket) {

					if (i != 0)
						outputfile_ht << endl;

					outputfile_ht << current_bucket << ": " << *item << endl;
				}
				else {
					outputfile_ht << "OVERFLOW: " << *item << endl;
				}

				previous_bucket = current_bucket;
			}
			else if (i == hashTableData->getSize() - 1) {

				*item = hashTableEnumerator->peek();
				current_bucket = hashTableData->getBucketNumber(item);

				if (current_bucket != previous_bucket) {

					if (i != 0)
						outputfile_ht << endl;

					outputfile_ht << current_bucket << ": " << *item << endl;
				}
				else {
					outputfile_ht << "OVERFLOW: " << *item << endl;
				}
			}

		}

		outputfile_ht << endl << "Base Capacity: " << hashTableData->getBaseCapacity() << "; Total Capacity: " << hashTableData->getTotalCapacity() << "; Load Factor: " << hashTableData->getLoadFactor() << endl;
		outputfile_ht << "Data lines read: " << linesCtr << "; Valid NVRA records read: " << recordCtr << "; NVRA records in memory: " << hashTableData->getSize() << endl;

		delete hashTableEnumerator;
		delete item;

		outputfile_ht.close();
	}
	catch (ExceptionHashTableAccess *e) {
		
		delete e;
		throw new ExceptionHashTableAccess();

	}
	catch (ExceptionEnumerationBeyondEnd *e) {
		cout << "ERROR iterating into the data for printing (enumerator error). Standar Output." << endl;
		delete e;
	}

}

void printInfoHashTable(HashTable<NvraRecord>*& hashTableData)
{
	if (hashTableData->getSize() > 0)
		cout << endl;

	cout << "Base Capacity: " << hashTableData->getBaseCapacity() << "; Total Capacity: " << hashTableData->getTotalCapacity() << "; Load Factor: " << hashTableData->getLoadFactor() << endl;
}

void printTalliesHashTable(unsigned long & linesCtr, unsigned long & recordCtr, HashTable<NvraRecord>*& hashTableData) {
	
	cout << "Data lines read: " << linesCtr << "; Valid NVRA records read: " << recordCtr << "; NVRA records in memory: " << hashTableData->getSize() << endl;

}

void printDataAVLTree(AVLTree<NvraRecord> *& AVLTree_data, AVLTreeOrder order) { //print the data from the root tree
	
	try {

		AVLTreeEnumerator<NvraRecord> *enumTree = new AVLTreeEnumerator<NvraRecord>(AVLTree_data, order); 

		while (enumTree->hasNext()) { //iterate all the tree with the enumerator methods
			
			cout << enumTree->next() << endl;

		}

	}// exception handling
	catch (ExceptionEnumerationBeyondEnd *e) {

		cout << "Enumerator beyond end, error" << endl;
		
		delete e;


	}
	catch (ExceptionAVLTreeAccess *e) {

		//cout << "Error constructor enumerator (empty tree)" << endl;
		delete e;
		throw new ExceptionAVLTreeAccess();

	}
}

void printFileAVLTree(ofstream &outputfile_tree, AVLTree<NvraRecord> *& AVLTree_data, AVLTreeOrder order, unsigned long &linesCtr, unsigned long &recordCtr) { //samer proccess as above, usign an output stream insted the standar output

	try {

		if (AVLTree_data->getSize() > 0) {
			
			AVLTreeEnumerator<NvraRecord> *enumTree = new AVLTreeEnumerator<NvraRecord>(AVLTree_data, order);

			while (enumTree->hasNext()) {

				outputfile_tree << enumTree->next() << endl;

			}

		} 

		outputfile_tree << "Data lines read: " << linesCtr << "; Valid NVRA records read: " << recordCtr << "; NVRA records in memory: " << AVLTree_data->getSize() << endl;

	}
	catch (ExceptionEnumerationBeyondEnd *e) {

		cout << "Enumerator beyond end, error" << endl;

		delete e;

	}
	catch (ExceptionAVLTreeAccess *e) {

		//cout << "Error constructor enumerator (empty tree)" << endl;
		delete e;
		throw new ExceptionAVLTreeAccess();

	}

}

void printTalliesAVLTree(unsigned long & linesCtr, unsigned long & recordCtr, AVLTree<NvraRecord> *& AVLTree_data) {
	
	cout << "Data lines read: " << linesCtr << "; Valid NVRA records read: " << recordCtr << "; NVRA records in memory: " << AVLTree_data->getSize() << endl;

}

void quitMenu(bool &screen) { //quit menu option, close  VOTER1
	cout << "Thanks for using VoteR." << endl;
	screen = false;
}

//functions that print the error data messages

void printInvalidId(long id, unsigned long line)
{
	cout << "Duplicate record ID " << id << " at line " << line << "." << endl;
}
void printInvalidData(unsigned long line)
{
	cout << "Invalid data at line " << line << "." << endl;
}

int fromString(string value) // takes a string s and returns an integer equivalent
{        

	int number;                     // this will hold the final integer value
	stringstream stringValue(value);          // create a stringstream from s ... this is like writing s to a "file" or cout,
											  // but instead it is in computer memory
	stringValue >> number;                   // now read back from the stringstream, but this time into the integer value

	return number;                  // return this integer from the program
}