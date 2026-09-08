# BLE Device Tracker Firmware

## Purpose

Sniff and track Bluetooth Low Energy advertisements, identify device types, and export RSSI tracking data.

## Board

- **Board**: ESP32-C6
- **FQBN**: `esp32:esp32:esp32c6`
- **Sketch**: `h2_ble_tracker/h2_ble_tracker.ino`

## Wiring

```
Standalone ESP32-C6 (BLE antenna built-in). USB-C for serial/power.
```

## Build

```bash
arduino-cli compile --fqbn esp32:esp32:esp32c6 firmware/h2_ble_tracker
# upload (example, ESP32-C6):
# arduino-cli upload --fqbn esp32:esp32:esp32c6 --port /dev/ttyACM0 firmware/h2_ble_tracker
```

## Runtime

See the root README "IMPORTANT" section before powering on. This firmware is
for authorized own-lab study. Serial console exposes the interactive command
set described in the root README. All identifiers in the sketch are
placeholders (`lab-*` SSIDs, `00:11:22:33:44:55`, RFC 5737 / example.com).
