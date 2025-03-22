cmake_minimum_required(VERSION 3.5)
project(vanetza_ns3_adapter VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
# Set the paths to NS3 and Vanetza relative to this project
set(NS3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ns-3-dev" CACHE PATH "NS3 directory")
set(VANETZA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vanetza" CACHE PATH "Vanetza directory")
set(VANETZA_STUBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stubs" CACHE PATH "Vanetza stubs directory")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    # NS3 directories
    ${NS3_DIR}/build
    ${NS3_DIR}/src
    # Vanetza stubs directory (prioritize stubs over actual headers)
    ${VANETZA_STUBS_DIR}
    # Vanetza directory
    ${VANETZA_DIR}
    ${VANETZA_DIR}/vanetza
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
message(STATUS "  Vanetza stubs directory: ${VANETZA_STUBS_DIR}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")