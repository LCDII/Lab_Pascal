cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME Pascal)
project(${PROJECT_NAME})


set(LIBRARY "${PROJECT_NAME}")
set(INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE  "${CMAKE_CURRENT_SOURCE_DIR}/src")



file(GLOB LIB_SRCS "${SOURCE}/*.cpp")
file(GLOB LIB_H "${INCLUDE}/*.h")

add_library(${LIBRARY} STATIC ${LIB_SRCS} ${LIB_H})


target_include_directories(${LIBRARY} PUBLIC ${INCLUDE})

add_subdirectory(samples)