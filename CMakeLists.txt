cmake_minimum_required(VERSION 3.9)
project(JobBot)

set(CMAKE_CXX_STANDARD 11)


add_executable(JobBot main.cpp JsonParse.h VK.h Telegram.h WebServer.h DB.h)
target_link_libraries(JobBot curl)
