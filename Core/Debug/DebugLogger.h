// Core/Debug/DebugLogger.h
#pragma once
#include "Core/Utils/Logger.h"
#include <string>

/**
 * @file DebugLogger.h
 * @brief Defines the DebugLogger class which wraps the main Logger for debug-specific filtered logging.
 */

 /**
  * @class DebugLogger
  * @brief A static utility class for logging debug, warning, and error messages with runtime filtering.
  *        Wraps around the core Logger and allows filtering based on severity levels.
  */
class DebugLogger {
public:
    /**
     * @enum FilterLevel
     * @brief Specifies the minimum severity of messages that should be logged.
     */
    enum class FilterLevel {
        ALL,              ///< Log all messages (Log, Warn, and Error).
        WARN_AND_ERROR,   ///< Only log warnings and errors.
        ERROR_ONLY        ///< Only log errors.
    };

    /**
     * @brief Initializes the debug logger.
     *        Should be called once during application startup before using logging.
     */
    static void Initialize();

    /**
     * @brief Logs a general debug/info message.
     * @param msg The message to log.
     */
    static void Log(const std::string& msg);

    /**
     * @brief Logs a warning message.
     * @param msg The message to log.
     */
    static void Warn(const std::string& msg);

    /**
     * @brief Logs an error message.
     * @param msg The message to log.
     */
    static void Error(const std::string& msg);

    /**
     * @brief Sets the logging filter level to control which messages are output.
     * @param level The minimum severity to log.
     */
    static void SetFilter(FilterLevel level);

private:
    /**
     * @brief The current filter level used to determine which messages are allowed to be logged.
     */
    static FilterLevel filter_;
};
