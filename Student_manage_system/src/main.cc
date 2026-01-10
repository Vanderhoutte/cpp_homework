#include "system.hh"
#include <iostream>
#include <string>
#include <windows.h>  // Windows API头文件

/**
 * @brief 设置控制台为中文编码（UTF-8）
 * @return bool 设置成功返回true，失败返回false
 */
bool set_console_chinese() {
    // 设置控制台输出代码页为UTF-8
    if (!SetConsoleOutputCP(CP_UTF8)) {
        std::cerr << "警告：无法设置控制台输出编码为UTF-8" << std::endl;
        return false;
    }
    
    // 设置控制台输入代码页为UTF-8
    if (!SetConsoleCP(CP_UTF8)) {
        std::cerr << "警告：无法设置控制台输入编码为UTF-8" << std::endl;
        return false;
    }
    
    // 设置控制台标题为中文（使用ANSI版本）
    SetConsoleTitleA("学生信息管理系统");
    
    return true;
}

/**
 * @brief 显示系统启动信息
 */
void show_welcome_message() {
    std::cout << "==========================================" << std::endl;
    std::cout << "       学生信息管理系统 v1.0" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "系统初始化中..." << std::endl;
}

void show_menu() {
    std::cout << "\n=== 学生信息管理系统 ===" << std::endl;
    std::cout << "1. 添加学生" << std::endl;
    std::cout << "2. 删除学生（按学号）" << std::endl;
    std::cout << "3. 删除学生（按姓名）" << std::endl;
    std::cout << "4. 查询学生（按学号）" << std::endl;
    std::cout << "5. 查询学生（按姓名）" << std::endl;
    std::cout << "6. 显示所有学生" << std::endl;
    std::cout << "7. 设置学生成绩" << std::endl;
    std::cout << "8. 查询学生成绩" << std::endl;
    std::cout << "9. 保存数据到Excel文件" << std::endl;
    std::cout << "10. 加载数据" << std::endl;
    std::cout << "0. 退出系统" << std::endl;
    std::cout << "请选择操作: ";
}

