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
            src/drawpaneFunctions.cpp
            src/elements.cpp
            ../airflow/lib/leakage.cpp
            ../airflow/lib/multizone.cpp)

# Define executable
set(EXECUTABLE_NAME main)
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Include headers
include_directories(include)

# Include airflow headers
#include_directories (home/fredrik/Git/airflow/lib)
#include_directories (C:/Git/airflow/lib)
include_directories (../airflow/lib)

# Include Eigen
IF (WIN32)
				include_directories (C:/Users/domfr/Downloads/Eigen)
ELSE()
				include_directories (/usr/local/include/eigen3)
ENDIF()

find_package(wxWidgets COMPONENTS net gl core base)
if(wxWidgets_FOUND)
  include(${wxWidgets_USE_FILE})
  # and for each of your dependent executable/library targets:
  target_link_libraries(${EXECUTABLE_NAME} ${wxWidgets_LIBRARIES})
endif()
