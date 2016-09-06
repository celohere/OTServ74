#include "otpch.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "actions.h"
#include "beds.h"
#include "combat.h"
#include "container.h"
#include "depot.h"
#include "game.h"
#include "house.h"
#include "item.h"
#include "luascript.h"
#include "mailbox.h"
#include "teleport.h"
#include "trashholder.h"
#include "weapons.h"



extern Game g_game;
extern Weapons *g_weapons;

Items Item::items;


Item* Item::CreateItem(const uint16_t _type, uint16_t _count)
{
	Item* newItem = nullptr;
	const ItemType &it = Item::items[_type];

	if (it.id != 0) {
		if (it.isDepot()) {
			newItem = new Depot(_type);
		} else if (it.isContainer()) {
			newItem = new Container(_type);
		} else if (it.isTeleport()) {
			newItem = new Teleport(_type);
		} else if (it.isMagicField()) {
			newItem = new MagicField(_type);
		} else if (it.isDoor()) {
			newItem = new Door(_type);
		} else if (it.isTrashHolder()) {
			newItem = new TrashHolder(_type, it.magicEffect);
		} else if (it.isMailbox()) {
			newItem = new Mailbox(_type);
		}
		//[ added for beds system
		else if (it.isBed()) {
			newItem = new BedItem(_type);
		}
		//]
		else {
			newItem = new Item(_type, _count);
		}

		newItem->useThing2();
	}
	return newItem;
}



Item* Item::CreateItem(PropStream &propStream)
{
	uint16_t _id;
	if (!propStream.GET_UINT16(_id)) {
		return nullptr;
	}

	const ItemType &iType = Item::items[_id];
	uint8_t _count = 0;

	if (iType.stackable || iType.isSplash() || iType.isFluidContainer()) {
		if (!propStream.GET_UINT8(_count)) {
			return nullptr;
		}
	}

	return Item::CreateItem(_id, _count);
}



Item::Item(const uint16_t _id, uint16_t _count) :
	Thing(),
	ItemAttributes(),
	id(_id)
{
	const ItemType& it = items[id];
	count = 1;

	if (it.charges != 0)
		setCharges(it.charges);

	if (it.isFluidContainer() || it.isSplash())
		setFluidType(_count);
	else if (it.stackable && _count != 0)
		count = _count;
	else if (it.charges != 0 && _count != 0)
		setCharges(_count);

	setDefaultDuration();
	loadedOnMap = false;
}



Item::Item(const Item& other) : 
	Thing(), 
	ItemAttributes(other),
	id(other.id),
	count(other.count)
{

}



Item* Item::clone() const
{
	Item* const newItem = CreateItem(id, count);
	newItem->m_attrFlags = this->m_attrFlags;
	newItem->m_attributes = this->m_attributes;
	return newItem;
}


void Item::copyAttributes(Item* const other)
{
	other->m_attrFlags = this->m_attrFlags;
	other->m_attributes = this->m_attributes;
	removeAttribute(ATTR_ITEM_DECAYING);
	removeAttribute(ATTR_ITEM_DURATION);
}



Item::~Item()
{
	// std::cout << "Item destructor " << this << std::endl;

	if (getUniqueId() != 0) {
		ScriptEnviroment::removeUniqueThing(this);
	}
}

void Item::setDefaultSubtype()
{
	const ItemType &it = items[id];

	count = 1;
	if (it.charges != 0) {
		setCharges(it.charges);
	}
}

void Item::setID(uint16_t newid)
{
	const ItemType &prevIt = Item::items[id];
	id = newid;

	const ItemType &it = Item::items[newid];
	uint32_t newDuration = it.decayTime * 1000;

	if (newDuration == 0 && !it.stopTime && it.decayTo == -1) {
		removeAttribute(ATTR_ITEM_DECAYING);
		removeAttribute(ATTR_ITEM_DURATION);
	}

	if (newDuration > 0 && (!prevIt.stopTime || !hasAttribute(ATTR_ITEM_DURATION))) {
		setDecaying(DECAYING_FALSE);
		setDuration(newDuration);
	}
}

bool Item::hasSubType() const
{
	const ItemType &it = items[id];
	return it.hasSubType();
}

uint16_t Item::getSubType() const
{
	const ItemType &it = items[getID()];

	if (it.isFluidContainer() || it.isSplash()) {
		return getFluidType();
	} else if (it.charges != 0) {
		return getCharges();
	}

	return count;
}

