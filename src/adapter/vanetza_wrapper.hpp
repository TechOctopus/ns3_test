#ifndef VANETZA_WRAPPER_HPP
#define VANETZA_WRAPPER_HPP

#include <memory>
#include <cstdint>

// Forward declarations for Vanetza components
namespace vanetza {
    namespace btp {
        class PortDispatcher;
    }
    namespace geonet {
        class Router;
        class MIB;
        class LinkLayer;
    }
    namespace dcc {
        class AccessControl;
    }
    namespace facilities {
        class Timer;
        class CamService;
    }
}

namespace vanetza_ns3 {

/**
 * @brief Wrapper class for Vanetza components
 * 
 * This class encapsulates the Vanetza protocol stack components
 * and provides a simplified interface for the NS3 adapter to use.
 * It manages the lifecycle of Vanetza components and handles
 * message passing between the adapter and Vanetza.
 */
class VanetzaWrapper {
public:
    /**
     * @brief Constructor
     * @param link_layer The link layer interface to use
     * @param station_id The station ID to use
     */
    VanetzaWrapper(vanetza::geonet::LinkLayer* link_layer, uint32_t station_id);

    /**
     * @brief Destructor
     */
    ~VanetzaWrapper();

    /**
     * @brief Receive a packet from the network
     * @param buffer The packet data
     * @param length The length of the packet data
     */
    void receivePacket(const uint8_t* buffer, std::size_t length);

    /**
     * @brief Trigger the transmission of a CAM message
     */
    void triggerCamTransmission();

    /**
     * @brief Register a callback for received CAM messages
     * @param cb The callback function
     */
    void registerCamReceiver(std::function<void(const uint8_t*, std::size_t)> cb);

private:
    /**
     * @brief Initialize the Vanetza components
     */
    void initializeComponents();

    // Vanetza components
    std::unique_ptr<vanetza::geonet::MIB> m_mib;                  ///< Management Information Base
    std::unique_ptr<vanetza::geonet::Router> m_router;            ///< GeoNetworking router
    std::unique_ptr<vanetza::btp::PortDispatcher> m_dispatcher;   ///< BTP port dispatcher
    std::unique_ptr<vanetza::dcc::AccessControl> m_accessControl; ///< DCC access control
    std::unique_ptr<vanetza::facilities::Timer> m_timer;          ///< Timer service
    std::unique_ptr<vanetza::facilities::CamService> m_camService; ///< CAM service

    // Configuration
    vanetza::geonet::LinkLayer* m_linkLayer; ///< Link layer interface
    uint32_t m_stationId;                    ///< Station ID

    // Callbacks
    std::function<void(const uint8_t*, std::size_t)> m_camReceiverCallback; ///< Callback for received CAM messages
};

} // namespace vanetza_ns3

#endif // VANETZA_WRAPPER_HPP