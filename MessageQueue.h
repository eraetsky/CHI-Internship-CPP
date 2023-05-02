#pragma once
#include "Message.h"
#include "Utils.h"
#include <queue>
#include <deque>
#include <mutex>
#include <condition_variable>

const size_t max_capacity = 10;

class MessageQueue
{
private:
	using ptr_msg = std::shared_ptr<MessageBase>;
	std::priority_queue<ptr_msg, std::vector<ptr_msg>, CompareUrgency> pq;
	std::deque<ptr_msg> dq; //considered that this was my mistake, but met a lack of time
	size_t cnt_elements;
	size_t capacity;
	std::mutex mtx;
	std::condition_variable queue_full;
public: 
	MessageQueue() : cnt_elements(0), capacity(max_capacity) {}
	MessageQueue(const MessageQueue& q) : cnt_elements(q.cnt_elements), capacity(q.capacity) {} //not usefull
	//copy contructor do not copying pq elements, because of empty priority stack. Decided to use deque as a class field

	bool isEmpty() const { return pq.empty(); }

	friend class MessageQueueAnalyzer;

	std::mutex& getMutex() { return mtx; }
	std::condition_variable& getCv() { return queue_full; }

	template<typename T>
	void push(const Message<T>& message, bool& full)
	{
		std::unique_lock<std::mutex> lock(mtx);
		while (!pq.empty() && pq.top()->is_expired()) { pq.pop(); --cnt_elements; }
		try
		{
			if (cnt_elements < capacity)
			{
				auto m = std::make_shared<Message<T>>(message);
				pq.push(m);
				dq.push_front(m);
				++cnt_elements;
				std::cout << "Message added.\n";
				full = false;
			}
			else
				throw(std::range_error("Failed to push: Queue is full\n"));

		}
		catch (std::range_error err) { std::cerr << err.what(); full = true; queue_full.notify_one();  }
	}

	
	ptr_msg top_and_pop(bool& full)
	{
		std::unique_lock<std::mutex> lock(mtx);
		while (!pq.empty() && pq.top()->is_expired()) 
		{ 
			pq.pop(); 
			--cnt_elements; 
		}

		if (pq.empty())
		{
			std::cout << "Failed to delete: queue is empty.\n";
			return nullptr;
		}
		auto res = pq.top();
		pq.pop();
		dq.pop_front();
		--cnt_elements;
		full = false;
		std::cout << "Message deleted.\n";
		return res;
	}
	
	void print(std::ostream& out) 
	{
		std::unique_lock<std::mutex> lock(mtx);
		for (auto msg : dq)
			msg->print(out);
	}
};