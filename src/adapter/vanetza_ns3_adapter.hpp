#ifndef VANETZA_NS3_ADAPTER_HPP
#define VANETZA_NS3_ADAPTER_HPP

#include <memory>
#include <string>
#include <functional>
#include <ns3/node.h>
#include <ns3/net-device.h>
#include <ns3/application.h>
#include <ns3/event-id.h>
#include <ns3/ptr.h>
#include <ns3/ipv4-address.h>
#include <ns3/traced-callback.h>

// Forward declarations for Vanetza components
namespace vanetza {
    namespace btp {
        class PortDispatcher;
    }
    namespace geonet {
        class Router;
        class MIB;
    }
    namespace dcc {
        class AccessControl;
    }
}

namespace vanetza_ns3 {

// Forward declarations
class VanetzaWrapper;
class NS3Interface;

/**
 * @brief Main adapter class that integrates Vanetza with NS3
 * 
 * This class serves as the primary interface between NS3 simulation
 * environment and the Vanetza C-ITS protocol stack. It manages the
 * lifecycle of Vanetza components and handles message passing between
 * the two frameworks.
 */
class VanetzaNS3Adapter : public ns3::Application {
public:
    /**
     * @brief Constructor
     */
    VanetzaNS3Adapter();

    /**
     * @brief Destructor
     */
    virtual ~VanetzaNS3Adapter();

    /**
     * @brief Get the TypeId for this class
     * @return The TypeId
     */
    static ns3::TypeId GetTypeId();

    /**
     * @brief Set the network device to use for communication
     * @param device The network device
     */
    void SetDevice(ns3::Ptr<ns3::NetDevice> device);

    /**
     * @brief Set the station ID for this node
     * @param id The station ID
     */
    void SetStationId(uint32_t id);

    /**
     * @brief Send a CAM message
     * @param data The message data
     * @param size The size of the message data
     * @return True if the message was sent successfully
     */
    bool SendCam(const uint8_t* data, std::size_t size);

    /**
     * @brief Register a callback for received CAM messages
     * @param cb The callback function
     */
    void RegisterCamReceiver(std::function<void(const uint8_t*, std::size_t)> cb);

protected:
    /**
     * @brief Start the application
     * 
     * This method is called when the application starts. It initializes
     * the Vanetza components and sets up the communication interfaces.
     */
    virtual void StartApplication() override;

    /**
     * @brief Stop the application
     * 
     * This method is called when the application stops. It cleans up
     * the Vanetza components and releases resources.
     */
    virtual void StopApplication() override;

private:
    /**
     * @brief Initialize the Vanetza components
     */
    void InitializeVanetza();

    /**
     * @brief Handle a received packet from NS3
     * @param device The device that received the packet
     * @param packet The received packet
     * @param protocol The protocol number
     * @param from The source address
     * @param to The destination address
     * @param packetType The packet type
     * @return True if the packet was handled successfully
     */
    bool ReceiveFromNS3(ns3::Ptr<ns3::NetDevice> device,
                        ns3::Ptr<const ns3::Packet> packet,
                        uint16_t protocol,
                        const ns3::Address& from,
                        const ns3::Address& to,
                        ns3::NetDevice::PacketType packetType);

    /**
     * @brief Schedule the next CAM transmission
     */
    void ScheduleNextCamTransmission();

    /**
     * @brief Generate and send a CAM message
     */
    void GenerateAndSendCam();

    // NS3 components
    ns3::Ptr<ns3::NetDevice> m_device;  ///< The network device
    ns3::EventId m_camEvent;            ///< Event for CAM transmission
    uint32_t m_stationId;               ///< Station ID

    // Vanetza components
    std::unique_ptr<VanetzaWrapper> m_vanetzaWrapper;  ///< Wrapper for Vanetza components
    std::unique_ptr<NS3Interface> m_ns3Interface;      ///< Interface to NS3

    // Callbacks
    std::function<void(const uint8_t*, std::size_t)> m_camReceiverCallback;  ///< Callback for received CAM messages

    // Configuration
    double m_camInterval;  ///< Interval between CAM transmissions in seconds
};

} // namespace vanetza_ns3

#endif // VANETZA_NS3_ADAPTER_HPP