#pragma once

#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <cstdio>
#include <cstdlib>

/**
 * @enum LogLevel
 * @brief 日志级别枚举
 */
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    FATAL = 4
};

/**
 * @class Logger
 * @brief 独立日志类（类似ROS风格）
 * 
 * 提供多级别日志输出，支持时间戳、日志级别、模块名称等功能。
 * 可以全局使用，也可以为每个模块创建独立的日志实例。
 */
class Logger {
public:
    /**
     * @brief 构造函数
     * @param name 日志器名称（通常是模块名）
     * @param level 日志级别阈值
     */
    explicit Logger(const std::string& name = "default", LogLevel level = LogLevel::INFO)
        : name_(name), level_(level) {}
    
    /**
     * @brief 设置全局日志级别
     * @param level 新的日志级别
     */
    static void set_global_level(LogLevel level) {
        global_level_ = level;
    }
    
    /**
     * @brief 设置当前日志器级别
     * @param level 新的日志级别
     */
    void set_level(LogLevel level) {
        level_ = level;
    }
    
    /**
     * @brief DEBUG级别日志
     * @param message 日志消息
     */
    void debug(const std::string& message) const {
        log(LogLevel::DEBUG, message);
    }
    
    /**
     * @brief INFO级别日志
     * @param message 日志消息
     */
    void info(const std::string& message) const {
        log(LogLevel::INFO, message);
    }
    
    /**
     * @brief WARN级别日志
     * @param message 日志消息
     */
    void warn(const std::string& message) const {
        log(LogLevel::WARN, message);
    }
    
    /**
     * @brief ERROR级别日志
     * @param message 日志消息
     */
    void error(const std::string& message) const {
        log(LogLevel::ERROR, message);
    }
    
    /**
     * @brief FATAL级别日志
     * @param message 日志消息
     */
    void fatal(const std::string& message) const {
        log(LogLevel::FATAL, message);
        std::exit(EXIT_FAILURE);
    }
    
    /**
     * @brief 格式化日志（类似printf风格）
     * @tparam Args 参数类型
     * @param level 日志级别
     * @param format 格式字符串
     * @param args 参数列表
     */
    template<typename... Args>
    void logf(LogLevel level, const std::string& format, Args... args) const {
        if (should_log(level)) {
            char buffer[1024];
            std::snprintf(buffer, sizeof(buffer), format.c_str(), args...);
            output_log(level, buffer);
        }
    }
    
    /**
     * @brief 创建子日志器
     * @param sub_name 子日志器名称
     * @return Logger 新的日志器实例
     */
    Logger create_child(const std::string& sub_name) const {
        return Logger(name_ + "." + sub_name, level_);
    }

private:
    std::string name_;
    LogLevel level_;
    static LogLevel global_level_;
    
    /**
     * @brief 判断是否应该记录日志
     * @param level 要记录的日志级别
     * @return bool 应该记录返回true
     */
    bool should_log(LogLevel level) const {
        return static_cast<int>(level) >= static_cast<int>(global_level_) &&
               static_cast<int>(level) >= static_cast<int>(level_);
    }
    
    /**
     * @brief 输出日志到控制台
     * @param level 日志级别
     * @param message 日志消息
     */
    void output_log(LogLevel level, const std::string& message) const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::cout << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] "
                  << "[" << level_to_string(level) << "] "
                  << "[" << name_ << "] "
                  << message << std::endl;
    }
    
    /**
     * @brief 统一的日志记录方法
     * @param level 日志级别
     * @param message 日志消息
     */
    void log(LogLevel level, const std::string& message) const {
        if (should_log(level)) {
            output_log(level, message);
        }
    }
    
    /**
     * @brief 日志级别转字符串
     * @param level 日志级别
     * @return std::string 级别字符串
     */
    static std::string level_to_string(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::FATAL: return "FATAL";
            default: return "UNKNOWN";
        }
    }
};

// 静态成员定义
LogLevel Logger::global_level_ = LogLevel::INFO;