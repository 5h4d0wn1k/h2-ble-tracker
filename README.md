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

## Legal Disclaimer

**IMPORTANT: Read before use.**

This project is provided for **educational and authorized security testing purposes only**. 

### Authorization Requirements
- You MUST have explicit written permission from the network owner before using this tool
- Unauthorized interception of network communications is illegal under federal and state laws
- This tool should ONLY be used on networks you own or have written authorization to test

### Legal Framework
- **Computer Fraud and Abuse Act (CFAA)**: Unauthorized access to computer systems is a federal crime
- **Wiretap Act (18 U.S.C. § 2511)**: Interception of electronic communications without consent is illegal
- **State Laws**: Many states have additional computer crime and wiretapping statutes
- **GDPR/CCPA**: Data collection may be subject to privacy regulations

### Acceptable Use
- Testing security of your own networks
- Authorized penetration testing with written scope
- Academic research in controlled lab environments
- Security education and training

### Prohibited Use
- Intercepting communications on networks you do not own
- Attacking infrastructure without authorization
- Any activity that violates applicable laws or regulations
- Commercial use without proper licensing

### No Warranty
This software is provided "AS IS" without warranty of any kind. The author is not responsible for any misuse or damage caused by this software.

### Responsible Disclosure
If you discover vulnerabilities using this tool, follow responsible disclosure practices:
1. Report to the vendor/owner privately
2. Allow reasonable time for remediation
3. Do not exploit beyond proof of concept
