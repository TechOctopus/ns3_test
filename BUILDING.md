# Building and Running the Vanetza-NS3 Adapter

This document provides detailed instructions for building and running the Vanetza-NS3 adapter for CAM message simulation.

## Prerequisites

Before you begin, ensure you have the following installed on your system:

1. **C++ Compiler** with C++14 support (GCC 5.0+ or Clang 3.4+)
2. **CMake** (version 3.5 or higher)
3. **NS3** (Network Simulator 3)
4. **Vanetza** (ETSI C-ITS protocol implementation)

## Setting Up the Environment

### Installing NS3

If you don't have NS3 installed, follow these steps:

```bash
# Clone NS3 repository
git clone https://gitlab.com/nsnam/ns-3-dev.git
cd ns-3-dev

# Configure and build NS3
./waf configure --enable-examples --enable-tests
./waf build
```

Note the path to your NS3 installation, as you'll need it when building the adapter.

### Installing Vanetza

If you don't have Vanetza installed, follow these steps:

```bash
# Clone Vanetza repository
git clone https://github.com/riebl/vanetza.git
cd vanetza

# Create build directory
mkdir build
cd build

# Configure and build Vanetza
cmake ..
make
```

Note the path to your Vanetza installation, as you'll need it when building the adapter.

## Building the Vanetza-NS3 Adapter

1. Clone the Vanetza-NS3 adapter repository (or use the provided code):

```bash
git clone https://github.com/yourusername/vanetza-ns3-adapter.git
cd vanetza-ns3-adapter
```

2. Create a build directory:

```bash
mkdir build
cd build
```

3. Configure the build with CMake, specifying the paths to NS3 and Vanetza:

```bash
cmake .. -DNS3_DIR=/path/to/ns3 -DVANETZA_DIR=/path/to/vanetza
```

Replace `/path/to/ns3` and `/path/to/vanetza` with the actual paths to your NS3 and Vanetza installations.

4. Build the adapter:

```bash
make
```

5. (Optional) Install the adapter:

```bash
make install
```

## Running the Example Simulation

The repository includes an example simulation that demonstrates the use of the Vanetza-NS3 adapter for CAM message exchange between vehicles.

### Basic Usage

To run the example simulation with default parameters:

```bash
# From the build directory
./examples/cam_simulation_example
```

### Customizing the Simulation

You can customize the simulation using command-line parameters:

```bash
./examples/cam_simulation_example --nVehicles=20 --simTime=200 --roadLength=2000
```

Available parameters:

- `--nVehicles`: Number of vehicles in the simulation (default: 10)
- `--simTime`: Duration of the simulation in seconds (default: 100)
- `--roadLength`: Length of the road in meters (default: 1000)

### Analyzing the Results

The simulation generates PCAP files that capture all the network traffic. You can analyze these files using tools like Wireshark:

```bash
wireshark cam-simulation-0-0.pcap
```

## Troubleshooting

### Common Issues

1. **CMake can't find NS3 or Vanetza**
   
   Ensure you've specified the correct paths to NS3 and Vanetza in your CMake command.

2. **Compilation errors**
   
   Make sure your NS3 and Vanetza installations are compatible with the adapter. The adapter requires NS3 with WAVE module support and Vanetza with CAM facilities.

3. **Runtime errors**
   
   Ensure that the NS3 and Vanetza libraries are in your library path or properly installed.

4. **No CAM messages being transmitted**
   
   Check that the mobility model is properly configured and that the adapter is correctly attached to the network device.

### Getting Help

If you encounter issues not covered in this guide, please open an issue on the GitHub repository or contact the maintainers.

## Advanced Usage

### Integrating with Your Own Simulation

To use the Vanetza-NS3 adapter in your own simulation:

1. Include the necessary headers in your code:

```cpp
#include "adapter/vanetza_ns3_adapter.hpp"
#include "adapter/cam_application.hpp"
```

2. Link against the adapter library when building your simulation:

```bash
g++ -o my_simulation my_simulation.cc -I/path/to/adapter/include -L/path/to/adapter/lib -lvanetza_ns3_adapter
```

3. Follow the example in `examples/cam_simulation_example.cc` to set up the adapter and CAM application in your simulation.

### Customizing CAM Messages

The default implementation uses a simple binary format for CAM messages. To use ETSI-compliant CAM messages, you'll need to modify the `GenerateCam()` and `ReceiveCam()` methods in `src/adapter/cam_application.cpp` to use Vanetza's facilities for proper CAM encoding/decoding.

## Performance Considerations

- For large-scale simulations with many vehicles, consider adjusting the CAM generation interval to reduce network load.
- The adapter is designed to work with NS3's WAVE module, which provides realistic modeling of IEEE 802.11p communication.
- For realistic vehicle mobility patterns, consider using NS3's SUMO integration instead of the simple mobility model used in the example.