#ifndef __OTSERV_THING_H__
#define __OTSERV_THING_H__

#include <sstream>

#include "definitions.h"
#include "position.h"


class Tile;
class Cylinder;
class Item;
class Creature;




/*Notice: remember to add new error codes to global.lua*/
enum ReturnValue {
	RET_NOERROR = 1,
	RET_NOTPOSSIBLE = 2,
	RET_NOTENOUGHROOM = 3,
	RET_PLAYERISPZLOCKED = 4,
	RET_PLAYERISNOTINVITED = 5,
	RET_CANNOTTHROW = 6,
	RET_THEREISNOWAY = 7,
	RET_DESTINATIONOUTOFREACH = 8,
	RET_CREATUREBLOCK = 9,
	RET_NOTMOVEABLE = 10,
	RET_DROPTWOHANDEDITEM = 11,
	RET_BOTHHANDSNEEDTOBEFREE = 12,
	RET_CANONLYUSEONEWEAPON = 13,
	RET_NEEDEXCHANGE = 14,
	RET_CANNOTBEDRESSED = 15,
	RET_PUTTHISOBJECTINYOURHAND = 16,
	RET_PUTTHISOBJECTINBOTHHANDS = 17,
	RET_TOOFARAWAY = 18,
	RET_FIRSTGODOWNSTAIRS = 19,
	RET_FIRSTGOUPSTAIRS = 20,
	RET_CONTAINERNOTENOUGHROOM = 21,
	RET_NOTENOUGHCAPACITY = 22,
	RET_CANNOTPICKUP = 23,
	RET_THISISIMPOSSIBLE = 24,
	RET_DEPOTISFULL = 25,
	RET_CREATUREDOESNOTEXIST = 26,
	RET_CANNOTUSETHISOBJECT = 27,
	RET_PLAYERWITHTHISNAMEISNOTONLINE = 28,
	RET_NOTREQUIREDLEVELTOUSERUNE = 29,
	RET_YOUAREALREADYTRADING = 30,
	RET_THISPLAYERISALREADYTRADING = 31,
	RET_YOUMAYNOTLOGOUTDURINGAFIGHT = 32,
	RET_DIRECTPLAYERSHOOT = 33,
	RET_NOTENOUGHLEVEL = 34,
	RET_NOTENOUGHMAGICLEVEL = 35,
	RET_NOTENOUGHMANA = 36,
#ifdef __PROTOCOL_76__
	RET_NOTENOUGHSOUL = 37,
#endif
	RET_YOUAREEXHAUSTED = 38,
	RET_PLAYERISNOTREACHABLE = 39,
	RET_CANONLYUSETHISRUNEONCREATURES = 40,
	RET_ACTIONNOTPERMITTEDINPROTECTIONZONE = 41,
	RET_YOUMAYNOTATTACKTHISPLAYER = 42,
	RET_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE = 43,
	RET_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE = 44,
	RET_YOUMAYNOTATTACKTHISCREATURE = 45,
	RET_YOUCANONLYUSEITONCREATURES = 46,
	RET_CREATUREISNOTREACHABLE = 47,
	RET_TURNSECUREMODETOATTACKUNMARKEDPLAYERS = 48,
	RET_YOUNEEDPREMIUMACCOUNT = 49,
	RET_YOUNEEDTOLEARNTHISSPELL = 50,
	RET_YOURVOCATIONCANNOTUSETHISSPELL = 51,
	RET_YOUNEEDAWEAPONTOUSETHISSPELL = 52,
	RET_PLAYERISPZLOCKEDLEAVEPVPZONE = 53,
	RET_PLAYERISPZLOCKEDENTERPVPZONE = 54,
	RET_ACTIONNOTPERMITTEDINANOPVPZONE = 55,
	RET_YOUCANNOTLOGOUTHERE = 56,
	RET_YOUNEEDAMAGICITEMTOCASTSPELL = 57,
	RET_CANNOTCONJUREITEMHERE = 58,
	RET_YOUNEEDTOSPLITYOURSPEARS = 59,
	RET_NAMEISTOOAMBIGIOUS = 60,
	RET_YOUARENOTTHEOWNER = 61,
	RET_NOTREQUIREDPROFESSION = 62,
	RET_NOTREQUIREDLEVEL = 63,
	RET_NEEDPREMIUMTOEQUIPITEM = 64
};







class Thing
{
public:
	virtual ~Thing() = default;
	
	const Cylinder* getParent() const;
	const Cylinder* getTopParent() const;
	Cylinder* getParent();
	Cylinder* getTopParent(); // returns Tile/Container or a Player

	virtual bool isRemoved() const;
	virtual const Tile* getTile() const;
	virtual const Position& getPosition() const;
	virtual std::string getXRayDescription() const;
	virtual const Item* getItem() const;
	virtual const Creature* getCreature() const;
		
	virtual Tile* getTile();
	virtual Item* getItem();
	virtual Creature* getCreature();
	virtual void setParent(Cylinder* cylinder);

	virtual bool isPushable() const = 0;
	virtual int getThrowRange() const = 0;
	virtual std::string getDescription(int32_t lookDistance) const = 0;


	void useThing2();
	void releaseThing2();
	
protected:
	Thing() = default;


private:
	Cylinder* parent = nullptr;
	int32_t useCount = 0;

};




inline const Cylinder* Thing::getParent() const
{
	return parent;
}


inline const Item* Thing::getItem() const
{
	return nullptr;
}


inline const Creature* Thing::getCreature() const
{
	return nullptr;
}


inline Cylinder* Thing::getParent()
{
	return parent;
}


inline Item* Thing::getItem()
{
	return nullptr;
}


inline Creature* Thing::getCreature()
{
	return nullptr;
}


inline void Thing::setParent(Cylinder* cylinder)
{
	parent = cylinder;
}



inline void Thing::useThing2()
{
	++useCount;
}


inline void Thing::releaseThing2()
{
	--useCount;
	if (useCount <= 0)
		delete this;
}




#endif
