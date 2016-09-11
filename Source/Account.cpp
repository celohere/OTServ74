#include "otpch.h"

#include <math.h>
#include <time.h>

#include <algorithm>
#include <functional>
#include <iostream>

#include "Account.h"


uint16_t Account::getPremiumDaysLeft(time_t end)
{
	const auto now = time(nullptr);
	if (end < now)
		return 0;

	const auto days_left = (end - now) / 86400.00;	
	return static_cast<uint16_t>(ceil(days_left));
}
