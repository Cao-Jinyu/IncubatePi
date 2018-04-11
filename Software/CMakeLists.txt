cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pthread" )

project(INCUPI)

add_subdirectory(Test)

# Find includes in corresponding build directories
# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
# set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
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

# Use the Widgets module from Qt 5.
target_link_libraries(TempControl Qt5::Widgets)