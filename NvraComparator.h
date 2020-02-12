#ifndef NVRA_COMPARATOR_H
#define NVRA_COMPARATOR_H

#include "NvraRecord.h"
#include "Comparator.h"

class NvraComparator : public Comparator<NvraRecord>
{
private:
	unsigned int column = 0;
public:
	NvraComparator(unsigned int column);
	int compare(const NvraRecord& itrem1, const NvraRecord& item2) const;
};

#endif // !NVRA_COMPARATOR_H




