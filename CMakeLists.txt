cmake_minimum_required(VERSION 2.6)
project(boardview)

add_executable(boardview main.cpp)

install(TARGETS boardview RUNTIME DESTINATION bin)
