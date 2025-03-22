#include "cam_application.hpp"
#include "vanetza_ns3_adapter.hpp"

#include <ns3/log.h>
#include <ns3/simulator.h>
#include <ns3/uinteger.h>
#include <ns3/double.h>
#include <ns3/mobility-model.h>
#include <ns3/vector.h>

namespace vanetza_ns3 {

NS_LOG_COMPONENT_DEFINE("CamApplication");

NS_OBJECT_ENSURE_REGISTERED(CamApplication);

CamApplication::CamApplication() :
    m_adapter(nullptr),
    m_stationId(0),
    m_camGenerationInterval(1.0) // Default: 1 second
{
    NS_LOG_FUNCTION(this);
}

CamApplication::~CamApplication()
{
    NS_LOG_FUNCTION(this);
}

ns3::TypeId
CamApplication::GetTypeId()
{
    static ns3::TypeId tid = ns3::TypeId("vanetza_ns3::CamApplication")
        .SetParent<ns3::Application>()
        .SetGroupName("VANET")
        .AddConstructor<CamApplication>()
        .AddAttribute("StationId",
                      "Station ID of this ITS station",
                      ns3::UintegerValue(0),
                      ns3::MakeUintegerAccessor(&CamApplication::m_stationId),
                      ns3::MakeUintegerChecker<uint32_t>())
        .AddAttribute("CamGenerationInterval",
                      "Interval between CAM generations in seconds",
                      ns3::DoubleValue(1.0),
                      ns3::MakeDoubleAccessor(&CamApplication::m_camGenerationInterval),
                      ns3::MakeDoubleChecker<double>(0.1, 10.0));
    return tid;
}

void
CamApplication::SetAdapter(ns3::Ptr<VanetzaNS3Adapter> adapter)
{
    NS_LOG_FUNCTION(this << adapter);
    m_adapter = adapter;
    
    // Register as CAM receiver
    if (m_adapter) {
        m_adapter->RegisterCamReceiver(
            std::bind(&CamApplication::ReceiveCam, this, 
                      std::placeholders::_1, std::placeholders::_2));
    }
}

void
CamApplication::StartApplication()
{
    NS_LOG_FUNCTION(this);
    
    if (!m_adapter) {
        NS_LOG_ERROR("No adapter set for CamApplication");
        return;
    }
    
    // Schedule first CAM generation
    ScheduleNextCamGeneration();
}

void
CamApplication::StopApplication()
{
    NS_LOG_FUNCTION(this);
    
    // Cancel any pending events
    if (m_camEvent.IsRunning()) {
        m_camEvent.Cancel();
    }
}

void
CamApplication::ScheduleNextCamGeneration()
{
    NS_LOG_FUNCTION(this);
    
    // Schedule next CAM generation
    m_camEvent = ns3::Simulator::Schedule(
        ns3::Seconds(m_camGenerationInterval),
        &CamApplication::GenerateCam,
        this);
}

void
CamApplication::GenerateCam()
{
    NS_LOG_FUNCTION(this);
    
    // Get node's current position and speed from mobility model
    ns3::Ptr<ns3::MobilityModel> mobility = GetNode()->GetObject<ns3::MobilityModel>();
    if (!mobility) {
        NS_LOG_WARN("No mobility model found for node");
        ScheduleNextCamGeneration();
        return;
    }
    
    ns3::Vector position = mobility->GetPosition();
    ns3::Vector velocity = mobility->GetVelocity();
    
    // Create CAM message with position, speed, and other vehicle data
    // In a real implementation, this would create a proper CAM message according to ETSI standards
    // For now, we'll create a simple binary message with basic information
    
    // Simple CAM format (example):
    // - 4 bytes: Station ID
    // - 4 bytes: Timestamp (seconds since simulation start)
    // - 4 bytes: X position (in meters)
    // - 4 bytes: Y position (in meters)
    // - 4 bytes: Speed (in m/s)
    // - 4 bytes: Heading (in degrees)
    
    uint8_t cam_buffer[24];
    uint32_t* station_id_ptr = reinterpret_cast<uint32_t*>(&cam_buffer[0]);
    uint32_t* timestamp_ptr = reinterpret_cast<uint32_t*>(&cam_buffer[4]);
    float* pos_x_ptr = reinterpret_cast<float*>(&cam_buffer[8]);
    float* pos_y_ptr = reinterpret_cast<float*>(&cam_buffer[12]);
    float* speed_ptr = reinterpret_cast<float*>(&cam_buffer[16]);
    float* heading_ptr = reinterpret_cast<float*>(&cam_buffer[20]);
    
    // Fill in the values
    *station_id_ptr = m_stationId;
    *timestamp_ptr = static_cast<uint32_t>(ns3::Simulator::Now().GetSeconds());
    *pos_x_ptr = static_cast<float>(position.x);
    *pos_y_ptr = static_cast<float>(position.y);
    *speed_ptr = static_cast<float>(std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y));
    *heading_ptr = static_cast<float>(std::atan2(velocity.y, velocity.x) * 180.0 / M_PI);
    
    // Send CAM message using the adapter
    if (m_adapter) {
        m_adapter->SendCam(cam_buffer, sizeof(cam_buffer));
    }
    
    // Schedule next CAM generation
    ScheduleNextCamGeneration();
}

void
CamApplication::ReceiveCam(const uint8_t* data, std::size_t size)
{
    NS_LOG_FUNCTION(this << data << size);
    
    // Process received CAM message
    // In a real implementation, this would parse the CAM message according to ETSI standards
    // For now, we'll parse our simple binary format
    
    if (size < 24) {
        NS_LOG_WARN("Received CAM message is too small: " << size << " bytes");
        return;
    }
    
    // Parse the CAM message
    uint32_t station_id = *reinterpret_cast<const uint32_t*>(&data[0]);
    uint32_t timestamp = *reinterpret_cast<const uint32_t*>(&data[4]);
    float pos_x = *reinterpret_cast<const float*>(&data[8]);
    float pos_y = *reinterpret_cast<const float*>(&data[12]);
    float speed = *reinterpret_cast<const float*>(&data[16]);
    float heading = *reinterpret_cast<const float*>(&data[20]);
    
    // Log the received CAM information
    NS_LOG_INFO("Received CAM from station " << station_id
                << " at time " << timestamp
                << ": position=" << pos_x << "," << pos_y
                << ", speed=" << speed
                << ", heading=" << heading);
    
    // Emit signal for received CAM
    m_camReceivedSignal(station_id, pos_x, pos_y, speed, heading);
}

} // namespace vanetza_ns3