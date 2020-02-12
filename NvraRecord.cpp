#include "NvraRecord.h"

#include "NvraRecord.h"
#include <string>

void NvraRecord::addNum(unsigned int num)
{
	if (numCtr >= MAX_NUMS)
		throw new ExceptionIndexOutOfRange();

	nums[numCtr] = num;
	numCtr++;

}

void NvraRecord::addString(std::string string)
{
	if (strCtr >= MAX_STRINGS)
		throw new ExceptionIndexOutOfRange();

	strings[strCtr] = string;
	strCtr++;

}

unsigned int NvraRecord::getNum(unsigned int index) const
{
	if (index >= MAX_NUMS)
		throw new ExceptionIndexOutOfRange();

	return nums[index];
}

std::string NvraRecord::getString(unsigned int index) const
{
	if (index >= MAX_STRINGS)
		throw new ExceptionIndexOutOfRange();

	return strings[index];
}

void NvraRecord::setNum(unsigned int num, unsigned int index)
{
	if (index >= MAX_NUMS)
		throw new ExceptionIndexOutOfRange();

	nums[index] = num;
}

void NvraRecord::setString(std::string string, unsigned int index)
{
	if (index >= MAX_STRINGS)
		throw new ExceptionIndexOutOfRange();

	strings[index] = string;
}

std::ostream & operator<<(std::ostream & os, const NvraRecord & record)
{
	for (int i = 0; i < MAX_NUMS; i++) // loop for access the info of all the columns of each record
	{
		if (i == 3) { //print string columns
			os << record.strings[0] << ";";
		}
		else if (i == 10) {
			os << record.strings[1] << ";";
			os << record.strings[2] << ";";
		}

		os << record.nums[i];

		if (i < MAX_NUMS - 1) {
			os << ";";
		}
	}
	return os;
}


