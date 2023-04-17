#include <iostream>
#include <chrono>
#include <iomanip>
#include <stdio.h>
#include <sstream>
#include <thread>
#include "CurrentTime.h"
#include "Message.h"
#include "Analyzer.h"
#include "MessageQueue.h"
using namespace std::literals;



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
    
     
    queue.push(msg1);
    queue.push(msg2);
    queue.push(msg3);

    queue.push(msg4);
    queue.push(vec);
    
    queue.print_and_clean(std::cout);
    
    std::cout << current_time_and_date_string() << std::endl;

    MessageQueueAnalyzer a;
    a.analyze(queue);
   
    
    return 0;
}

