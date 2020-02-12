#ifndef HASHER_H
#define HASHER_H

template<typename T>
class Hasher //interface for differents hashers
{
public:
	virtual unsigned long hash(const T& item) const = 0;

};

#endif // !HASHER_H



