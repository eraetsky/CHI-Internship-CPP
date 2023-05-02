#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <stdio.h>
#include <random>
#include <sstream>
#include <condition_variable>
#include <mutex>
#include "MessageQueue.h"
#include "Analyzer.h"

extern std::atomic<int> callCounter;

namespace mt //stands for multithreading
{
    size_t max_wait_time_ms = 100; //upper bound for wait time between random push or pop operations
    const int minutes_limit = 5;  //working time limit
    
    template <typename T>
    void push_or_pop(MessageQueue& q, const Message<T>& msg, bool& full) 
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        if (dis(gen) == 0)
            q.push(msg,full);
        else q.top_and_pop(full);
    }

    template <typename T>
    void emulate_load(MessageQueue& q, std::vector<Message<T>> msgs, bool& full) //random push or pop with random delay 
    {
        auto start_time = std::chrono::steady_clock::now();
        int i = 0;
        std::random_device rd;
        std::mt19937 gen(rd());
        while (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start_time).count() < minutes_limit)
        {
            if (i == msgs.size())
                i = 0;
            push_or_pop(q, msgs.at(i), full);
            std::uniform_int_distribution<> wait_dis(0, max_wait_time_ms);
            auto wait_time = std::chrono::milliseconds(wait_dis(gen));
            std::this_thread::sleep_for(wait_time);
            ++i;
        }
    }

    void pop_every_second(MessageQueue& q, bool& full)
    {
        auto start_time = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start_time).count() < minutes_limit)
        {
            q.top_and_pop(full);
            std::this_thread::sleep_for(1000ms);
        }
    }

    void invoke_analyzer(MessageQueue& q, MessageQueueAnalyzer& analyzer, std::condition_variable& cv, bool& full) //invokes analyzer when queue is full and every minute
    {
        auto start_time = std::chrono::steady_clock::now();
        auto next_invoke_time = start_time + std::chrono::minutes(1);
        std::mutex m;
        while (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start_time).count() < minutes_limit)
        {
            std::unique_lock<std::mutex> lock(m);
            cv.wait_until(lock, next_invoke_time, [&full]() { return full; });
            if (full)
            {
                std::cout << "Queue is full, invoking analyzer.\n";
                analyzer.analyze(q);
                ++callCounter;
                full = false;
            }
            else
            {
                std::cout << "1min pass, invoking analyzer.\n";
                analyzer.analyze(q);
                ++callCounter;
                next_invoke_time += std::chrono::minutes(1);
            }
        }
    }

};