#pragma once
#include "Severity.hpp"

class LogMessage
{
private:
	Severity _severity;
	std::string _file;
	unsigned int _lineNumber;
	std::string _function;
	std::string _msg;

public:
	LogMessage(Severity severity, std::string file, unsigned int lineNumber, std::string function, std::string msg)
		: _severity(severity), _file(std::move(file)), _lineNumber(lineNumber), _function(std::move(function)), _msg(std::move(msg))
	{}

	Severity getSeverity() const
	{
		return _severity;
	}
	const std::string& getFile() const
	{
		return _file;
	}
	unsigned int getLineNumber() const
	{
		return _lineNumber;
	}
	const std::string& getFunction() const
	{
		return _function;
	}
	const std::string& getMessage() const
	{
		return _msg;
	}
};