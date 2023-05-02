#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>
#include <mutex>
#include "MessageQueue.h"
#include "Utils.h"
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
	void clear_output_file();
	std::mutex mtx;
public:
	MessageQueueAnalyzer() { clear_output_file(); };
	void analyze(const MessageQueue&);
};

void MessageQueueAnalyzer::clear_output_file()
{
	const fs::path workdir = fs::current_path();
	const fs::path output_file = workdir / "analyze.txt";
	std::ofstream out;
	out.open(output_file, std::ios_base::trunc);
}

std::tuple<double, double, double> MessageQueueAnalyzer::get_urgency(const MessageQueue& queue)
{
	if (queue.isEmpty()) { return { 0,0,0 }; }
	std::unique_lock<std::mutex> lock(mtx);
	double high = 0, normal = 0, low = 0, total;
	for (auto msg : queue.dq)
	{
		if (msg->getUrgency() == High) ++high;
		else if (msg->getUrgency() == Normal) ++normal;
		else ++low;
	}
	lock.unlock();
	total = high + normal + low;
	return { 100*high/total, 100*normal/total, 100*low/total };
	
}

std::pair<double, double> MessageQueueAnalyzer::get_size_in_kb(const MessageQueue& queue)
{
	if (queue.isEmpty()) { return std::make_pair(0,0); }
	double size = 0;
	std::unique_lock<std::mutex> lock(mtx);
	size = std::accumulate(queue.dq.begin(), queue.dq.end(), 0, [](size_t acc, const auto& msg) { return acc + sizeof(*msg); });;
	return std::make_pair(static_cast<double>(sizeof(queue)) / 1000, size / 1000);
}

time_t MessageQueueAnalyzer::max_differenc_t(const MessageQueue& queue)
{
	if (queue.isEmpty()) { return 0; }
	std::unique_lock<std::mutex> lock(mtx);
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
	std::ofstream out = std::ofstream(output_file.native(), std::ios_base::app);
	out << util::current_time_and_date_string() << std::endl;
	out << "Capacity: " << this->get_capacity(queue) << ", Number of elements: " << this->get_cnt_elements(queue) << std::endl;
	double h, n, l, total_size, elem_size;
	std::tie(h,n,l) = get_urgency(queue);
	out << "High urgency - " << h << "%, Normal urgency - " << n << "%, Low urgency - " << l << "%" << std::endl;
	std::tie(total_size, elem_size) = get_size_in_kb(queue);
	out << "Size of all elements: " << elem_size << "Kb, total size: " << total_size << "Kb" << std::endl;
	out << "The maximum difference between expires of messages is " << util::string_time(max_differenc_t(queue)) << std::endl;
	out << std::endl;
}