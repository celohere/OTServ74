#ifndef OTSERV_WAITING_LIST_H_
#define OTSERV_WAITING_LIST_H_

#include <vector>

#include "definitions.h"
#include "game.h"
#include "networkmessage.h"

struct WaitingClient {
	uint32_t acc;
	uint32_t ip;
	std::string name;
	bool premium;
	int64_t timeout;
};

class WaitingList
{
public:
	using WaitList = std::vector<WaitingClient>;
	using WaitListItr  = std::vector<WaitingClient>::iterator;
	using slot_t = std::vector<WaitingClient>::size_type;

	virtual ~WaitingList() = default;
	bool clientLogin(const Player& player);
	int32_t getClientSlot(const Player& player);


	static WaitingList& getInstance();
	static int32_t getTime(slot_t slot);

private:
	WaitListItr findClient(const Player& player, slot_t* slot);
	int32_t getTimeOut(slot_t slot);
	WaitList m_waitList;
	static WaitingList s_instance;
};


inline WaitingList& WaitingList::getInstance()
{
	return s_instance;
}









#endif
