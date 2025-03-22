# Vanetza-NS3 Adapter for CAM Message Simulation

This project provides an adapter for integrating Vanetza (an open-source implementation of ETSI C-ITS protocols) with NS3 (Network Simulator 3) to simulate Cooperative Awareness Messages (CAM) in vehicular networks according to ETSI C-ITS standards.

## Overview

The adapter enables the simulation of Vehicle-to-Vehicle (V2V) and Vehicle-to-Infrastructure (V2I) communication using CAM messages within the NS3 simulation environment while leveraging Vanetza's implementation of ETSI C-ITS protocols.

## Project Structure

```
├── src/                    # Source code directory
│   ├── adapter/            # Vanetza-NS3 adapter implementation
│   ├── messages/           # CAM message definitions
│   └── utils/              # Utility functions
├── examples/               # Example simulation scenarios
├── scripts/                # Helper scripts for building and running
└── docs/                   # Documentation
```

## Prerequisites

To use this adapter, you'll need:

- NS3 (Network Simulator 3)
- Vanetza (ETSI C-ITS protocol implementation)
- C++ compiler with C++14 support
- CMake (version 3.5 or higher)

## Building

Detailed build instructions will be provided in the documentation.

## Usage

Example usage scenarios are provided in the `examples/` directory.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Vanetza: https://github.com/riebl/vanetza
- NS3: https://www.nsnam.org/