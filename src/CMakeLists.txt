# Add all subdirectories
add_subdirectory(adapter)
# Add messages and utils directories if they contain source files
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/messages/CMakeLists.txt)
    add_subdirectory(messages)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/utils/CMakeLists.txt)
    add_subdirectory(utils)
endif()

# Create the main library
add_library(vanetza_ns3_adapter STATIC
    $<TARGET_OBJECTS:adapter>
)

# Link against NS3 and Vanetza libraries
target_link_libraries(vanetza_ns3_adapter
    # NS3 libraries - using the correct library names
    ${NS3_DIR}/build/lib/libns3.35-core-debug.so
    ${NS3_DIR}/build/lib/libns3.35-network-debug.so
    ${NS3_DIR}/build/lib/libns3.35-internet-debug.so
    ${NS3_DIR}/build/lib/libns3.35-wave-debug.so
    ${NS3_DIR}/build/lib/libns3.35-wifi-debug.so
    ${NS3_DIR}/build/lib/libns3.35-mobility-debug.so
    
    # Vanetza stub libraries - these are not actually needed since we're using stubs
    # but we keep them here for completion
    # ${VANETZA_DIR}/build/lib/libvanetza_asn1.so
    # ${VANETZA_DIR}/build/lib/libvanetza_btp.so
    # ${VANETZA_DIR}/build/lib/libvanetza_common.so
    # ${VANETZA_DIR}/build/lib/libvanetza_dcc.so
    # ${VANETZA_DIR}/build/lib/libvanetza_facilities.so
    # ${VANETZA_DIR}/build/lib/libvanetza_geonet.so
    # ${VANETZA_DIR}/build/lib/libvanetza_gnss.so
    # ${VANETZA_DIR}/build/lib/libvanetza_security.so
)

# Export the library
install(TARGETS vanetza_ns3_adapter
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)