int main() {
    // 设置控制台中文编码
    if (!set_console_chinese()) {
        std::cout << "注意：控制台编码设置可能不完整，中文字符显示可能异常" << std::endl;
    }
    
    // 显示欢迎信息
    show_welcome_message();
    
    StudentManagementSystem system;
    
    // 尝试自动加载数据（添加异常处理）
    try {
        if (system.load_from_file("students.csv")) {
            std::cout << "[成功] 自动加载学生数据成功！" << std::endl;
        } else {
            std::cout << "[信息] 未找到历史数据，将创建新的学生数据库" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "[警告] 自动加载数据时出现问题：" << e.what() << std::endl;
        std::cout << "[信息] 将继续使用空数据库" << std::endl;
    }
    
    while (true) {
        show_menu();
        int choice;
        
        // 改进的输入验证：处理非数字输入和输入错误
        if (!(std::cin >> choice)) {
            // 清除错误状态
            std::cin.clear();
            // 忽略错误的输入行
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[警告] 输入无效，请输入数字选项！" << std::endl;
            continue;  // 重新显示菜单
        }
        std::cin.ignore();
        
        switch (choice) {
            case 0:
                std::cout << "感谢使用，再见！" << std::endl;
                return 0;
                
            case 1: {
                std::string id, name, gender, class_id, phone, email;
                std::cout << "请输入学号: ";
                std::getline(std::cin, id);
                std::cout << "请输入姓名: ";
                std::getline(std::cin, name);
                std::cout << "请输入性别: ";
                std::getline(std::cin, gender);
                std::cout << "请输入班级: ";
                std::getline(std::cin, class_id);
                std::cout << "请输入电话（可选）: ";
                std::getline(std::cin, phone);
                std::cout << "请输入邮箱（可选）: ";
                std::getline(std::cin, email);
                
                try {
                    Student student(id, name, gender, class_id, phone, email);
                    if (system.add_student(student)) {
                        std::cout << "[成功] 添加成功！" << std::endl;
                    } else {
                        std::cout << "[失败] 添加失败！" << std::endl;
                    }
                } catch (const std::invalid_argument& e) {
                    std::cout << "[失败] 输入数据验证失败：" << e.what() << std::endl;
                    std::cout << "[信息] 请检查输入格式并重试" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "[失败] 添加学生时发生未知错误：" << e.what() << std::endl;
                }
                break;
            }
                
            case 2: {
                std::string id;
                std::cout << "请输入要删除的学生学号: ";
                std::getline(std::cin, id);
                try {
                    if (system.delete_student(id)) {
                        std::cout << "[成功] 删除成功！" << std::endl;
                    } else {
                        std::cout << "[失败] 删除失败！" << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "[失败] 删除学生时发生错误：" << e.what() << std::endl;
                }
                break;
            }
                
            case 3: {
                std::string name;
                std::cout << "请输入要删除的学生姓名: ";
                std::getline(std::cin, name);
                try {
                    if (system.delete_student_by_name(name)) {
                        std::cout << "[成功] 删除成功！" << std::endl;
                    } else {
                        std::cout << "[失败] 删除失败！" << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "[失败] 删除学生时发生错误：" << e.what() << std::endl;
                }
                break;
            }
                
            case 4: {
                std::string id;
                std::cout << "请输入要查询的学生学号: ";
                std::getline(std::cin, id);
                try {
                    Student* student = system.find_student_by_id(id);
                    if (student) {
                        student->show_info();
                    } else {
                        std::cout << "[失败] 学生不存在！" << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "[失败] 查询学生时发生错误：" << e.what() << std::endl;
                }
                break;
            }
                
            case 5: {
                std::string name;
                std::cout << "请输入要查询的学生姓名: ";
                std::getline(std::cin, name);
                try {
                    auto students = system.find_students_by_name_exact(name);
                    if (students.empty()) {
                        std::cout << "[失败] 未找到匹配的学生！" << std::endl;
                    } else {
                        std::cout << "[成功] 找到 " << students.size() << " 个匹配的学生：" << std::endl;
                        for (auto* student : students) {
                            student->show_info();
                            std::cout << "-------------------" << std::endl;
                        }
                    }
                } catch (const std::exception& e) {
                    std::cout << "[失败] 查询学生时发生错误：" << e.what() << std::endl;
                }
                break;
            }
                
            case 6:
                try {
                    system.show_all_students();
                } catch (const std::exception& e) {
                    std::cout << "[失败] 显示学生列表时发生错误：" << e.what() << std::endl;
                }
                break;
                
            case 7: {
                std::string id, subject;
                float score;
                std::cout << "请输入学生学号: ";
                std::getline(std::cin, id);
                std::cout << "请输入科目名称: ";
                std::getline(std::cin, subject);
                std::cout << "请输入成绩(0-100): ";
                
                if (!(std::cin >> score)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[失败] 成绩输入无效，请输入数字！" << std::endl;
                    break;
                }
                std::cin.ignore();
                
                try {
                    if (system.set_student_score(id, subject, score)) {
                        std::cout << "[成功] 成绩设置成功！" << std::endl;
                    } else {
                        std::cout << "[失败] 成绩设置失败！" << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "[失败] 设置成绩时发生错误：" << e.what() << std::endl;
                }
                break;
            }
                
            case 8: {
                std::string id;
                std::cout << "请输入学生学号: ";
                std::getline(std::cin, id);
                
                try {
                    std::string scores_info = system.get_student_scores_info(id);
                    std::cout << "=== 学生成绩信息 ===" << std::endl;
                    std::cout << scores_info << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "[失败] 查询成绩时发生错误：" << e.what() << std::endl;
                }
                break;
            }
                
            case 9:
                try {
                    if (system.save_to_excel_file("students.csv")) {
                        std::cout << "[成功] 保存成功！数据已按学号排序并保存为Excel格式。" << std::endl;
                    } else {
                        std::cout << "[失败] 保存失败！" << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "[失败] 保存数据时发生错误：" << e.what() << std::endl;
                }
                break;
                
            case 10:
                try {
                    if (system.load_from_file("students.csv")) {
                        std::cout << "[成功] 加载成功！" << std::endl;
                    } else {
                        std::cout << "[失败] 加载失败！" << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "[失败] 加载数据时出错：" << e.what() << std::endl;
                }
                break;
                
            default:
                std::cout << "[警告] 无效选择，请重新输入！" << std::endl;
        }
    }
    
    return 0;
}