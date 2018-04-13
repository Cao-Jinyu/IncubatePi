cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_AUTOMOC ON)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pthread" )

project(INCUPI)

add_subdirectory(Test)

find_package(Qt5Widgets)

enable_testing()

add_executable(
	TempControl
	GPIOWriter.cpp
	PWMCtrl.cpp
	TempReader.cpp
	PID.cpp
	main.cpp
	window.cpp
)

target_link_libraries(TempControl Qt5::Widgets qwt-qt5 m pthread)
