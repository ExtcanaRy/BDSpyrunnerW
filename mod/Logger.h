#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

enum LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

enum LoggerType {
    ORIGINAL,
    BDXCORE,
    LiteLoader
};

class Logger {
public:
    Logger(const std::string& name);
    void log(LogLevel level, const std::string& message, const std::string& info = "");
    void debug(const std::string& message, const std::string& info = "");
    void info(const std::string& message, const std::string& info = "");
    void warn(const std::string& message, const std::string& info = "");
    void error(const std::string& message, const std::string& info = "");
    void fatal(const std::string& message, const std::string& info = "");
private:
    std::string name_;
    std::string getFormattedTime(LoggerType loggerType);
};

#endif // LOGGER_H
