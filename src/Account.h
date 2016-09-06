#ifndef __OTSERV_ACCOUNT_H__
#define __OTSERV_ACCOUNT_H__

#include <vector>
#include <string>

#include "definitions.h"

class Account
{
public:
	static uint16_t getPremiumDaysLeft(int32_t _premEnd);
	uint32_t accnumber = 0;
	int32_t warnings = 0;
	time_t premEnd = 0; // < current time is none, (time_t)(-1) is infinite
	std::string password;
	std::vector<std::string> charList;
};

#endif // #ifndef __ACCOUNT_H__
