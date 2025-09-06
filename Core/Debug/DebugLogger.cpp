// Core/Debug/DebugLogger.cpp
#include "DebugLogger.h"
#include <iostream>

DebugLogger::FilterLevel DebugLogger::filter_ = DebugLogger::FilterLevel::ALL;

void DebugLogger::Initialize() {
    // Setup anything needed
    Logger::Log(Logger::Level::INFO, "DebugLogger initialized.");
}

void DebugLogger::Log(const std::string& msg) {
    if (filter_ == FilterLevel::ALL)
        Logger::Log(Logger::Level::DEBUG, msg);
}

void DebugLogger::Warn(const std::string& msg) {
    if (filter_ == FilterLevel::ALL || filter_ == FilterLevel::WARN_AND_ERROR)
        Logger::Log(Logger::Level::WARN, msg);
}

void DebugLogger::Error(const std::string& msg) {
    Logger::Log(Logger::Level::FAILED, msg);
}

void DebugLogger::SetFilter(FilterLevel level) {
    filter_ = level;
}
