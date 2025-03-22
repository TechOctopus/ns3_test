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
    # NS3 directories
    ${NS3_DIR}/build
    ${NS3_DIR}/src
    # Vanetza stubs directory (prioritize stubs over actual headers)
    ${VANETZA_STUBS_DIR}
    # Vanetza directory
    ${VANETZA_DIR}
    ${VANETZA_DIR}/vanetza
)

# Set compile options
target_compile_options(adapter PRIVATE -Wall -Wextra)