# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(LoadBalancer VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directory for the binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(include)

# Add the executable
add_executable(LoadBalancer 
    src/main.cpp 
    src/LoadBalancer.cpp 
    src/Server.cpp 
    src/ThreadPool.cpp
)

# Manually add the -pthread flag for both compilation and linking
target_compile_options(LoadBalancer PRIVATE -pthread)
target_link_libraries(LoadBalancer PRIVATE -pthread)

# Compiler flags (optional)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(LoadBalancer PRIVATE -Wall -Wextra -pedantic)
elseif (MSVC)
    target_compile_options(LoadBalancer PRIVATE /W4 /permissive-)
endif()

# Optional: Copy configuration files (e.g., servers.json) to output directory
file(COPY ${CMAKE_SOURCE_DIR}/config/servers.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
