#include "Logger.h"

#include <iostream>
#include <chrono>
#include <ctime>

namespace Connect
{
	const std::string Logger::LEVELS[] = {"TRACE", "WARN ", "ERROR"};
	Logger* Logger::s_Instance = nullptr;

	Logger::Logger(std::string loggerName)
	{
		// Check that the logger hasn't already been created
		if (s_Instance)
		{
			s_Instance->PrintMessage("Attmpt to create multiple logger objects", LOG_LEVEL::WARN);
			return;
		}
		else
		{
			s_Instance = this;
		}

		// Set default log level
		m_LogLevel = LOG_LEVEL::WARN;

		if (loggerName.empty())
			m_LoggerName = "Default Logger";
		else
			m_LoggerName = loggerName;

		return;
	}

	Logger::~Logger() {}

	void Logger::PrintMessage(std::string message, LOG_LEVEL level)
	{
		// Only print messages above the current log level
		if (level < m_LogLevel)
			return;
		
		// Print in "Level - [DateTime] - LoggerName: Message" Format
		std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		printf("%s - [%.19s] - %s: %s\n", LEVELS[(int)level].c_str(), std::ctime(&currentTime), m_LoggerName.c_str(), message.c_str());
	}
}