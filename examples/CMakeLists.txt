# Add example executables
add_executable(cam_simulation_example cam_simulation_example.cc)

# Set include directories
target_include_directories(cam_simulation_example PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${NS3_DIR}/build
    ${NS3_DIR}/src
    ${VANETZA_STUBS_DIR}
    ${VANETZA_DIR}
    ${VANETZA_DIR}/vanetza
)

# Link against the adapter library and NS3 libraries
target_link_libraries(cam_simulation_example
    vanetza_ns3_adapter
    
    # NS3 libraries - using the correct library names
    ${NS3_DIR}/build/lib/libns3.35-core-debug.so
    ${NS3_DIR}/build/lib/libns3.35-network-debug.so
    ${NS3_DIR}/build/lib/libns3.35-internet-debug.so
    ${NS3_DIR}/build/lib/libns3.35-wave-debug.so
    ${NS3_DIR}/build/lib/libns3.35-wifi-debug.so
    ${NS3_DIR}/build/lib/libns3.35-mobility-debug.so
    ${NS3_DIR}/build/lib/libns3.35-applications-debug.so
)

# Set compile options
target_compile_options(cam_simulation_example PRIVATE -Wall -Wextra)

# Install examples
install(TARGETS cam_simulation_example
    RUNTIME DESTINATION bin/examples
)