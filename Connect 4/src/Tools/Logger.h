#pragma once

#include <string>

#define LOG_TRACE(x) Connect::Logger::s_Instance->PrintMessage(x, Connect::LOG_LEVEL::TRACE);
#define LOG_WARN(x) Connect::Logger::s_Instance->PrintMessage(x, Connect::LOG_LEVEL::WARN);
#define LOG_ERROR(x) Connect::Logger::s_Instance->PrintMessage(x, Connect::LOG_LEVEL::ERROR);

namespace Connect
{
	enum class LOG_LEVEL
	{
		TRACE = 0,
		WARN  = 1,
		ERROR = 2
	};

	// Simple Logging System to print info, warnings and errors
	class Logger
	{
	public:
		Logger(std::string loggerName);
		virtual ~Logger();

	public:
		void PrintMessage(std::string message, LOG_LEVEL level);

		inline void SetLogLevel(LOG_LEVEL level) { m_LogLevel = level; }

	private:
		LOG_LEVEL m_LogLevel;
		std::string m_LoggerName;

		static const std::string LEVELS[];

	public:
		static Logger* s_Instance;
	};
}