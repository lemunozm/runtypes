cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(runtypes VERSION "0.1.0" LANGUAGES CXX)

set(SRCS example/main.cpp)

add_executable(${PROJECT_NAME}_example ${SRCS})

target_include_directories(${PROJECT_NAME}_example
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME}_example
    PROPERTIES
        CXX_STANDARD 11
)
