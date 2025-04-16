#include "./src/pc.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
// 测试函数1：无参数任务
void task1() {
  std::cout << "Task1 executed by thread " << std::this_thread::get_id() << std::endl;
}

// 测试函数2：带参数的任务
int add(int a, int b) {
  std::cout << "add(" << a << "," << b << ") in thread " << std::this_thread::get_id() << std::endl;
  return a + b;
}

// 测试函数3：长时间运行的任务
void longTask(const std::string& name, int duration_ms) {
  std::cout << "[" << name << "] started in thread " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
  std::cout << "[" << name << "] finished" << std::endl;
}

int main() {
  // 创建线程池：队列容量100，4个工作线程
  ThreadPool pool(100, 4);

  // 测试1：提交简单lambda任务
  pool.EnQueue([] {
      std::cout << "Lambda task in thread " << std::this_thread::get_id() << std::endl;
      });


  // 测试2：提交普通函数
  pool.EnQueue(std::function<void()>(task1));

  // 测试3：提交带参数的函数（使用bind）
  std::function<void()> boundAdd = std::bind([] {
      // 注意：这里演示了如何获取返回值（实际你的设计不直接支持）
      auto result = add(3, 4);
      std::cout << "add result: " << result << std::endl;
      });
  pool.EnQueue(boundAdd);

  // 测试4：提交多个长时间任务（验证队列阻塞）
  for (int i = 1; i <= 8; ++i) {
    pool.EnQueue(std::bind(longTask, "Task" + std::to_string(i), 500));
  }

  // 等待所有任务完成（简单延迟）
  std::this_thread::sleep_for(std::chrono::seconds(3));

  // 测试5：验证线程池关闭
  std::cout << "Shutting down pool..." << std::endl;
  // 注意：你的实现中析构函数会自动调用stop和join

  return 0;
}
