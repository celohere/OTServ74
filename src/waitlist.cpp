#include "otpch.h"

#include <iostream>
#include <sstream>
#include <map>
#include <boost/algorithm/string/predicate.hpp>

#include "status.h"
#include "waitlist.h"


static void clean_wait_list(WaitingList::WaitList* waitList);


WaitingList WaitingList::s_waitingList;


WaitingList::WaitListItr WaitingList::findClient(const Player& player, slot_t* const slot)
{
	using boost::algorithm::iequals;
	*slot = 1;

	const auto itr = std::find_if(m_waitList.begin(), m_waitList.end(),
	[&](const Wait& wait) {
		if (wait.acc == player.getAccount() 
		    && wait.ip == player.getIP()
		    && iequals(wait.name, player.getName())) {
			return true;
		} else {
			++(*slot);
			return false;
		}	
	});

	return itr;

}






int32_t WaitingList::getTime(const slot_t slot)
{
	if (slot < 5) {
		return 5;
	} else if (slot < 10) {
		return 10;
	} else if (slot < 20) {
		return 20;
	} else if (slot < 50) {
		return 60;
	} else {
		return 120;
	}
}

int32_t WaitingList::getTimeOut(slot_t slot)
{
	// timeout is set to 15 seconds longer than expected retry attempt
	return getTime(slot) + 15;
}

bool WaitingList::clientLogin(const Player& player)
{
	if (player.hasFlag(PlayerFlag_CanAlwaysLogin)) {
		return true;
	}

	const auto& status = *Status::instance();

	if (m_waitList.empty() && status.getPlayersOnline() < status.getMaxPlayersOnline()) {
		// no waiting list and enough room
		return true;
	}

	clean_wait_list(&m_waitList);
	slot_t slot;

	auto it = findClient(player, &slot);
	if (it != m_waitList.end()) {
		auto& wait = *it;
		if ((status.getPlayersOnline() + slot) <= status.getMaxPlayersOnline()) {
			LOG_DEBUG_WAITLIST("Name: "s + wait.name " can now login");
			m_waitList.erase(it);
			return true;
		} else {
			// let them wait a bit longer
			wait.timeout = OTSYS_TIME() + getTimeOut(slot) * 1000;
			return false;
		}
	}

	slot = m_waitList.size();
	Wait wait;
	wait.name = player.getName();
	wait.acc = player.getAccount();
	wait.ip = player.getIP();
	wait.premium = player.isPremium();
	wait.timeout = OTSYS_TIME() + getTimeOut(slot) * 1000;

	LOG_DEBUG_WAITLIST("Name: " + wait.name + "(" +
          std::to_string(m_waitList.size() + 1) + ")" + " has been added to the waiting list");

	// TODO: premium priority
	m_waitList.push_back(std::move(wait));
	return false;
}

int32_t WaitingList::getClientSlot(const Player& player)
{
	slot_t slot;
	const auto itr = findClient(player, &slot);
	if (itr != m_waitList.end()) {
		return slot;
	}

	LOG_DEBUG_WAITLIST("error, trying to find slot for unknown acc: "s
	                  + player.getAccount() + " with ip "s + player->getIP());

	return -1;
}




static void clean_wait_list(WaitingList::WaitList* const waitList)
{
	const auto begin = waitList->begin();
	const auto end = waitList->end();
	for (auto itr = begin; itr != end; ++itr) {
		if (itr->timeout - OTSYS_TIME() <= 0) {
			LOG_DEBUG_WAITLIST("Name: " + itr->name + " has timed out!");
			waitList->erase(itr);
		}
	}
}
