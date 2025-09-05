// Core/Utils/Logger.h

#pragma once
#include <string>
#include <iostream>
#include <mutex>

/**
 * @class Logger
 * @brief Thread-safe logging utility for debugging, informational, and error messages.
 *
 * This class provides static methods for logging messages to the standard output stream.
 * It ensures thread-safety using a mutex so that logs from multiple threads do not interleave.
 */
class Logger {
public:
	/**
	 * @enum Level
	 * @brief Defines the severity level of a log message.
	 */
	enum class Level 
	{
		DEBUG,  ///< Detailed debugging messages, typically used in development.
		INFO,   ///< General informational messages.
		WARN,   ///< Warning messages indicating potential issues.
		FAILED  ///< Error messages indicating failure or serious issues.
	};

	/**
	 * @brief Logs a message at the specified log level.
	 *
	 * This function is thread-safe and writes the formatted log message to `std::cout`.
	 *
	 * @param level The severity level of the log message.
	 * @param message The message to log.
	 */
	static void Log(Level level, const std::string& message)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		std::cout << "[" << ToString(level) << "] " << message << std::endl;
	}

private:
	static std::mutex mutex_; /// Mutex to ensure thread-safe logging.

	/**
	 * @brief Converts a log level enum to its string representation.
	 *
	 * @param level The log level to convert.
	 * @return A string representation of the level.
	 */
	static std::string ToString(Level level)
	{
		switch (level)
		{
			case Level::DEBUG: return "DEBUG";
			case Level::INFO: return "INFO";
			case Level::WARN: return "WARN";
			case Level::FAILED: return "ERROR";
		}
		return "UNKNOWN";
	}
};

inline std::mutex Logger::mutex_;