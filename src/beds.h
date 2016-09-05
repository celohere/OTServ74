#ifndef __OTS_BEDS_H__
#define __OTS_BEDS_H__

#include <ctime>
#include <list>


#include "definitions.h"
#include "item.h"
#include "position.h"



class House;
class Player;


class BedItem : public Item
{
public:
	BedItem(uint16_t id);
	virtual ~BedItem();

	virtual BedItem *getBed()
	{
		return this;
	}
	virtual const BedItem *getBed() const
	{
		return this;
	}

	// serialization
	virtual Attr_ReadValue readAttr(AttrTypes_t attr, PropStream &propStream);
	virtual bool serializeAttr(PropWriteStream &propWriteStream) const;

	// override
	virtual bool canRemove() const
	{
		return (house == NULL);
	}

	uint32_t getSleeper() const
	{
		return sleeperGUID;
	}
	void setSleeper(uint32_t guid)
	{
		sleeperGUID = guid;
	}
	time_t getSleepStart() const
	{
		return sleepStart;
	}
	void setSleepStart(time_t now)
	{
		sleepStart = now;
	}

	House *getHouse() const
	{
		return house;
	}
	void setHouse(House *h)
	{
		house = h;
	}

	bool canUse(Player *player);
	void sleep(Player *player);
	void wakeUp(Player *player);

	BedItem *getNextBedItem();

protected:
	void updateAppearance(const Player *player);
	void regeneratePlayer(Player *player) const;
	void internalSetSleeper(const Player *player);
	void internalRemoveSleeper();

	uint32_t sleeperGUID;
	time_t sleepStart;
	House *house;
};


class Beds
{
public:
	static Beds &instance();
	BedItem *getBedBySleeper(uint32_t guid);
	void setBedSleeper(BedItem *bed, uint32_t guid);

protected:
	std::map<uint32_t, BedItem*> BedSleepersMap;
};








#endif
