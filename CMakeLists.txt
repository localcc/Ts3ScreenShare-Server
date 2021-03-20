project(Ts3ScreenShare-Server C CXX)

cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
add_subdirectory(${LIBS_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${LIBS_DIR}/enet/include)

add_executable(${PROJECT_NAME} main.cpp
    include/network/udp_server.h src/network/udp_server.cpp
    include/network/connected_client.h src/network/connected_client.cpp
    include/network/NetworkHelper.h)
