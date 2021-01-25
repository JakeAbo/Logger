#pragma once

#include <iostream>
#include "Streamer.hpp"

class ConsoleStreamer : public Streamer
{
public:
	virtual void sendLog(const LogMessage& msg) override
	{
		if (static_cast<unsigned>(msg.getSeverity()) <= static_cast<unsigned>(Severity::LOG_ERR))
		{
			std::clog << msg.getMessage() << std::endl;
		}
		else
		{
			std::cout << msg.getMessage() << std::endl;
		}
	}
};