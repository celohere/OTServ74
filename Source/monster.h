//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////


#ifndef __OTSERV_MONSTER_H__
#define __OTSERV_MONSTER_H__

#include "definitions.h"
#include "monsters.h"
#include "tile.h"

class Creature;
class Game;
class Spawn;

typedef std::list<Creature*> CreatureList;

enum TargetSearchType_t { TARGETSEARCH_DEFAULT, TARGETSEARCH_RANDOM, TARGETSEARCH_ATTACKRANGE };

class Monster : public Creature
{
private:
	Monster(MonsterType* mtype);
	// const Monster& operator=(const Monster& rhs);

public:
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	static uint32_t monsterCount;
#endif

	static Monster* createMonster(MonsterType* mType);
	static Monster* createMonster(const std::string& name);
	static int32_t despawnRange;
	static int32_t despawnRadius;

	~Monster() override;

	Monster* getMonster() override
	{
		return this;
	}
	const Monster* getMonster() const override
	{
		return this;
	}

	uint32_t idRange() override
	{
		return 0x40000000;
	}
	static AutoList<Monster> listMonster;
	void removeList() override
	{
		listMonster.removeList(getID());
	}
	void addList() override
	{
		listMonster.addList(this);
	}

#ifndef __PROTOCOL_76__
	bool canSee(const Position& pos) const override;
#endif

	const std::string& getName() const override
	{
		return mType->name;
	}
	const std::string& getNameDescription() const override
	{
		return mType->nameDescription;
	}
	std::string getDescription(int32_t lookDistance) const override
	{
		return strDescription + '.';
	}

	RaceType_t getRace() const override
	{
		return mType->race;
	}
	int32_t getArmor() const override
	{
		return mType->armor;
	}
	int32_t getDefense() const override
	{
		return mType->defense;
	}
	bool isPushable() const override
	{
		return mType->pushable && (baseSpeed > 0);
	}
	bool isAttackable() const override
	{
		return mType->isAttackable;
	}

	bool canPushItems() const
	{
		return mType->canPushItems;
	}
	bool canPushCreatures() const
	{
		return mType->canPushCreatures;
	}
	bool isHostile() const
	{
		return mType->isHostile;
	}
	bool canSeeInvisibility() const override
	{
		return isImmune(CONDITION_INVISIBLE);
	}
	uint32_t getManaCost() const
	{
		return mType->manaCost;
	}
	void setSpawn(Spawn* _spawn)
	{
		spawn = _spawn;
	}

	void onAttackedCreatureDissapear(bool isLogout) override;
	void onFollowCreatureDissapear(bool isLogout) override;

	void onCreatureAppear(const Creature* creature, bool isLogin) override;
	void onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout) override;
	void onCreatureMove(const Creature* creature,
	                    const Tile* newTile,
	                    const Position& newPos,
	                    const Tile* oldTile,
	                    const Position& oldPos,
	                    uint32_t oldStackPos,
	                    bool teleport) override;

	void drainHealth(Creature* attacker, CombatType_t combatType, int32_t damage) override;
	void changeHealth(int32_t healthChange) override;

	void onWalk() override;
	bool getNextStep(Direction& dir) override;
	void onFollowCreatureComplete(const Creature* creature) override;

	void onThink(uint32_t interval) override;

	bool challengeCreature(Creature* creature) override;
	bool convinceCreature(Creature* creature) override;

	void setNormalCreatureLight() override;
	bool getCombatValues(int32_t& min, int32_t& max) override;

	void doAttacking(uint32_t interval) override;
	bool hasExtraSwing() override
	{
		return extraMeleeAttack;
	}

	bool searchTarget(TargetSearchType_t searchType = TARGETSEARCH_DEFAULT);
	bool selectTarget(Creature* creature);

	const CreatureList& getTargetList()
	{
		return targetList;
	}
	const CreatureList& getFriendList()
	{
		return friendList;
	}

	bool isTarget(Creature* creature);
	bool isFleeing() const
	{
		return getHealth() <= mType->runAwayHealth;
	}

	BlockType_t blockHit(Creature* attacker,
	                     CombatType_t combatType,
	                     int32_t& damage,
	                     bool checkDefense = false,
	                     bool checkArmor = false) override;

private:
	CreatureList targetList;
	CreatureList friendList;

	MonsterType* mType;

	int32_t minCombatValue;
	int32_t maxCombatValue;
	uint32_t attackTicks;
	uint32_t targetTicks;
	uint32_t targetChangeTicks;
	uint32_t defenseTicks;
	uint32_t yellTicks;
	int32_t targetChangeCooldown;
	bool resetTicks;
	bool isActivated;
	bool extraMeleeAttack;

	Spawn* spawn;
	bool isMasterInRange;

	std::string strDescription;

	virtual void onCreatureEnter(Creature* creature);
	virtual void onCreatureLeave(Creature* creature);
	void onCreatureFound(Creature* creature, bool pushFront = false);

	void updateLookDirection();

	void updateTargetList();
	void clearTargetList();
	void clearFriendList();

	void die() override;
	Item* getCorpse() override;
	bool despawn();
	bool inDespawnRange(const Position& pos);

	bool activate(bool forced = false);
	bool deactivate(bool forced = false);

	void onAddCondition(ConditionType_t type) override;
	void onEndCondition(ConditionType_t type) override;
	void onCreatureConvinced(const Creature* convincer, const Creature* creature) override;

	bool canUseAttack(const Position& pos, const Creature* target) const;
	bool canUseSpell(const Position& pos, const Position& targetPos, const spellBlock_t& sb, uint32_t interval, bool& inRange);
	bool getRandomStep(const Position& creaturePos, Direction& dir);
	bool getDanceStep(const Position& creaturePos, Direction& dir, bool keepAttack = true, bool keepDistance = true);
	bool isInSpawnRange(const Position& toPos);
	bool canWalkTo(Position pos, Direction dir);

	bool pushItem(Item* item, int32_t radius);
	void pushItems(Tile* tile);
	bool pushCreature(Creature* creature);
	void pushCreatures(Tile* tile);

	void onThinkTarget(uint32_t interval);
	void onThinkYell(uint32_t interval);
	void onThinkDefense(uint32_t interval);

	bool isFriend(const Creature* creature);
	bool isOpponent(const Creature* creature);

	uint64_t getLostExperience() const override
	{
		return ((skillLoss ? mType->experience : 0));
	}
	virtual uint16_t getLookCorpse()
	{
		return mType->lookCorpse;
	}
	void dropLoot(Container* corpse) override;
	uint32_t getDamageImmunities() const override
	{
		return mType->damageImmunities;
	}
	uint32_t getConditionImmunities() const override
	{
		return mType->conditionImmunities;
	}
	uint16_t getLookCorpse() const override
	{
		return mType->lookCorpse;
	}
	void getPathSearchParams(const Creature* creature, FindPathParams& fpp) const override;
	bool useCacheMap() const override
	{
		return true;
	}
};

#endif
