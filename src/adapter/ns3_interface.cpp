#include "ns3_interface.hpp"

#include <ns3/log.h>
#include <ns3/packet.h>
#include <ns3/simulator.h>
#include <ns3/mac48-address.h>

namespace vanetza_ns3 {

NS_LOG_COMPONENT_DEFINE("NS3Interface");

NS3Interface::NS3Interface(ns3::Ptr<ns3::NetDevice> device) :
    m_device(device)
{
    NS_LOG_FUNCTION(this << device);
}

NS3Interface::~NS3Interface()
{
    NS_LOG_FUNCTION(this);
}

void
NS3Interface::indicatePacket(const uint8_t* buffer, std::size_t length)
{
    NS_LOG_FUNCTION(this << buffer << length);
    
    // Forward the packet to the registered handler if available
    if (m_packetHandler) {
        m_packetHandler(buffer, length);
    }
}

bool
NS3Interface::sendPacket(const uint8_t* buffer, std::size_t length)
{
    NS_LOG_FUNCTION(this << buffer << length);
    
    if (!m_device) {
        NS_LOG_ERROR("No device set for NS3Interface");
        return false;
    }
    
    // Create NS3 packet from buffer
    ns3::Ptr<ns3::Packet> packet = ns3::Create<ns3::Packet>(buffer, length);
    
    // Send packet using the device
    // Protocol number 0x8947 is used for ETSI ITS-G5 (example value)
    return m_device->Send(packet, ns3::Mac48Address::GetBroadcast(), 0x8947);
}

ns3::Mac48Address
NS3Interface::getMacAddress() const
{
    NS_LOG_FUNCTION(this);
    
    // Get the MAC address from the device
    ns3::Address addr = m_device->GetAddress();
    
    // Convert to Mac48Address
    return ns3::Mac48Address::ConvertFrom(addr);
}

void
NS3Interface::registerPacketHandler(std::function<void(const uint8_t*, std::size_t)> cb)
{
    NS_LOG_FUNCTION(this);
    m_packetHandler = cb;
}

} // namespace vanetza_ns3