/**
 * @file student.hh
 * @brief 学生信息管理类头文件
 * 
 * 定义了学生信息管理类Student，包含学生基本信息和成绩管理功能。
 * 提供完整的数据验证机制和操作接口。
 */

#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <iostream>
#include <regex>
#include <stdexcept>
#include "logger.hh"

/**
 * @class Student
 * @brief 学生信息管理类
 * 
 * 管理学生的基本信息和成绩数据，提供完整的验证机制和操作接口。
 * 支持学号、姓名、性别、班级、联系方式等信息的存储和验证。
 */
class Student {
public:
    /**
     * @brief 默认构造函数
     */
    Student() = default;
    
    /**
     * @brief 参数化构造函数
     * @param id 学号（10位数字）
     * @param name 姓名（2-20个字符）
     * @param gender 性别（"男"或"女"）
     * @param class_id 班级号
     * @param phone 手机号（11位数字，可选）
     * @param email 邮箱地址（可选）
     * 
     * 构造函数会自动验证输入参数的合法性。
     */
    Student(std::string id, std::string name, std::string gender, 
            std::string class_id, std::string phone = "", std::string email = "");
    
    /**
     * @brief 显示学生详细信息到控制台
     */
    void show_info() const;
    
    /**
     * @brief 设置学生成绩
     * @param subject 科目名称
     * @param score 成绩（0-100分）
     * @throws std::invalid_argument 当科目名为空或成绩不在0-100范围内
     */
    void set_score(const std::string& subject, float score);
    
    /**
     * @brief 获取指定科目成绩
     * @param subject 科目名称
     * @return float 成绩值，科目不存在时返回-1
     */
    float get_score(const std::string& subject) const;
    
    /**
     * @brief 计算所有科目平均分
     * @return float 平均分，无成绩时返回0
     */
    float get_average_score() const;
    
    /**
     * @brief 验证学生基本信息是否完整
     * @return bool 信息完整返回true，否则返回false
     * 
     * 检查学号、姓名、性别、班级号是否都已设置。
     */
    bool is_valid() const;
    
    // Getter方法
    const std::string& get_id() const { return id_; }           ///< 获取学号
    const std::string& get_name() const { return name_; }       ///< 获取姓名
    const std::string& get_gender() const { return gender_; }   ///< 获取性别
    const std::string& get_class_id() const { return class_id_; } ///< 获取班级号
    const std::string& get_phone() const { return phone_; }     ///< 获取电话
    const std::string& get_email() const { return email_; }     ///< 获取邮箱
    const std::unordered_map<std::string, float>& get_scores() const { return scores_; } ///< 获取成绩映射表
    
    // Setter方法
    void set_id(const std::string& id);         ///< 设置学号（带验证）
    void set_name(const std::string& name);     ///< 设置姓名（带验证）
    void set_gender(const std::string& gender); ///< 设置性别（带验证）
    void set_class_id(const std::string& class_id); ///< 设置班级号（带验证）
    void set_phone(const std::string& phone);   ///< 设置电话（带验证）
    void set_email(const std::string& email);   ///< 设置邮箱（带验证）

private:
    std::string id_;           ///< 学号
    std::string name_;         ///< 姓名
    std::string gender_;       ///< 性别
    std::string class_id_;     ///< 班级号
    std::string phone_;        ///< 电话
    std::string email_;        ///< 邮箱
    std::unordered_map<std::string, float> scores_; ///< 成绩映射表（科目->成绩）
    
    // 验证方法
    void validate_id(const std::string& id) const;         ///< 验证学号格式
    void validate_name(const std::string& name) const;     ///< 验证姓名格式
    void validate_gender(const std::string& gender) const; ///< 验证性别格式
    void validate_class_id(const std::string& class_id) const; ///< 验证班级号格式
    void validate_phone(const std::string& phone) const;   ///< 验证电话格式
    void validate_email(const std::string& email) const;   ///< 验证邮箱格式
};

/**
 * @typedef StudentList
 * @brief 学生链表类型别名
 * 
 * 使用STL的std::list容器存储学生对象列表。
 */
using StudentList = std::list<Student>;