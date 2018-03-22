cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

enable_testing()
include_directories(..)

add_executable (GPIOWriterTest GPIOWriterTest.cpp ../GPIOWriter.cpp)
add_executable (TempReaderTest TempReaderTest.cpp ../TempReader.cpp)
add_executable (PWMCtrlTest PWMCtrlTest.cpp ../PWMCtrl.cpp)

add_test(TestGPIOWriter GPIOWriterTest)
add_test(TestTempReader TempReaderTest)
add_test(TestPWMCtrl PWMCtrlTest)

