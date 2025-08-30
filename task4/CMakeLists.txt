cmake_minimum_required(VERSION 3.10.0)
project(Task4 VERSION 0.1.0 LANGUAGES C CXX)

find_package(Boost REQUIRED COMPONENTS thread)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

if(BUILD_SERVER)
    add_executable(
        main_server
        ./src/server/Logger.cpp
        ./src/server/Server.cpp
        ./src/server/Session.cpp
        ./src/server/main.cpp)
    
    target_link_libraries(
        main_server
        PRIVATE
        Boost::thread)
endif()

if(BUILD_CLIENT)
    add_executable(
        main_client
        ./src/client/Client.cpp
        ./src/client/main.cpp)
    
    target_link_libraries(
        main_client
        PRIVATE
        Boost::thread)
endif()
