cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(4-STL LANGUAGES CXX)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDART_REQUIRED ON)

add_executable(4-STL.x "")

target_sources(
	4-STL.x
	PRIVATE
		main.cpp
		Analyzer.h
		CurrentTime.h
		CustomStringBuffer.h
		Message.h
		MessageQueue.h
)