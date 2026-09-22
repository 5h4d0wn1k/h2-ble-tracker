> **⚠️ EDUCATIONAL USE ONLY — AUTHORIZED TESTING ONLY.**
> This project exists for education, research, and **defense of systems you own
> or hold explicit written authorization to assess**. Unauthorized use is
> prohibited and may be illegal. Read [ETHICS.md](ETHICS.md) and
> [SCOPE.md](SCOPE.md) before use. Use at your own risk; **AS IS**, no warranty.

# H2 — BLE Device Tracker

An ESP32-C6 **Bluetooth Low Energy (BLE)** sniffer and tracker: scan nearby
advertisements, fingerprint device types, monitor signal strength (RSSI), and
export movement data as CSV — for authorized **wireless security** labs.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Stars](https://img.shields.io/github/stars/5h4d0wn1k/h2-ble-tracker)](https://github.com/5h4d0wn1k/h2-ble-tracker)
[![Last commit](https://img.shields.io/github/last-commit/5h4d0wn1k/h2-ble-tracker)](https://github.com/5h4d0wn1k/h2-ble-tracker)
[![Issues](https://img.shields.io/github/issues/5h4d0wn1k/h2-ble-tracker)](https://github.com/5h4d0wn1k/h2-ble-tracker)

## Why H2

BLE broadcasts make every phone, watch, and tag publicly discoverable — and
their advertisement payloads reveal device class and presence. H2 is a
**wireless/Bluetooth security** study device: it scans BLE advertisements,
classifies device types from `appearance` codes, tracks RSSI changes, and
exports sightings. It exists to understand device fingerprinting and proximity
tracking so defenders can measure their own exposure. Track only devices you
own or are authorized to observe, and respect privacy laws.

## Features

- **BLE advertisement scanning** — configurable scan duration, RSSI threshold filtering
- **Device type classification** — `appearance` codes → phone, headset, keyboard, mouse, watch, glasses
- **RSSI movement tracking** — log and follow signal strength changes over time
- **Activity logging** — first-seen/last-seen, packet counts, active status
- **CSV export** — sightings for offline analysis
- **Serial command console** — `scan`, `stop`, `devices`, `info N`, `track N`, `export`, `clear`, `help`
- **Host helper** — offline analysis of captured sighting logs (`host/h2_cli.py`)

## Quickstart

```bash
# ESP32-C6 firmware
arduino-cli compile --fqbn esp32:esp32:esp32c6 firmware/h2_ble_tracker
arduino-cli upload --fqbn esp32:esp32:esp32c6 --port /dev/ttyACM0 firmware/h2_ble_tracker

# Host-side offline analysis
python3 host/h2_cli.py --demo
python3 host/h2_cli.py --file fixtures/sightings.log

# Tests
python3 -m unittest discover -s tests
```

## Examples

- `fixtures/sightings.log` — sample BLE sightings for offline demos

## Project structure

- `firmware/h2_ble_tracker/` — ESP32-C6 Arduino sketch
- `host/` — Python helpers (`h2_cli.py`, `hw_common.py`)
- `fixtures/`, `docs/`, `tests/` — sample data, architecture notes, unit tests

## Documentation

- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) — firmware ⇄ host architecture
- [firmware/README.md](firmware/README.md) — build and flash

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) and [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

## License

MIT — see [LICENSE](LICENSE).

## Legal

- [ETHICS.md](ETHICS.md) · [SCOPE.md](SCOPE.md) · [SECURITY.md](SECURITY.md)