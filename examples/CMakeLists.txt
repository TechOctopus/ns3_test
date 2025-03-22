# Add example executables
add_executable(cam_simulation_example cam_simulation_example.cc)

# Set include directories
target_include_directories(cam_simulation_example PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${NS3_DIR}/include
    ${VANETZA_DIR}/build/include
)

# Link against the adapter library and NS3 libraries
target_link_libraries(cam_simulation_example
    vanetza_ns3_adapter
    
    # NS3 libraries
    ${NS3_DIR}/lib/libns3-dev-core.so
    ${NS3_DIR}/lib/libns3-dev-network.so
    ${NS3_DIR}/lib/libns3-dev-internet.so
    ${NS3_DIR}/lib/libns3-dev-wave.so
    ${NS3_DIR}/lib/libns3-dev-wifi.so
    ${NS3_DIR}/lib/libns3-dev-mobility.so
    ${NS3_DIR}/lib/libns3-dev-applications.so
)

# Set compile options
target_compile_options(cam_simulation_example PRIVATE -Wall -Wextra)

# Install examples
install(TARGETS cam_simulation_example
    RUNTIME DESTINATION bin/examples
)