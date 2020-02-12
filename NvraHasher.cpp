#include "NvraHasher.h"

unsigned long NvraHasher::hash(const NvraRecord & item) const //used tu get the key for hashing (ID NVRA RECORD)
{
	unsigned int id = 0;
	return item.getNum(id);
}
