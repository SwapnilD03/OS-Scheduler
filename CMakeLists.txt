cmake_minimum_required(VERSION 3.12)

project(cpu_scheduler
    VERSION 1.0
    DESCRIPTION "CPU Scheduling Simulator (Modular, Phase-1 Architecture)"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers in `include/`
include_directories(${CMAKE_SOURCE_DIR}/include)

# Allow subdirectory builds
add_subdirectory(src)

