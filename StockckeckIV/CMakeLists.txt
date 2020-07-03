cmake_minimum_required(VERSION 3.12.4)

if (NOT CMAKE_MAKE_PROGRAM)
set(CMAKE_MAKE_PROGRAM "C:/Ninja/ninja.exe" CACHE FILEPATH "Ninja Generator" FORCE)
endif()

# 3rd party tools
find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)

# Directory with source code
add_subdirectory(src)
