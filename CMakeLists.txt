set(CMAKE_MAKE_PROGRAM "C:/Ninja/ninja.exe" CACHE FILEPATH "Ninja Generator" FORCE)
set(QT_QPA_PLATFORM_PLUGIN_PATH "C:/Qt/5.15.0/mingw81_64/plugins")

cmake_minimum_required(VERSION 3.12.4)

set(PROJECT "MinimalQml")
project(${PROJECT})

set (CMAKE_PREFIX_PATH "C:\\Qt\\5.15.0\\mingw81_64\\lib\\cmake\\Qt5")

# 3rd party tools
find_package(Qt5 COMPONENTS Widgets Qml Quick Charts REQUIRED)

# Directory with source code
add_subdirectory(src)
