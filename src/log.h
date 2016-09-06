#ifndef __OTSERV_LOG_H__
#define __OTSERV_LOG_H__
#include <iostream>


class Log
{
public:
	static inline void Msg(const char* const msg)
	{
		std::cout << msg << '\n';
	}

	static inline void Warning(const char* function, const char* const msg)
	{
		std::cout << WARNING_PREFIX << "in " << function << " " << msg << '\n';
	}

	static inline void Error(const char* function, const char* const msg)
	{
		std::cerr << ERROR_PREFIX << "in " << function << " " << msg << '\n';
	}

	static inline void Debug(const char* function, const char* sys, const char* msg)
	{
		std::cout << DEBUG_PREFIX << sys << "in " << function << msg << '\n';
	}

private:
	static constexpr const char* const ERROR_PREFIX = "[Error] ";
	static constexpr const char* const WARNING_PREFIX = "[Warning] ";
	static constexpr const char* const DEBUG_PREFIX = "[Debug] ";
};


#define LOG_MSG(msg) Log::Msg(msg)
#define LOG_ERROR(msg) Log::Error(__func__, msg)
#define LOG_WARNING(msg) Log::Warning(__func__, msg)

#ifdef __DEBUG_MOVESYS__
#define LOG_DEBUG_MOVESYS(msg) Log::Debug(__func__, "MOVESYS", msg)
#else
#define LOG_DEBUG_MOVESYS(...)
#endif


#endif