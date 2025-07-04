cmake_minimum_required(VERSION 3.31)
project(WinSockBridge)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(HEADER_PATH_PREFIX bridge/headers)
set(SRC_PATH_PREFIX bridge/src)

set(HEADERS_BRIDGE
        "${HEADER_PATH_PREFIX}/bridge.h"
        "${HEADER_PATH_PREFIX}/safeguards.h"
        "${HEADER_PATH_PREFIX}/user_aggregator.h"
)

set(SOURCES_BRIDGE main.cpp
        "${SRC_PATH_PREFIX}/bridge.cpp"
        "${SRC_PATH_PREFIX}/safeguards.cpp"
        "${SRC_PATH_PREFIX}/user_aggregator.cpp"
)

add_subdirectory("external/simpleserver")

add_executable(bridge ${HEADERS_BRIDGE} ${SOURCES_BRIDGE})
target_link_libraries(bridge PRIVATE simpleserver)
target_link_libraries(bridge PRIVATE ws2_32 mswsock)
target_include_directories(bridge PRIVATE
        "${CMAKE_SOURCE_DIR}/external/websocketpp"
        "${CMAKE_SOURCE_DIR}/external/asio-1.30.2/include"
)

target_compile_definitions(bridge PRIVATE ASIO_STANDALONE=1 _WEBSOCKETPP_CPP11_THREAD_)
