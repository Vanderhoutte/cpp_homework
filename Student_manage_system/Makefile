# 学生信息管理系统 Makefile
# 编译器设置
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./include
TARGET = student_management_system
SRCDIR = src
INCDIR = include
BUILDDIR = build
SOURCES = $(wildcard $(SRCDIR)/*.cc)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)

# 默认目标
all: $(BUILDDIR) $(TARGET)

# 创建构建目录
$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

# 链接生成可执行文件
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "✅ 编译完成！可执行文件: $(TARGET)"

# 编译源文件
$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 调试版本
debug: CXXFLAGS += -g -DDEBUG
debug: clean all

# 发布版本
release: CXXFLAGS += -O3 -DNDEBUG
release: clean all

# 运行程序
run: $(TARGET)
	@echo "🚀 运行学生信息管理系统..."
	./$(TARGET)

# 清理构建文件
clean:
	@rm -rf $(BUILDDIR) $(TARGET)
	@echo "🧹 清理完成！"

# 重新构建
rebuild: clean all

# 显示帮助信息
help:
	@echo "学生信息管理系统 Makefile 使用说明:"
	@echo "  make all       - 编译项目（默认）"
	@echo "  make debug     - 编译调试版本"
	@echo "  make release   - 编译发布版本"
	@echo "  make run       - 编译并运行程序"
	@echo "  make clean     - 清理构建文件"
	@echo "  make rebuild   - 重新构建项目"
	@echo "  make help      - 显示此帮助信息"

# 显示项目信息
info:
	@echo "项目结构:"
	@echo "  源文件目录: $(SRCDIR)"
	@echo "  头文件目录: $(INCDIR)"
	@echo "  构建目录: $(BUILDDIR)"
	@echo "  源文件: $(SOURCES)"
	@echo "  目标文件: $(OBJECTS)"

# 伪目标声明
.PHONY: all debug release run clean rebuild help info