/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wave-module.h"
#include "ns3/applications-module.h"

#include "adapter/vanetza_ns3_adapter.hpp"
#include "adapter/cam_application.hpp"

using namespace ns3;
using namespace vanetza_ns3;

NS_LOG_COMPONENT_DEFINE("CamSimulationExample");

int main(int argc, char *argv[])
{
    // Enable logging
    LogComponentEnable("CamSimulationExample", LOG_LEVEL_INFO);
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
    NS_LOG_INFO("Creating " << nVehicles << " vehicles");
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
    }
    
    // Create and configure WAVE devices
    YansWifiChannelHelper waveChannel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper wavePhy = YansWifiPhyHelper::Default();
    wavePhy.SetChannel(waveChannel.Create());
    wavePhy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11);
    
    // Set up 802.11p
    QosWaveMacHelper waveMac = QosWaveMacHelper::Default();
    WaveHelper waveHelper = WaveHelper::Default();
    
    // Install devices
    NetDeviceContainer waveDevices = waveHelper.Install(wavePhy, waveMac, vehicles);
    
    // Install Vanetza-NS3 adapter and CAM application on each vehicle
    for (uint32_t i = 0; i < nVehicles; i++) {
        // Create and configure the Vanetza-NS3 adapter
        Ptr<VanetzaNS3Adapter> adapter = CreateObject<VanetzaNS3Adapter>();
        adapter->SetDevice(waveDevices.Get(i));
        adapter->SetStationId(i + 1); // Station IDs start from 1
        
        // Create and configure the CAM application
        Ptr<CamApplication> camApp = CreateObject<CamApplication>();
        camApp->SetAdapter(adapter);
        camApp->SetAttribute("StationId", UintegerValue(i + 1));
        camApp->SetAttribute("CamGenerationInterval", DoubleValue(1.0)); // 1 second interval
        
        // Install applications on the vehicle
        vehicles.Get(i)->AddApplication(adapter);
        vehicles.Get(i)->AddApplication(camApp);
    }
    
    // Enable packet captures (PCAP)
    wavePhy.EnablePcap("cam-simulation", waveDevices);
    
    // Run simulation
    NS_LOG_INFO("Running simulation for " << simTime << " seconds");
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    Simulator::Destroy();
    
    NS_LOG_INFO("Simulation completed successfully");
    
    return 0;
}