Player *Item::getHoldingPlayer()
{
	Cylinder *p = getParent();
	while (p) {
		if (p->getCreature())
			// Must be a player, creatures are not cylinders
			return p->getCreature()->getPlayer();
		p = p->getParent();
	}

	return nullptr;
}

const Player *Item::getHoldingPlayer() const
{
	return const_cast<Item *>(this)->getHoldingPlayer();
}

void Item::setSubType(uint16_t n)
{
	const ItemType &it = items[id];

	if (it.isFluidContainer() || it.isSplash()) {
		setFluidType(n);
	} else if (it.charges != 0) {
		setCharges(n);
	} else {
		count = n;
	}
}

Attr_ReadValue Item::readAttr(AttrTypes_t attr, PropStream &propStream)
{
	switch (attr) {
	case ATTR_COUNT: {
		uint8_t _count = 0;
		if (!propStream.GET_UINT8(_count)) {
			return ATTR_READ_ERROR;
		}

		setSubType(_count);
		break;
	}

	case ATTR_ACTION_ID: {
		uint16_t _actionid = 0;
		if (!propStream.GET_UINT16(_actionid)) {
			return ATTR_READ_ERROR;
		}

		setActionId(_actionid);
		break;
	}

	case ATTR_UNIQUE_ID: {
		uint16_t _uniqueid;
		if (!propStream.GET_UINT16(_uniqueid)) {
			return ATTR_READ_ERROR;
		}

		setUniqueId(_uniqueid);
		break;
	}

	case ATTR_TEXT: {
		std::string _text;
		if (!propStream.GET_STRING(_text)) {
			return ATTR_READ_ERROR;
		}

		setText(_text);
		break;
	}

	case ATTR_WRITTENBY: {
		std::string _writer;
		if (!propStream.GET_STRING(_writer)) {
			return ATTR_READ_ERROR;
		}

		setWriter(_writer);
		break;
	}

	case ATTR_DESC: {
		std::string _text;
		if (!propStream.GET_STRING(_text)) {
			return ATTR_READ_ERROR;
		}

		setSpecialDescription(_text);
		break;
	}

	case ATTR_RUNE_CHARGES: {
		uint8_t _charges = 1;
		if (!propStream.GET_UINT8(_charges)) {
			return ATTR_READ_ERROR;
		}

		setSubType(_charges);
		break;
	}

	case ATTR_DURATION: {
		uint32_t duration = 0;
		if (!propStream.GET_UINT32(duration)) {
			return ATTR_READ_ERROR;
		}

		if (((int32_t)duration) < 0) {
			duration = 0;
		}
		setDuration(duration);
		break;
	}

	case ATTR_DECAYING_STATE: {
		uint8_t state = 0;
		if (!propStream.GET_UINT8(state)) {
			return ATTR_READ_ERROR;
		}

		if (state != DECAYING_FALSE) {
			setDecaying(DECAYING_PENDING);
		}
		break;
	}

	// these should be handled through derived classes
	// If these are called then something has changed in the items.otb since the map was saved
	// just read the values

	// Depot class
	case ATTR_DEPOT_ID: {
		uint16_t _depotId;
		if (!propStream.GET_UINT16(_depotId)) {
			return ATTR_READ_ERROR;
		}

		return ATTR_READ_CONTINUE;
	}

	// Door class
	case ATTR_HOUSEDOORID: {
		uint8_t _doorId;
		if (!propStream.GET_UINT8(_doorId)) {
			return ATTR_READ_ERROR;
		}

		return ATTR_READ_CONTINUE;
	}

	// Bed class
	case ATTR_SLEEPERGUID: {
		uint32_t _guid;
		if (!propStream.GET_UINT32(_guid)) {
			return ATTR_READ_ERROR;
		}

		return ATTR_READ_CONTINUE;
	}

	case ATTR_SLEEPSTART: {
		uint32_t sleep_start;
		if (!propStream.GET_UINT32(sleep_start)) {
			return ATTR_READ_ERROR;
		}

		return ATTR_READ_CONTINUE;
	}

	// Teleport class
	case ATTR_TELE_DEST: {
		TeleportDest tele_dest;
		if (!propStream.GET_UINT16(tele_dest._x) || !propStream.GET_UINT16(tele_dest._y) ||
		    !propStream.GET_UINT8(tele_dest._z)) {
			return ATTR_READ_ERROR;
		}

		return ATTR_READ_CONTINUE;
	}

	// Container class
	case ATTR_CONTAINER_ITEMS: {
		uint32_t count;
		if (!propStream.GET_UINT32(count)) {
			return ATTR_READ_ERROR;
		}

		// We cant continue parse attributes since there is
		// container data after this attribute.
		return ATTR_READ_ERROR;
	}

	default:
		std::cout << "attr=" << attr << "\n";
		return ATTR_READ_ERROR;
	}

	return ATTR_READ_CONTINUE;
}

