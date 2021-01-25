#pragma once

#include <map>
#include <string>

enum class Severity
{
	LOG_EMERG = 0,			/* system is unusable  - A panic condition was reported to all proccesses */
	LOG_ALERT = 1,			/* action must be taken immediately - Should be corrected immediately */
	LOG_CRIT = 2,			/* critical conditions */
	LOG_ERR = 3,			/* error conditions */
	LOG_WARNING = 4,		/* warning conditions */
	LOG_NOTICE = 5,			/* normal but significant condition - Requiring special hanlding */
	LOG_INFO = 6,			/* informational - General information message */
	LOG_DEBUG = 7,			/* debug-level messages - Messages useful for debugging programs */
	SEVERITY_HIGH_BOUND = 7 /* LOG_DEBUG */,
	LOG_UNKNOWN = 8			/* LOG_DEBUG */,
	LOG_ALL = LOG_EMERG | LOG_ALERT | LOG_CRIT | LOG_ERR | LOG_WARNING | LOG_NOTICE | LOG_INFO | LOG_DEBUG
};

static const Severity getSeverityByString(const std::string& severityStr)
{
	Severity retVal;
	static std::map<std::string, Severity> stringifySeverityMap;
	if (!stringifySeverityMap.size())
	{
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("LOG_EMERG", Severity::LOG_EMERG));
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("LOG_ALERT", Severity::LOG_ALERT));
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("LOG_CRIT", Severity::LOG_CRIT));
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("LOG_ERR", Severity::LOG_ERR));
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("LOG_WARNING", Severity::LOG_WARNING));
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("LOG_NOTICE", Severity::LOG_NOTICE));
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("LOG_INFO", Severity::LOG_INFO));
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("LOG_DEBUG", Severity::LOG_DEBUG));
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("SEVERITY_HIGH_BOUND", Severity::SEVERITY_HIGH_BOUND));
		stringifySeverityMap.insert(std::make_pair<std::string, Severity>("LOG_ALL", Severity::LOG_ALL));
	}

	if (stringifySeverityMap.count(severityStr)) retVal = stringifySeverityMap[severityStr];
	else retVal = Severity::LOG_UNKNOWN;

	return retVal;
}