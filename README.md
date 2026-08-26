# H2 — BLE Device Tracker

Sniff and track Bluetooth Low Energy advertisements to monitor nearby devices.

## Overview

This project implements a BLE advertisement sniffer that:
- Scans for all nearby BLE devices
- Tracks device movement by RSSI
- Identifies device types (phones, headphones, keyboards, etc.)
- Logs appearance changes and activity
- Exports tracking data as CSV

**WARNING: Educational use only. Respect privacy laws.**

## Hardware

| Component | Connection | Role |
|-----------|------------|------|
| ESP32-C6 | Main board | BLE radio + analysis |

## Serial Commands

```
scan      - Start BLE scan (10 seconds)
stop      - Stop scanning
devices   - List tracked devices
info N    - Show details for device N
track N   - Track device N (RSSI monitoring)
export    - Export tracking data as CSV
clear     - Clear all tracked devices
help      - Show commands
```

## Device Types

| Appearance | Type |
|------------|------|
| 0x0100 | Phone |
| 0x0103 | Smartphone |
| 0x0201 | Headset |
| 0x0205 | Headphones |
| 0x0307 | Keyboard |
| 0x0308 | Mouse |
| 0x0701 | Watch |
| 0x0705 | Glasses |

## Example Session

```
=== H2 — BLE Device Tracker ===
BLE initialized

[NEW DEVICE] AA:BB:CC:DD:EE:FF
  Name: iPhone
  RSSI: -45 dBm
  Type: Smartphone

[NEW DEVICE] 11:22:33:44:55:66
  Name: AirPods
  RSSI: -62 dBm
  Type: Headphones

=== Tracked BLE Devices ===
[  0] iPhone               | RSSI: -45 | Smartphone | ACTIVE
[  1] AirPods              | RSSI: -62 | Headphones | ACTIVE
==========================
```

## Build & Flash

```bash
arduino-cli compile --fqbn esp32:esp32:esp32c6 h2_ble_tracker
arduino-cli upload --fqbn esp32:esp32:esp32c6 --port /dev/ttyACM0 h2_ble_tracker
```

## Research Value

- **H2 — BLE Device Tracker**: This project
- **H8 — BLE MITM Proxy**: Extend to GATT characteristic manipulation
- **W9 — BLE Spoofer**: Clone BLE device identities
- **AI5 — Deepfake Detector**: Analyze BLE fingerprint spoofing

## References

- Bluetooth Low Energy Specification
- ESP32-C6 BLE Documentation
- BLE Advertisement Types

## License

MIT
