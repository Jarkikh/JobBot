cmake_minimum_required(VERSION 3.9)
project(JobBot)

set(CMAKE_CXX_STANDARD 11)


add_executable(JobBot main.cpp JsonParse.h VK.h Telegram.h WebServer.h DB.h)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package (Boost REQUIRED system thread)

include_directories(${Boost_INCLUDE_DIR} /usr/include/cppconn)
link_directories(${Boost_LIBRARY_DIR} /usr/lib)
target_link_libraries(JobBot mysqlcppconn ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} curl)
