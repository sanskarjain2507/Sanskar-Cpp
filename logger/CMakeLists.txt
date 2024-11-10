cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(logger
        VERSION 1.0.0
        LANGUAGES C CXX)

add_subdirectory(lib.src)


message(STATUS "${CMAKE_CURRENT_SOURCE_DIR} -- ${PROJECT_NAME}")