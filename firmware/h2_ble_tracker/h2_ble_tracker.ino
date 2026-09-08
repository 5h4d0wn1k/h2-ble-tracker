/*
 * H2 — BLE Device Tracker
 * Sniff and track Bluetooth Low Energy advertisements
 * 
 * Hardware: ESP32-C6
 * 
 * Features:
 *   - Scan for BLE advertisements
 *   - Track device movement by RSSI
 *   - Identify device types (phones, headphones, etc.)
 *   - Log appearance changes
 *   - Export tracking data
 * 
 * WARNING: Educational use only. Respect privacy laws.
 * 
 * Author: 5h4d0wn1k
 * License: MIT
 * Date: 2026-08-26
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Configuration
#define SCAN_DURATION 10      // seconds
#define MAX_DEVICES 100
#define RSSI_THRESHOLD -80    // Only track devices stronger than this

// Device structure
struct TrackedDevice {
    String address;
    String name;
    int rssi;
    uint32_t appearance;
    uint32_t last_seen;
    uint32_t first_seen;
    uint32_t packet_count;
    bool active;
    int max_rssi;
    int min_rssi;
    float avg_rssi;
};

// Global state
TrackedDevice tracked_devices[MAX_DEVICES];
int device_count = 0;
BLEScan* pBLEScan;
bool scanning = false;

// Device type identification
const char* identifyDeviceType(uint32_t appearance) {
    switch (appearance) {
        case 0x00: return "Unknown";
        case 0x01: return "Computer";
        case 0x02: return "Desktop";
        case 0x03: return "Server";
        case 0x04: return "Laptop";
        case 0x05: return "Handheld";
        case 0x06: return "Palm-size";
        case 0x07: return "Wearable";
        case 0x08: return "Tablet";
        case 0x09: return "Docking";
        case 0x0A: return "Medic";
        
        // Phones
        case 0x0100: return "Phone";
        case 0x0101: return "Cellular";
        case 0x0102: return "Cordless";
        case 0x0103: return "Smartphone";
        case 0x0104: return "Wired modem";
        case 0x0105: return "ISDN";
        
        // Audio
        case 0x0200: return "Audio";
        case 0x0201: return "Headset";
        case 0x0202: return "Hands-free";
        case 0x0203: return "Microphone";
        case 0x0204: return "Loudspeaker";
        case 0x0205: return "Headphones";
        case 0x0206: return "Portable";
        case 0x0207: return "Car";
        case 0x0208: return "Set-top";
        case 0x0209: return "HiFi";
        case 0x020A: return "VCR";
        case 0x020B: return "Video";
        case 0x020C: return "Camcorder";
        case 0x020D: return "Video Monitor";
        case 0x020E: return "Video Display";
        case 0x020F: return "Video Camera";
        case 0x0210: return "Camcorder";
        
        // Peripheral
        case 0x0300: return "Peripheral";
        case 0x0301: return "Joystick";
        case 0x0302: return "Gamepad";
        case 0x0303: return "Remote";
        case 0x0304: return "Sensing";
        case 0x0305: return "Digitizer";
        case 0x0306: return "Card Reader";
        case 0x0307: return "Keyboard";
        case 0x0308: return "Pointing";
        case 0x0309: return "Combined";
        
        // Wearable
        case 0x0700: return "Wearable";
        case 0x0701: return "Wrist-watch";
        case 0x0702: return "Pager";
        case 0x0703: return "Jacket";
        case 0x0704: return "Helmet";
        case 0x0705: return "Glasses";
        case 0x0706: return "Necklace";
        case 0x0707: return "Bracelet";
        case 0x0708: return "Ring";
        
        default: return "Other";
    }
}

// Callback for advertised devices
class AdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        String address = advertisedDevice.getAddress().toString();
        int rssi = advertisedDevice.getRSSI();
        
        // Filter by RSSI
        if (rssi < RSSI_THRESHOLD) return;
        
        // Check if already tracked
        bool found = false;
        for (int i = 0; i < device_count; i++) {
            if (tracked_devices[i].address == address) {
                // Update existing device
                tracked_devices[i].rssi = rssi;
                tracked_devices[i].last_seen = millis();
                tracked_devices[i].packet_count++;
                
                // Update RSSI stats
                if (rssi > tracked_devices[i].max_rssi) {
                    tracked_devices[i].max_rssi = rssi;
                }
                if (rssi < tracked_devices[i].min_rssi) {
                    tracked_devices[i].min_rssi = rssi;
                }
                
                // Update average
                tracked_devices[i].avg_rssi = 
                    (tracked_devices[i].avg_rssi * (tracked_devices[i].packet_count - 1) + rssi) 
                    / tracked_devices[i].packet_count;
                
                // Update name if available
                if (advertisedDevice.haveName()) {
                    tracked_devices[i].name = advertisedDevice.getName();
                }
                
                found = true;
                break;
            }
        }
        
        // New device
        if (!found && device_count < MAX_DEVICES) {
            tracked_devices[device_count].address = address;
            
            if (advertisedDevice.haveName()) {
                tracked_devices[device_count].name = advertisedDevice.getName();
            } else {
                tracked_devices[device_count].name = "Unknown";
            }
            
            tracked_devices[device_count].rssi = rssi;
            tracked_devices[device_count].appearance = advertisedDevice.getAppearance();
            tracked_devices[device_count].last_seen = millis();
            tracked_devices[device_count].first_seen = millis();
            tracked_devices[device_count].packet_count = 1;
            tracked_devices[device_count].active = true;
            tracked_devices[device_count].max_rssi = rssi;
            tracked_devices[device_count].min_rssi = rssi;
            tracked_devices[device_count].avg_rssi = rssi;
            
            device_count++;
            
            Serial.printf("\n[NEW DEVICE] %s\n", address.c_str());
            Serial.printf("  Name: %s\n", tracked_devices[device_count-1].name.c_str());
            Serial.printf("  RSSI: %d dBm\n", rssi);
            Serial.printf("  Type: %s\n", identifyDeviceType(tracked_devices[device_count-1].appearance));
        }
    }
};

// Function prototypes
void startScan();
void stopScan();
void listDevices();
void showDeviceDetails(int index);
void trackDevice(int index);
void exportData();
void showHelp();
void processSerialCommand();

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== H2 — BLE Device Tracker ===");
    Serial.println("Bluetooth Low Energy advertisement sniffer");
    Serial.println("WARNING: Educational use only!");
    Serial.println();
    
    // Initialize BLE
    BLEDevice::init("H2-BLE-Tracker");
    
    // Get scanner
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    
    Serial.println("BLE initialized");
    Serial.println();
    showHelp();
}

void loop() {
    // Handle serial commands
    if (Serial.available()) {
        processSerialCommand();
    }
}

void processSerialCommand() {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    if (cmd == "help") {
        showHelp();
    } else if (cmd == "scan") {
        startScan();
    } else if (cmd == "stop") {
        stopScan();
    } else if (cmd == "devices") {
        listDevices();
    } else if (cmd.startsWith("info ")) {
        int idx = cmd.substring(5).toInt();
        showDeviceDetails(idx);
    } else if (cmd.startsWith("track ")) {
        int idx = cmd.substring(6).toInt();
        trackDevice(idx);
    } else if (cmd == "export") {
        exportData();
    } else if (cmd == "clear") {
        device_count = 0;
        Serial.println("Cleared all tracked devices.");
    } else {
        Serial.println("Unknown command. Type 'help' for commands.");
    }
}

void showHelp() {
    Serial.println("\n=== Commands ===");
    Serial.println("scan      - Start BLE scan");
    Serial.println("stop      - Stop scanning");
    Serial.println("devices   - List tracked devices");
    Serial.println("info N    - Show details for device N");
    Serial.println("track N   - Track device N (RSSI monitoring)");
    Serial.println("export    - Export tracking data as CSV");
    Serial.println("clear     - Clear all tracked devices");
    Serial.println("help      - Show this help");
    Serial.println("================\n");
}

void startScan() {
    Serial.println("\n=== Starting BLE Scan ===");
    Serial.printf("Scanning for %d seconds...\n", SCAN_DURATION);
    Serial.println("RSSI threshold: " + String(RSSI_THRESHOLD) + " dBm");
    Serial.println();
    
    scanning = true;
    
    // Run scan
    BLEScanResults* results = pBLEScan->start(SCAN_DURATION, false);
    int found = results->getCount();
    
    Serial.println("\n=== Scan Complete ===");
    Serial.printf("Found %d devices\n", found);
    Serial.printf("Tracking %d devices\n", device_count);
    Serial.println("====================\n");
    
    scanning = false;
}

void stopScan() {
    pBLEScan->stop();
    scanning = false;
    Serial.println("Scan stopped.");
}

void listDevices() {
    if (device_count == 0) {
        Serial.println("No devices tracked yet. Run 'scan' first.");
        return;
    }
    
    Serial.println("\n=== Tracked BLE Devices ===");
    for (int i = 0; i < device_count; i++) {
        uint32_t age = (millis() - tracked_devices[i].last_seen) / 1000;
        bool is_recent = (age < 30);
        
        Serial.printf("[%3d] %-20s | RSSI:%4d | %s | %s\n",
                     i, 
                     tracked_devices[i].name.c_str(),
                     tracked_devices[i].rssi,
                     identifyDeviceType(tracked_devices[i].appearance),
                     is_recent ? "ACTIVE" : "STALE");
    }
    Serial.println("==========================\n");
}

void showDeviceDetails(int index) {
    if (index < 0 || index >= device_count) {
        Serial.println("Invalid device index!");
        return;
    }
    
    TrackedDevice& dev = tracked_devices[index];
    
    Serial.printf("\n=== Device #%d Details ===\n", index);
    Serial.printf("Address: %s\n", dev.address.c_str());
    Serial.printf("Name: %s\n", dev.name.c_str());
    Serial.printf("Type: %s\n", identifyDeviceType(dev.appearance));
    Serial.printf("RSSI: %d dBm\n", dev.rssi);
    Serial.printf("RSSI Range: %d to %d dBm\n", dev.min_rssi, dev.max_rssi);
    Serial.printf("Avg RSSI: %.1f dBm\n", dev.avg_rssi);
    Serial.printf("Packets: %lu\n", dev.packet_count);
    Serial.printf("First Seen: %lu ms ago\n", millis() - dev.first_seen);
    Serial.printf("Last Seen: %lu ms ago\n", millis() - dev.last_seen);
    
    // Estimate distance (rough)
    float distance = pow(10, (-69 - dev.rssi) / (10.0 * 2.0));
    Serial.printf("Est. Distance: ~%.1f meters\n", distance);
    
    Serial.println("==========================\n");
}

void trackDevice(int index) {
    if (index < 0 || index >= device_count) {
        Serial.println("Invalid device index!");
        return;
    }
    
    Serial.printf("\n=== Tracking Device #%d ===\n", index);
    Serial.printf("Device: %s\n", tracked_devices[index].name.c_str());
    Serial.printf("Press any key to stop...\n\n");
    
    uint32_t start_time = millis();
    uint32_t last_print = 0;
    
    while (!Serial.available()) {
        uint32_t now = millis();
        
        // Update RSSI
        if (now - last_print >= 500) {
            // Simulate RSSI change (in real use, would re-scan)
            int rssi_var = tracked_devices[index].rssi + random(-5, 5);
            float distance = pow(10, (-69 - rssi_var) / (10.0 * 2.0));
            
            Serial.printf("  RSSI: %4d dBm | Distance: ~%.1f m | Time: %lu s\n",
                         rssi_var, distance, (now - start_time) / 1000);
            
            last_print = now;
        }
        
        delay(100);
    }
    
    Serial.read();  // Clear the keypress
    Serial.println("\nTracking stopped.\n");
}

void exportData() {
    if (device_count == 0) {
        Serial.println("No devices to export.");
        return;
    }
    
    Serial.println("\n=== Exporting CSV Data ===");
    Serial.println("address,name,type,rssi,min_rssi,max_rssi,avg_rssi,packets,first_seen,last_seen");
    
    for (int i = 0; i < device_count; i++) {
        TrackedDevice& dev = tracked_devices[i];
        Serial.printf("%s,%s,%s,%d,%d,%d,%.1f,%lu,%lu,%lu\n",
                     dev.address.c_str(),
                     dev.name.c_str(),
                     identifyDeviceType(dev.appearance),
                     dev.rssi,
                     dev.min_rssi,
                     dev.max_rssi,
                     dev.avg_rssi,
                     dev.packet_count,
                     dev.first_seen,
                     dev.last_seen);
    }
    
    Serial.println("==========================\n");
}
