#ifndef NS3_INTERFACE_HPP
#define NS3_INTERFACE_HPP

#include <cstdint>
#include <memory>
#include <functional>
#include <ns3/net-device.h>
#include <ns3/ptr.h>
#include <ns3/mac48-address.h>
#include <vanetza/geonet/link_layer.hpp>

// Forward declarations for other Vanetza components
namespace vanetza {
    namespace geonet {
        // LinkLayer is now included directly
    }
}

namespace vanetza_ns3 {

/**
 * @brief Interface class that connects NS3 with Vanetza's link layer
 * 
 * This class implements the necessary functionality to bridge NS3's
 * network devices with Vanetza's link layer interface. It handles
 * packet transmission and reception between the two frameworks.
 */
class NS3Interface : public vanetza::geonet::LinkLayer {
public:
    /**
     * @brief Constructor
     * @param device The NS3 network device to use
     */
    explicit NS3Interface(ns3::Ptr<ns3::NetDevice> device);

    /**
     * @brief Destructor
     */
    virtual ~NS3Interface();

    /**
     * @brief Indicate a packet reception from NS3 to Vanetza
     * @param buffer The packet data
     * @param length The length of the packet data
     */
    void indicatePacket(const uint8_t* buffer, std::size_t length);

    /**
     * @brief Send a packet from Vanetza to NS3
     * @param buffer The packet data
     * @param length The length of the packet data
     * @return True if the packet was sent successfully
     */
    bool sendPacket(const uint8_t* buffer, std::size_t length);

    /**
     * @brief Get the MAC address of the interface
     * @return The MAC address
     */
    ns3::Mac48Address getMacAddress() const;

    /**
     * @brief Register a callback for received packets
     * @param cb The callback function
     */
    void registerPacketHandler(std::function<void(const uint8_t*, std::size_t)> cb);

private:
    ns3::Ptr<ns3::NetDevice> m_device;  ///< The NS3 network device
    std::function<void(const uint8_t*, std::size_t)> m_packetHandler;  ///< Callback for received packets
};

} // namespace vanetza_ns3

#endif // NS3_INTERFACE_HPP