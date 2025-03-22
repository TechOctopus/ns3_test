# Add all subdirectories
add_subdirectory(adapter)
add_subdirectory(messages)
add_subdirectory(utils)

# Create the main library
add_library(vanetza_ns3_adapter STATIC
    $<TARGET_OBJECTS:adapter>
    $<TARGET_OBJECTS:messages>
    $<TARGET_OBJECTS:utils>
)

# Link against NS3 and Vanetza libraries
target_link_libraries(vanetza_ns3_adapter
    # NS3 libraries
    ${NS3_DIR}/build/lib/libns3-dev-core.so
    ${NS3_DIR}/build/lib/libns3-dev-network.so
    ${NS3_DIR}/build/lib/libns3-dev-internet.so
    ${NS3_DIR}/build/lib/libns3-dev-wave.so
    ${NS3_DIR}/build/lib/libns3-dev-wifi.so
    ${NS3_DIR}/build/lib/libns3-dev-mobility.so
    
    # Vanetza libraries
    ${VANETZA_DIR}/build/lib/libvanetza_asn1.so
    ${VANETZA_DIR}/build/lib/libvanetza_btp.so
    ${VANETZA_DIR}/build/lib/libvanetza_common.so
    ${VANETZA_DIR}/build/lib/libvanetza_dcc.so
    ${VANETZA_DIR}/build/lib/libvanetza_facilities.so
    ${VANETZA_DIR}/build/lib/libvanetza_geonet.so
    ${VANETZA_DIR}/build/lib/libvanetza_gnss.so
    ${VANETZA_DIR}/build/lib/libvanetza_security.so
)

# Export the library
install(TARGETS vanetza_ns3_adapter
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)