# Smart Wireless Sensor Network (WSN) Parking Lot
## Overview
This repository is a collection of research, hardware implementation and software implementation for a smart parking lot that displays the vacancy status of each parking space. The scope of the project was limitted to ten spaces and is meant to be a proof of concept to satisfy the final project requirement for CMPE-684: Wireless Sensor Networks at the University of Maryland, Baltimore County.

## Hardware
All PCB design files were created using KiCad and all stl files for 3D prints were developed using Autodesk TinyCAD. The filament used for the 3D prints is PLA.

For electronics, the project truly revolves around the Arduino Nano. All of the sensor nodes and the base station utilize this microcontroller. Time of Flight (ToF) sensors are used to detect if a vehicle is present and the NRF24L01 radio is used for network communication between sensor nodes and the base station.

The output of the base station is composite video with a limitted display resolution. However, the final resolution seems to scale well with most display scalers.

## Software
The Arduino IDE was used for the research examples. However, PlatformIO was used for the actual implementation since it offered superior project structure and organization.
