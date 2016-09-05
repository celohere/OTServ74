#include "otpch.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

#include "configmanager.h"
#include "database.h"
#include "ioaccount.h"

extern ConfigManager g_config;

Account IOAccount::loadAccount(uint32_t accno)
{
	Account acc;

	Database *db = Database::instance();
	DBQuery query;
	DBResult *result;

	query << "SELECT `id`, `password`, `premend`, `warnings` FROM `accounts` WHERE `id` = " << accno;
	if ((result = db->storeQuery(query.str()))) {
		acc.accnumber = result->getDataInt("id");
		acc.password = result->getDataString("password");
		acc.premEnd = result->getDataInt("premend");
		acc.warnings = result->getDataInt("warnings");
		db->freeResult(result);

		query.str("");
		query << "SELECT `name` FROM `players` WHERE `account_id` = " << accno;
		if ((result = db->storeQuery(query.str()))) {
			acc.charList.reserve(result->size());
			do {
				const auto char_name = result->getDataString("name");
				acc.charList.push_back(char_name);
			} while (result->next());

			std::sort(acc.charList.begin(), acc.charList.end());
			db->freeResult(result);
		}
	}

	return acc;
}

bool IOAccount::saveAccount(Account acc)
{
	Database *db = Database::instance();
	DBQuery query;

	if (acc.premEnd > 0 && acc.premEnd < std::time(NULL)) {
		acc.premEnd = 0;
	}

	query << "UPDATE `accounts` SET `warnings` = " << acc.warnings
	      << ", premend = " << acc.premEnd << " WHERE `id` = " << acc.accnumber;
	return db->executeQuery(query.str());
}

bool IOAccount::getPassword(uint32_t accno, const std::string &name, std::string &password)
{
	Database *db = Database::instance();
	DBQuery query;
	DBResult *result;

	query << "SELECT `accounts`.`password` AS `password` FROM `accounts`, `players` WHERE "
	         "`accounts`.`id` = "
	      << accno << " AND `accounts`.`id` = `players`.`account_id` AND `players`.`name` = "
	      << db->escapeString(name);
	if ((result = db->storeQuery(query.str()))) {
		password = result->getDataString("password");
		db->freeResult(result);
		return true;
	}

	return false;
}
