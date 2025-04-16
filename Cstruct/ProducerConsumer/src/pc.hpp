#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>

// 对于使用了template的.h文件,建议写成.hpp文件,直接将定义和声明都完成
template<typename T>
class BlockQueue {
private:
	std::queue<T> m_BQueue;
	mutable std::mutex m_Mutex;
	std::condition_variable m_Cond;
	/*
		可以用一个条件变量,但是DeepSeek说会有性能问题,推荐我用两个,单独管理满不满或者空不空.
		但是对于 简单的项目,单个就可以了.无伤大雅.
	*/
	int m_MaxSize;

	// bool isFull() const {
	// 	std::unique_lock<std::mutex> lock(this->m_Mutex);
	// 	return this->m_BQueue.size() == this->m_MaxSize;
	// }

public:

	bool isEmpty() const {
		// because of mutable mutex
		std::unique_lock<std::mutex> lock(this->m_Mutex);
		return this->m_BQueue.empty();
	}
	BlockQueue(int size = 10) {
		{
			// if size < 0??????????   bug
			this->m_MaxSize = size;
			//assert(size > 0);
		}

	}


	void push(const T& data) {
		std::unique_lock<std::mutex> lock(this->m_Mutex);
		m_Cond.wait(lock, [this]() { return m_BQueue.size() < m_MaxSize; });
		this->m_BQueue.push(data);
		m_Cond.notify_one();
	}
	T pop() {
		std::unique_lock<std::mutex> lock(this->m_Mutex);
		m_Cond.wait(lock, [this]() { return !m_BQueue.empty(); });
		T CallBack = this->m_BQueue.front();	//时刻注意对象生命周期
		this->m_BQueue.pop();
		m_Cond.notify_one();
		return CallBack;
	}
};


// ===========================================
class ThreadPool {
private:
	int m_ThreadNum;
	std::vector<std::thread> m_Threads;
	BlockQueue<std::function<void()>> m_TaskQueue;
	//std::atomic<bool> stop;
	bool stop;
	std::mutex m_Mutex;
	std::condition_variable m_Cond;

	void EventLoop() {
		while (!stop) {
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Cond.wait(lock, [this]() {
				return stop || !m_TaskQueue.isEmpty();  // 如果 stop=true，立即退出
				});

			if (stop && m_TaskQueue.isEmpty()) {
				break;  // 退出循环
			}

			std::function<void(void)> CallBack = m_TaskQueue.pop();
			CallBack();
		}
	}
public:
	ThreadPool(int BQsuze, int threadnum = 8) :m_TaskQueue(BQsuze),
		m_ThreadNum(threadnum),
		stop(false)
	{
		for (int i = 0;i < m_ThreadNum; i++) {
			this->m_Threads.emplace_back(std::thread(&ThreadPool::EventLoop, this));
		}
	}


	~ThreadPool() {
		stop = true;
		m_Cond.notify_all();
		for (int i = 0;i < m_ThreadNum; i++) {
			if (this->m_Threads[i].joinable()) {
				this->m_Threads[i].join();
			}
		}
	}



	template<typename T>
	void EnQueue(T&& func) {
		m_TaskQueue.push(std::forward<T>(func));
	}
};