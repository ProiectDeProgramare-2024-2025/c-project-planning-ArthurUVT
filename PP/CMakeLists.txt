cmake_minimum_required(VERSION 3.10)

project(SportsTracker)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler warnings for better code quality
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Create app_1 executable (Core application management)
add_executable(app_1 
    app_1.cpp
    activity.h
)

# Create app_2 executable (User interaction features)
add_executable(app_2 
    app_2.cpp
    activity.h
)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Print build information
message(STATUS "Building Sports Tracker Applications")
message(STATUS "  - app_1: Core application management")
message(STATUS "  - app_2: User interaction features")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Include directories if headers are separated (optional for this simple case)
# target_include_directories(sports_tracker_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries if needed (none external needed here)
# target_link_libraries(sports_tracker_cpp ...)

# Enable warnings (optional but recommended)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sports_tracker_cpp PRIVATE -Wall -Wextra -pedantic)
endif()
