#ifndef CAM_APPLICATION_HPP
#define CAM_APPLICATION_HPP

#include <ns3/application.h>
#include <ns3/ptr.h>
#include <ns3/event-id.h>
#include <ns3/traced-callback.h>

namespace ns3 {
    class NetDevice;
}

namespace vanetza_ns3 {

// Forward declarations
class VanetzaNS3Adapter;

/**
 * @brief Application class for generating and processing CAM messages
 * 
 * This class implements an NS3 application that generates Cooperative
 * Awareness Messages (CAMs) based on the vehicle's mobility model and
 * processes received CAM messages from other vehicles.
 */
class CamApplication : public ns3::Application {
public:
    /**
     * @brief Constructor
     */
    CamApplication();

    /**
     * @brief Destructor
     */
    virtual ~CamApplication();

    /**
     * @brief Get the TypeId for this class
     * @return The TypeId
     */
    static ns3::TypeId GetTypeId();

    /**
     * @brief Set the Vanetza-NS3 adapter to use
     * @param adapter The adapter
     */
    void SetAdapter(ns3::Ptr<VanetzaNS3Adapter> adapter);

    /**
     * @brief Traced callback for received CAM messages
     * 
     * Parameters: station_id, position_x, position_y, speed, heading
     */
    typedef ns3::TracedCallback<uint32_t, float, float, float, float> CamReceivedCallback;

protected:
    /**
     * @brief Start the application
     * 
     * This method is called when the application starts. It initializes
     * the CAM generation process.
     */
    virtual void StartApplication() override;

    /**
     * @brief Stop the application
     * 
     * This method is called when the application stops. It cleans up
     * resources and stops CAM generation.
     */
    virtual void StopApplication() override;

private:
    /**
     * @brief Schedule the next CAM generation
     */
    void ScheduleNextCamGeneration();

    /**
     * @brief Generate and send a CAM message
     */
    void GenerateCam();

    /**
     * @brief Process a received CAM message
     * @param data The message data
     * @param size The size of the message data
     */
    void ReceiveCam(const uint8_t* data, std::size_t size);

    // NS3 components
    ns3::Ptr<VanetzaNS3Adapter> m_adapter;  ///< The Vanetza-NS3 adapter
    ns3::EventId m_camEvent;                ///< Event for CAM generation

    // Configuration
    uint32_t m_stationId;                   ///< Station ID
    double m_camGenerationInterval;         ///< Interval between CAM generations in seconds

    // Traced callbacks
    CamReceivedCallback m_camReceivedSignal;  ///< Signal for received CAM messages
};

} // namespace vanetza_ns3

#endif // CAM_APPLICATION_HPP