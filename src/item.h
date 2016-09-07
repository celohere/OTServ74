#ifndef OTSERV_ITEM_H_
#define OTSERV_ITEM_H_

#include <iostream>
#include <list>
#include <vector>

#include "definitions.h"
#include "ItemAttributes.h"
#include "thing.h"
#include "items.h"

class Creature;
class Player;
class Container;
class Depot;
class Teleport;
class TrashHolder;
class Mailbox;
class Door;
class MagicField;
class BedItem;


enum ITEMPROPERTY {
	BLOCKSOLID = 0,
	HASHEIGHT,
	BLOCKPROJECTILE,
	BLOCKPATH,
	ISVERTICAL,
	ISHORIZONTAL,
	MOVEABLE,
	IMMOVABLEBLOCKSOLID,
	IMMOVABLEBLOCKPATH,
	IMMOVABLENOFIELDBLOCKPATH,
	NOFIELDBLOCKPATH,
	SUPPORTHANGABLE
};


enum TradeEvents_t {
	ON_TRADE_TRANSFER,
	ON_TRADE_CANCEL,
};


/*from iomapotbm.h*/
#pragma pack(1)
struct TeleportDest {
	uint16_t _x;
	uint16_t _y;
	uint8_t _z;
};
#pragma pack()


enum AttrTypes_t {
	// attr 0 means end of attribute list
	// ATTR_DESCRIPTION = 1,
	// ATTR_EXT_FILE = 2,
	ATTR_TILE_FLAGS = 3,
	ATTR_ACTION_ID = 4,
	ATTR_UNIQUE_ID = 5,
	ATTR_TEXT = 6,
	ATTR_DESC = 7,
	ATTR_TELE_DEST = 8,
	ATTR_ITEM = 9,
	ATTR_DEPOT_ID = 10,
	// ATTR_EXT_SPAWN_FILE = 11,
	ATTR_RUNE_CHARGES = 12,
	// ATTR_EXT_HOUSE_FILE = 13,
	ATTR_HOUSEDOORID = 14,
	ATTR_COUNT = 15,
	ATTR_DURATION = 16,
	ATTR_DECAYING_STATE = 17,
	ATTR_WRITTENBY = 19,
	ATTR_SLEEPERGUID = 20,
	ATTR_SLEEPSTART = 21,
	// This is NOT stored in serializeAttr, but rather used by IOMapSerialize
	// look at that code for the ugly hack that makes this work. :)
	ATTR_CONTAINER_ITEMS = 22,
};


class Item : virtual public Thing, public ItemAttributes
{
public:
	// Factory member to create item of right type based on type
	static Item* CreateItem(const uint16_t _type, uint16_t _count = 1);
	static Item* CreateItem(PropStream& propStream);
	static Items items;

	// Constructor for items
	Item(const uint16_t _type, uint16_t _count = 0);
	Item(const Item& i);
	virtual Item* clone() const;
	virtual void copyAttributes(Item* item);

	~Item() override;

	Item* getItem() override
	{
		return this;
	}
	const Item* getItem() const override
	{
		return this;
	}
	virtual Container* getContainer()
	{
		return nullptr;
	}
	virtual const Container* getContainer() const
	{
		return nullptr;
	}
	virtual Teleport* getTeleport()
	{
		return nullptr;
	}
	virtual const Teleport* getTeleport() const
	{
		return nullptr;
	}
	virtual TrashHolder* getTrashHolder()
	{
		return nullptr;
	}
	virtual const TrashHolder* getTrashHolder() const
	{
		return nullptr;
	}
	virtual Mailbox* getMailbox()
	{
		return nullptr;
	}
	virtual const Mailbox* getMailbox() const
	{
		return nullptr;
	}
	virtual Door* getDoor()
	{
		return nullptr;
	}
	virtual const Door* getDoor() const
	{
		return nullptr;
	}
	virtual MagicField* getMagicField()
	{
		return nullptr;
	}
	virtual const MagicField* getMagicField() const
	{
		return nullptr;
	}

	virtual BedItem* getBed()
	{
		return nullptr;
	}
	virtual const BedItem* getBed() const
	{
		return nullptr;
	}

	static std::string
	getDescription(const ItemType& it, int32_t lookDistance, const Item* item = nullptr, int32_t subType = -1);

	static std::string getWeightDescription(const ItemType& it, double weight, uint32_t count = 1);

