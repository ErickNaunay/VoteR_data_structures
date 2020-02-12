#ifndef NVRAHASHER_H
#define NVRAHASHER_H

#include "Hasher.h"
#include "NvraRecord.h"

class NvraHasher : public Hasher<NvraRecord>
{
public:
	unsigned long hash(const NvraRecord& item) const; //just returns the key (NVRA ID)
};

#endif // !NVRAHASHER_H



