#include "system.hh"
#include <sstream>
#include <iostream>
#include <algorithm>

StudentManagementSystem::StudentManagementSystem() : logger_("StudentManagementSystem") {
    logger_.info("学生管理系统初始化完成");
}

bool StudentManagementSystem::add_student(const Student& student) {
    if (!student.is_valid()) {
        logger_.warn("添加学生失败：学生信息不完整");
        return false;
    }
    
    // 检查学号是否重复
    auto it = std::find_if(students_.begin(), students_.end(),
        [&student](const Student& s) {
            return s.get_id() == student.get_id();
        });
    
    if (it != students_.end()) {
        logger_.warn("添加学生失败：学号 " + student.get_id() + " 已存在");
        return false;
    }
    
    students_.push_back(student);
    logger_.info("成功添加学生：" + student.get_id() + " - " + student.get_name());
    return true;
}

bool StudentManagementSystem::delete_student(const std::string& student_id) {
    auto it = std::find_if(students_.begin(), students_.end(),
        [&student_id](const Student& student) {
            return student.get_id() == student_id;
        });
    
    if (it == students_.end()) {
        logger_.warn("删除学生失败：学号 " + student_id + " 不存在");
        return false;
    }
    
    students_.erase(it);
    logger_.info("成功删除学生：" + student_id);
    return true;
}

bool StudentManagementSystem::update_student(const std::string& student_id, const Student& new_student) {
    auto it = std::find_if(students_.begin(), students_.end(),
        [&student_id](const Student& student) {
            return student.get_id() == student_id;
        });
    
    if (it == students_.end()) {
        logger_.warn("修改学生失败：学号 " + student_id + " 不存在");
        return false;
    }
    
    if (!new_student.is_valid()) {
        logger_.warn("修改学生失败：新学生信息不完整");
        return false;
    }
    
    *it = new_student;
    logger_.info("成功修改学生信息：" + student_id);
    return true;
}

Student* StudentManagementSystem::find_student_by_id(const std::string& student_id) {
    auto it = std::find_if(students_.begin(), students_.end(),
        [&student_id](const Student& student) {
            return student.get_id() == student_id;
        });
    
    if (it != students_.end()) {
        return &(*it);
    }
    return nullptr;
}

std::list<Student*> StudentManagementSystem::find_students_by_name(const std::string& name) {
    std::list<Student*> result;
    
    for (auto& student : students_) {
        if (student.get_name().find(name) != std::string::npos) {
            result.push_back(&student);
        }
    }
    
    return result;
}

const std::list<Student>& StudentManagementSystem::get_all_students() const {
    return students_;
}

size_t StudentManagementSystem::get_student_count() const {
    return students_.size();
}

void StudentManagementSystem::clear_all_students() {
    students_.clear();
    logger_.info("清空所有学生数据");
}

bool StudentManagementSystem::save_to_file(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        logger_.error("无法打开文件进行保存：" + filename);
        return false;
    }
    
    // 添加表头（包含成绩字段标识）
    file << "学号,姓名,性别,班级,电话,邮箱,成绩信息\n";
    
    for (const auto& student : students_) {
        file << student.get_id() << ","
             << student.get_name() << ","
             << student.get_gender() << ","
             << student.get_class_id() << ","
             << student.get_phone() << ","
             << student.get_email() << ",";
        
        // 保存成绩信息（格式：科目1:成绩1;科目2:成绩2;...）
        const auto& scores = student.get_scores();
        if (!scores.empty()) {
            bool first = true;
            for (const auto& [subject, score] : scores) {
                if (!first) file << ";";
                file << subject << ":" << score;
                first = false;
            }
        } else {
            file << "无成绩";
        }
        file << "\n";
    }
    
    file.close();
    logger_.info("成功保存数据到文件：" + filename);
    return true;
}

