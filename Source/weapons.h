#ifndef OTSERV_WEAPONS_H_
#define OTSERV_WEAPONS_H_

#include "Actions.h"
#include "baseevents.h"
#include "combat.h"
#include "const.h"
#include "definitions.h"
#include "game.h"
#include "luascript.h"
#include "player.h"
#include "talkaction.h"

class Weapon;
class WeaponMelee;
class WeaponDistance;
class WeaponWand;

class Weapons : public BaseEvents
{
public:
	Weapons();
	~Weapons() override;

	bool loadDefaults();
	const Weapon* getWeapon(const Item* item) const;

	static int32_t weaponExhaustionTime;
	static int32_t weaponInFightTime;

	static int32_t getMaxMeleeDamage(int32_t attackSkill, int32_t attackValue);
	static int32_t getMaxMeleeWeaponDamage(int32_t attackSkill, int32_t attackValue, float attackFactor);
	static int32_t getMaxDistanceWeaponDamage(int32_t attackSkill, int32_t attackValue, float attackFactor);

protected:
	void clear() override;
	LuaScriptInterface& getScriptInterface() override;
	std::string getScriptBaseName() override;
	Event* getEvent(const std::string& nodeName) override;
	bool registerEvent(Event* event, xmlNodePtr p) override;

	typedef std::map<uint32_t, Weapon*> WeaponMap;
	WeaponMap weapons;

	LuaScriptInterface m_scriptInterface;
};

class Weapon : public Event
{
public:
	Weapon(LuaScriptInterface* _interface);
	~Weapon() override;

	bool configureEvent(xmlNodePtr p) override;
	bool loadFunction(const std::string& functionName) override;
	virtual bool configureWeapon(const ItemType& it);
	virtual bool interruptSwing() const
	{
		return false;
	}

	virtual int32_t playerWeaponCheck(Player* player, Creature* target) const;
	virtual bool useWeapon(Player* player, Item* item, Creature* target) const;

	void setCombatParam(const CombatParams& _params);

	uint16_t getID() const
	{
		return id;
	}

	static bool useFist(Player* player, Creature* target);
	virtual int32_t getWeaponDamage(const Player* player,
	                                const Creature* target,
	                                const Item* item,
	                                bool maxDamage = false) const = 0;
	virtual int32_t getElementDamage(const Player* player, const Creature* target) const
	{
		return 0;
	}

	const uint32_t getReqLevel() const
	{
		return level;
	}
	const uint32_t getReqMagLv() const
	{
		return magLevel;
	}
	const bool hasExhaustion() const
	{
		return exhaustion;
	}
	const bool isWieldedUnproperly() const
	{
		return wieldUnproperly;
	}
	const bool isPremium() const
	{
		return premium;
	}

protected:
	std::string getScriptEventName() override;

	bool executeUseWeapon(Player* player, const LuaVariant& var) const;
	bool internalUseWeapon(Player* player, Item* item, Creature* target, int32_t damageModifier) const;
	bool internalUseWeapon(Player* player, Item* item, Tile* tile) const;

	virtual void onUsedWeapon(Player* player, Item* item, Tile* destTile) const;
	virtual void onUsedAmmo(Player* player, Item* item, Tile* destTile) const;
	virtual bool getSkillType(const Player* player, const Item* item, skills_t& skill, uint32_t& skillpoint) const
	{
		return false;
	}

	int32_t getManaCost(const Player* player) const;

	uint16_t id;
	bool enabled;
	bool premium;
	bool exhaustion;
	bool wieldUnproperly;
	int32_t level;
	int32_t magLevel;
	int32_t mana;
	int32_t manaPercent;
#ifdef __PROTOCOL_76__
	int32_t soul;
#endif // __PROTOCOL_76__
	int32_t range;
	AmmoAction_t ammoAction;
	CombatParams params;

private:
	typedef std::map<int32_t, bool> VocWeaponMap;
	VocWeaponMap vocWeaponMap;
};

class WeaponMelee : public Weapon
{
public:
	WeaponMelee(LuaScriptInterface* _interface);
	~WeaponMelee() override{};

	bool configureEvent(xmlNodePtr p) override;
	bool configureWeapon(const ItemType& it) override;

	bool useWeapon(Player* player, Item* item, Creature* target) const override;
	int32_t getWeaponDamage(const Player* player,
	                        const Creature* target,
	                        const Item* item,
	                        bool maxDamage = false) const override;
	using Weapon::getElementDamage;
	virtual int32_t getElementDamage(const Player* player, const Item* item) const;

protected:
	void onUsedWeapon(Player* player, Item* item, Tile* destTile) const override;
	void onUsedAmmo(Player* player, Item* item, Tile* destTile) const override;
	bool getSkillType(const Player* player, const Item* item, skills_t& skill, uint32_t& skillpoint) const override;

	CombatType_t elementType;
	int16_t elementDamage;
};

class WeaponDistance : public Weapon
{
public:
	WeaponDistance(LuaScriptInterface* _interface);
	~WeaponDistance() override{};

	bool configureEvent(xmlNodePtr p) override;
	bool configureWeapon(const ItemType& it) override;
	bool interruptSwing() const override
	{
		return true;
	}

	int32_t playerWeaponCheck(Player* player, Creature* target) const override;
	bool useWeapon(Player* player, Item* item, Creature* target) const override;
	int32_t getWeaponDamage(const Player* player,
	                        const Creature* target,
	                        const Item* item,
	                        bool maxDamage = false) const override;

protected:
	void onUsedWeapon(Player* player, Item* item, Tile* destTile) const override;
	void onUsedAmmo(Player* player, Item* item, Tile* destTile) const override;
	bool getSkillType(const Player* player, const Item* item, skills_t& skill, uint32_t& skillpoint) const override;

	int32_t hitChance;
	int32_t maxHitChance;
	int32_t breakChance;
	int32_t ammuAttackValue;
};

class WeaponWand : public Weapon
{
public:
	WeaponWand(LuaScriptInterface* _interface);
	~WeaponWand() override{};

	bool configureEvent(xmlNodePtr p) override;
	bool configureWeapon(const ItemType& it) override;
	int32_t getWeaponDamage(const Player* player,
	                        const Creature* target,
	                        const Item* item,
	                        bool maxDamage = false) const override;

protected:
	bool getSkillType(const Player* player, const Item* item, skills_t& skill, uint32_t& skillpoint) const override
	{
		return false;
	}

	int32_t minChange;
	int32_t maxChange;
};

#endif
