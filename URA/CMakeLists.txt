cmake_minimum_required(VERSION 3.17)
project(URA)

set(CMAKE_CXX_STANDARD 14)

add_executable(URA
        URA.cpp
        times.cpp
        times.h
        DopplerChannel/DopplerChannel.cpp
        DopplerChannel/DopplerChannel.h
        DopplerChannel/FileUtils.cpp
        DopplerChannel/FileUtils.h
        DopplerChannel/GeneralUtils.h
        DopplerChannel/RNG.cpp
        DopplerChannel/RNG.h)

target_link_libraries(URA

        )

