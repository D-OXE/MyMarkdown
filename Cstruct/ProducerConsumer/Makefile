# 编译器设置
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -I./include
LDFLAGS := -lpthread
TARGET := threadpool

# 源文件设置
SRC_DIR := .
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=%.o)

# 头文件依赖
DEPS := include/BlockQueue.hpp

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

rebuild: clean all