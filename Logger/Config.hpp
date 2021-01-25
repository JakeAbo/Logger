#pragma once

#include "Severity.hpp"

class Config
{
	friend class FileStreamer;
	friend class Logger;
	friend class LoggerTester;

private:
	static constexpr auto LOGGER_DIR_PATH = "C:\\LOGS";
	static const Severity SEVERITY_LEVEL = Severity::LOG_INFO;

};