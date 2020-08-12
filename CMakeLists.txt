cmake_minimum_required(VERSION 3.5)

project(WinEventListener LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)

add_executable(WinEventListener
  main.cpp
  wineventlistener.cpp
  focuschangedeventhandler.cpp
  generaleventshandler.cpp
)
target_link_libraries(WinEventListener Qt5::Core)
