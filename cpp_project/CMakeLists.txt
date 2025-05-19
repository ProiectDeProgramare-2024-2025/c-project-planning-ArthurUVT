cmake_minimum_required(VERSION 3.10)

project(SportsTrackerCpp VERSION 1.0)

# Require C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
add_executable(sports_tracker_cpp 
    main.cpp
    tracker.cpp
)

# Include directories if headers are separated (optional for this simple case)
# target_include_directories(sports_tracker_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries if needed (none external needed here)
# target_link_libraries(sports_tracker_cpp ...)

# Enable warnings (optional but recommended)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sports_tracker_cpp PRIVATE -Wall -Wextra -pedantic)
endif()
