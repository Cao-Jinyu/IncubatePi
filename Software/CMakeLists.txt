cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pthread" )

project(INCUPI)

add_subdirectory(test)

enable_testing()

add_executable(
	TempControl
	GPIOWriter.cpp
	PWMCtrl.cpp
	ReadTemp.cpp
        PID.cpp
	main.cpp
)
