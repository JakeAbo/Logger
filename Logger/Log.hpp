#pragma once

#include <sstream>

#include "Logger.hpp"

class Log
{
private:
	Severity _severity;
	std::string _fileName;
	unsigned int _lineNumber;
	std::string _method;
	std::stringstream _buffer;

	typedef std::basic_ostream<char, std::char_traits<char>> CoutType;
	typedef CoutType& (*StandardEndLine)(CoutType&);

	Log& setSeverity(const Severity severity)
	{
		_severity = severity;
		return *this;
	}

public:
	Log(const std::string& fileName, unsigned int lineNumber, const std::string& method)
		: _severity(Severity::LOG_INFO), _fileName(fileName), _lineNumber(lineNumber), _method(method)
	{}
	Log& info()
	{
		return setSeverity(Severity::LOG_INFO);
	}
	Log& notice()
	{
		return setSeverity(Severity::LOG_NOTICE);
	}
	Log& debug()
	{
		return setSeverity(Severity::LOG_DEBUG);
	}
	Log& error()
	{
		return setSeverity(Severity::LOG_ERR);
	}

	template<class T>
	Log& operator<<(const T& t)
	{
		_buffer << t;
		return *this;
	}
	Log& operator<<(const StandardEndLine& t)
	{
		Logger::instance().pushLog(LogMessage(_severity, _fileName, _lineNumber, _method, _buffer.str()));
		return *this;
	}
};

#define LOG_INFO	Log(__FILE__, __LINE__, __FUNCTION__).info()
#define LOG_NOTICE	Log(__FILE__, __LINE__, __FUNCTION__).notice()
#define LOG_DEBUG	Log(__FILE__, __LINE__, __FUNCTION__).debug()
#define LOG_ERROR	Log(__FILE__, __LINE__, __FUNCTION__).error()
