#include "otpch.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>

#include "Account.h"


uint16_t Account::getPremiumDaysLeft(int32_t _premEnd)
{
	using std::ceil;
	if (_premEnd < time(nullptr)) {
		return 0;
	}

	return static_cast<uint16_t>(ceil(static_cast<double>(_premEnd - time(nullptr)) / 86400.));
}
