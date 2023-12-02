cmake_minimum_required(VERSION 3.5.0)

set(SOURCES
    
    )

include_directories(
    ./src/
    )
add_executable(application ./src/main.cpp ${SOURCES})