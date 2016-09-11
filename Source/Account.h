#ifndef OTSERV_ACCOUNT_H_
#define OTSERV_ACCOUNT_H_

#include <string>
#include <vector>

#include "definitions.h"

struct Account {
	static uint16_t getPremiumDaysLeft(time_t end);
	uint32_t accnumber = 0;
	int32_t warnings = 0;
	time_t premEnd = 0; // < current time is none, (time_t)(-1) is infinite
	std::string password;
	std::vector<std::string> charList;
};

#endif

