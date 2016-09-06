#ifndef __OTSERV__ASSERT_H__
#define __OTSERV__ASSERT_H__



#ifndef NDEBUG
	#include <iostream>

	#define ASSERT_MSG(cond) { \
		if(!(cond)) { \
			std::cout << "ASSERTATION FAILED: " << #cond << '\n'; \
			std::terminate(); \
		} \
	}
#else
	#define ASSERT_MSG(cond)
#endif

















#endif