bool StudentManagementSystem::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        logger_.error("无法打开文件进行加载：" + filename);
        return false;
    }
    
    students_.clear();
    std::string line;
    int count = 0;
    int error_count = 0;
    
    // 跳过Excel表头（如果存在）
    if (std::getline(file, line)) {
        // 检查是否是表头（包含"学号"等字段）
        if (line.find("学号") != std::string::npos) {
            // 这是表头，跳过
            logger_.info("检测到Excel表头，已跳过");
        } else {
            // 不是表头，重新处理这一行
            file.seekg(0);
        }
    } else {
        file.seekg(0);
    }
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string id, name, gender, class_id, phone, email, scores_str;
        
        // 解析CSV行（包含成绩信息）
        if (std::getline(iss, id, ',') &&
            std::getline(iss, name, ',') &&
            std::getline(iss, gender, ',') &&
            std::getline(iss, class_id, ',') &&
            std::getline(iss, phone, ',') &&
            std::getline(iss, email, ',') &&
            std::getline(iss, scores_str)) {
            
            try {
                // 创建学生对象
                Student student(id, name, gender, class_id, phone, email);
                
                // 解析成绩信息（如果存在）
                if (scores_str != "无成绩" && !scores_str.empty()) {
                    std::istringstream scores_stream(scores_str);
                    std::string subject_score;
                    
                    while (std::getline(scores_stream, subject_score, ';')) {
                        size_t colon_pos = subject_score.find(':');
                        if (colon_pos != std::string::npos) {
                            std::string subject = subject_score.substr(0, colon_pos);
                            std::string score_str = subject_score.substr(colon_pos + 1);
                            
                            try {
                                float score = std::stof(score_str);
                                student.set_score(subject, score);
                            } catch (const std::invalid_argument& e) {
                                logger_.warn("跳过无效成绩：" + subject + "=" + score_str);
                            }
                        }
                    }
                }
                
                if (student.is_valid()) {
                    students_.push_back(student);
                    count++;
                } else {
                    logger_.warn("跳过无效学生数据：" + id + " - " + name);
                    error_count++;
                }
            } catch (const std::invalid_argument& e) {
                logger_.warn("加载学生数据时跳过验证失败的数据：" + id + " - " + name + " (" + e.what() + ")");
                error_count++;
            }
        } else {
            logger_.warn("跳过格式错误的行：" + line);
            error_count++;
        }
    }
    
    file.close();
    
    if (error_count > 0) {
        logger_.warn("从文件加载数据完成，成功加载 " + std::to_string(count) + 
                     " 个学生，跳过 " + std::to_string(error_count) + " 个无效数据：" + filename);
    } else {
        logger_.info("从文件加载了 " + std::to_string(count) + " 个学生数据：" + filename);
    }
    
    return count > 0;
}

bool StudentManagementSystem::save_to_excel_file(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        logger_.error("无法打开文件进行保存：" + filename);
        return false;
    }
    
    // 按学号排序
    sort_students_by_id();
    
    // 添加Excel表头（包含成绩信息）
    file << "学号,姓名,性别,班级,电话,邮箱,成绩信息\n";
    
    // 保存数据（包含成绩信息）
    for (const auto& student : students_) {
        file << student.get_id() << ","
             << student.get_name() << ","
             << student.get_gender() << ","
             << student.get_class_id() << ","
             << student.get_phone() << ","
             << student.get_email() << ",";
        
        // 保存成绩信息
        const auto& scores = student.get_scores();
        if (!scores.empty()) {
            bool first = true;
            for (const auto& [subject, score] : scores) {
                if (!first) file << ";";
                file << subject << ":" << score;
                first = false;
            }
        } else {
            file << "无成绩";
        }
        file << "\n";
    }
    
    file.close();
    logger_.info("成功保存Excel格式数据到文件：" + filename);
    return true;
}

