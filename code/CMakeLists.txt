cmake_minimum_required(VERSION 3.10)
project(nxtcore LANGUAGES CXX  VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

set(NXTCORE_SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR})
file(GLOB NXTCORE_HEADER_FILES ${NXTCORE_SOURCE_DIRECTORY}/include/*.h)
file(GLOB NXTCORE_SOURCE_FILES ${NXTCORE_SOURCE_DIRECTORY}/src/*.cpp)

#add different files to different source groups
source_group("Header Files"   FILES ${ESSCORE_HEADER_FILES})
source_group("Source Files"   FILES ${ESSCORE_SOURCE_FILES})

add_library(nxtcore SHARED ${NXTCORE_SOURCE_FILES} ${NXTCORE_HEADER_FILES})