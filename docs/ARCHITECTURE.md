# H2 — BLE Device Tracker Architecture

## System Overview

The H2 BLE Device Tracker scans for Bluetooth Low Energy advertisements and tracks nearby devices by monitoring RSSI (signal strength) over time.

## Tracking Flow

```
┌─────────────────┐
│  BLE Scanner    │
│  (Advertisements)│
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Device Filter  │
│  (RSSI > -80)   │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Device Tracker │
│  (Address/Name) │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  RSSI Analysis  │
│  (Distance Est.)│
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Data Export    │
│  (CSV/Serial)   │
└─────────────────┘
```

## BLE Advertisement Structure

### Packet Format

```
Preamble (1-2 bytes)
Access Address (4 bytes, always 0x8E89BED6)
PDU (Protocol Data Unit)
  - Header (2 bytes)
  - Advertising Address (6 bytes)
  - Data (0-31 bytes)
CRC (3 bytes)
```

### Advertisement Types

| Type | Name | Use Case |
|------|------|----------|
| 0x00 | ADV_IND | Connectable undirected |
| 0x01 | ADV_DIRECT_IND | Connectable directed |
| 0x02 | ADV_NONCONN_IND | Non-connectable |
| 0x03 | SCAN_RSP | Scan response |
| 0x04 | ADV_DIRECT_IND | Low duty cycle |

### Advertisement Data Fields

| Type | Length | Description |
|------|--------|-------------|
| 0x01 | 1 | Flags |
| 0x02-0x03 | 1+ | Incomplete list of 16-bit service UUIDs |
| 0x08-0x09 | 1+ | Shortened/Complete local name |
| 0x0A | 1 | TX power level |
| 0x0D | 1 | Class of device |
| 0xFF | 1+ | Manufacturer specific data |

## Device Identification

### By Appearance

The appearance field identifies device type:
- 0x0000: Unknown
- 0x0100-0x01FF: Phones
- 0x0200-0x02FF: Audio
- 0x0300-0x03FF: Peripherals
- 0x0700-0x07FF: Wearables

### By Manufacturer Data

Many vendors embed identifiers:
- Apple: 0x004C (AirDrop, AirPods)
- Google: 0x00E0 (Fast Pair)
- Microsoft: 0x0006 (Swift Pair)

## RSSI-Based Distance Estimation

### Path Loss Model

```
RSSI = -10 * n * log10(d) + A

Where:
  RSSI = Received Signal Strength Indicator
  n = Path loss exponent (2-4)
  d = Distance in meters
  A = RSSI at 1 meter (typically -69 dBm)
```

### Distance Calculation

```
d = 10 ^ ((A - RSSI) / (10 * n))

Example:
  A = -69 dBm (calibrated)
  RSSI = -45 dBm
  n = 2 (free space)
  
  d = 10 ^ ((-69 - (-45)) / (10 * 2))
  d = 10 ^ (-24 / 20)
  d = 10 ^ (-1.2)
  d = 0.063 meters (~6 cm)
```

### RSSI Variability

RSSI fluctuates due to:
- Multipath interference
- Body absorption
- Antenna orientation
- Environmental changes

**Mitigation**: Average multiple readings, apply Kalman filter.

## Tracking Applications

### Indoor Positioning

Track device movement through a space:
1. Place multiple BLE scanners
2. Triangulate position from RSSI
3. Build movement heatmap

### Attendance Monitoring

Track who is present:
1. Scan for personal devices (phones, watches)
2. Log entry/exit times
3. Generate attendance reports

### Asset Tracking

Monitor valuable equipment:
1. Attach BLE tags to assets
2. Track location changes
3. Alert on unauthorized movement

## Privacy Considerations

### Legal Requirements

- **GDPR (EU)**: Requires consent for tracking
- **CCPA (California)**: Right to know about data collection
- **BIPA (Illinois)**: Biometric data protection

### Ethical Guidelines

1. **Transparency**: Inform users of tracking
2. **Consent**: Obtain permission where required
3. **Anonymization**: Remove personally identifiable information
4. **Retention**: Limit data storage duration
5. **Security**: Protect collected data

## Defensive Countermeasures

### For Device Owners

1. **Disable BLE** when not in use
2. **Use random addresses** (iOS/Android support)
3. **Limit advertisement data**
4. **Enable privacy features**

### For Network Administrators

1. **BLE intrusion detection**: Monitor for tracking
2. **Rogue device detection**: Identify unauthorized BLE devices
3. **Signal jamming**: Disrupt tracking (where legal)
4. **Physical security**: Control physical access

## Integration with Other Projects

```
H2 (BLE Tracker) ──feeds──▶ H8 (BLE MITM)
     │
     ▼
W9 (BLE Spoofer) ◀──uses── H2 data
     │
     ▼
I1 (Firmware Extract) ──targets── BLE devices
```

## References

- Bluetooth Core Specification v5.3
- ESP32-C6 BLE Documentation
- BLE Security Analysis
- RSSI-Based Localization
