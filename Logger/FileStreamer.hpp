#pragma once

#include <iostream>
#include <fstream>
#include <mutex>
#include <filesystem>

#include "Config.hpp"
#include "TimeAdapter.hpp"
#include "Streamer.hpp"

class FileStreamer : public Streamer
{
	friend class LoggerTester;

private:
	std::map<Severity, std::string> _stringifySeverityMap;
	std::string _currentLogFileName;
	std::ofstream _logFile;
	std::string _logsDir;
	std::string _lastLogDate;
	std::mutex _m;

	const std::string getSeverityString(const Severity severity)
	{
		return _stringifySeverityMap.count(severity) ? _stringifySeverityMap[severity] : "LOG_UNKNOWN";
	}

	void createDirIfNotExist()
	{
		if (!std::filesystem::exists(Config::LOGGER_DIR_PATH))
			std::filesystem::create_directory(Config::LOGGER_DIR_PATH);
	}

public:
	FileStreamer()
		: _logsDir(Config::LOGGER_DIR_PATH), _currentLogFileName()
	{
		createDirIfNotExist();

		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::LOG_EMERG, "LOG_EMERG"));
		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::LOG_ALERT, "LOG_ALERT"));
		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::LOG_CRIT, "LOG_CRIT"));
		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::LOG_ERR, "LOG_ERR"));
		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::LOG_WARNING, "LOG_WARNING"));
		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::LOG_NOTICE, "LOG_NOTICE"));
		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::LOG_INFO, "LOG_INFO"));
		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::LOG_DEBUG, "LOG_DEBUG"));
		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::SEVERITY_HIGH_BOUND, "SEVERITY_HIGH_BOUND"));
		_stringifySeverityMap.insert(std::make_pair<Severity, std::string>(Severity::LOG_ALL, "LOG_ALL"));
	}

	virtual void sendLog(const LogMessage& msg) override
	{
		bool isLogFileOpened;
		const std::string currentDate = TimeAdapter::getCurrentDate();

		isLogFileOpened = _logFile.is_open();
		if (isLogFileOpened && _lastLogDate != currentDate) _logFile.close();

		isLogFileOpened = _logFile.is_open();
		if (!isLogFileOpened)
		{
			_lastLogDate = currentDate;
			_currentLogFileName = currentDate + ".txt";
			_logFile.open(_logsDir + "\\" + _currentLogFileName, std::ios::app);
		}

		_logFile << "[" + currentDate + " " << TimeAdapter::getTimeFormatX() << "] ["
			+ getSeverityString(msg.getSeverity()) + "] "
			+ msg.getFile() + ":" + std::to_string(msg.getLineNumber()) + " "
			+ msg.getFunction() + "() "
			+ msg.getMessage()
			<< std::endl;
	}

	virtual void done()
	{
		_logFile.close();
	}
};

