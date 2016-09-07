#ifndef OTSERV_WAITLIST_H_
#define OTSERV_WAITLIST_H_

#include <vector>

#include "definitions.h"
#include "game.h"
#include "networkmessage.h"

struct Wait {
	uint32_t acc;
	uint32_t ip;
	std::string name;
	bool premium;
	int64_t timeout;
};


class WaitingList
{
public:
	using WaitList = std::vector<Wait>;
	using WaitListItr  = std::vector<Wait>::iterator;
	using slot_t = std::vector<Wait>::size_type;

	virtual ~WaitingList() = default;
	bool clientLogin(const Player& player);
	int32_t getClientSlot(const Player& player);


	static WaitingList& getInstance();
	static int32_t getTime(slot_t slot);
protected:
	WaitListItr findClient(const Player& player, slot_t* slot);
	int32_t getTimeOut(slot_t slot);

	std::vector<Wait> m_waitListPriority;
	std::vector<Wait> m_waitList;
	static WaitingList s_waitingList;
};


inline WaitingList& WaitingList::getInstance()
{
	return s_waitingList;
}









#endif