	virtual Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream);
	virtual bool unserializeAttr(PropStream& propStream);
	virtual bool unserializeItemNode(FileLoader& f, NODE node, PropStream& propStream);

	virtual bool serializeAttr(PropWriteStream& propWriteStream) const;

	bool isPushable() const override
	{
		return !isNotMoveable();
	}
	int getThrowRange() const override
	{
		return (isPickupable() ? 15 : 2);
	}

	std::string getDescription(int32_t lookDistance) const override;
	std::string getXRayDescription() const override;
	std::string getWeightDescription() const;

	uint16_t getID() const
	{
		return id;
	}
	uint16_t getClientID() const
	{
		return items[id].clientId;
	}
	void setID(uint16_t newid);

	// Returns the player that is holding this item in his inventory
	Player* getHoldingPlayer();
	const Player* getHoldingPlayer() const;

	WeaponType_t getWeaponType() const
	{
		return items[id].weaponType;
	}
	Ammo_t getAmuType() const
	{
		return items[id].amuType;
	}
	int32_t getShootRange() const
	{
		return items[id].shootRange;
	}

	virtual double getWeight() const;
	int getAttack() const
	{
		return items[id].attack;
	}
	int getArmor() const
	{
		return items[id].armor;
	}
	int getDefense() const
	{
		return items[id].defense;
	}
	int getSlotPosition() const
	{
		return items[id].slot_position;
	}
	int getWieldPosition() const
	{
		return items[id].wield_position;
	}
	int getHitChance() const
	{
		return items[id].hitChance;
	}

	bool isReadable() const
	{
		return items[id].canReadText;
	}
	bool canWriteText() const
	{
		return items[id].canWriteText;
	}
	uint16_t getMaxWriteLength() const
	{
		return items[id].maxTextLen;
	}

	int getWorth() const;
	void getLight(LightInfo& lightInfo);

	bool hasProperty(enum ITEMPROPERTY prop) const;
	bool isBlocking() const
	{
		return items[id].blockSolid;
	}
	bool isStackable() const
	{
		return items[id].stackable;
	}
	bool isRune() const
	{
		return items[id].isRune();
	}
	bool isFluidContainer() const
	{
		return (items[id].isFluidContainer());
	}
	bool isAlwaysOnTop() const
	{
		return items[id].alwaysOnTop;
	}
	bool isGroundTile() const
	{
		return items[id].isGroundTile();
	}
	bool isSplash() const
	{
		return items[id].isSplash();
	}
	bool isMagicField() const
	{
		return items[id].isMagicField();
	}
	bool isMoveable() const
	{
		return items[id].moveable;
	}
	bool isNotMoveable() const
	{
		return !items[id].moveable;
	}
	bool isPickupable() const
	{
		return items[id].pickupable;
	}
	bool isWeapon() const
	{
		return (items[id].weaponType != WEAPON_NONE);
	}
	bool isUseable() const
	{
		return items[id].useable;
	}
	bool isHangable() const
	{
		return items[id].isHangable;
	}
	bool isRoteable() const
	{
		const ItemType& it = items[id];
		return it.rotable && it.rotateTo;
	}
	bool isDoor() const
	{
		return items[id].isDoor();
	}
	bool isBed() const
	{
		return items[id].isBed();
	}
	bool hasCharges() const
	{
		return items[id].charges != 0;
	}

	bool floorChangeDown() const
	{
		return items[id].floorChangeDown;
	}
	bool floorChangeNorth() const
	{
		return items[id].floorChangeNorth;
	}
	bool floorChangeSouth() const
	{
		return items[id].floorChangeSouth;
	}
	bool floorChangeEast() const
	{
		return items[id].floorChangeEast;
	}
	bool floorChangeWest() const
	{
		return items[id].floorChangeWest;
	}

	const std::string& getName() const
	{
		return items[id].name;
	}
	const std::string& getPluralName() const
	{
		return items[id].pluralName;
	}

	// get the number of items
	uint16_t getItemCount() const
	{
		return count;
	}
	void setItemCount(uint8_t n)
	{
		count = n;
	}

	static uint32_t countByType(const Item* i, int checkType, bool multiCount);

	void setDefaultSubtype();
	bool hasSubType() const;
	uint16_t getSubType() const;
	void setSubType(uint16_t n);

	void setUniqueId(uint16_t n);

	void setDefaultDuration()
	{
		uint32_t duration = getDefaultDuration();
		if (duration != 0) {
			setDuration(duration);
		}
	}
	uint32_t getDefaultDuration() const
	{
		return items[id].decayTime * 1000;
	}
	bool canDecay();

	virtual bool canRemove() const
	{
		return true;
	}
	virtual bool canTransform() const
	{
		return true;
	}
	virtual void onRemoved(){};
	virtual bool onTradeEvent(TradeEvents_t event, Player* owner)
	{
		return true;
	}

	virtual void __startDecaying();

	bool isCleanable() const
	{
		return (!loadedOnMap && (getUniqueId() == 0 && getActionId() == 0) &&
		        isPickupable() && canRemove());
	}

	bool loadedOnMap = false;

protected:
	// If weight description is needed from outside of item class
	// use the other getWeightDescription
	std::string getWeightDescription(double weight) const;

	uint16_t id; // the same id as in ItemType
	uint8_t count; // number of stacked items

	// Don't add variables here, use the ItemAttribute class.
};

typedef std::list<Item*> ItemList;

inline uint32_t Item::countByType(const Item* i, int checkType, bool multiCount)
{
	if (checkType == -1 || checkType == i->getSubType()) {
		if (multiCount) {
			return i->getItemCount();
		}

		if (i->isRune()) {
			return i->getCharges();
		}

		return i->getItemCount();
	}

	return 0;
}


#endif
