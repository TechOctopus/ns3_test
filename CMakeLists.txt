cmake_minimum_required(VERSION 3.5)
project(vanetza_ns3_adapter VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
# Note: In actual implementation, you'll need to set proper paths to NS3 and Vanetza
# These are placeholders that will need to be configured on the target machine
set(NS3_DIR "$ENV{HOME}/ns-3-dev" CACHE PATH "NS3 directory")
set(VANETZA_DIR "$ENV{HOME}/vanetza" CACHE PATH "Vanetza directory")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${NS3_DIR}/include
    ${VANETZA_DIR}/build/include
)

# Add subdirectories
add_subdirectory(src)

# Examples are optional
option(BUILD_EXAMPLES "Build example applications" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation settings
install(DIRECTORY src/
    DESTINATION include/vanetza-ns3-adapter
    FILES_MATCHING PATTERN "*.hpp"
)

# Print configuration summary
message(STATUS "Configuration Summary")
message(STATUS "  NS3 directory: ${NS3_DIR}")
message(STATUS "  Vanetza directory: ${VANETZA_DIR}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")