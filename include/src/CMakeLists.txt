# Collect all algorithm source files automatically
file(GLOB ALGORITHM_SRC
     "${CMAKE_SOURCE_DIR}/src/algorithms/*.cpp"
)

# Core library: scheduler + algorithms
add_library(core
    scheduler.cpp
    ${ALGORITHM_SRC}
)

target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Main executable
add_executable(cpu-sim main.cpp)

target_link_libraries(cpu-sim PRIVATE core)

target_include_directories(cpu-sim PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
