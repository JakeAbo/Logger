#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <unordered_map>

#include "Config.hpp"
#include "Log.hpp"

class LoggerTester
{
private:
	/* =================================================================== */
	/* Enable/Disable std::cout */
	/* =================================================================== */
	static void enableCout()
	{
		std::cout.clear();
	}

	static void disableCout()
	{
		std::cout.setstate(std::ios_base::failbit);
	}

	/* =================================================================== */
	/* Colours */
	/* =================================================================== */
	/* Const */
	static constexpr auto COLOUR_GREEN_BRIGHT = 10;
	static constexpr auto COLOUR_RED = 4;
	static constexpr auto COLOUR_DEFAULT_WHITE = 7;

	/* Methods */
	static void Color(int color)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
	static void setCorrect()
	{
		Color(COLOUR_GREEN_BRIGHT);
	}
	static void setWrong()
	{
		Color(COLOUR_RED);
	}
	static void setDefaultWhite()
	{
		Color(COLOUR_DEFAULT_WHITE);
	}
	static void printCorrectMsg(const std::string& msg)
	{
		enableCout();
		setCorrect();
		std::cout << msg << std::endl;
		setDefaultWhite();
		disableCout();
	}
	static void printWrongMsg(const std::string& msg)
	{
		enableCout();
		setWrong();
		std::cout << msg << std::endl;
		setDefaultWhite();
		disableCout();
	}

	/* =================================================================== */
	/* Test load - multiple threads attack logger with many log messages */
	/* =================================================================== */
	/* Helperes methods */
	static void sendMsgs(int msgNumber)
	{
		for (int i = 0; i < msgNumber; ++i)
			LOG_INFO << "Thread " << std::this_thread::get_id() << " says Hello!" << std::endl;
	}
	static void makeThreadsAndSend(int threadsNumber, int msgNumber)
	{
		std::vector<std::thread> vec;

		for (int i = 0; i < threadsNumber; ++i)
		{
			vec.push_back(std::thread(sendMsgs, msgNumber));
		}

		for (int i = 0; i < threadsNumber; ++i)
		{
			vec[i].join();
		}
	}
	static long long countNumberOfLinesCurrentLogFile()
	{
		const std::string currentDateFile = std::string(Config::LOGGER_DIR_PATH) + "\\" + TimeAdapter::getCurrentDate() + ".txt";
		std::ifstream f(currentDateFile);
		long long lines_count = std::count(std::istreambuf_iterator<char>(f),
			std::istreambuf_iterator<char>(), '\n');
		return lines_count;
	}
	static void deleteCurrentLogFile()
	{
		const std::string currentDateFile = std::string(Config::LOGGER_DIR_PATH) + "\\" + TimeAdapter::getCurrentDate() + ".txt";
		std::filesystem::remove(currentDateFile);
	}
	
	/* load test consts */
	static constexpr auto NUMBER_OF_THREADS = 5;
	static constexpr auto NUMBER_OF_MSGS = 1000;

	/* load test */
	static bool loadTest()
	{
		deleteCurrentLogFile();
		makeThreadsAndSend(NUMBER_OF_THREADS, NUMBER_OF_MSGS);
		Logger::instance().done();
		if (countNumberOfLinesCurrentLogFile() == NUMBER_OF_THREADS * NUMBER_OF_MSGS)
		{
			printCorrectMsg("load test passed 1 of 1");
			return true;
		}
		else
		{
			printWrongMsg("load test passed 0 of 1");
			printWrongMsg(std::to_string(countNumberOfLinesCurrentLogFile()) + " instead of " + std::to_string(NUMBER_OF_THREADS * NUMBER_OF_MSGS));
			return false;
		}
	}

	/* Delay test - Put sleeps between log messages and check if number of lines equals to number of msgs */
	static void delayTestHelper(int numMsg)
	{
		for (int i = 0; i < numMsg; ++i)
		{
			if (i == numMsg / 2)
				std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			LOG_INFO << "Thread " << std::this_thread::get_id() << " says Hello!" << std::endl;
		}
	}
	static bool delayTest()
	{
		Logger::instance().reset();
		deleteCurrentLogFile();
		delayTestHelper(NUMBER_OF_MSGS);
		Logger::instance().done();
		if (countNumberOfLinesCurrentLogFile() == NUMBER_OF_MSGS)
		{
			printCorrectMsg("delay test passed 1 of 1");
			return true;
		}
		else
		{
			printWrongMsg("delay test passed 0 of 1");
			printWrongMsg(std::to_string(countNumberOfLinesCurrentLogFile()) + " instead of " + std::to_string(NUMBER_OF_MSGS));
			return false;
		}
	}

public:
	static void printSum(const std::unordered_map<std::string, bool>& tests)
	{
		std::cout << "Sum: " << std::endl;
		for (const auto test : tests)
		{
			if (test.second)
			{
				printCorrectMsg("Test: " + test.first + " passed.");
			}
			else
			{
				printWrongMsg("Test: " + test.first + " failed.");
			}
		}
	}
	static void startTester()
	{
		std::unordered_map<std::string, bool> tests;

		tests.emplace(std::make_pair("load_test", false));
		tests.emplace(std::make_pair("delay_test", false));

		disableCout();
		tests["load_test"] = loadTest();
		tests["delay_test"] = delayTest();
		enableCout();

		printSum(tests);
	}
};
