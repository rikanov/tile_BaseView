cmake_minimum_required(VERSION 2.6)
project(boardview)

add_library(boardview STATIC boardview.cpp select.cpp)

install(TARGETS boardview RUNTIME DESTINATION bin ARCHIVE DESTINATION lib)
