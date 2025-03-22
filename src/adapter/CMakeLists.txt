# Create object library for adapter components
add_library(adapter OBJECT
    vanetza_ns3_adapter.cpp
    cam_application.cpp
    ns3_interface.cpp
    vanetza_wrapper.cpp
)

# Set include directories
target_include_directories(adapter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${NS3_DIR}/include
    ${VANETZA_DIR}/build/include
)

# Set compile options
target_compile_options(adapter PRIVATE -Wall -Wextra)