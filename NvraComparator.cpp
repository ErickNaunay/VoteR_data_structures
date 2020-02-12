#include "NvraComparator.h"

NvraComparator::NvraComparator(unsigned int column)
{
	if (column > 23)
		throw new ExceptionIndexOutOfRange();

	this->column = column;
}

int NvraComparator::compare(const NvraRecord & item1, const NvraRecord & item2) const // item1 is less than 2, return -1 ; item 1 bigger than item 2, return 1; item1 equals to 2 , reutnr 0
{
	try { //depends of the column, the item get an specific num or string value 

		if (column < 3)
		{
			if (item1.getNum(column) < item2.getNum(column)) {
				return -1;
			}
			else if (item1.getNum(column) == item2.getNum(column)) {
				return 0;
			}
			else if (item1.getNum(column) > item2.getNum(column)) {
				return 1;
			}
		}
		else if (column == 3) {

			if (item1.getString(0) < item2.getString(0)) {
				return -1;
			}
			else if (item1.getString(0) == item2.getString(0)) {
				return 0;
			}
			else if (item1.getString(0) > item2.getString(0)) {
				return 1;
			}
		}
		else if (column > 3 && column < 11) {

			if (item1.getNum(column - 1) < item2.getNum(column - 1)) {
				return -1;
			}
			else if (item1.getNum(column - 1) == item2.getNum(column - 1)) {
				return 0;
			}
			else if (item1.getNum(column - 1) > item2.getNum(column - 1)) {
				return 1;
			}
		}
		else if (column == 11) {

			if (item1.getString(1) < item2.getString(1)) {
				return -1;
			}
			else if (item1.getString(1) == item2.getString(1)) {
				return 0;
			}
			else if (item1.getString(1) > item2.getString(1)) {
				return 1;
			}
		}
		else if (column == 12) {

			if (item1.getString(2) < item2.getString(2)) {
				return -1;
			}
			else if (item1.getString(2) == item2.getString(2)) {
				return 0;
			}
			else if (item1.getString(2) > item2.getString(2)) {
				return 1;
			}
		}
		else if (column > 12 && column < 24) {

			if (item1.getNum(column - 3) < item2.getNum(column - 3)) {
				return -1;
			}
			else if (item1.getNum(column - 3) == item2.getNum(column - 3)) {
				return 0;
			}
			else if (item1.getNum(column - 3) > item2.getNum(column - 3)) {
				return 1;
			}
		}

	}
	catch (Exception *e)
	{
		delete e;
		throw new ExceptionIndexOutOfRange();
	}


	return 0;
}
