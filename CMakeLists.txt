cmake_minimum_required(VERSION 3.6)
project(HackAssembler)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        source/Assembler/assembler.cpp
        source/Assembler/assembler.h
        source/Assembler/Main.cpp)

add_executable(HackAssembler ${SOURCE_FILES})