bool Item::unserializeAttr(PropStream &propStream)
{
	uint8_t attr_type;
	while (propStream.GET_UINT8(attr_type) && attr_type != 0) {
		Attr_ReadValue ret = readAttr((AttrTypes_t)attr_type, propStream);
		if (ret == ATTR_READ_ERROR) {
			return false;
		} else if (ret == ATTR_READ_END) {
			return true;
		}
	}

	return true;
}

bool Item::unserializeItemNode(FileLoader &f, NODE node, PropStream &propStream)
{
	return unserializeAttr(propStream);
}

bool Item::serializeAttr(PropWriteStream &propWriteStream) const
{
	if (isStackable() || isSplash() || isFluidContainer()) {
		uint8_t _count = getSubType();
		propWriteStream.ADD_UINT8(ATTR_COUNT);
		propWriteStream.ADD_UINT8(_count);
	}

	if (hasCharges()) {
		uint8_t _count = getCharges();
		propWriteStream.ADD_UINT8(ATTR_RUNE_CHARGES);
		propWriteStream.ADD_UINT8(_count);
	}

	if (!isNotMoveable() /*moveable*/) {
		uint16_t _actionId = getActionId();
		if (_actionId) {
			propWriteStream.ADD_UINT8(ATTR_ACTION_ID);
			propWriteStream.ADD_UINT16(_actionId);
		}
	}

	const std::string &_text = getText();
	if (_text.length() > 0) {
		propWriteStream.ADD_UINT8(ATTR_TEXT);
		propWriteStream.ADD_STRING(_text);
	}

	const std::string &_writer = getWriter();
	if (_writer.length() > 0) {
		propWriteStream.ADD_UINT8(ATTR_WRITTENBY);
		propWriteStream.ADD_STRING(_writer);
	}

	const std::string &_specialDesc = getSpecialDescription();
	if (_specialDesc.length() > 0) {
		propWriteStream.ADD_UINT8(ATTR_DESC);
		propWriteStream.ADD_STRING(_specialDesc);
	}

	uint32_t duration = getDuration();
	if (duration != 0) {
		propWriteStream.ADD_UINT8(ATTR_DURATION);
		propWriteStream.ADD_UINT32(duration);
	}

	uint32_t decayState = getDecaying();
	if (decayState == DECAYING_TRUE || decayState == DECAYING_PENDING) {
		propWriteStream.ADD_UINT8(ATTR_DECAYING_STATE);
		propWriteStream.ADD_UINT8(decayState);
	}

	return true;
}

bool Item::hasProperty(enum ITEMPROPERTY prop) const
{
	const ItemType &it = items[id];

	switch (prop) {
	case BLOCKSOLID:
		if (it.blockSolid) return true;
		break;

	case MOVEABLE:
		if (it.moveable && getUniqueId() == 0) return true;
		break;

	case HASHEIGHT:
		if (it.hasHeight) return true;
		break;

	case BLOCKPROJECTILE:
		if (it.blockProjectile) return true;
		break;

	case BLOCKPATH:
		if (it.blockPathFind) return true;
		break;

	case ISVERTICAL:
		if (it.isVertical) return true;
		break;

	case ISHORIZONTAL:
		if (it.isHorizontal) return true;
		break;

	case IMMOVABLEBLOCKSOLID:
		if (it.blockSolid && (!it.moveable || getUniqueId() != 0)) return true;
		break;

	case IMMOVABLEBLOCKPATH:
		if (it.blockPathFind && (!it.moveable || getUniqueId() != 0)) return true;
		break;

	case SUPPORTHANGABLE:
		if (it.isHorizontal || it.isVertical) return true;
		break;

	case IMMOVABLENOFIELDBLOCKPATH:
		if (!it.isMagicField() && it.blockPathFind && (!it.moveable || getUniqueId() != 0))
			return true;
		break;

	case NOFIELDBLOCKPATH:
		if (!it.isMagicField() && it.blockPathFind) return true;
		break;

	default:
		return false;
	}
	return false;
}

