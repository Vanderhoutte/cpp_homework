#include "system.hh"
#include <iostream>
#include <string>

void show_menu() {
    std::cout << "\n=== 学生信息管理系统 ===" << std::endl;
    std::cout << "1. 添加学生" << std::endl;
    std::cout << "2. 删除学生（按学号）" << std::endl;
    std::cout << "3. 删除学生（按姓名）" << std::endl;
    std::cout << "4. 查询学生（按学号）" << std::endl;
    std::cout << "5. 查询学生（按姓名）" << std::endl;
    std::cout << "6. 显示所有学生" << std::endl;
    std::cout << "7. 保存数据到Excel文件" << std::endl;
    std::cout << "8. 加载数据" << std::endl;
    std::cout << "0. 退出系统" << std::endl;
    std::cout << "请选择操作: ";
}

int main() {
    StudentManagementSystem system;
    
    while (true) {
        show_menu();
        int choice;
        std::cin >> choice;
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
                
                Student student(id, name, gender, class_id, phone, email);
                if (system.add_student(student)) {
                    std::cout << "添加成功！" << std::endl;
                } else {
                    std::cout << "添加失败！" << std::endl;
                }
                break;
            }
                
            case 2: {
                std::string id;
                std::cout << "请输入要删除的学生学号: ";
                std::getline(std::cin, id);
                if (system.delete_student(id)) {
                    std::cout << "删除成功！" << std::endl;
                } else {
                    std::cout << "删除失败！" << std::endl;
                }
                break;
            }
                
            case 3: {
                std::string name;
                std::cout << "请输入要删除的学生姓名: ";
                std::getline(std::cin, name);
                if (system.delete_student_by_name(name)) {
                    std::cout << "删除成功！" << std::endl;
                } else {
                    std::cout << "删除失败！" << std::endl;
                }
                break;
            }
                
            case 4: {
                std::string id;
                std::cout << "请输入要查询的学生学号: ";
                std::getline(std::cin, id);
                Student* student = system.find_student_by_id(id);
                if (student) {
                    student->show_info();
                } else {
                    std::cout << "学生不存在！" << std::endl;
                }
                break;
            }
                
            case 5: {
                std::string name;
                std::cout << "请输入要查询的学生姓名: ";
                std::getline(std::cin, name);
                auto students = system.find_students_by_name_exact(name);
                if (students.empty()) {
                    std::cout << "未找到匹配的学生！" << std::endl;
                } else {
                    std::cout << "找到 " << students.size() << " 个匹配的学生：" << std::endl;
                    for (auto* student : students) {
                        student->show_info();
                        std::cout << "-------------------" << std::endl;
                    }
                }
                break;
            }
                
            case 6:
                system.show_all_students();
                break;
                
            case 7:
                if (system.save_to_excel_file("students.csv")) {
                    std::cout << "保存成功！数据已按学号排序并保存为Excel格式。" << std::endl;
                } else {
                    std::cout << "保存失败！" << std::endl;
                }
                break;
                
            case 8:
                if (system.load_from_file("students.csv")) {
                    std::cout << "加载成功！" << std::endl;
                } else {
                    std::cout << "加载失败！" << std::endl;
                }
                break;
                
            default:
                std::cout << "无效选择，请重新输入！" << std::endl;
        }
    }
    
    return 0;
}