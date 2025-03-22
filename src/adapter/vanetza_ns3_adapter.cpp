#include "vanetza_ns3_adapter.hpp"
#include "ns3_interface.hpp"
#include "vanetza_wrapper.hpp"

#include <ns3/log.h>
#include <ns3/simulator.h>
#include <ns3/uinteger.h>
#include <ns3/double.h>
#include <ns3/packet.h>
#include <ns3/wave-net-device.h>

namespace vanetza_ns3 {

NS_LOG_COMPONENT_DEFINE("VanetzaNS3Adapter");

NS_OBJECT_ENSURE_REGISTERED(VanetzaNS3Adapter);

VanetzaNS3Adapter::VanetzaNS3Adapter() :
    m_device(nullptr),
    m_stationId(0),
    m_camInterval(1.0) // Default CAM interval: 1 second
{
    NS_LOG_FUNCTION(this);
}

VanetzaNS3Adapter::~VanetzaNS3Adapter()
{
    NS_LOG_FUNCTION(this);
}

ns3::TypeId
VanetzaNS3Adapter::GetTypeId()
{
    static ns3::TypeId tid = ns3::TypeId("vanetza_ns3::VanetzaNS3Adapter")
        .SetParent<ns3::Application>()
        .SetGroupName("VANET")
        .AddConstructor<VanetzaNS3Adapter>()
        .AddAttribute("StationId",
                      "Station ID of this ITS station",
                      ns3::UintegerValue(0),
                      ns3::MakeUintegerAccessor(&VanetzaNS3Adapter::m_stationId),
                      ns3::MakeUintegerChecker<uint32_t>())
        .AddAttribute("CamInterval",
                      "Interval between CAM transmissions in seconds",
                      ns3::DoubleValue(1.0),
                      ns3::MakeDoubleAccessor(&VanetzaNS3Adapter::m_camInterval),
                      ns3::MakeDoubleChecker<double>(0.1, 10.0));
    return tid;
}

void
VanetzaNS3Adapter::SetDevice(ns3::Ptr<ns3::NetDevice> device)
{
    NS_LOG_FUNCTION(this << device);
    m_device = device;
}

void
VanetzaNS3Adapter::SetStationId(uint32_t id)
{
    NS_LOG_FUNCTION(this << id);
    m_stationId = id;
}

void
VanetzaNS3Adapter::StartApplication()
{
    NS_LOG_FUNCTION(this);
    
    if (!m_device) {
        NS_LOG_ERROR("No device set for VanetzaNS3Adapter");
        return;
    }
    
    // Initialize Vanetza components
    InitializeVanetza();
    
    // Set up packet reception callback
    m_device->SetReceiveCallback(
        ns3::MakeCallback(&VanetzaNS3Adapter::ReceiveFromNS3, this));
    
    // Schedule first CAM transmission
    ScheduleNextCamTransmission();
}

void
VanetzaNS3Adapter::StopApplication()
{
    NS_LOG_FUNCTION(this);
    
    // Cancel any pending events
    if (m_camEvent.IsRunning()) {
        m_camEvent.Cancel();
    }
    
    // Clean up Vanetza components
    m_vanetzaWrapper.reset();
    m_ns3Interface.reset();
}

void
VanetzaNS3Adapter::InitializeVanetza()
{
    NS_LOG_FUNCTION(this);
    
    // Create NS3 interface for Vanetza
    m_ns3Interface = std::make_unique<NS3Interface>(m_device);
    
    // Create Vanetza wrapper with the interface
    m_vanetzaWrapper = std::make_unique<VanetzaWrapper>(m_ns3Interface.get(), m_stationId);
}

bool
VanetzaNS3Adapter::ReceiveFromNS3(ns3::Ptr<ns3::NetDevice> device,
                                  ns3::Ptr<const ns3::Packet> packet,
                                  uint16_t protocol,
                                  const ns3::Address& from,
                                  const ns3::Address& to,
                                  ns3::NetDevice::PacketType packetType)
{
    NS_LOG_FUNCTION(this << device << packet << protocol << from << to << packetType);
    
    // Only process packets for this device
    if (device != m_device) {
        return false;
    }
    
    // Check if this is a CAM message (based on protocol)
    // In a real implementation, you would check for ETSI ITS protocol identifiers
    if (protocol == 0x8947) { // Example protocol number for ETSI ITS-G5
        // Copy packet data
        uint32_t size = packet->GetSize();
        uint8_t* buffer = new uint8_t[size];
        packet->CopyData(buffer, size);
        
        // Forward to Vanetza for processing
        if (m_vanetzaWrapper) {
            m_vanetzaWrapper->receivePacket(buffer, size);
        }
        
        // If there's a registered callback for CAM messages, call it
        if (m_camReceiverCallback) {
            m_camReceiverCallback(buffer, size);
        }
        
        delete[] buffer;
        return true;
    }
    
    return false;
}

void
VanetzaNS3Adapter::ScheduleNextCamTransmission()
{
    NS_LOG_FUNCTION(this);
    
    // Schedule next CAM transmission
    m_camEvent = ns3::Simulator::Schedule(
        ns3::Seconds(m_camInterval),
        &VanetzaNS3Adapter::GenerateAndSendCam,
        this);
}

void
VanetzaNS3Adapter::GenerateAndSendCam()
{
    NS_LOG_FUNCTION(this);
    
    // Generate CAM message using Vanetza
    if (m_vanetzaWrapper) {
        m_vanetzaWrapper->triggerCamTransmission();
    }
    
    // Schedule next transmission
    ScheduleNextCamTransmission();
}

bool
VanetzaNS3Adapter::SendCam(const uint8_t* data, std::size_t size)
{
    NS_LOG_FUNCTION(this << data << size);
    
    if (!m_device) {
        NS_LOG_ERROR("No device set for VanetzaNS3Adapter");
        return false;
    }
    
    // Create NS3 packet from data
    ns3::Ptr<ns3::Packet> packet = ns3::Create<ns3::Packet>(data, size);
    
    // Send packet using the device
    // In a real implementation, you would set the appropriate protocol number and address
    return m_device->Send(packet, ns3::Mac48Address::GetBroadcast(), 0x8947);
}

void
VanetzaNS3Adapter::RegisterCamReceiver(std::function<void(const uint8_t*, std::size_t)> cb)
{
    NS_LOG_FUNCTION(this);
    m_camReceiverCallback = cb;
}

} // namespace vanetza_ns3