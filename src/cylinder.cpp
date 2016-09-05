#include "otpch.h"
#include "cylinder.h"




int32_t Cylinder::__getIndexOfThing(const Thing* thing) const
{
	return -1;
}

int32_t Cylinder::__getFirstIndex() const
{
	return -1;
}

int32_t Cylinder::__getLastIndex() const
{
	return -1;
}

uint32_t Cylinder::__getItemTypeCount(uint16_t itemId, int32_t subType, bool itemCount) const
{
	return 0;
}

std::map<uint32_t, uint32_t>& Cylinder::__getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap, bool itemCount) const
{
	return countMap;
}

Thing* Cylinder::__getThing(uint32_t index) const
{
	return nullptr;
}

void Cylinder::__internalAddThing(Thing* thing)
{
	//
}

void Cylinder::__internalAddThing(uint32_t index, Thing* thing)
{
	//
}

void Cylinder::__startDecaying()
{
	//
}
