#pragma once
#include "LogMessage.hpp"

class Streamer
{
public:
	virtual void sendLog(const LogMessage& msg) = 0;
	virtual void done() {}
};