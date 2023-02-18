#include <pyrw/Logger.h>

Logger::Logger(const std::string& name) : name_(name) {}

void Logger::log(LogLevel level, const std::string& message, const std::string& info) {
    std::string levelStr;
    switch (level) {
    case DEBUG:
        levelStr = "DEBUG";
        break;
    case INFO:
        levelStr = "INFO";
        break;
    case WARN:
        levelStr = "WARN";
        break;
    case ERROR:
        levelStr = "ERROR";
        break;
    case FATAL:
        levelStr = "FATAL";
        break;
    default:
        levelStr = "UNKNOWN";
        break;
    }
    FILE* file;
    if (fopen_s(&file, "bedrock_server_mod.exe", "r") == 0 && file != nullptr) {
        fclose(file);
        if (fopen_s(&file, "BDXCORE.dll", "r") != 0 || file == nullptr) {
            std::cout << getFormattedTime(LoggerType::LiteLoader) << " " << levelStr << " [" << name_ << "]";
        }
    }
    else {
        std::cout << getFormattedTime(LoggerType::ORIGINAL) << " " << levelStr << "][" << name_ << "]";
    }
    if (!info.empty()) {
        std::cout << "[" << info << "]";
    }
    std::cout << " " << message << std::endl;
}

void Logger::debug(const std::string& message, const std::string& info) {
    log(DEBUG, message, info);
}

void Logger::info(const std::string& message, const std::string& info) {
    log(INFO, message, info);
}

void Logger::warn(const std::string& message, const std::string& info) {
    log(WARN, message, info);
}

void Logger::error(const std::string& message, const std::string& info) {
    log(ERROR, message, info);
}

void Logger::fatal(const std::string& message, const std::string& info) {
    log(FATAL, message, info);
}

std::string Logger::getFormattedTime(LoggerType loggerType) {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    auto now_us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;

    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;
    localtime_s(&now_tm, &now_c);

    std::ostringstream oss;
    switch (loggerType) {
    case ORIGINAL:
        oss << std::put_time(&now_tm, "[%Y-%m-%d %H:%M:%S:") << std::setfill('0') << std::setw(3) << (now_us.count() / 1000);
        break;
    case BDXCORE:
        oss << std::put_time(&now_tm, "[%Y-%m-%d %H:%M:%S:") << std::setfill('0') << std::setw(3) << (now_us.count() / 1000);
        break;
    case LiteLoader:
        oss << std::put_time(&now_tm, "%H:%M:%S");
        break;
    default:
        break;
    }
    return oss.str();
}