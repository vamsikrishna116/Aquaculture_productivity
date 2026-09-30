# Real-Time IoT Monitoring System for Enhancing Aquaculture Productivity

An ESP32-based IoT system that monitors fish-pond water conditions (**temperature, pH, turbidity and water level**), sends the data to the cloud and a web server, raises alerts when values cross safe limits, and lets you **switch a water pump and a cooling fan remotely** through relays.


## Problem

Traditional aquaculture relies on manual, periodic checking of water parameters. Harmful changes are noticed late, labour is high, and fish mortality risk increases. This project provides continuous monitoring, remote access, alerts and basic automation.

## Features

- Real-time monitoring of temperature, pH, turbidity and water level
- Mobile dashboard and remote relay control using the **Blynk** app
- Web dashboard (PHP + HTML) with data logging, SAFE / UNSAFE status per parameter, and CSV download
- Instant alerts and notifications when a parameter exceeds its threshold
- Relay control of a **water pump** and **cooling fan** (2-channel 5V relay module)

## System Architecture


1. Sensors (pH, turbidity, temperature, water level) send readings to the ESP32.
2. The ESP32 processes the data and sends it over Wi-Fi to the Blynk cloud and to a web server (HTTP POST).
3. Users monitor the data and receive alerts on the Blynk app or the web dashboard.
4. Control signals from the ESP32 drive a 2-channel relay module that switches the water pump and cooling fan.

## Hardware Required

| Component | Purpose |
|---|---|
| ESP32 microcontroller | Main controller with Wi-Fi |
| pH sensor (with signal module) | Water pH |
| Turbidity sensor | Water clarity |
| Temperature sensor | Water temperature |
| Water level sensor | Pond / tank level |
| 2-channel relay module (5V) | Switches pump and fan |
| Water pump | Maintains water level |
| Cooling fan | Regulates temperature |
| 5V power adapter, connecting wires | Power and wiring |

## Software Required

- Arduino IDE with ESP32 board support
- Blynk (mobile app and Blynk Cloud)
- Web server with PHP and a database (for data logging and the web dashboard)

## Setup

1. **Wire the hardware** (see the prototype photo above and add your pin table / circuit diagram, see *Documentation* below).
2. **Blynk:** create a template and device, add widgets for Turbidity, pH, Water Level and Temperature, plus two switches (`Fan ctl`, `Motor ctl`), and set notification events for the thresholds.
3. **Web server:** upload the PHP files from `server/`, create the database table, and set your credentials in `config.php` (not uploaded to GitHub).
4. **Firmware:** copy `firmware/secrets_example.h` to `secrets.h`, fill in your Wi-Fi, Blynk token and server URL, then upload the sketch to the ESP32.
5. Power the system and open the Blynk app or the web dashboard.

## How It Works

1. **Sensor data acquisition:** sensors read pond conditions in real time.
2. **Processing:** the ESP32 reads and formats the values.
3. **Wireless transmission:** data goes to Blynk and to the web server over Wi-Fi.
4. **Control action:** the pump and fan relays are switched from the Blynk app based on the readings.
5. **Storage and visualization:** the server stores every reading and shows logs with SAFE / UNSAFE classification (threshold-based).
6. **Alerts:** notifications are sent when a parameter goes beyond its safe limit.

## Results

### Blynk mobile app

Live turbidity, pH, water level and temperature, with switches for the fan and motor (pump).


### Web dashboard

Timestamped data log with a SAFE / UNSAFE condition for each parameter and CSV export.



> The pH readings in these screenshots (about 2.8 to 4.2) come from a test liquid and are flagged UNSAFE, which shows the threshold alert working. They are not readings from a calibrated fish pond.

## Test Cases

| Test case | Input | Expected output | Result |
|---|---|---|---|
| Sensor data reading | Valid sensor values from ESP32 | Correct values displayed | Pass |
| Data transmission | ESP32 sends data via Wi-Fi | Data reaches server without loss | Pass |
| Blynk monitoring | Real-time sensor input | Values update on mobile app | Pass |
| Relay operation | Motor relay switched from app | Water pump turns ON | Pass |
| Relay operation | Fan relay switched from app | Cooling fan turns ON | Pass |
| Alert system | Parameter exceeds threshold | Notification triggered | Pass |
| Web interface | Data sent to server | Data logged and shown in dashboard | Pass |
| System integration | Continuous operation | Stable performance without errors | Pass |

## Applications

Fish farming ponds, aquariums, aquaponics, research laboratories, and environmental monitoring of lakes and water bodies.

## Limitations and Future Work

- Pump and fan are switched manually from the app; fully automatic threshold-based switching can be added.
- SAFE / UNSAFE is a simple threshold check, not a trained prediction model.
- Sensors need regular calibration (especially pH) and waterproofing for long-term field use.
- Possible upgrades: solar power, dissolved-oxygen sensor, ML-based prediction, SMS alerts.
