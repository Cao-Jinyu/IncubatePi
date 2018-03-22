cmake_minimum_required(VERSION 3.5)

enable_testing()
include_directories(..)

add_executable (GPIOWriterTest GPIOWriterTest.cpp GPIOWriter.cpp)
add_executable (TempReaderTest TempReaderTest.cpp TempReader.cpp)
add_executable (PWMCtrlTest PWMCtrlTest.cpp PWMCtrl.cpp)

add_test(TestGPIO GPIOWriterTest)
add_test(TestTemp TempReaderTest)
add_test(TestPWM PWMCtrlTest)

