/**
 * @file logger.hh
 * @brief 日志系统头文件
 * 
 * 提供多级别日志输出功能，支持时间戳、模块名称等特性。
 * 类似ROS风格的日志系统，可以全局使用或为每个模块创建独立实例。
 */

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
 * 
 * 定义了五种日志级别，从调试信息到致命错误。
 */
enum class LogLevel {
    DEBUG = 0,  ///< 调试信息，用于开发调试
    INFO = 1,   ///< 普通信息，用于常规操作记录
    WARN = 2,   ///< 警告信息，用于潜在问题提示
    ERROR = 3,  ///< 错误信息，用于操作错误记录
    FATAL = 4   ///< 致命错误，程序无法继续运行
};

/**
 * @class Logger
 * @brief 独立日志类
 * 
 * 提供完整的日志记录功能，支持多级别日志输出、时间戳、模块名称等。
 * 可以设置全局日志级别和实例级别，支持格式化输出和子日志器创建。
 */
class Logger {
public:
    /**
     * @brief 构造函数
     * @param name 日志器名称，通常为模块名
     * @param level 日志级别阈值，低于此级别的日志将被过滤
     */
    explicit Logger(const std::string& name = "default", LogLevel level = LogLevel::INFO);
    
    /**
     * @brief 设置全局日志级别
     * @param level 新的全局日志级别
     * 
     * 全局日志级别影响所有Logger实例，低于此级别的日志将被过滤。
     */
    static void set_global_level(LogLevel level);
    
    /**
     * @brief 设置当前日志器级别
     * @param level 新的日志级别
     */
    void set_level(LogLevel level);
    
    /**
     * @brief 输出DEBUG级别日志
     * @param message 日志消息内容
     */
    void debug(const std::string& message) const;
    
    /**
     * @brief 输出INFO级别日志
     * @param message 日志消息内容
     */
    void info(const std::string& message) const;
    
    /**
     * @brief 输出WARN级别日志
     * @param message 日志消息内容
     */
    void warn(const std::string& message) const;
    
    /**
     * @brief 输出ERROR级别日志
     * @param message 日志消息内容
     */
    void error(const std::string& message) const;
    
    /**
     * @brief 输出FATAL级别日志并终止程序
     * @param message 日志消息内容
     * 
     * 输出致命错误日志后调用std::exit终止程序运行。
     */
    void fatal(const std::string& message) const;
    
    /**
     * @brief 格式化日志输出（类似printf风格）
     * @tparam Args 可变参数类型
     * @param level 日志级别
     * @param format 格式字符串
     * @param args 格式化参数
     */
    template<typename... Args>
    void logf(LogLevel level, const std::string& format, Args... args) const;
    
    /**
     * @brief 创建子日志器
     * @param sub_name 子日志器名称
     * @return Logger 新的日志器实例
     * 
     * 子日志器名称将附加到当前日志器名称后，形成层次结构。
     */
    Logger create_child(const std::string& sub_name) const;

private:
    std::string name_;           ///< 日志器名称
    LogLevel level_;             ///< 当前日志级别
    static LogLevel global_level_; ///< 全局日志级别
    
    /**
     * @brief 判断是否应该记录日志
     * @param level 要记录的日志级别
     * @return bool 应该记录返回true
     */
    bool should_log(LogLevel level) const;
    
    /**
     * @brief 输出日志到控制台
     * @param level 日志级别
     * @param message 日志消息
     */
    void output_log(LogLevel level, const std::string& message) const;
    
    /**
     * @brief 统一的日志记录方法
     * @param level 日志级别
     * @param message 日志消息
     */
    void log(LogLevel level, const std::string& message) const;
    
    /**
     * @brief 日志级别转字符串
     * @param level 日志级别
     * @return std::string 级别字符串
     */
    static std::string level_to_string(LogLevel level);
};