double Item::getWeight() const
{
	if (isStackable()) {
		return items[id].weight * std::max((int32_t)1, (int32_t)count);
	}

	return items[id].weight;
}

std::string Item::getDescription(const ItemType &it, int32_t lookDistance, const Item *item /*= NULL*/, int32_t subType /*= -1*/)
{
	std::stringstream s;

	if (item) {
		subType = item->getSubType();
	}

	if (it.name.length()) {
		if (it.stackable && item && item->getItemCount() > 1) {
			if (it.showCount) {
				s << (int)item->getItemCount() << " ";
			}

			s << it.pluralName;
		} else {
			if (!it.article.empty()) {
				s << it.article << " ";
			}
			s << it.name;
		}
	} else {
		s << "an item of type " << it.id;
	}

	if (it.isRune()) {
		uint32_t charges =
		std::max((uint32_t)1, (uint32_t)(item == nullptr ? it.charges : item->getCharges()));

		if (it.runeLevel > 0 || it.runeMagLevel > 0) {
			if (it.runeLevel > 0) {
				s << " for level " << it.runeLevel;
			}
			if (it.runeMagLevel > 0) {
				if (it.runeLevel > 0) {
					s << " and";
				}
				s << " for magic level " << it.runeMagLevel;
			}
		}
		s << ". It's " << it.article << " \"" << it.runeSpellName << "\"-spell (" << charges << "x). ";
	} else if (it.weaponType != WEAPON_NONE) {
		if (it.weaponType == WEAPON_DIST && it.amuType != AMMO_NONE) {
			if (it.attack != 0) {
				s << ", Atk" << std::showpos << it.attack << std::noshowpos;
			}
		} else if (it.weaponType != WEAPON_AMMO &&
		           it.weaponType != WEAPON_WAND) { // Arrows and Bolts doesn't show atk
			s << " (";
			if (it.attack != 0) {
				s << "Atk:" << (int)it.attack;
			}

			if (it.defense != 0) {
				if (it.attack != 0) s << " ";

				s << "Def:" << (int)it.defense;
			}

			if (it.abilities.stats[STAT_MAGICPOINTS] != 0) {
				if (it.attack != 0 || it.defense != 0) s << ", ";

				s << "magic level " << std::showpos
				  << (int)it.abilities.stats[STAT_MAGICPOINTS] << std::noshowpos;
			}
			s << ")";
		}
		s << ".";
	} else if (it.armor != 0 || it.abilities.absorb.any()) {
		if (it.showCharges) {
			if (subType > 1) {
				s << " that has " << (int32_t)subType << " charges left";
			} else {
				s << " that has 1 charge left";
			}
		}

		s << " (Arm:" << it.armor << ").";
	} else if (it.isFluidContainer()) {
		if (item && item->getFluidType() != 0) {
			s << " of " << items[item->getFluidType()].name << ".";
		} else {
			s << ". It is empty.";
		}
	} else if (it.isSplash()) {
		s << " of ";
		if (item && item->getFluidType() != 0) {
			s << items[item->getFluidType()].name;
		} else {
			s << items[1].name;
		}
	} else if (it.isContainer()) {
		s << " (Vol:" << (int)it.maxItems << ").";
	} else if (it.isKey()) {
		if (item) {
			s << " (Key:" << (int)item->getActionId() << ").";
		} else {
			s << " (Key:" << (int)0 << ").";
		}
	} else if (it.allowDistRead) {
		s << std::endl;

		if (item && item->getText() != "") {
			if (lookDistance <= 4) {
				if (item->getWriter().length()) {
					s << item->getWriter() << " wrote: ";
				} else {
					s << "You read: ";
				}

				s << item->getText();
			} else {
				s << "You are too far away to read it.";
			}
		} else {
			s << "Nothing is written on it.";
		}
	} else if (it.showCharges) {
		uint32_t charges = (item == nullptr ? it.charges : item->getCharges());
		if (charges > 1) {
			s << " that has " << (int)charges << " charges left.";
		} else {
			s << " that has 1 charge left.";
		}
	} else if (it.showDuration) {
		if (item && item->hasAttribute(ATTR_ITEM_DURATION)) {
			int32_t duration = item->getDuration() / 1000;
			s << " that has energy for ";

			if (duration >= 120) {
				s << duration / 60 << " minutes left.";
			} else if (duration > 60) {
				s << "1 minute left.";
			} else {
				s << " less than a minute left.";
			}
		} else {
			s << " that is brand-new.";
		}
	} else {
		s << ".";
	}

	if (it.wieldInfo != 0) {
		s << std::endl << "It can only be wielded properly by ";
		if (it.wieldInfo & WIELDINFO_PREMIUM) {
			s << "premium ";
		}

		if (it.wieldInfo & WIELDINFO_VOCREQ) {
			s << it.vocationString;
		} else {
			s << "players";
		}

		if (it.wieldInfo & WIELDINFO_LEVEL) {
			s << " of level " << (int)it.minReqLevel << " or higher";
		}

		if (it.wieldInfo & WIELDINFO_MAGLV) {
			if (it.wieldInfo & WIELDINFO_LEVEL) {
				s << " and";
			} else {
				s << " of";
			}
			s << " magic level " << (int)it.minReqMagicLevel << " or higher";
		}

		s << ".";
	}

	if (lookDistance <= 1) {
		double weight = (item == nullptr ? it.weight : item->getWeight());
		if (weight > 0) {
			s << std::endl << getWeightDescription(it, weight);
		}
	}

	if (item && item->getSpecialDescription() != "") {
		s << std::endl << item->getSpecialDescription().c_str();
	} else if (it.description.length() && lookDistance <= 1) {
		s << std::endl << it.description;
	}

	return s.str();
}

