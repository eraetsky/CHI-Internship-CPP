cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(6-Multithreading LANGUAGES CXX)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDART_REQUIRED ON)

add_executable(6-Multithreading.x "")

target_sources(
	6-Multithreading.x
	PRIVATE
		main.cpp
		Analyzer.h
		Utils.h
		MtUtils.h
		CustomStringBuffer.h
		Message.h
		MessageQueue.h
)