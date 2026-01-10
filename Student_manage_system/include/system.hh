/**
 * @file system.hh
 * @brief 学生信息管理系统核心类头文件
 * 
 * 定义了学生信息管理系统核心类StudentManagementSystem，
 * 提供完整的增删改查操作和文件存储功能。
 */

#pragma once

#include "student.hh"
#include "logger.hh"
#include <list>
#include <string>
#include <fstream>
#include <algorithm>

/**
 * @class StudentManagementSystem
 * @brief 学生信息管理系统核心类
 * 
 * 负责学生信息的完整生命周期管理，包括增删改查操作和文件存储。
 * 使用STL链表存储学生数据，提供完整的操作接口和错误处理。
 */
class StudentManagementSystem {
public:
    /**
     * @brief 构造函数
     * 
     * 初始化学生管理系统，创建日志记录器实例。
     */
    StudentManagementSystem();
    
    /**
     * @brief 添加学生
     * @param student 要添加的学生对象
     * @return bool 添加成功返回true，失败返回false
     * 
     * 添加前会检查学生信息完整性和学号唯一性。
     */
    bool add_student(const Student& student);
    
    /**
     * @brief 删除学生
     * @param student_id 要删除的学生学号
     * @return bool 删除成功返回true，失败返回false
     * 
     * 根据学号查找并删除学生，学号不存在时返回失败。
     */
    bool delete_student(const std::string& student_id);
    
    /**
     * @brief 修改学生信息
     * @param student_id 要修改的学生学号
     * @param new_student 新的学生信息
     * @return bool 修改成功返回true，失败返回false
     * 
     * 替换指定学号的学生信息，新信息必须完整有效。
     */
    bool update_student(const std::string& student_id, const Student& new_student);
    
    /**
     * @brief 根据学号查询学生
     * @param student_id 要查询的学生学号
     * @return Student* 找到返回学生指针，否则返回nullptr
     */
    Student* find_student_by_id(const std::string& student_id);
    
    /**
     * @brief 根据姓名查询学生（支持模糊查询）
     * @param name 要查询的学生姓名
     * @return std::list<Student*> 找到的学生指针列表
     * 
     * 使用字符串包含匹配进行模糊查询，返回所有匹配的学生。
     */
    std::list<Student*> find_students_by_name(const std::string& name);
    
    /**
     * @brief 获取所有学生
     * @return const std::list<Student>& 学生列表常量引用
     */
    const std::list<Student>& get_all_students() const;
    
    /**
     * @brief 获取学生数量
     * @return size_t 当前系统中的学生数量
     */
    size_t get_student_count() const;
    
    /**
     * @brief 清空所有学生数据
     */
    void clear_all_students();
    
    /**
     * @brief 保存数据到文件（包含成绩信息）
     * @param filename 文件名
     * @return bool 保存成功返回true，失败返回false
     * 
     * 将学生数据和成绩信息保存为CSV格式文件。
     */
    bool save_to_file(const std::string& filename);
    
    /**
     * @brief 从文件加载数据（包含成绩信息）
     * @param filename 文件名
     * @return bool 加载成功返回true，失败返回false
     * 
     * 从CSV格式文件加载学生数据和成绩信息，自动验证数据有效性。
     */
    bool load_from_file(const std::string& filename);
    
    /**
     * @brief 保存数据到Excel格式文件（包含成绩信息）
     * @param filename 文件名
     * @return bool 保存成功返回true，失败返回false
     * 
     * 按照学号从小到大排序后保存为CSV格式（Excel兼容），包含成绩信息。
     */
    bool save_to_excel_file(const std::string& filename);
    
    /**
     * @brief 显示所有学生信息
     * 
     * 在控制台格式化显示所有学生的详细信息。
     */
    void show_all_students() const;

    /**
     * @brief 按姓名删除学生（处理重名情况）
     * @param name 要删除的学生姓名
     * @return bool 删除成功返回true，失败返回false
     * 
     * 如果出现重名，显示所有匹配的学生信息供用户选择。
     */
    bool delete_student_by_name(const std::string& name);
    
    /**
     * @brief 按姓名查找学生（处理重名情况）
     * @param name 要查找的学生姓名
     * @return std::list<Student*> 找到的学生指针列表
     * 
     * 支持精确匹配，显示所有匹配的学生信息。
     */
    std::list<Student*> find_students_by_name_exact(const std::string& name);
    
    /**
     * @brief 按学号排序学生列表
     */
    void sort_students_by_id();

    /**
     * @brief 设置学生成绩
     * @param student_id 学生学号
     * @param subject 科目名称
     * @param score 成绩（0-100分）
     * @return bool 设置成功返回true，失败返回false
     */
    bool set_student_score(const std::string& student_id, const std::string& subject, float score);
    
    /**
     * @brief 获取学生成绩信息
     * @param student_id 学生学号
     * @return std::string 格式化的成绩信息字符串
     */
    std::string get_student_scores_info(const std::string& student_id);

private:
    std::list<Student> students_;  ///< 学生列表（使用STL链表存储）
    Logger logger_;                ///< 日志记录器实例
};