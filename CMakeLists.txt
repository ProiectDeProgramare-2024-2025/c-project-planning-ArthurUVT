cmake_minimum_required(VERSION 3.10)
project(SportsActivityTracker VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add app_1 executable
add_executable(app_1 
    app_1_main.cpp
    app_1.cpp
)

# Add app_2 executable
add_executable(app_2 
    app_2_main.cpp
    app_2.cpp
)

# Include directories
target_include_directories(app_1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(app_2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Set output directory for both executables
set_target_properties(app_1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(app_2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add compile flags
if(MSVC)
    target_compile_options(app_1 PRIVATE /W4)
    target_compile_options(app_2 PRIVATE /W4)
else()
    target_compile_options(app_1 PRIVATE -Wall -Wextra)
    target_compile_options(app_2 PRIVATE -Wall -Wextra)
endif()

# Print status message
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}") 