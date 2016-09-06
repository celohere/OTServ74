#ifndef __OTSERV_TILE_H__
#define __OTSERV_TILE_H__

#include "cylinder.h"
#include "definitions.h"
#include "item.h"

class Creature;
class Teleport;
class TrashHolder;
class Mailbox;
class MagicField;
class QTreeLeafNode;
class BedItem;

typedef std::vector<Item*> ItemVector;
typedef std::vector<Creature*> CreatureVector;

enum tileflags_t {
	TILESTATE_NONE = 0,
	TILESTATE_PROTECTIONZONE = 1 << 0,
	TILESTATE_DEPRICATED_HOUSE = 1 << 1,
	TILESTATE_NOPVPZONE = 1 << 2,
	TILESTATE_NOLOGOUT = 1 << 3,
	TILESTATE_PVPZONE = 1 << 4,
	TILESTATE_REFRESH = 1 << 5,

	// internal usage
	TILESTATE_HOUSE = 1 << 6,
	TILESTATE_FLOORCHANGE = 1 << 7,
	TILESTATE_FLOORCHANGE_DOWN = 1 << 8,
	TILESTATE_FLOORCHANGE_NORTH = 1 << 9,
	TILESTATE_FLOORCHANGE_SOUTH = 1 << 10,
	TILESTATE_FLOORCHANGE_EAST = 1 << 11,
	TILESTATE_FLOORCHANGE_WEST = 1 << 12,
	TILESTATE_POSITIONCHANGE = 1 << 13,
	TILESTATE_MAGICFIELD = 1 << 14,
	TILESTATE_BLOCKSOLID = 1 << 15,
	TILESTATE_BLOCKPATH = 1 << 16,
	TILESTATE_IMMOVABLEBLOCKSOLID = 1 << 17,
	TILESTATE_IMMOVABLEBLOCKPATH = 1 << 18,
	TILESTATE_IMMOVABLENOFIELDBLOCKPATH = 1 << 19,
	TILESTATE_NOFIELDBLOCKPATH = 1 << 20,
};

class HouseTile;

class Tile : public Cylinder
{
public:
	static Tile null_tile;
	Tile(int x, int y, int z)
	{
		tilePos.x = x;
		tilePos.y = y;
		tilePos.z = z;
		qt_node = nullptr;

		thingCount = 0;
		m_flags = 0;
		ground = nullptr;
	}

	~Tile() override
	{
#ifdef _DEBUG
		delete ground;

		ItemVector::iterator it;
		for (it = topItems.begin(); it != topItems.end(); ++it) {
			delete *it;
		}
		topItems.clear();

		for (it = downItems.begin(); it != downItems.end(); ++it) {
			delete *it;
		}
		downItems.clear();
#endif // _DEBUG
	}

	HouseTile *getHouseTile();
	const HouseTile *getHouseTile() const;
	bool isHouseTile() const;

	int getThrowRange() const override
	{
		return 0;
	}
	bool isPushable() const override
	{
		return false;
	}

	Item *ground;
	ItemVector topItems;
	CreatureVector creatures;
	ItemVector downItems;
	QTreeLeafNode *qt_node;

	MagicField *getFieldItem() const;
	Teleport *getTeleportItem() const;
	TrashHolder *getTrashHolder() const;
	Mailbox *getMailbox() const;
	BedItem *getBedItem() const;

	Creature *getTopCreature();
	Item *getTopTopItem();
	Item *getTopDownItem();
	bool isMoveableBlocking() const;
	Thing *getTopThing();
	Item *getItemByTopOrder(uint32_t topOrder);

	uint32_t getThingCount() const
	{
		return thingCount;
	}

	bool hasProperty(enum ITEMPROPERTY prop) const;
	bool hasProperty(Item *exclude, enum ITEMPROPERTY prop) const;

	bool hasFlag(tileflags_t flag) const
	{
		return ((m_flags & (uint32_t)flag) == (uint32_t)flag);
	}
	void setFlag(tileflags_t flag)
	{
		m_flags |= (uint32_t)flag;
	}
	void resetFlag(tileflags_t flag)
	{
		m_flags &= ~(uint32_t)flag;
	}

	bool positionChange() const
	{
		return hasFlag(TILESTATE_POSITIONCHANGE);
	}
	bool floorChange() const
	{
		return hasFlag(TILESTATE_FLOORCHANGE);
	}
	bool floorChangeDown() const
	{
		return hasFlag(TILESTATE_FLOORCHANGE_DOWN);
	}
	bool floorChange(Direction direction) const
	{
		switch (direction) {
		case NORTH:
			return hasFlag(TILESTATE_FLOORCHANGE_NORTH);
		case SOUTH:
			return hasFlag(TILESTATE_FLOORCHANGE_SOUTH);
		case EAST:
			return hasFlag(TILESTATE_FLOORCHANGE_EAST);
		case WEST:
			return hasFlag(TILESTATE_FLOORCHANGE_WEST);
		default:
			return false;
		}
	}
	bool hasHeight(uint32_t n) const;
	int32_t getHeight() const;

	std::string getDescription(int32_t lookDistance) const override;

	void moveCreature(Creature *creature, Cylinder *toCylinder, bool teleport = false);

	// cylinder implementations
	ReturnValue __queryAdd(int32_t index, const Thing *thing, uint32_t count, uint32_t flags) const override;
	ReturnValue
	__queryMaxCount(int32_t index, const Thing *thing, uint32_t count, uint32_t &maxQueryCount, uint32_t flags) const override;
	ReturnValue __queryRemove(const Thing *thing, uint32_t count, uint32_t flags) const override;
	Cylinder *
	__queryDestination(int32_t &index, const Thing *thing, Item **destItem, uint32_t &flags) override;

	void __addThing(Thing *thing) override;
	void __addThing(int32_t index, Thing *thing) override;

	void __updateThing(Thing *thing, uint16_t itemId, uint32_t count) override;
	void __replaceThing(uint32_t index, Thing *thing) override;

	void __removeThing(Thing *thing, uint32_t count) override;

	int32_t __getIndexOfThing(const Thing *thing) const override;
	int32_t __getFirstIndex() const override;
	int32_t __getLastIndex() const override;
	uint32_t __getItemTypeCount(uint16_t itemId, int32_t subType = -1, bool itemCount = true) const override;
	Thing *__getThing(uint32_t index) const override;

	void
	postAddNotification(Thing *thing, const Cylinder *oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(Thing *thing,
	                                    const Cylinder *newParent,
	                                    int32_t index,
	                                    bool isCompleteRemoval,
	                                    cylinderlink_t link = LINK_OWNER) override;

	void __internalAddThing(Thing *thing) override;
	void __internalAddThing(uint32_t index, Thing *thing) override;

	const Position &getPosition() const override
	{
		return tilePos;
	}
	const Position &getTilePosition() const
	{
		return tilePos;
	}

	bool isRemoved() const override
	{
		return false;
	}

private:
	void onAddTileItem(Item *item);
	void onUpdateTileItem(uint32_t index, Item *oldItem, const ItemType &oldType, Item *newItem, const ItemType &newType);
	void onRemoveTileItem(uint32_t index, Item *item);
	void onUpdateTile();

	void updateTileFlags(Item *item, bool removing);

protected:
	uint32_t thingCount;
	Position tilePos;
	uint32_t m_flags;
};

#endif
