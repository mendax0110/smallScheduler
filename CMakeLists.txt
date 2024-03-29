# set cmake version
cmake_minimum_required(VERSION 3.0)

# set project name
project(smallScheduler)

# set c++ standard
set(CMAKE_CXX_STANDARD 17)

# set include directories
include_directories(include)

# set source files
set(SOURCE_FILES
    src/main.cpp
    src/schedule.cpp
    src/task.cpp
)

# add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})