/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wave-module.h"
#include "ns3/applications-module.h"

#include "adapter/vanetza_ns3_adapter.hpp"
#include "adapter/cam_application.hpp"

#include <iostream>
#include <sstream>

using namespace ns3;
using namespace vanetza_ns3;

NS_LOG_COMPONENT_DEFINE("CamSimulationExample");

// Trace callbacks
static void
TraceMobility (Ptr<const MobilityModel> mobility)
{
    std::cout << "Node position: " << mobility->GetPosition() << 
                " velocity: " << dynamic_cast<const ConstantVelocityMobilityModel*>(PeekPointer(mobility))->GetVelocity() << std::endl;
}

// Trace callback for received packets
static void
TraceCamPacket (std::string context, uint32_t stationId, const Time& time)
{
    std::cout << context << " CAM received from station " << stationId << " at time " << time.GetSeconds() << "s" << std::endl;
}

// Helper function for simulation time logging
static void
LogSimTime (double timeValue)
{
    std::cout << "Simulation time: " << timeValue << "s" << std::endl;
}

int main(int argc, char *argv[])
{
    // Enable logging
    LogComponentEnable("CamSimulationExample", LOG_LEVEL_ALL);
    LogComponentEnable("VanetzaNS3Adapter", LOG_LEVEL_INFO);
    LogComponentEnable("CamApplication", LOG_LEVEL_INFO);
    
    // Simulation parameters
    uint32_t nVehicles = 10;
    double simTime = 100.0; // seconds
    double roadLength = 1000.0; // meters
    
    // Allow command line arguments
    CommandLine cmd;
    cmd.AddValue("nVehicles", "Number of vehicles", nVehicles);
    cmd.AddValue("simTime", "Simulation time in seconds", simTime);
    cmd.AddValue("roadLength", "Length of the road in meters", roadLength);
    cmd.Parse(argc, argv);
    
    // Create nodes for vehicles
    std::cout << "Creating " << nVehicles << " vehicles" << std::endl;
    NodeContainer vehicles;
    vehicles.Create(nVehicles);
    
    // Set up mobility model (straight line road)
    MobilityHelper mobility;
    
    // Configure positions along a straight road
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    for (uint32_t i = 0; i < nVehicles; i++) {
        // Distribute vehicles along the road
        positionAlloc->Add(Vector(i * (roadLength / nVehicles), 0.0, 0.0));
    }
    mobility.SetPositionAllocator(positionAlloc);
    
    // Set mobility model with constant velocity
    mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    mobility.Install(vehicles);
    
    // Set vehicle speeds (between 10-30 m/s)
    for (uint32_t i = 0; i < nVehicles; i++) {
        Ptr<ConstantVelocityMobilityModel> model = vehicles.Get(i)->GetObject<ConstantVelocityMobilityModel>();
        // Vary speeds to create realistic scenario
        double speed = 10.0 + (20.0 * i / nVehicles); // 10-30 m/s (36-108 km/h)
        model->SetVelocity(Vector(speed, 0.0, 0.0));
        
        // Connect mobility trace
        Ptr<MobilityModel> mobility = vehicles.Get(i)->GetObject<MobilityModel>();
        mobility->TraceConnectWithoutContext("CourseChange", MakeCallback(&TraceMobility));
    }
    
    // Create and configure Wi-Fi devices instead of WAVE
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper wifiPhy;  
    wifiPhy.SetChannel(wifiChannel.Create());
    
    // Enable PCAP tracing on all WiFi devices
    wifiPhy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11);
    
    // Use regular Wi-Fi MAC instead of WAVE
    WifiMacHelper wifiMac;
    wifiMac.SetType("ns3::AdhocWifiMac");
    
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a); // Similar to 802.11p used in WAVE
    
    // Install devices
    NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, vehicles);
    
    // Set MAC addresses explicitly
    for (uint32_t i = 0; i < devices.GetN(); i++) {
        Ptr<WifiNetDevice> device = DynamicCast<WifiNetDevice>(devices.Get(i));
        if (device) {
            std::stringstream ss;
            ss << "00:00:00:00:00:" << std::hex << (i + 1);
            Mac48Address addr = Mac48Address(ss.str().c_str());
            
            device->SetAddress(addr);
            
            std::cout << "Configured WifiNetDevice " << i << " with MAC address " 
                << device->GetAddress() << std::endl;
        }
    }
    
    // Install Vanetza-NS3 adapter and CAM application on each vehicle
    for (uint32_t i = 0; i < nVehicles; i++) {
        // Create and configure the Vanetza-NS3 adapter
        Ptr<VanetzaNS3Adapter> adapter = CreateObject<VanetzaNS3Adapter>();
        adapter->SetDevice(devices.Get(i));  // Use Wi-Fi devices
        adapter->SetStationId(i + 1); // Station IDs start from 1
        
        // Create and configure the CAM application
        Ptr<CamApplication> camApp = CreateObject<CamApplication>();
        camApp->SetAdapter(adapter);
        camApp->SetAttribute("StationId", UintegerValue(i + 1));
        camApp->SetAttribute("CamGenerationInterval", DoubleValue(0.2)); // 200ms interval for more traffic
        
        // Connect trace source for received CAMs
        std::ostringstream context;
        context << "Vehicle[" << i << "] ";
        camApp->TraceConnect("CamReceived", context.str(), MakeCallback(&TraceCamPacket));
        
        // Install applications on the vehicle
        vehicles.Get(i)->AddApplication(adapter);
        vehicles.Get(i)->AddApplication(camApp);
        
        std::cout << "Installed Vanetza adapter and CAM application on vehicle " << i << std::endl;
    }
    
    // Schedule logging during simulation
    for (double t = 1.0; t < simTime; t += 1.0) {
        // Use a helper function
        Simulator::Schedule(Seconds(t), &LogSimTime, t);
    }
    
    // Run simulation
    std::cout << "Running simulation for " << simTime << " seconds" << std::endl;
    
    // Enable PCAP tracing for all devices
    wifiPhy.EnablePcap("cam-simulation", devices);
    
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    Simulator::Destroy();
    
    std::cout << "Simulation completed successfully" << std::endl;
    
    return 0;
}