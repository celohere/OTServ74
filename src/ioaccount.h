#ifndef __OTSERV_IOACCOUNT_H__
#define __OTSERV_IOACCOUNT_H__

#include "Account.h"
#include "definitions.h"

#include <string>

/** Baseclass for all Player-Loaders */
class IOAccount
{
public:
	IOAccount()
	{
	}
	~IOAccount()
	{
	}

	static IOAccount *instance()
	{
		static IOAccount instance;
		return &instance;
	}

	Account loadAccount(uint32_t accno);
	bool saveAccount(Account acc);
	bool getPassword(uint32_t accno, const std::string &name, std::string &password);
};

#endif
