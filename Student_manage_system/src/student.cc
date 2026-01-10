#include "student.hh"

Student::Student(std::string id, std::string name, std::string gender, 
                std::string class_id, std::string phone, std::string email)
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

void Student::show_info() const {
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

void Student::set_score(const std::string& subject, float score) {
    if (subject.empty()) {
        throw std::invalid_argument("科目名不能为空");
    }
    if (score < 0 || score > 100) {
        throw std::invalid_argument("成绩必须在0-100之间");
    }
    scores_[subject] = score;
}

float Student::get_score(const std::string& subject) const {
    auto it = scores_.find(subject);
    return it != scores_.end() ? it->second : -1.0f;
}

float Student::get_average_score() const {
    if (scores_.empty()) return 0.0f;
    float sum = 0.0f;
    for (const auto& [_, score] : scores_) {
        sum += score;
    }
    return sum / scores_.size();
}

bool Student::is_valid() const {
    return !id_.empty() && !name_.empty() && !gender_.empty() && !class_id_.empty();
}

void Student::set_id(const std::string& id) {
    validate_id(id);
    id_ = id;
}

void Student::set_name(const std::string& name) {
    validate_name(name);
    name_ = name;
}

void Student::set_gender(const std::string& gender) {
    validate_gender(gender);
    gender_ = gender;
}

void Student::set_class_id(const std::string& class_id) {
    validate_class_id(class_id);
    class_id_ = class_id;
}

void Student::set_phone(const std::string& phone) {
    if (!phone.empty()) validate_phone(phone);
    phone_ = phone;
}

void Student::set_email(const std::string& email) {
    if (!email.empty()) validate_email(email);
    email_ = email;
}

void Student::validate_id(const std::string& id) const {
    if (id.empty()) throw std::invalid_argument("学号不能为空");
    if (!std::regex_match(id, std::regex("^\\d{10}$"))) {
        throw std::invalid_argument("学号必须为10位数字");
    }
}

void Student::validate_name(const std::string& name) const {
    if (name.empty()) throw std::invalid_argument("姓名不能为空");
    if (name.length() < 2 || name.length() > 20) {
        throw std::invalid_argument("姓名长度必须在2-20个字符之间");
    }
}

void Student::validate_gender(const std::string& gender) const {
    if (gender != "男" && gender != "女") {
        throw std::invalid_argument("性别必须为'男'或'女'");
    }
}

void Student::validate_class_id(const std::string& class_id) const {
    if (class_id.empty()) throw std::invalid_argument("班级号不能为空");
    if (class_id.length() < 3) throw std::invalid_argument("班级号格式不正确");
}

void Student::validate_phone(const std::string& phone) const {
    if (!std::regex_match(phone, std::regex("^1[3-9]\\d{9}$"))) {
        throw std::invalid_argument("手机号格式不正确（必须是11位数字）");
    }
}

void Student::validate_email(const std::string& email) const {
    if (!std::regex_match(email, std::regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"))) {
        throw std::invalid_argument("邮箱格式不正确");
    }
}