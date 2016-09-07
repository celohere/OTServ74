#ifndef OTSERV_LOG_H_
#define OTSERV_LOG_H_
#include <iostream>


class Log
{
public:
	static inline void Msg(const char* const msg)
	{
		std::cout << LOG_PREFIX << msg << '\n';
	}

	static inline void Warning(const char* function, std::string msg)
	{
		std::cout << WARNING_PREFIX << "at " << function << ", " << msg << '\n';
	}

	static inline void Error(const char* function, std::string msg)
	{
		std::cerr << ERROR_PREFIX << "at " << function << ", " << msg << '\n';
	}

	static inline void Debug(const char* function, const char* sys, std::string msg)
	{
		std::cout << DEBUG_PREFIX << sys << ": at " << function << ", "<<  msg << '\n';
	}

private:
	static constexpr const char* const LOG_PREFIX = "[Log] ";
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

#ifdef __DEBUG_WAITINGLIST__
#define LOG_DEBUG_WAITLIST(msg) Log::Debug(__func__, "WaitingList", msg)
#else
#define LOG_DEBUG_WAITLIST(...)
#endif







#endif
