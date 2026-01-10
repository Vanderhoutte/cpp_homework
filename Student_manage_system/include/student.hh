#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <regex>
#include <stdexcept>
#include "logger.hh"

/**
 * @class Student
 * @brief 学生信息管理类
 * 
 * 该类用于管理学生的基本信息和成绩数据。
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
     */
    Student(std::string id, std::string name, std::string gender, 
            std::string class_id, std::string phone = "", std::string email = "")
        : id_(std::move(id)), name_(std::move(name)), gender_(std::move(gender)),
          class_id_(std::move(class_id)), phone_(std::move(phone)), email_(std::move(email))
    {
        validate_id(id_);
        validate_name(name_);
        validate_gender(gender_);
        validate_class_id(class_id_);
        if (!phone_.empty()) validate_phone(phone_);
        if (!email_.empty()) validate_email(email_);
    }

    /**
     * @brief 显示学生详细信息到控制台
     */
    void show_info() const {
        std::cout << "学号: " << id_ << "\n";
        std::cout << "姓名: " << name_ << "\n";
        std::cout << "性别: " << gender_ << "\n";
        std::cout << "班级: " << class_id_ << "\n";
        std::cout << "电话: " << (phone_.empty() ? "未设置" : phone_) << "\n";
        std::cout << "邮箱: " << (email_.empty() ? "未设置" : email_) << "\n";
        
        if (!scores_.empty()) {
            std::cout << "成绩:\n";
            for (const auto& [subject, score] : scores_) {
                std::cout << "  " << subject << ": " << score << "\n";
            }
        }
    }

    /**
     * @brief 设置学生成绩
     * @param subject 科目名称
     * @param score 成绩（0-100分）
     */
    void set_score(const std::string& subject, float score) {
        if (subject.empty()) {
            throw std::invalid_argument("科目名不能为空");
        }
        if (score < 0 || score > 100) {
            throw std::invalid_argument("成绩必须在0-100之间");
        }
        scores_[subject] = score;
    }

    /**
     * @brief 获取指定科目成绩
     * @param subject 科目名称
     * @return float 成绩值，科目不存在时返回-1
     */
    float get_score(const std::string& subject) const {
        auto it = scores_.find(subject);
        return it != scores_.end() ? it->second : -1.0f;
    }

    /**
     * @brief 计算所有科目平均分
     * @return float 平均分，无成绩时返回0
     */
    float get_average_score() const {
        if (scores_.empty()) return 0.0f;
        float sum = 0.0f;
        for (const auto& [_, score] : scores_) {
            sum += score;
        }
        return sum / scores_.size();
    }

    /**
     * @brief 验证学生基本信息是否完整
     * @return bool 信息完整返回true，否则返回false
     */
    bool is_valid() const {
        return !id_.empty() && !name_.empty() && !gender_.empty() && !class_id_.empty();
    }

    // 简单的Getter方法
    const std::string& get_id() const { return id_; }
    const std::string& get_name() const { return name_; }
    const std::string& get_gender() const { return gender_; }
    const std::string& get_class_id() const { return class_id_; }
    const std::string& get_phone() const { return phone_; }
    const std::string& get_email() const { return email_; }
    const std::unordered_map<std::string, float>& get_scores() const { return scores_; }

    // 简单的Setter方法
    void set_id(const std::string& id) {
        validate_id(id);
        id_ = id;
    }

    void set_name(const std::string& name) {
        validate_name(name);
        name_ = name;
    }

    void set_gender(const std::string& gender) {
        validate_gender(gender);
        gender_ = gender;
    }

    void set_class_id(const std::string& class_id) {
        validate_class_id(class_id);
        class_id_ = class_id;
    }

    void set_phone(const std::string& phone) {
        if (!phone.empty()) validate_phone(phone);
        phone_ = phone;
    }

    void set_email(const std::string& email) {
        if (!email.empty()) validate_email(email);
        email_ = email;
    }

private:
    std::string id_;
    std::string name_;
    std::string gender_;
    std::string class_id_;
    std::string phone_;
    std::string email_;
    std::unordered_map<std::string, float> scores_;

    // 验证方法
    void validate_id(const std::string& id) const {
        if (id.empty()) throw std::invalid_argument("学号不能为空");
        if (!std::regex_match(id, std::regex("^\\d{10}$"))) {
            throw std::invalid_argument("学号必须为10位数字");
        }
    }

    void validate_name(const std::string& name) const {
        if (name.empty()) throw std::invalid_argument("姓名不能为空");
        if (name.length() < 2 || name.length() > 20) {
            throw std::invalid_argument("姓名长度必须在2-20个字符之间");
        }
    }

    void validate_gender(const std::string& gender) const {
        if (gender != "男" && gender != "女") {
            throw std::invalid_argument("性别必须为'男'或'女'");
        }
    }

    void validate_class_id(const std::string& class_id) const {
        if (class_id.empty()) throw std::invalid_argument("班级号不能为空");
        if (class_id.length() < 3) throw std::invalid_argument("班级号格式不正确");
    }

    void validate_phone(const std::string& phone) const {
        if (!std::regex_match(phone, std::regex("^1[3-9]\\d{9}$"))) {
            throw std::invalid_argument("手机号格式不正确（必须是11位数字）");
        }
    }

    void validate_email(const std::string& email) const {
        if (!std::regex_match(email, std::regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"))) {
            throw std::invalid_argument("邮箱格式不正确");
        }
    }
};

/**
 * @typedef StudentList
 * @brief 学生链表类型别名
 */
using StudentList = std::list<Student>;