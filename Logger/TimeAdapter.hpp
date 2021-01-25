#pragma once

#include <chrono>
#include <ctime>
#include <string>

class TimeAdapter
{
public:
	/* Local time helper methods */
	static std::tm localtime_xp(std::time_t timer)
	{
		std::tm bt{};
#if defined(__unix__)
		localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
		localtime_s(&bt, &timer);
#else
		static std::mutex mtx;
		std::lock_guard<std::mutex> lock(mtx);
		bt = *std::localtime(&timer);
#endif
		return bt;
	}
	static std::string time_stamp(const std::string& fmt)
	{
		char buf[64];
		auto bt = localtime_xp(std::time(0));
		return { buf, std::strftime(buf, sizeof(buf), fmt.c_str(), &bt) };
	}
	static auto getTimeFormatX()
	{
		return time_stamp("%X");
	}
	static const std::string getCurrentDate()
	{
		return time_stamp("%d.%m.%Y");
	}
};
