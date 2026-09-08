#!/usr/bin/env python3
"""H2 - BLE Device Tracker host helper: offline BLE sighting analysis.
Educational/authorized own-lab use only (see README "IMPORTANT").
"""
import argparse
import os
import re
import sys

MOD = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, MOD)
from hw_common import DEMO_TAG, read_target

SIGHT_RE = re.compile(r"([0-9A-Fa-f:]{17})\s+(-?\d+)\s+([\w /-]+)")


def analyze(text):
    devices = {}
    for m in SIGHT_RE.finditer(text):
        mac, rssi, kind = m.group(1).upper(), int(m.group(2)), m.group(3).strip()
        d = devices.setdefault(mac, {"mac": mac, "rssi": [], "kind": kind})
        d["rssi"].append(rssi)
    out = []
    for d in sorted(devices.values(), key=lambda x: x["mac"]):
        out.append({
            "mac": d["mac"], "kind": d["kind"],
            "max": max(d["rssi"]), "min": min(d["rssi"]),
            "avg": round(sum(d["rssi"]) / len(d["rssi"]), 1),
            "count": len(d["rssi"]),
        })
    return out


def run_demo():
    print("=== H2 BLE sighting analysis (offline) ===")
    text = read_target("fixtures/sightings.log",
                       "00:11:22:33:44:55 -45 Smartphone\n"
                       "00:11:22:33:44:55 -42 Smartphone\n"
                       "00:11:22:33:44:66 -62 Headphones\n")
    for d in analyze(text):
        print("  %s  min=%s dBm max=%s dBm avg=%s dBm (%d sightings)"
              % (d["mac"], d["min"], d["max"], d["avg"], d["count"]))
    print(DEMO_TAG)
    return 0


def main(argv=None):
    p = argparse.ArgumentParser(
        description="H2 BLE Tracker - offline BLE sighting analysis")
    p.add_argument("--demo", action="store_true", help="run offline demo (exit 0)")
    p.add_argument("--file", help="sightings log path")
    args = p.parse_args(argv)
    if args.demo or not args.file:
        return run_demo()
    text = open(args.file).read() if os.path.exists(args.file) else ""
    for d in analyze(text):
        print(d)
    return 0


if __name__ == "__main__":
    sys.exit(main())
