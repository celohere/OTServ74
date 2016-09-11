#ifndef OTSERV_ACTIONS_H_
#define OTSERV_ACTIONS_H_

#include <map>

#include "baseevents.h"
#include "definitions.h"
#include "luascript.h"
#include "position.h"
#include "thing.h"


class Actions;
class Container;
class ItemType;




class Action : public Event
{
public:
	enum Type {
		Any,
		UniqueID,
		ActionID,
		ItemID,
		RuneID,
	};
	
	Action(LuaScriptInterface* _interface);
	~Action() override;

	bool configureEvent(xmlNodePtr p) override;

	// scripting
	virtual bool executeUse(Player* player,
	                        Item* item,
	                        const PositionEx& posFrom,
	                        const PositionEx& posTo,
	                        bool extendedUse,
	                        uint32_t creatureId);
	//

	bool getAllowFarUse() const
	{
		return allowFarUse;
	}
	void setAllowFarUse(bool v)
	{
		allowFarUse = v;
	}

	bool getCheckLineOfSight() const
	{
		return checkLineOfSight;
	}
	void setCheckLineOfSight(bool v)
	{
		checkLineOfSight = v;
	}

	virtual ReturnValue canExecuteAction(const Player* player, const Position& toPos);
	virtual bool hasOwnErrorHandler()
	{
		return false;
	}

protected:
	std::string getScriptEventName() override;

	bool allowFarUse;
	bool checkLineOfSight;
};












class Actions : public BaseEvents
{
public:
	Actions();
	~Actions() override;

	bool useItem(Player* player, const Position& pos, uint8_t index, Item* item);
	bool useItemEx(Player* player,
	               const Position& fromPos,
	               const Position& toPos,
	               uint8_t toStackPos,
	               Item* item,
	               uint32_t creatureId = 0);

	bool openContainer(Player* player, Container* container, const uint8_t index);

	ReturnValue canUse(const Player* player, const Position& pos);
	ReturnValue canUse(const Player* player, const Position& pos, const Item* item);
	ReturnValue canUseFar(const Creature* creature, const Position& toPos, bool checkLineOfSight);
	bool hasAction(const Item* item) const;

protected:
	bool executeUse(Action* action, Player* player, Item* item, const PositionEx& posEx, uint32_t creatureId);
	ReturnValue internalUseItem(Player* player, const Position& pos, uint8_t index, Item* item, uint32_t creatureId);

	bool executeUseEx(Action* action,
	                  Player* player,
	                  Item* item,
	                  const PositionEx& fromPosEx,
	                  const PositionEx& toPosEx,
	                  uint32_t creatureId);

	ReturnValue internalUseItemEx(Player* player,
	                              const PositionEx& fromPosEx,
	                              const PositionEx& toPosEx,
	                              Item* item,
	                              uint32_t creatureId,
	                              bool& isSuccess);

	void clear() override;
	LuaScriptInterface& getScriptInterface() override;
	std::string getScriptBaseName() override;
	Event* getEvent(const std::string& nodeName) override;
	bool registerEvent(Event* event, xmlNodePtr p) override;

	typedef std::map<unsigned short, Action*> ActionUseMap;
	ActionUseMap useItemMap;
	ActionUseMap uniqueItemMap;
	ActionUseMap actionItemMap;

	Action* getAction(const Item* item, Action::Type type = Action::Any) const;
	void clearMap(ActionUseMap& map);

	LuaScriptInterface m_scriptInterface;
};










#endif

