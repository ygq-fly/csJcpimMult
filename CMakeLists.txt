cmake_minimum_required(VERSION 3.2)
project(JcPimMultiBandV2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_DEBUG_POSTFIX -d)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_subdirectory(JcPimMultiBandV2)
set (EXTRA_LIBS ${EXTRA_LIBS} JcPimMultiBandV2)

set(DEMO_SOURCE_FILES
    TestDemo/stdafx.cpp
    TestDemo/stdafx.h
    TestDemo/targetver.h
    TestDemo/TestDemo.cpp)

add_executable(TestDemo ${DEMO_SOURCE_FILES} TestDemo/TestDemo.h)