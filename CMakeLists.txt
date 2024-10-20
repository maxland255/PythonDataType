cmake_minimum_required(VERSION 3.29)
project(PythonDataType)

set(CMAKE_CXX_STANDARD 20)

add_library(PythonDataType STATIC library.cpp list.h
        String.cpp
        String.h)
