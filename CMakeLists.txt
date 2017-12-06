# Project name
project(main)

# CMake version
cmake_minimum_required(VERSION 3.5)

# -
add_definitions(-std=c++11 -g)

# Define sources 
set(SOURCES src/main.cpp
            src/drawpane.cpp
            src/myleakage.cpp
            src/myzone.cpp
            src/mycolors.cpp
            src/air/leakage.cpp
            src/air/multizone.cpp)

# Define executable
set(EXECUTABLE_NAME main)
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Inlcude headers
include_directories(include)

# Include Eigen
include_directories (/usr/local/include/eigen3)

find_package(wxWidgets COMPONENTS net gl core base)
if(wxWidgets_FOUND)
  include(${wxWidgets_USE_FILE})
  # and for each of your dependent executable/library targets:
  target_link_libraries(${EXECUTABLE_NAME} ${wxWidgets_LIBRARIES})
endif()