void StudentManagementSystem::show_all_students() const {
    if (students_.empty()) {
        std::cout << "当前没有学生数据。" << std::endl;
        return;
    }
    
    std::cout << "=== 学生信息列表 ===" << std::endl;
    std::cout << "总数：" << students_.size() << std::endl;
    std::cout << "-------------------" << std::endl;
    
    for (const auto& student : students_) {
        student.show_info();
        std::cout << "-------------------" << std::endl;
    }
}

bool StudentManagementSystem::delete_student_by_name(const std::string& name) {
    auto matching_students = find_students_by_name_exact(name);
    
    if (matching_students.empty()) {
        logger_.warn("删除学生失败：姓名 " + name + " 不存在");
        return false;
    }
    
    if (matching_students.size() == 1) {
        // 只有一个匹配，直接删除
        students_.remove_if([&matching_students](const Student& s) {
            return s.get_id() == matching_students.front()->get_id();
        });
        logger_.info("成功删除学生：" + name);
        return true;
    } else {
        // 重名情况，显示所有匹配学生
        std::cout << "发现 " << matching_students.size() << " 个同名学生：" << std::endl;
        
        // 使用迭代器来遍历列表，而不是使用[]
        int index = 1;
        for (auto it = matching_students.begin(); it != matching_students.end(); ++it, ++index) {
            std::cout << "[" << index << "] ";
            (*it)->show_info();
            std::cout << "-------------------" << std::endl;
        }
        
        std::cout << "请输入要删除的学生编号: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice < 1 || choice > static_cast<int>(matching_students.size())) {
            logger_.warn("删除学生失败：无效的选择");
            return false;
        }
        
        // 使用迭代器找到对应位置的学生
        auto it = matching_students.begin();
        std::advance(it, choice - 1); // 移动到选择的位置
        
        students_.remove_if([it](const Student& s) {
            return s.get_id() == (*it)->get_id();
        });
        logger_.info("成功删除学生：" + name + " (编号" + std::to_string(choice) + ")");
        return true;
    }
}

std::list<Student*> StudentManagementSystem::find_students_by_name_exact(const std::string& name) {
    std::list<Student*> result;
    
    for (auto& student : students_) {
        if (student.get_name() == name) {  // 精确匹配
            result.push_back(&student);
        }
    }
    
    return result;
}

void StudentManagementSystem::sort_students_by_id() {
    students_.sort([](const Student& a, const Student& b) {
        return a.get_id() < b.get_id();
    });
    logger_.info("按学号排序完成");
}

bool StudentManagementSystem::set_student_score(const std::string& student_id, const std::string& subject, float score) {
    auto it = std::find_if(students_.begin(), students_.end(),
        [&student_id](const Student& student) {
            return student.get_id() == student_id;
        });
    
    if (it == students_.end()) {
        logger_.warn("设置成绩失败：学号 " + student_id + " 不存在");
        return false;
    }
    
    try {
        it->set_score(subject, score);
        logger_.info("成功设置学生成绩：" + student_id + " - " + subject + " = " + std::to_string(score));
        return true;
    } catch (const std::invalid_argument& e) {
        logger_.warn("设置成绩失败：" + student_id + " - " + subject + " (" + e.what() + ")");
        return false;
    }
}

std::string StudentManagementSystem::get_student_scores_info(const std::string& student_id) {
    auto it = std::find_if(students_.begin(), students_.end(),
        [&student_id](const Student& student) {
            return student.get_id() == student_id;
        });
    
    if (it == students_.end()) {
        return "学生不存在";
    }
    
    std::stringstream ss;
    ss << "学号: " << it->get_id() << "\n";
    ss << "姓名: " << it->get_name() << "\n";
    
    const auto& scores = it->get_scores();
    if (scores.empty()) {
        ss << "该学生暂无成绩记录";
    } else {
        ss << "成绩列表:\n";
        for (const auto& [subject, score] : scores) {
            ss << "  " << subject << ": " << score << "\n";
        }
        ss << "平均分: " << it->get_average_score();
    }
    
    return ss.str();
}