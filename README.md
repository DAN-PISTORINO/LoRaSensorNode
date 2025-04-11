# LoRaWAN Sensor Network for IoT4Ag
Welcome to the wiki for the LoRaWAN Sensor Network project, part of the IMG initiative at the University of Florida. This project focuses on developing and integrating a low-power, long-range wireless sensor network for precision agriculture applications, specifically for monitoring subterranean soil conditions.

## Project Overview
This project details the design, implementation, and testing of a LoRaWAN-based sensor network integrated with an existing IoT4Ag drone base station and information system. The system utilizes low-power sensor nodes designed for permanent underground installation (target depth 35-45 cm), supported by wireless charging and communication. The nodes collect soil condition data (e.g., moisture, NPK levels) and system health information, transmitting it via a 433 MHz LoRaWAN channel to a central gateway. This data facilitates improved irrigation system efficiency and effectiveness. The research aims to overcome the challenges of through-soil wireless communication for agricultural monitoring.

## System Architecture
* **LoRaWAN Sensor Nodes:** Buried underground to collect soil data.
* **LoRaWAN Gateway:** Receives data from nodes via LoRaWAN and forwards it to the local server.
* **Network & Application Server:** Hosted on a mini-PC (part of the drone port system), responsible for device authentication, data processing, and user interface.

[Placeholder: System Architecture Diagram - showing node, gateway, mini-PC, servers, and data flow]  

## LoRaWAN Node Design
The node is designed for long-term, unattended operation underground, focusing on low power consumption, environmental resilience, and sensor flexibility.

### Hardware
**Microcontroller & LoRa:** STMicroelectronics (STM) NUCLEO-WL55JC2 development board, chosen for integrated LoRaWAN and low power modes. Power consumption is measured at ~5 mA in standby and ~20 mA during transmission.

**Sensors:** Supports various sensors via UART, SPI, I2C, and ADC channels. The prototype includes:
* Analog Capacitive Soil Moisture Sensor (3.3V-5.5V operation).
* Soil NPK Sensor (12V-24V operation).
  
### Power Supply
**Battery:** Uses 3 parallel 18650 Li-ion batteries, totaling 7.8 Ah.

**Power Management:** Utilizes a TI BQ25570EVM-based board for energy harvesting management (wireless charging input) and provides regulated 3.3 V power rail, 3.7 V to 4.2 V charging rail, and 13.5 V power rail. The 13.5 V power rail is a boost converter designed around the TI TPS61288 controller.

**Wireless Charging Input:** Designed to interface with the prototype Electromotive Wireless Power Transfer (eWPT) system via a 3-phase AC full-bridge rectifier.

**Antenna:** LPRS company antenna connected to the Semtech SX1276MB1LAS transceiver on the NUCLEO board.

**Enclosure:** Custom-designed, 3D-printed PLA box with a gasket seal for environmental protection. Includes openings for sensors and antenna.   

![Placeholder: Image of Assembled Node PCB]  
![Placeholder: Image of Node Power Tree Diagram similar to Figure 2 in LoRaWAN Sensor Network IoT4Ag Integration.docx]  
![Placeholder: Image of 3D Model of Node Enclosure similar to Figure 3 in LoRaWAN Sensor Network IoT4Ag Integration.docx]  

### Operation
* Collects data from connected sensors (e.g., moisture, NPK).
* Transmits data packets periodically (e.g., every 30 seconds for testing) using LoRaWAN protocol on the 433 MHz band. Transmission utilizes Chirp Spread Spectrum (CSS) modulation, with Spreading Factor (SF) 12 used in tests for maximum range.
* Receives acknowledgements from the gateway on a 434 MHz channel.

### FCC Considerations
Operation in the 433/434 MHz spectrum falls under FCC regulations (Title 47 CFR Part 15).
Analysis suggests that underground transmission with expected soil path loss keeps radiated power below FCC limits for periodic operation.

## LoRaWAN Gateway Design
The gateway acts as the bridge between the sensor nodes and the network server.

### Hardware
* STM NUCLEO-F746ZG controller board.
* RisingHF LRWAN_GS_HF1 LoRa riser module (based on Semtech SX1301/SX1257).
* LPRS ANT-SS450-510 Antenna.

### Functionality
* Receives LoRaWAN packets from nodes on the 433 MHz channel.
* Measures signal strength (RSSI) and quality (LSNR).
* Sends acknowledgement (ACK) packets back to nodes on the 434 MHz channel.
* Forwards received node data to the mini-PC via UART. Connects to the network server via UDP over Ethernet.
[Placeholder: Image of Gateway Hardware Setup]  

## Network & Application Server
Hosted on a mini-PC, these servers manage the LoRaWAN network and process the collected data.