std::string Item::getDescription(int32_t lookDistance) const
{
	std::stringstream s;
	const ItemType &it = items[id];

	s << getDescription(it, lookDistance, this);
	return s.str();
}

std::string Item::getXRayDescription() const
{
	std::stringstream ret;

	ret << "ID: " << getID() << std::endl;
	uint16_t actionId = getActionId();
	uint16_t uniqueId = getUniqueId();
	if (actionId > 0) ret << "Action ID: " << actionId << std::endl;
	if (uniqueId > 0) ret << "Unique ID: " << uniqueId << std::endl;
#ifdef __DEBUG__
	if (getContainer())
		ret << "There are " << getContainer()->getTotalAmountOfItemsInside() - 1
		    << " things inside of this." << std::endl;
#endif
	ret << Thing::getXRayDescription();
	return ret.str();
}

std::string Item::getWeightDescription() const
{
	double weight = getWeight();
	if (weight > 0) {
		return getWeightDescription(weight);
	} else {
		return "";
	}
}

std::string Item::getWeightDescription(double weight) const
{
	const ItemType &it = Item::items[id];
	return getWeightDescription(it, weight, count);
}

std::string Item::getWeightDescription(const ItemType &it, double weight, uint32_t count /*= 1*/)
{
	std::stringstream ss;
	if (it.stackable && count > 1) {
		ss << "They weigh " << std::fixed << std::setprecision(2) << weight << " oz.";
	} else {
		ss << "It weighs " << std::fixed << std::setprecision(2) << weight << " oz.";
	}
	return ss.str();
}

void Item::setUniqueId(uint16_t n)
{
	if (getUniqueId() != 0) return;

	ItemAttributes::setUniqueId(n);
	ScriptEnviroment::addUniqueThing(this);
}

bool Item::canDecay()
{
	if (isRemoved()) {
		return false;
	}

	if (getUniqueId() != 0 || getActionId() != 0) {
		return false;
	}

	return (items[id].decayTo != -1);
}

int Item::getWorth() const
{
	switch (getID()) {
	case ITEM_COINS_GOLD:
		return getItemCount();
	case ITEM_COINS_PLATINUM:
		return getItemCount() * 100;
	case ITEM_COINS_CRYSTAL:
		return getItemCount() * 10000;
	default:
		return 0;
	}
}

void Item::getLight(LightInfo &lightInfo)
{
	const ItemType &it = items[id];
	lightInfo.color = it.lightColor;
	lightInfo.level = it.lightLevel;
}






void Item::__startDecaying()
{
	g_game.startDecay(this);
}
