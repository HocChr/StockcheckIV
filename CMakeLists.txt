set(CMAKE_MAKE_PROGRAM "C:/Ninja/ninja.exe" CACHE FILEPATH "Ninja Generator" FORCE)
#set(QT_QPA_PLATFORM_PLUGIN_PATH "C:/Qt/5.15.0/mingw81_64/plugins")
#set(QT_QPA_PLATFORM_PLUGIN_PATH "C:\\Qt\\Qt5.12.2_NeuInstallation\\5.12.2\\mingw73_64\\plugins")

cmake_minimum_required(VERSION 3.12.4)

set(PROJECT "MinimalQml")
project(${PROJECT})

#set (CMAKE_PREFIX_PATH "C:\\Qt\\5.15.0\\mingw81_64\\lib\\cmake\\Qt5")
#set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.12.2_NeuInstallation\\5.12.2\\mingw73_64\\")

# 3rd party tools
find_package(Qt5 COMPONENTS Widgets Qml Quick QuickControls2 Charts REQUIRED)

# Directory with source code
add_subdirectory(src)
