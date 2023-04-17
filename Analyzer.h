#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include "MessageQueue.h"
#include "CurrentTime.h"
#include "Message.h"
namespace fs = std::filesystem;

class MessageQueueAnalyzer
{
private:
	size_t get_cnt_elements(const MessageQueue& q) { return q.cnt_elements; }
	size_t get_capacity(const MessageQueue& q) { return q.capacity; }
	std::tuple<double, double, double> get_urgency(const MessageQueue&);
	std::pair<double, double> get_size_in_kb(const MessageQueue&);
	time_t max_differenc_t(const MessageQueue&);
public:
	MessageQueueAnalyzer() = default;
	void analyze(const MessageQueue&);
};

std::tuple<double, double, double> MessageQueueAnalyzer::get_urgency(const MessageQueue& queue)
{
	double high = 0, normal = 0, low = 0, total;
	for (auto msg : queue.dq)
	{
		if (msg->getUrgency() == High) ++high;
		else if (msg->getUrgency() == Normal) ++normal;
		else ++low;
	}
	total = high + normal + low;
	return { 100*high/total, 100*normal/total, 100*low/total };
	
}

std::pair<double, double> MessageQueueAnalyzer::get_size_in_kb(const MessageQueue& queue)
{
	double size = 0;
	for (auto msg : queue.dq)
		size += sizeof(*msg);
	
	return std::make_pair(static_cast<double>(sizeof(queue)) / 1000, size / 1000);
}

time_t MessageQueueAnalyzer::max_differenc_t(const MessageQueue& queue)
{
	std::vector<time_t> v;
	for (auto msg : queue.dq)
		v.emplace_back(msg->getExpires());
	std::sort(v.begin(), v.end());
	return difftime(v.back(), v.front());
}


void MessageQueueAnalyzer::analyze(const MessageQueue& queue)
{
	const fs::path workdir = fs::current_path();
	const fs::path output_file = workdir / "analyze.txt";
	std::ofstream out = std::ofstream(output_file.native());
	out << current_time_and_date_string() << std::endl;
	out << "Capacity: " << this->get_capacity(queue) << ", Number of elements: " << this->get_cnt_elements(queue) << std::endl;
	double h, n, l, total_size, elem_size;
	std::tie(h,n,l) = get_urgency(queue);
	out << "High urgency - " << h << "%, Normal urgency - " << n << "%, Low urgency - " << l << "%" << std::endl;
	std::tie(total_size, elem_size) = get_size_in_kb(queue);
	out << "Size of all elements: " << elem_size << "Kb, total size: " << total_size << "Kb" << std::endl;
	out << "The maximum difference between expires of messages is " << string_time(max_differenc_t(queue)) << std::endl;
}