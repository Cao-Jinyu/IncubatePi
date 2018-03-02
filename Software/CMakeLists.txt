cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(NENONATE)

add_executable(
	TempControl
	GPIOWriter.cpp
	PWMCtrl.cpp
	ReadTemp.cpp
	main.cpp
)