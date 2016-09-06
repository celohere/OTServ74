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
	~BedItem() override;

	BedItem *getBed() override
	{
		return this;
	}
	const BedItem *getBed() const override
	{
		return this;
	}

	// serialization
	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream &propStream) override;
	bool serializeAttr(PropWriteStream &propWriteStream) const override;

	// override
	bool canRemove() const override
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
