#pragma once

#include <queue>
#include <mutex>
#include <exception>
#include <condition_variable>
#include <optional>

#include "FileStreamer.hpp"
#include "ConsoleStreamer.hpp"
#include "Config.hpp"

class Logger
{
	friend class LoggerTester;

private:
	bool _isRunning;
	std::thread _thd;
	mutable std::mutex _m_queue;
	Severity _severityLevel;
	std::queue<LogMessage> _queue;
	std::condition_variable _dataCond;
	std::vector<std::unique_ptr<Streamer>> _streamers;

	Logger& operator=(const Logger&) = delete;
	Logger(const Logger& other) = delete;
	Logger() : _isRunning(true), _severityLevel(Config::SEVERITY_LEVEL)
	{
		_streamers.emplace_back(std::make_unique<ConsoleStreamer>());
		_streamers.emplace_back(std::make_unique<FileStreamer>());
		_thd = std::thread(&Logger::run, this);
	}
	~Logger()
	{
		if (_thd.joinable())
		{
			done();
		}
	}

	void reset()
	{
		_isRunning = true;
		_severityLevel = Config::SEVERITY_LEVEL;
		_streamers.clear();
		_streamers.emplace_back(std::make_unique<ConsoleStreamer>());
		_streamers.emplace_back(std::make_unique<FileStreamer>());
		_thd = std::thread(&Logger::run, this);
	}
	void done()
	{
		_isRunning = false; /* Non need of lock because destructor call onced */
		_thd.join();
		for (auto& s : _streamers)
			s->done();
	}

	void run()
	{
		while (true)
		{
			while (auto potential_item = waitAndPopItem())
			{
				auto item = *potential_item;
				if (checkSeverity(item.getSeverity()))
				{
					for (const auto& streamer : _streamers)
					{
						streamer->sendLog(item);
					}
				}
			}

			if (!_isRunning) break;
		}
	}

	std::optional<LogMessage> waitAndPopItem()
	{
		std::unique_lock<std::mutex> lock(_m_queue);

		if (_queue.empty())
		{
			auto now = std::chrono::system_clock::now();
			auto timeout_point = now + std::chrono::milliseconds(200);

			if (_dataCond.wait_until(lock, timeout_point) == std::cv_status::timeout)
				return{};
		}

		if (!_queue.empty())
		{
			auto item = _queue.front();
			_queue.pop();
			return{ std::move(item) };
		}

		return{};
	}

	bool checkSeverity(const Severity severity)
	{
		return static_cast<unsigned>(severity) <= static_cast<unsigned>(_severityLevel);
	}

public:
	static Logger& instance()
	{
		static Logger instance;
		return instance;
	}
	void pushLog(LogMessage&& item)
	{
		std::lock_guard<std::mutex> lock(_m_queue);
		_queue.emplace(std::move(item));
		_dataCond.notify_one();
	}
};
