#include <iostream>
#include <chrono>
#include <iomanip>
#include <stdio.h>
#include <sstream>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "Utils.h"
#include "MtUtils.h"
#include "Message.h"
#include "Analyzer.h"
#include "MessageQueue.h"

using namespace std::literals;

//wanted to try both approaches: managing global variable between threads and simple atomic
std::atomic<int> callCounter{ 0 };
bool isfull = false; 

int main()
{
    
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now) + 60;
    time_t tt = std::chrono::system_clock::to_time_t(now) + 3660;
    Message<int> msg1(High, t, 42);
    Message<std::string> msg2(Low, t, "Hello");
    Message<int> msg3(Normal, t, 1444);
    Message<std::string> msg4(High, t, "Hello!!");
    Message<std::vector<int>> vec(High, tt, {1,2,5,8});

    MessageQueue queue; 
    MessageQueueAnalyzer a;

    std::vector<Message<int>> intv;
    intv.push_back(msg1); intv.push_back(msg3);
    std::vector<Message<std::string>> strv;
    strv.push_back(msg2); strv.push_back(msg4);
    std::vector<Message<std::vector<int>>> vecv;
    vecv.push_back(vec);
     
    std::thread t1(mt::emulate_load<int>, std::ref(queue), std::ref(intv), std::ref(isfull)); //t1,t2,t3 randomly push or pop messages from corresponding vector  
    std::thread t2(mt::emulate_load<std::string>, std::ref(queue), std::ref(strv), std::ref(isfull)); //one by one. When messages ends, starts from the vector begin
    std::thread t3(mt::emulate_load<std::vector<int>>, std::ref(queue), std::ref(vecv), std::ref(isfull));
    std::thread t4(mt::pop_every_second, std::ref(queue), std::ref(isfull));
    std::thread t5(mt::invoke_analyzer, std::ref(queue), std::ref(a), std::ref(queue.getCv()), std::ref(isfull));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
  
    std::cout << callCounter << std::endl;
    return 0;
}

