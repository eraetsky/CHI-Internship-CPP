#pragma once
#include "Message.h"
#include <queue>
#include <deque>

const size_t max_capacity = 10;

//Використовую priority_queue та дублюючу деку, для того щоб можна було ітеруватись в аналізаторі черги
//Була ідея використати multimap і зберігати для кожного меседжу також порядок подачі в чергу, але є підозра що
//такий варіант буде менш оптимальним за часом і менш зрозумілий для читання

class MessageQueue
{
private:
	using ptr_msg = std::shared_ptr<MessageBase>;
	std::priority_queue<ptr_msg, std::vector<ptr_msg>, CompareUrgency> pq;
	std::deque<ptr_msg> dq; //in order to iterate on it
	size_t cnt_elements;
	size_t capacity;
public: 
	MessageQueue() : cnt_elements(0), capacity(max_capacity) {}
	MessageQueue(const MessageQueue& q) : cnt_elements(q.cnt_elements), capacity(q.capacity) {} //not usefull
	//copy contructor do not copying pq elements, because of empty priority stack. Decided to use deque as a class field
	friend class MessageQueueAnalyzer;
	
	template<typename T>
	void push(const Message<T>& message)
	{
		while (!pq.empty() && pq.top()->is_expired()) { pq.pop(); --cnt_elements; }
		try
		{
			if (cnt_elements < capacity)
			{
				auto m = std::make_shared<Message<T>>(message);
				pq.push(m);
				dq.push_front(m);
				++cnt_elements;
			}
			else
				throw("Failed to push: Message queue is full\n");
		}
		catch (const char* err) { std::cerr << err; }
		
	}

	
	ptr_msg top_and_pop()
	{
		while (!pq.empty() && pq.top()->is_expired()) { pq.pop(); --cnt_elements; }
		if (pq.empty()) return nullptr;
		auto res = pq.top();
		pq.pop();
		dq.pop_front();
		--cnt_elements;
		return res;
	}
	

	void print_and_clean(std::ostream& out) 
	{
		while (!pq.empty())
		{
			pq.top()->print(out);
			pq.pop();
		}
	}
};
