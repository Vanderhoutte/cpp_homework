#include "logger.hh"

Logger::Logger(const std::string& name, LogLevel level)
    : name_(name), level_(level) {}

void Logger::set_global_level(LogLevel level) {
    global_level_ = level;
}

void Logger::set_level(LogLevel level) {
    level_ = level;
}

void Logger::debug(const std::string& message) const {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) const {
    log(LogLevel::INFO, message);
}

void Logger::warn(const std::string& message) const {
    log(LogLevel::WARN, message);
}

void Logger::error(const std::string& message) const {
    log(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message) const {
    log(LogLevel::FATAL, message);
    std::exit(EXIT_FAILURE);
}

template<typename... Args>
void Logger::logf(LogLevel level, const std::string& format, Args... args) const {
    if (should_log(level)) {
        char buffer[1024];
        std::snprintf(buffer, sizeof(buffer), format.c_str(), args...);
        output_log(level, buffer);
    }
}

Logger Logger::create_child(const std::string& sub_name) const {
    return Logger(name_ + "." + sub_name, level_);
}

bool Logger::should_log(LogLevel level) const {
    return static_cast<int>(level) >= static_cast<int>(global_level_) &&
           static_cast<int>(level) >= static_cast<int>(level_);
}

void Logger::output_log(LogLevel level, const std::string& message) const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::cout << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] "
              << "[" << level_to_string(level) << "] "
              << "[" << name_ << "] "
              << message << std::endl;
}

void Logger::log(LogLevel level, const std::string& message) const {
    if (should_log(level)) {
        output_log(level, message);
    }
}

std::string Logger::level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

LogLevel Logger::global_level_ = LogLevel::INFO;