### Network Server
* **Software:** Uses the open-source GothardP LoRaWAN server.
* **Functionality:** Handles device authentication, manages network sessions, and forwards data packets via MQTT.
* **Configuration Challenges:** Required careful setup of IP addresses, channel configurations (bandwidth, frequency), device identifiers (EUI, MACs), and join delays (RX1/RX2) to establish reliable node communication.

### Application Server
* **Node-Red:** a flow-based development tool.
* **Dashboard 2.0:** a Node-Red module for creating user interfaces.
* **mySQL:** a database for storing signal logs.

### Functionality
* Receives data from the network server via an MQTT broker.
* Processes and formats the sensor data using JavaScript functions within nodes.
* Stores data in a MySQL database.
* Provides a graphical user interface (GUI) dashboard created with Node-Red dashboard nodes for visualizing data (e.g., charts, gauges) and controlling logging.

![Placeholder: Screenshot of Network Server Configuration UI]  
![Placeholder: Screenshot of Node-Red Application Server Flow]  
![Placeholder: Screenshot of Node-Red Dashboard UI]  

# Through-Soil Communication Study
An experimental study was conducted to assess the LoRa communication range at 433 MHz, specifically focusing on through-soil transmission under varying moisture conditions.

## Experiment Setup
### Environments
* **Above Ground:** Node and gateway placed on the ground surface.
* **Subterranean:** Node buried with antenna at 45 cm depth (35 cm soil + 10 cm air gap in enclosure), gateway on surface.
* **Equipment:** LoRa Node (STM NUCLEO-WL55JC2), LoRa Gateway (STM NUCLEO-F746ZG + RisingHF), Mini-PC with Network/Application Servers, Power Supplies, Measuring Tape.
* **Data Logging:** Node-Red application server logged timestamp, frequency, RSSI, LSNR, and user-input distance for each packet.

![Placeholder: Image of Above Ground Experiment Setup similar to Figure 2 in LoRa Node Profile.docx]  
![Placeholder: Image of Subterranean Experiment Setup similar to Figure 3 in LoRa Node Profile.docx]  

## Methodology
* **Setup:** Configure network, application server, gateway, and node.
* **Above Ground Test (Trial 1):** Place node and gateway on the ground. Move gateway to measured distances, log data (RSSI, LSNR) for 5 minutes at each distance.
* **Subterranean Test (Trials 1 & 2):** Bury node (antenna at 45 cm depth). Place gateway on the surface at measured distances, log data for 5 minutes at each distance.

### Trials
* **Trial 1 (Wet):** Conducted after significant rainfall (4.7 cm, 92% humidity) to assess worst-case range. Included both above-ground and subterranean tests.
* **Trial 2 (Dry):** Conducted on a dry day (no recent rain, 77% humidity) to assess maximum range. Subterranean test only.

## Results & Discussion
* **Soil Attenuation:** Comparing subterranean vs. above-ground (Trial 1), soil introduced significant loss (~17-29 dB at shorter distances), less than reported in other studies (~100 dB), highlighting the impact of specific soil properties. Attenuation decreased at longer distances, suggesting complex propagation effects like surface waves.
* **Impact of Moisture:** Range was dramatically affected.
* **Wet Conditions (Trial 1):** Max range ~90 m (RSSI -111 dBm).
* **Dry Conditions (Trial 2):** Max range >200 m (up to 236 m, RSSI -113 dBm).
* **Link Quality:** LoRa demonstrated robustness, decoding packets at very low RSSI (near -114 dBm) and negative LSNR (below -20 dB). RSSI and LSNR showed variability not solely dependent on distance, likely due to multipath fading and interference. Dead zones were observed at longer ranges in dry conditions.

## Conclusion
LoRa is viable for buried sensors, but soil moisture is the dominant factor limiting range. Network planning (gateway density) must account for worst-case (wet) conditions.

![Placeholder: Plot of Trial 1 Metrics similar to Figure 4 in LoRa Node Profile.docx]  
![Placeholder: Plot of Trial 1 Subterranean Metrics similar to Figure 5 in LoRa Node Profile.docx]  
![Placeholder: Plot of Trial 2 Metrics similar to Figure 6 in LoRa Node Profile.docx]  
![Placeholder: Plot Comparing Above Ground vs. Subterranean RSSI similar to Figure 7 in LoRa Node Profile.docx]  

## Limitations & Future Work
**Limitations:** Single location/soil type, qualitative moisture assessment, single burial depth, potential RF interference not monitored, antenna specs slightly off-frequency.

**Future Work:** Test diverse soil types/moisture levels, different depths/orientations, quantify variability, long-term studies, compare LoRa parameters (e.g., different SF), investigate mitigation techniques. Consider a relay layer using irrigation pivots or drones.

## Repository Information
Source Code: [https://github.com/DAN-PISTORINO/LoRaWAN-Drone-Port.git](https://github.com/DAN-PISTORINO/LoRaWAN-Drone-Port.git)   
  
## References
