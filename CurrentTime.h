#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <stdio.h>
#include <sstream>

const size_t buff_size = 80;

std::string current_time_and_date_string()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

time_t current_time()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);
}

std::string string_time(time_t time)
{
    char buffer[buff_size];
    std::strftime(buffer, buff_size, "%H:%M:%S", std::gmtime(&time));
    std::string result(buffer);
    return result;
}
