#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <random>
#include <sstream>
#include "Utils.h"
#include "CustomStringBuffer.h"
using namespace std::literals;

enum urgency { Low, Normal, High};

bool operator < (urgency u1, urgency u2)
{
	return static_cast<int>(u1) < static_cast<int>(u2);
}


class MessageBase
{
public:
	virtual ~MessageBase() = default;
	virtual urgency getUrgency() const = 0;
	virtual time_t getExpires() const = 0;
	virtual bool is_expired() const = 0;
	virtual void print(std::ostream&) const = 0;
};

template<typename T>
class Message : public MessageBase
{
private:
	urgency u;
	std::time_t expires;
	T msg;
public:
	Message() : u(Low) {}
	Message(urgency urg, std::time_t time, const T& message) : u{ urg }, expires{ time }, msg{ message } {}
	~Message() {};

	urgency getUrgency() const override { return u; }
	time_t getExpires() const override { return expires;  }

	template <typename Y>
	bool operator < (const Message<Y>& other) const { return u < other.getUrgency(); }

	bool is_expired() const override { return difftime(expires, util::current_time()) < 0; }
	void print(std::ostream& out) const override { CustomStringBuffer csb; csb += msg; out << "Urgency: " << u << " , Message: "; csb.print(out);  }
};

struct CompareUrgency
{
	bool operator() (std::shared_ptr<MessageBase> ptr1, std::shared_ptr<MessageBase> ptr2)
	{
		return ptr1->getUrgency() < ptr2->getUrgency();
	}
};





