cmake_minimum_required(VERSION 3.16)

project(dump_simfile CXX)

add_executable(dump_simfile dump_simfile.cpp)
set_property(TARGET dump_simfile PROPERTY CXX_STANDARD 17)