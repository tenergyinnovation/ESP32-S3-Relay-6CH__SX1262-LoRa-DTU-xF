# Product Requirements Document (PRD)
## ESP32-S3-Relay-6CH Industrial Relay Control System
## เอกสารข้อกำหนดผลิตภัณฑ์ (PRD) ระบบควบคุมรีเลย์อุตสาหกรรม ESP32-S3-Relay-6CH

**Project Name**: ESP32-S3-Relay-6CH Wireless Control System  
**ชื่อโปรเจ็ค**: ระบบควบคุมรีเลย์ 6 ช่องไร้สายใช้งาน ESP32-S3-Relay-6CH  
**Version**: 1.0  
**Date**: 2026-06-29  
**Status**: Development (ขั้นตอนการพัฒนา)  
**Hardware**: Waveshare ESP32-S3-Relay-6CH  
**Official Link**: https://www.waveshare.com/wiki/ESP32-S3-Relay-6CH

---

## 📋 Executive Summary / บทสรุป

This document describes the **ESP32-S3-Relay-6CH**, an industrial-grade 6-channel relay control module with integrated WiFi, Bluetooth, and RS485 connectivity. The module is designed for remote control applications, supporting multiple communication protocols for flexible IoT deployments.

**คำอธิบายภาษาไทย**: เอกสารนี้อธิบายโมดูลควบคุมรีเลย์ 6 ช่องสำหรับการใช้งานอุตสาหกรรมขั้นสูง (ESP32-S3-Relay-6CH) ที่มีการรองรับ WiFi, Bluetooth และ RS485 ในตัว ได้ออกแบบมาสำหรับการใช้งานควบคุมระยะไกล โดยรองรับโปรโตคอลการสื่อสารหลากหลายประเภทเพื่อการใช้งาน IoT ที่ยืดหยุ่น

---

## 🎯 Project Objectives / วัตถุประสงค์ของโปรเจ็ค

1. **Multi-Protocol Support**: Provide wireless relay control via WiFi, Bluetooth, RS485, and Web Interface
   - **ภาษาไทย**: ให้ระบบควบคุมรีเลย์ผ่านทาง WiFi, Bluetooth, RS485 และหน้าเว็บ

2. **Industrial Reliability**: Ensure robust operation with proper isolation, protection circuits, and error handling
   - **ภาษาไทย**: ให้ระบบทำงานได้เสถียรด้วยวงจรปกป้องและการแยกไปแยลแต่ละช่อง

3. **Easy Configuration**: Support both smartphone apps (Bluetooth) and web-based WiFi configuration
   - **ภาษาไทย**: ให้ผู้ใช้สามารถตั้งค่าผ่านแอปพลิเคชัน Bluetooth และหน้าเว็บ

4. **Cloud Integration**: Connect to Waveshare Cloud, MQTT brokers, or custom IoT platforms
   - **ภาษาไทย**: การเชื่อมต่อไปยัง Waveshare Cloud, MQTT Broker หรือแพลตฟอร์ม IoT อื่นๆ

5. **Expandability**: Support hardware expansion via 40-pin Pico HAT compatible header for RTC, CAN, RS232, LoRa sensors
   - **ภาษาไทย**: รองรับการขยายขอบเขตของระบบผ่าน 40-pin Pico HAT compatible header สำหรับ RTC, CAN, RS232, LoRa และเซ็นเซอร์อื่นๆ

---

## 🏗️ System Architecture / สถาปัตยกรรมของระบบ

### 1. **Hardware Components / องค์ประกอบฮาร์ดแวร์**

#### Main Control Board / บอร์ดควบคุมหลัก
- **Model**: ESP32-S3-Relay-6CH (Waveshare)
- **Processor**: ESP32-S3 Dual-Core @ 240MHz (ตัวประมวลผล)
  - Xtensa 32-bit LX7 architecture
  - Built-in AI acceleration
  - Cryptographic hardware support
- **Memory**: 
  - Flash: 16MB (MemFlash)
  - PSRAM: 8MB (DDR2)
- **Wireless**: 
  - WiFi 2.4GHz (802.11 b/g/n)
  - Bluetooth 5.0, BLE
- **Relay Specifications**: 
  - Channels: 6
  - Contact Rating: ≤10A @ 250VAC or ≤10A @ 30VDC
  - Contact Form: 1NO/1NC (Normally Open/Closed)
  - Isolation: Optocoupler isolation
- **Communication Interfaces**:
  - RS485 (Isolated, with hardware auto-direction control)
  - 40-pin Pico HAT header (for RTC, CAN, RS232, LoRa, sensors)
  - USB Type-C (for power, debugging, firmware upload)
  - Power supply screw terminal: 7~36V DC wide voltage input
- **Onboard Features**:
  - RGB WS2812 LED (via GPIO38)
  - Passive Buzzer (via GPIO21)
  - LED Indicators: PWR (power), TXD (RS485 TX), RXD (RS485 RX)
  - 120R RS485 matching resistor (jumper selectable)
  - External antenna SMA connector for WiFi/BLE
- **Protection Circuits**:
  - Power supply isolation (unibody design, no extra power supply needed)
  - Digital isolation for signal protection
  - TVS diodes for surge suppression
  - Ceramic gas discharge tubes

#### Physical Design / การออกแบบทางกายภาพ
- **Enclosure**: Rail-mount ABS protective case (35mm rail compatible)
- **Dimensions**: 145 × 90 × 40 mm
- **Weight**: ~200g (estimated)
- **Installation**: DIN rail mounting for easy industrial installation
- **Safety**: Multiple isolation layers, suitable for harsh industrial environments

---

## 🔄 System Communication Flow

```
┌────────────────────────────────────────────────────────────┐
│         ESP32-S3-Relay-6CH Communication Architecture       │
└────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│  Multiple Control Interfaces / หลายอินเตอร์เฟสควบคุม   │
└─────────────────────────────────────────────────────────┘
    │               │               │              │
    ▼               ▼               ▼              ▼
┌───────┐     ┌──────────┐   ┌───────────┐  ┌──────────┐
│ WiFi  │     │Bluetooth │   │  RS485    │  │Web Page  │
│ AP/STA│     │   BLE    │   │(Industrial)  │(local LAN)
└───────┘     └──────────┘   └───────────┘  └──────────┘
    │               │               │              │
    └───────────────┼───────────────┼──────────────┘
                    │
                    ▼
        ┌──────────────────────┐
        │  ESP32-S3 Core       │
        │  • Relay Control     │
        │  • Command Parsing   │
        │  • Status Feedback   │
        └──────────────────────┘
                    │
        ┌───────────┼───────────┐
        │           │           │
        ▼           ▼           ▼
    ┌────────┐ ┌────────┐ ┌──────────────┐
    │Relay 1 │ │Relay 2 │ │Relay 3-6     │
    │  10A   │ │  10A   │ │  10A each    │
    └────────┘ └────────┘ └──────────────┘
        │           │           │
        └───────────┼───────────┘
                    │
        ┌───────────┴───────────┐
        │                       │
        ▼                       ▼
    ┌─────────────┐       ┌──────────────┐
    │ User Devices│       │Industrial    │
    │(Appliances) │       │ Systems      │
    └─────────────┘       └──────────────┘

Support for Expansion via 40-pin Pico HAT Header:
├── RTC (Real Time Clock) - DS3231
├── CAN Bus - for vehicle/industrial CAN networks
├── RS232/RS485 - additional serial communication
├── LoRa - long-range wireless data
└── Environmental Sensors
```

---

## 📡 Functional Requirements

### 1. WiFi Connectivity Management / การจัดการการเชื่อมต่อ WiFi

#### 1.1 Initial Setup Mode (Access Point) / โหมดตั้งค่าเริ่มต้น (Access Point)
- **Trigger**: System starts in AP mode if WiFi credentials are not saved
  - **ไทย**: ระบบเริ่มทำงานในโหมด AP ถ้ายังไม่ได้บันทึกข้อมูล WiFi
- **AP Configuration / ตั้งค่า AP**:
  - SSID: `GreenEnergy-Gateway-{MAC_ADDRESS}`
  - Security: WPA2-PSK
  - Channel: Auto-select
  - Max Clients: 5

#### 1.2 Configuration Web Interface / หน้าเว็บสำหรับตั้งค่า
- **Purpose**: Allow users to configure WiFi credentials
  - **ไทย**: ให้ผู้ใช้สามารถตั้งค่ารหัส WiFi
- **Features / ฟีเจอร์**:
  - WiFi network scanning
  - Password input interface
  - Configuration save/apply
  - Status indication page
  - Real-time connection status display

#### 1.3 WiFi Connection (Station Mode) / การเชื่อมต่อ WiFi (โหมด STA)
- **Connection Parameters / ข้อมูลการเชื่อมต่อ**:
  - Mode: WiFi Station (STA)
  - Auto-reconnect: Enabled (เปิดใช้การเชื่อมต่อใหม่อัตโนมัติ)
  - Reconnect timeout: 10 seconds
  - Max retry attempts: 5
  - Fallback to AP mode: If connection fails after 5 retries (ย้อนกลับไปโหมด AP ถ้าล้มเหลว 5 ครั้ง)

#### 1.4 Network Configuration Storage / การเก็บข้อมูลตั้งค่าเครือข่าย
- **Storage Method**: EEPROM/NVS (Non-Volatile Storage)
  - **ไทย**: วิธีเก็บข้อมูล EEPROM/NVS (หน่วยความจำถาวร)
- **Data Saved / ข้อมูลที่จะบันทึก**:
  - WiFi SSID
  - WiFi Password (รหัส WiFi)
  - MQTT Broker IP
  - Telegram Bot Token (optional)
  - Google Sheets API Key (optional)

---

### 2. MQTT Integration / การเชื่อมต่อ MQTT

#### 2.1 MQTT Broker Connection / การเชื่อมต่อ MQTT Broker
- **Broker Address**: `159.65.11.12` (ที่อยู่ MQTT Broker)
- **Broker Port**: `1883` (Standard MQTT)
- **Protocol**: MQTT 3.1.1
- **Connection Timeout**: 10 seconds (หมดเวลา 10 วินาที)
- **Keep-Alive Interval**: 60 seconds (ส่งสัญญาณทุก 60 วินาที)

#### 2.2 Authentication / การยืนยันตัวตน
- **Username**: `tiny32`
- **Password**: `tiny32`
- **Connection Method**: TCP with authentication (การเชื่อมต่อ TCP พร้อมการยืนยันตัวตน)

#### 2.3 Topic Subscription / การสมัครรับข้อมูลหัวข้อ
- **Primary Topic**: `/loragreenergy/#` (Subscribe to all subtopics - สมัครรับข้อมูลทั้งหมด)
- **Data Format**: JSON
- **QoS Level**: 1 (At least once delivery - ส่งถึงอย่างน้อย 1 ครั้ง)

#### 2.4 Data Publishing / การส่งข้อมูล
- **Publish Topics / หัวข้อที่ส่งข้อมูล**:
  - `/loragreenergy/gateway/status` - Gateway status heartbeat (สถานะการเต้นของหัวใจ)
  - `/loragreenergy/data/raw` - Raw data from LoRa (ข้อมูลดิบจาก LoRa)
  - `/loragreenergy/data/processed` - Processed energy data (ข้อมูลพลังงานที่ประมวลผลแล้ว)
  - `/loragreenergy/alerts/` - Alert and notification topics (หัวข้อการแจ้งเตือน)

#### 2.5 Connection Management / การจัดการการเชื่อมต่อ
- **Auto-reconnect**: Enabled (เปิดใช้การเชื่อมต่อใหม่อัตโนมัติ)
- **Reconnect Interval**: 5 seconds (ช่วงเวลาการเชื่อมต่อใหม่ 5 วินาที)
- **Max Connection Attempts**: Infinite (with exponential backoff) (ไม่จำกัดด้วยการชะลอเวลา)
- **Connection State Monitoring**: Every 10 seconds (ตรวจสอบทุก 10 วินาที)

---

### 3. LoRa Data Reception / การรับข้อมูล LoRa

#### 3.1 Serial Communication / การสื่อสารผ่านพอร์ต Serial
- **Interface**: RS485 or RS232 (พอร์ต RS485 หรือ RS232)
- **Baud Rate**: 9600 bps (configurable - ปรับแต่งได้)
- **Data Format**: Binary or Text (configurable - รูปแบบข้อมูล)
- **Frame Delimiter**: `\r\n` (ตัวคั่นเฟรม)
- **Buffer Size**: 256 bytes (ขนาดบัฟเฟอร์)

#### 3.2 Data Reception Flow / ขั้นตอนการรับข้อมูล
```
1. Listen on RS485/Serial port (ฟังพอร์ต RS485/Serial)
2. Receive data from SX1262 LoRa DTU (รับข้อมูลจาก SX1262 LoRa DTU)
3. Parse received data (แยกวิเคราะห์ข้อมูล)
4. Validate data integrity (CRC check - ตรวจสอบความสมบูรณ์ของข้อมูล)
5. Extract energy values (ดึงค่าพลังงาน)
6. Timestamp the data (เพิ่มเวลา)
7. Convert to JSON format (แปลงเป็น JSON)
8. Send to MQTT Broker (ส่งไปยัง MQTT Broker)
```

#### 3.3 Data Parsing / การแยกวิเคราะห์ข้อมูล
- **Expected Data Fields / ฟิลด์ข้อมูลที่คาดหวัง**:
  - Timestamp (Unix epoch) - เวลา
  - Energy Value (in kWh) - ค่าพลังงาน (กิโลวัตต์ชั่วโมง)
  - Signal Strength (RSSI) - ความแรงของสัญญาณ
  - Device ID - รหัสอุปกรณ์
  - Battery Level (if available) - ระดับแบตเตอรี่ (ถ้ามี)
  - Status Flags - ธงสถานะ

#### 3.4 Error Handling / การจัดการข้อผิดพลาด
- **Timeout**: 30 seconds without data → send alert (30 วินาทีไม่มีข้อมูล → ส่งการแจ้งเตือน)
- **CRC Failure**: Log error, skip frame (ล้มเหลว CRC → บันทึกข้อผิดพลาด)
- **Malformed Data**: Log error, attempt recovery (ข้อมูลผิดรูป → พยายามกู้คืน)
- **Serial Port Loss**: Attempt reconnect every 5 seconds (สูญเสียพอร์ต → เชื่อมต่อใหม่ทุก 5 วินาที)

---

### 4. Data Processing & JSON Format / การประมวลผลข้อมูลและรูปแบบ JSON

#### 4.1 JSON Data Structure / โครงสร้าง JSON ของข้อมูล
```json
{
  "timestamp": 1719316800,                    // เวลา (Unix epoch)
  "device_id": "ESP32-001",                   // รหัสอุปกรณ์
  "signal_strength": -95,                     // ความแรงของสัญญาณ (dBm)
  "battery_level": 85,                        // ระดับแบตเตอรี่ (%)
  "data": {
    "energy_kWh": 123.45,                     // พลังงาน (กิโลวัตต์ชั่วโมง)
    "power_W": 1200,                          // กำลังไฟฟ้า (วัตต์)
    "voltage_V": 220.5,                       // แรงดันไฟ (โวลต์)
    "current_A": 5.45,                        // กระแสไฟ (แอมแปร์)
    "frequency_Hz": 50.0,                     // ความถี่ (เฮิรตซ์)
    "power_factor": 0.95                      // ตัวประกอบกำลังไฟ
  },
  "gateway_id": "GreenEnergy-Gateway-{MAC}",  // รหัสเกตเวย์
  "status": "OK"                              // สถานะ
}
```

#### 4.2 Data Validation / การตรวจสอบความถูกต้องของข้อมูล
- Check timestamp validity (ตรวจสอบความถูกต้องของเวลา)
- Verify energy value range (0-10000 kWh) (ตรวจสอบค่าพลังงาน)
- Validate RSSI value (-200 to 0 dBm) (ตรวจสอบค่า RSSI)
- Check battery level (0-100%) (ตรวจสอบระดับแบตเตอรี่)
- Validate device ID format (ตรวจสอบรูปแบบรหัสอุปกรณ์)

#### 4.3 Data Storage (Local) / การเก็บข้อมูลในเครื่อง
- **Method**: SD Card (if available) or EEPROM (บัตร SD หรือ EEPROM)
- **Format**: JSON lines (JSONL)
- **Retention**: Last 1000 records (บันทึก 1000 รายการล่าสุด)
- **Purpose**: Offline backup and local analysis (สำรองข้อมูลและวิเคราะห์ในเครื่อง)

---

### 5. Google Sheets Integration / การเชื่อมต่อ Google Sheets

#### 5.1 Google Apps Script Integration / การเชื่อมต่อ Google Apps Script
- **Authentication**: API Key or Service Account (รหัส API หรือบัญชีบริการ)
- **Spreadsheet ID**: [To be configured] (ต้องตั้งค่า)
- **Sheet Name**: `LoRa-Energy-Data` (ชื่อแผ่นงาน)
- **Update Method**: HTTP POST to Google Apps Script webhook (วิธีส่ง HTTP POST)

#### 5.2 Data Columns / คอลัมน์ข้อมูล
| Column | Format | Description / คำอธิบาย |
|--------|--------|-------------|
| Timestamp | ISO 8601 | Date and time of data capture (วันและเวลาเก็บข้อมูล) |
| Device ID | Text | Identifier of transmitting device (รหัสอุปกรณ์ส่ง) |
| Energy (kWh) | Number | Energy consumption value (ค่าการใช้พลังงาน) |
| Power (W) | Number | Real-time power consumption (การใช้พลังงานแบบเรียลไทม์) |
| Voltage (V) | Number | Voltage reading (ค่าแรงดันไฟ) |
| Current (A) | Number | Current reading (ค่ากระแสไฟ) |
| RSSI (dBm) | Number | Signal strength (ความแรงของสัญญาณ) |
| Gateway ID | Text | Gateway identifier (รหัสเกตเวย์) |
| Status | Text | Data quality status (สถานะคุณภาพข้อมูล) |

#### 5.3 API Endpoint / จุดเชื่อมต่อ API
- **Method**: POST
- **Content-Type**: application/json
- **Retry Logic**: 3 retries with exponential backoff (3 ครั้งพร้อมการชะลอเวลา)
- **Timeout**: 10 seconds (หมดเวลา 10 วินาที)

#### 5.4 Data Upload Schedule / ตารางเวลาการส่งข้อมูล
- **Real-time**: Send immediately upon receiving data (ส่งทันทีเมื่อได้รับข้อมูล)
- **Batch Upload**: Every 60 seconds (backup mechanism - ทุก 60 วินาที)
- **Batch Size**: Up to 50 records per batch (ขนาดการส่ง 50 รายการต่อครั้ง)

---

### 6. Telegram Bot Notifications / การแจ้งเตือนผ่าน Telegram Bot

#### 6.1 Telegram Bot Configuration / ตั้งค่า Telegram Bot
- **Bot Token**: [To be configured] (ต้องตั้งค่า)
- **Chat ID**: [To be configured by user] (ต้องตั้งค่าโดยผู้ใช้)
- **API Endpoint**: `https://api.telegram.org/bot{TOKEN}/sendMessage`

#### 6.2 Notification Types / ประเภทการแจ้งเตือน

##### Type 1: Data Arrival Alert / ประเภท 1: การแจ้งเตือนข้อมูลใหม่
```
🟢 Green Energy Data Received / ได้รับข้อมูลพลังงานสีเขียว
━━━━━━━━━━━━━━━━━━━━━━
Device: ESP32-001 / อุปกรณ์
Energy: 123.45 kWh / พลังงาน
Power: 1200W / กำลังไฟ
Time: 2026-06-25 10:30:45 / เวลา
```

##### Type 2: Connection Status / ประเภท 2: สถานะการเชื่อมต่อ
```
🔗 Connection Status Changed / สถานะการเชื่อมต่อเปลี่ยนแปลง
━━━━━━━━━━━━━━━━━━━━━━
WiFi: Connected ✓ / เชื่อมต่อ
MQTT: Connected ✓ / เชื่อมต่อ
Gateway: Online 🟢 / ออนไลน์
```

##### Type 3: Alert/Warning / ประเภท 3: การแจ้งเตือน/คำเตือน
```
⚠️ Gateway Alert / การแจ้งเตือนเกตเวย์
━━━━━━━━━━━━━━━━━━━━━━
Issue: No data for 5 minutes / ปัญหา: ไม่มีข้อมูล 5 นาที
Last Data: 2026-06-25 10:25:45 / ข้อมูลล่าสุด
Status: Checking... / สถานะ: กำลังตรวจสอบ
```

#### 6.3 Notification Frequency / ความถี่ของการแจ้งเตือน
- **Data Alerts**: On each successful data reception (throttled to 1 per minute)
  - **ไทย**: ทุกครั้งที่รับข้อมูลสำเร็จ (จำกัดไว้ 1 ครั้งต่อนาที)
- **Status Updates**: Every 10 minutes (ทุก 10 นาที)
- **Error Alerts**: Immediate (ทันที)
- **Daily Summary**: 08:00 AM every day (ทุกวันเวลา 8 โมงเช้า)

#### 6.4 User Configuration / ตั้งค่าผู้ใช้
- **Enable/Disable**: Toggle notifications per category (เปิด/ปิดรายการแจ้งเตือน)
- **Quiet Hours**: Set time range for suppressed notifications (ตั้งเวลาปิดการแจ้งเตือน)
- **Notification Language**: Thai or English (ภาษาการแจ้งเตือน)

---

## 🔧 Technical Specifications / ข้อมูลจำเพาะทางเทคนิค

### Software Stack / กองเทคโนโลยีซอฟต์แวร์
- **Framework**: Arduino IDE / PlatformIO
- **Language**: C++ (ภาษา C++)
- **MQTT Library**: PubSubClient v2.8.0
- **JSON Library**: ArduinoJson v6.21.4
- **Network Library**: WiFi.h, Ethernet.h

### Network Requirements / ข้อกำหนดเครือข่าย
- **WiFi**: 2.4GHz network access (การเข้าถึง WiFi 2.4 GHz)
- **Internet**: Connection to external services (การเชื่อมต่ออินเทอร์เน็ต)
- **MQTT Broker**: Access to `159.65.11.12:1883`
- **Google Services**: HTTPS access to Google API endpoints (การเข้าถึง HTTPS ไปยัง Google API)
- **Telegram**: HTTPS access to Telegram API

### Power Requirements / ข้อกำหนดไฟฟ้า
- **Normal Operation**: 2W @ 12V (typical / ปกติ)
- **Peak Operation**: 3W @ 12V (การทำงานสูงสุด)
- **Backup Battery**: Optional UPS for continuous operation (แบตเตอรี่สำรอง)
- **PoE Support**: IEEE 802.3af (for Ethernet connection / สำหรับการเชื่อมต่อ Ethernet)

### Storage Requirements / ข้อกำหนดพื้นที่เก็บข้อมูล
- **Code Flash**: ~500 KB
- **Configuration EEPROM**: 4 KB
- **Data Buffer**: 50 KB (for local storage / สำหรับการเก็บข้อมูลในเครื่อง)
- **Total Available**: 16 MB (Flash) + 8 MB (PSRAM)

---

## 🚀 System Features / ฟีเจอร์ของระบบ

### Feature 1: Automatic Failover / ฟีเจอร์ 1: การสำรองข้อมูลอัตโนมัติ
- If MQTT connection fails, buffer data locally (ถ้า MQTT ขาดการเชื่อมต่อ เก็บข้อมูลในเครื่อง)
- Attempt reconnection every 5 seconds (เชื่อมต่อใหม่ทุก 5 วินาที)
- Resume transmission when connection restored (ส่งต่ออีกครั้งเมื่อเชื่อมต่อได้)
- Maximum buffer: 1000 records (บัฟเฟอร์สูงสุด 1000 รายการ)

### Feature 2: Health Monitoring / ฟีเจอร์ 2: การตรวจสอบสุขภาพระบบ
- Check WiFi connection every 10 seconds (ตรวจสอบ WiFi ทุก 10 วินาที)
- Monitor MQTT connection every 10 seconds (ตรวจสอบ MQTT ทุก 10 วินาที)
- Monitor serial port every 30 seconds (ตรวจสอบพอร์ต Serial ทุก 30 วินาที)
- Send heartbeat every 60 seconds (ส่งสัญญาณทุก 60 วินาที)

### Feature 3: Configuration Management / ฟีเจอร์ 3: การจัดการตั้งค่า
- Web-based configuration interface (หน้าเว็บสำหรับตั้งค่า)
- Local configuration persistence (บันทึกตั้งค่าในเครื่อง)
- Factory reset capability (ความสามารถรีเซ็ตเป็นค่าเริ่มต้น)
- Configuration backup to cloud (สำรองตั้งค่าไปคลาวด์)

### Feature 4: LED Status Indicators / ฟีเจอร์ 4: ตัวบ่งชี้ LED
- **RGB LED States / สถานะ LED RGB**:
  - 🔴 Red: System error / Not connected (ข้อผิดพลาดระบบ / ไม่เชื่อมต่อ)
  - 🟡 Yellow: WiFi searching / Connecting (ค้นหา WiFi / กำลังเชื่อมต่อ)
  - 🟢 Green: All systems OK (ระบบทั้งหมดตกลง)
  - 🔵 Blue: Data processing (กำลังประมวลผลข้อมูล)
  - 🟣 Purple: Firmware update (อัปเดตเฟิร์มแวร์)

### Feature 5: Serial Debug Output / ฟีเจอร์ 5: ผลลัพธ์ Debug ผ่าน Serial
- Enable HWCDC (USB CDC On Boot) (เปิด HWCDC)
- Real-time debug logging (บันทึก Debug แบบเรียลไทม์)
- Performance metrics (เมตริกประสิทธิภาพ)
- Error trace logging (บันทึกการติดตามข้อผิดพลาด)

---

## 📊 Performance Metrics / เมตริกประสิทธิภาพ

| Metric / เมตริก | Target / เป้าหมาย | Threshold / ขีดจำกัด |
|--------|--------|-----------|
| Data Latency (ความล่าช้าของข้อมูล) | < 1 second | < 5 seconds |
| MQTT Publish Rate (อัตราการส่ง MQTT) | 1/second | max 10/second |
| Memory Usage (การใช้หน่วยความจำ) | < 50% | < 80% |
| Uptime (เวลาทำงาน) | 99.5% | > 99% |
| WiFi Reconnect Time (เวลาเชื่อมต่อ WiFi ใหม่) | < 10 seconds | < 30 seconds |
| Data Loss Rate (อัตราการสูญหายข้อมูล) | 0% | < 0.1% |

---

## 🔐 Security Considerations / การพิจารณาด้านความปลอดภัย

### 1. Credentials Management / การจัดการข้อมูลประจำตัว
- **Storage**: Encrypted in NVS (Non-Volatile Storage) (เก็บในรูปแบบเข้ารหัส)
- **Transmission**: Always use HTTPS/TLS where available (ส่งผ่าน HTTPS/TLS เสมอ)
- **Rotation**: Support credential update via web interface (อัปเดตข้อมูลประจำตัว)

### 2. Network Security / ความปลอดภัยของเครือข่าย
- **WiFi**: WPA2 minimum (WPA2 ขั้นต่ำ)
- **MQTT**: Username/password authentication (ยืนยันตัวตน)
- **API Keys**: Secure storage in configuration (เก็บอย่างปลอดภัย)
- **Telegram Token**: Encrypted storage (เก็บแบบเข้ารหัส)

### 3. Data Integrity / ความสมบูรณ์ของข้อมูล
- **CRC Check**: Validate received LoRa data (ตรวจสอบข้อมูล LoRa)
- **JSON Validation**: Verify JSON structure before processing (ตรวจสอบโครงสร้าง JSON)
- **Timestamp Validation**: Check for time skew (±5 minutes) (ตรวจสอบการเบี่ยงเบนของเวลา)

### 4. Access Control / การควบคุมการเข้าถึง
- **Web Interface**: Authentication with default credentials (ยืนยันตัวตน)
- **Configuration Reset**: Physical button press required (จำเป็นต้องกดปุ่ม)
- **Debug Access**: Serial port authentication (optional) (ยืนยันตัวตนของพอร์ต Serial)

---

## 📋 Development Roadmap / แผนการพัฒนา

### Phase 1: Core Development (Week 1-2) / ระยะที่ 1: การพัฒนาหลัก (สัปดาห์ที่ 1-2)
- [ ] WiFi AP configuration interface (หน้าเว็บตั้งค่า WiFi AP)
- [ ] Basic MQTT connection (การเชื่อมต่อ MQTT ขั้นพื้นฐาน)
- [ ] Serial data reception from LoRa DTU (รับข้อมูล Serial จาก LoRa DTU)
- [ ] Local data processing (ประมวลผลข้อมูลในเครื่อง)

### Phase 2: Integration (Week 3-4) / ระยะที่ 2: การผสานรวม (สัปดาห์ที่ 3-4)
- [ ] Google Sheets API integration (ผสานรวม Google Sheets API)
- [ ] Telegram Bot notifications (การแจ้งเตือน Telegram Bot)
- [ ] Data validation and error handling (ตรวจสอบข้อมูลและจัดการข้อผิดพลาด)
- [ ] Local data storage (เก็บข้อมูลในเครื่อง)

### Phase 3: Testing & Optimization (Week 5-6) / ระยะที่ 3: การทดสอบและปรับแต่ง (สัปดาห์ที่ 5-6)
- [ ] Unit testing for all modules (ทดสอบหน่วยสำหรับโมดูลทั้งหมด)
- [ ] Integration testing (ทดสอบการผสานรวม)
- [ ] Performance optimization (ปรับแต่งประสิทธิภาพ)
- [ ] Memory optimization (ปรับแต่งหน่วยความจำ)

### Phase 4: Deployment (Week 7) / ระยะที่ 4: การปรับใช้ (สัปดาห์ที่ 7)
- [ ] Final firmware build (สร้างเฟิร์มแวร์ขั้นสุดท้าย)
- [ ] Documentation completion (สำเร็จเอกสาร)
- [ ] User manual creation (สร้างคำแนะนำผู้ใช้)
- [ ] Production deployment (ปรับใช้ในการผลิต)

---

## 📝 Configuration Guide / คำแนะนำการตั้งค่า

### WiFi Configuration / การตั้งค่า WiFi
1. Power on the gateway (เปิดเกตเวย์)
2. Wait for AP mode to activate (SSID: `GreenEnergy-Gateway-XXXX`) (รอให้โหมด AP เปิด)
3. Connect to AP using any WiFi device (เชื่อมต่อกับ AP ด้วยอุปกรณ์ WiFi)
4. Open browser and navigate to `192.168.4.1` (เปิดเบราวเซอร์และไปที่ 192.168.4.1)
5. Enter WiFi credentials and submit (ป้อนข้อมูล WiFi และส่ง)
6. System will reboot and connect to network (ระบบจะรีบูตและเชื่อมต่อ)

### MQTT Configuration / การตั้งค่า MQTT
- Broker IP: `159.65.11.12` (ที่อยู่ MQTT)
- Port: `1883`
- Username: `tiny32`
- Password: `tiny32`
- Auto-configured after WiFi setup (ตั้งค่าอัตโนมัติหลังตั้งค่า WiFi)

### Google Sheets Setup / การตั้งค่า Google Sheets
1. Create new Google Sheet (สร้าง Google Sheet ใหม่)
2. Set up Google Apps Script webhook (ตั้งค่า webhook ของ Google Apps Script)
3. Configure API endpoint in gateway web interface (ตั้งค่า API endpoint ในหน้าเว็บ)
4. Grant permissions through web form (อนุมัติสิทธิผ่านแบบฟอร์มเว็บ)

### Telegram Bot Setup / การตั้งค่า Telegram Bot
1. Create Telegram Bot (via @BotFather) (สร้าง Telegram Bot)
2. Obtain Bot Token (ได้รับ Token ของ Bot)
3. Get Chat ID of recipient (ได้รับ Chat ID ของผู้รับ)
4. Configure via gateway web interface (ตั้งค่าผ่านหน้าเว็บ)

---

## 🔍 Testing Checklist / รายการตรวจสอบการทดสอบ

- [ ] WiFi AP mode activates correctly (โหมด AP ทำงานถูกต้อง)
- [ ] Web configuration interface loads (หน้าเว็บโหลดสำเร็จ)
- [ ] WiFi credentials saved and restored (บันทึกและกู้คืน WiFi)
- [ ] MQTT connection establishes (เชื่อมต่อ MQTT สำเร็จ)
- [ ] Data received from LoRa DTU (รับข้อมูลจาก LoRa DTU)
- [ ] JSON data formatted correctly (ข้อมูล JSON ถูกต้อง)
- [ ] Google Sheets receives data (Google Sheets ได้รับข้อมูล)
- [ ] Telegram notifications sent (ส่งการแจ้งเตือน Telegram)
- [ ] Error handling works correctly (การจัดการข้อผิดพลาดทำงาน)
- [ ] Local data buffer functions (บัฟเฟอร์ข้อมูลทำงาน)
- [ ] Failover mechanisms work (กลไกสำรองข้อมูลทำงาน)
- [ ] Memory usage within limits (การใช้หน่วยความจำอยู่ในขีดจำกัด)
- [ ] Long-term stability (24+ hours) (เสถียรภาพระยะยาว)

---

## 📞 Support & Resources / การสนับสนุนและทรัพยากร

### Documentation / เอกสาร
- **RS485 Implementation Guide**: [RS485_IMPLEMENTATION_GUIDE.md](RS485_IMPLEMENTATION_GUIDE.md) (ใหม่! / NEW!)
- Waveshare ESP32-S3-POE-ETH-8DI-8DO: https://www.waveshare.com/wiki/ESP32-S3-POE-ETH-8DI-8DO
- Arduino ESP32 Docs: https://docs.espressif.com/projects/arduino-esp32/
- MQTT Specification: https://mqtt.org/
- Google Sheets API: https://developers.google.com/sheets
- Telegram Bot API: https://core.telegram.org/bots/api

### Key Contacts / ผู้ติดต่อหลัก
- Hardware Support: support@waveshare.com (บอร์ดฮาร์ดแวร์)
- Project Manager: [To be assigned] (ตัวจัดการโปรเจ็ค)
- Technical Lead: [To be assigned] (ผู้นำด้านเทคนิค)

---

## 📌 Revision History / ประวัติการแก้ไข

| Version | Date | Author | Changes / การเปลี่ยนแปลง |
|---------|------|--------|---------|
| 1.0 | 2026-06-25 | Team | Initial PRD creation (สร้าง PRD เริ่มต้น) |
| 1.1 | 2026-06-25 | Team | Thai language added (เพิ่มภาษาไทย) |
| 1.2 | 2026-06-26 | Team | RS485 Implementation Guide added (เพิ่มคู่มืน RS485) || 1.3 | 2026-06-26 | Team | loraConfigureStream/loraConfigurePacket functions enhanced with full AT command sequence (อัปเดตฟังก์ชันการตั้งค่า LoRa DTU) |
---

---

## 🔌 GPIO Pin Mapping / การแมปพิน GPIO

### Digital Input Pins (DI) / พิน Digital Input (8 ช่อง)
| Channel | GPIO | Function | Notes / หมายเหตุ |
|---------|------|----------|------|
| DI1 | GPIO4 | Detection pin for Digital Input CH1 (พิน DI ช่อง 1) | Optocoupler isolated input (อินพุตแยกแบบ Optocoupler) |
| DI2 | GPIO5 | Detection pin for Digital Input CH2 (พิน DI ช่อง 2) | Optocoupler isolated input |
| DI3 | GPIO6 | Detection pin for Digital Input CH3 (พิน DI ช่อง 3) | Optocoupler isolated input |
| DI4 | GPIO7 | Detection pin for Digital Input CH4 (พิน DI ช่อง 4) | Optocoupler isolated input |
| DI5 | GPIO8 | Detection pin for Digital Input CH5 (พิน DI ช่อง 5) | Optocoupler isolated input |
| DI6 | GPIO9 | Detection pin for Digital Input CH6 (พิน DI ช่อง 6) | Optocoupler isolated input |
| DI7 | GPIO10 | Detection pin for Digital Input CH7 (พิน DI ช่อง 7) | Optocoupler isolated input |
| DI8 | GPIO11 | Detection pin for Digital Input CH8 (พิน DI ช่อง 8) | Optocoupler isolated input |

### Digital Output Pins (DO) / พิน Digital Output (8 ช่อง)
| Channel | Expander Pin | Function | Notes / หมายเหตุ |
|---------|------|----------|------|
| DO1 | EXIO1 | Control pin for Digital Output CH1 (พิน DO ช่อง 1) | Controlled via TCA9554PWR I/O expander (ควบคุมผ่าน TCA9554PWR) |
| DO2 | EXIO2 | Control pin for Digital Output CH2 (พิน DO ช่อง 2) | Controlled via TCA9554PWR I/O expander |
| DO3 | EXIO3 | Control pin for Digital Output CH3 (พิน DO ช่อง 3) | Controlled via TCA9554PWR I/O expander |
| DO4 | EXIO4 | Control pin for Digital Output CH4 (พิน DO ช่อง 4) | Controlled via TCA9554PWR I/O expander |
| DO5 | EXIO5 | Control pin for Digital Output CH5 (พิน DO ช่อง 5) | Controlled via TCA9554PWR I/O expander |
| DO6 | EXIO6 | Control pin for Digital Output CH6 (พิน DO ช่อง 6) | Controlled via TCA9554PWR I/O expander |
| DO7 | EXIO7 | Control pin for Digital Output CH7 (พิน DO ช่อง 7) | Controlled via TCA9554PWR I/O expander |
| DO8 | EXIO8 | Control pin for Digital Output CH8 (พิน DO ช่อง 8) | Controlled via TCA9554PWR I/O expander |

### Communication Pins / พิน Communication

#### RS485 (UART1) - For LoRa DTU / สำหรับ LoRa DTU
| Pin | GPIO | Function | Notes / หมายเหตุ |
|-----|------|----------|------|
| TX1 | GPIO17 | RS485 TX (Transmit) | UART1 TX, Half-duplex enabled (UART1 ส่ง) |
| RX1 | GPIO18 | RS485 RX (Receive) | UART1 RX (UART1 รับ) |
| RTS1 | GPIO21 | RS485 RTS (Request to Send) | Control pin for half-duplex mode (พิน RTS สำหรับการควบคุม) |

#### CAN Bus (UART2) / บัส CAN
| Pin | GPIO | Function | Notes / หมายเหตุ |
|-----|------|----------|------|
| CAN_TX | GPIO2 | CAN TX (TWAI) | Isolated CAN interface (Isolated CAN) |
| CAN_RX | GPIO3 | CAN RX (TWAI) | Isolated CAN interface |

#### Ethernet (SPI) / Ethernet (SPI)
| Pin | GPIO | Function | Notes / หมายเหตุ |
|-----|------|----------|------|
| ETH_MOSI | GPIO13 | SPI MOSI | W5500 Ethernet chip (ชิป W5500) |
| ETH_MISO | GPIO14 | SPI MISO | W5500 Ethernet chip |
| ETH_SCLK | GPIO15 | SPI Clock | W5500 Ethernet chip |
| ETH_CS | GPIO16 | SPI Chip Select | W5500 Ethernet chip |
| ETH_RST | GPIO39 | Ethernet Reset | W5500 reset pin (พิน Reset) |
| ETH_INT | GPIO12 | Ethernet Interrupt | W5500 interrupt pin (พิน Interrupt) |

#### SD Card (SPI) / การ์ด SD (SPI)
| Pin | GPIO | Function | Notes / หมายเหตุ |
|-----|------|----------|------|
| SD_D0 | GPIO45 | SD Data 0 | SD card data pin (พินข้อมูล SD) |
| SD_CMD | GPIO47 | SD Command | SD card command pin |
| SD_SCK | GPIO48 | SD Clock | SD card clock pin |
| SD_D3, D1, D2 | NC | Not Connected | Only 1-bit mode supported (รองรับเฉพาะโหมด 1-bit) |

#### I2C (RTC & PCF85063) / I2C (RTC & PCF85063)
| Pin | GPIO | Function | Notes / หมายเหตุ |
|-----|------|----------|------|
| RTC_SCL | GPIO41 | I2C Clock | PCF85063 RTC clock (นาฬิกา I2C) |
| RTC_SDA | GPIO42 | I2C Data | PCF85063 RTC data (ข้อมูล I2C) |
| RTC_INT | GPIO40 | RTC Interrupt | PCF85063 interrupt pin |

### Indicator & Control Pins / พิน Indicator และ Control

#### RGB LED (WS2812B) / RGB LED
| Pin | GPIO | Function | Notes / หมายเหตุ |
|-----|------|----------|------|
| RGB_PIN | GPIO38 | RGB Data | Addressable RGB LED control (ควบคุม RGB LED) |

#### Buzzer / Buzzer
| Pin | GPIO | Function | Notes / หมายเหตุ |
|-----|------|----------|------|
| BUZZER | GPIO21 | Buzzer Control | PWM-controlled buzzer (PWM 1kHz, 256 levels) |

#### Boot Button / ปุ่ม Boot
| Pin | GPIO | Function | Notes / หมายเหตุ |
|-----|------|----------|------|
| BOOT | GPIO0 | BOOT Button | Used for boot mode selection & factory reset (ใช้สำหรับการเลือกโหมด Boot) |

### USB / USB
- **USB Type-C**: Connected to ESP32-S3 USB Serial interface (เชื่อมต่อกับอินเทอร์เฟส USB Serial)
- **CDC Mode**: Supports HWCDC for debug output (สนับสนุนโหมด HWCDC)

---

## 🏗️ Library Architecture / สถาปัตยกรรมของ Library

### Directory Structure / โครงสร้างไดเรกทอรี

```
lib/ESP32-S3-POE-ETH-8DI-8DO/
├── library.properties              ← Library metadata (ข้อมูล Library)
├── library.json                    ← PlatformIO config
├── LICENSE                         ← MIT License
├── README.md                       ← Documentation
├── keywords.txt                    ← Arduino IDE syntax highlighting
├── src/
│   ├── ESP32S3_8DI8DO.h           ← Main library header
│   ├── ESP32S3_8DI8DO.cpp         ← Main library implementation
│   ├── ESP32S3_DI.h                ← Digital Input header
│   ├── ESP32S3_DI.cpp              ← Digital Input implementation
│   ├── ESP32S3_DO.h                ← Digital Output header
│   ├── ESP32S3_DO.cpp              ← Digital Output implementation
│   ├── ESP32S3_RS485.h             ← RS485 Communication header
│   ├── ESP32S3_RS485.cpp           ← RS485 Communication implementation
│   ├── ESP32S3_CAN.h               ← CAN Bus header
│   ├── ESP32S3_CAN.cpp             ← CAN Bus implementation
│   ├── ESP32S3_Ethernet.h          ← Ethernet header
│   ├── ESP32S3_Ethernet.cpp        ← Ethernet implementation
│   ├── ESP32S3_RGB.h               ← RGB LED control header
│   ├── ESP32S3_RGB.cpp             ← RGB LED control implementation
│   ├── ESP32S3_Buzzer.h            ← Buzzer control header
│   ├── ESP32S3_Buzzer.cpp          ← Buzzer control implementation
│   ├── ESP32S3_RTC.h               ← RTC clock header
│   └── ESP32S3_RTC.cpp             ← RTC clock implementation
├── examples/
│   ├── 01_BasicDigitalIO/
│   │   └── BasicDigitalIO.ino      ← Digital I/O control demo
│   ├── 02_EthernetDemo/
│   │   └── EthernetDemo.ino        ← Ethernet connectivity demo
│   ├── 03_RS485LoRaDTU/
│   │   └── RS485LoRaDTU.ino        ← RS485 + LoRa DTU demo
│   ├── 04_CANBusDemo/
│   │   └── CANBusDemo.ino          ← CAN 2.0B communication demo
│   └── 05_CompleteSystem/
│       └── CompleteSystem.ino      ← Full system integration demo
└── docs/
    └── images/                     ← Board images & diagrams

```

### Core Classes / คลาสหลัก

#### 1. **ESP32S3_8DI8DO** (Main class / คลาสหลัก)
```cpp
class ESP32S3_8DI8DO {
    // Constructor & Initialization
    ESP32S3_8DI8DO();
    void begin();
    void end();
    
    // Digital Input (DI) Methods
    bool readDI(uint8_t channel);              // Read DI channel 1-8
    bool getDIState(uint8_t channel);
    void setDICallback(uint8_t channel, ...);  // Set interrupt callback
    
    // Digital Output (DO) Methods
    void setDO(uint8_t channel, bool state);   // Set DO on/off
    void turnOnDO(uint8_t channel);
    void turnOffDO(uint8_t channel);
    void toggleDO(uint8_t channel);
    bool getDOState(uint8_t channel);
    
    // System Control
    void softReset();
    void factoryReset();
    String getSystemStatus();
};
```

#### 2. **ESP32S3_DI** (Digital Input controller / ตัวควบคุม Digital Input)
```cpp
class ESP32S3_DI {
    ESP32S3_DI();
    void init();
    bool read(uint8_t channel);
    void setCallback(uint8_t channel, callback_t func);
    void enableDebounce(uint8_t ms);
};
```

#### 3. **ESP32S3_DO** (Digital Output controller / ตัวควบคุม Digital Output)
```cpp
class ESP32S3_DO {
    ESP32S3_DO();
    void init();
    void set(uint8_t channel, bool state);
    bool get(uint8_t channel);
    void toggle(uint8_t channel);
};
```

#### 4. **ESP32S3_RS485** (RS485 Communication / การสื่อสาร RS485)
```cpp
class ESP32S3_RS485 {
    ESP32S3_RS485();
    void begin(unsigned long baudrate = 9600);
    void sendData(uint8_t* data, size_t length);
    size_t readData(uint8_t* buffer, size_t max_len);
    void setDataCallback(callback_t func);
};
```

#### 5. **ESP32S3_RGB** (RGB LED control / ควบคุม RGB LED)
```cpp
class ESP32S3_RGB {
    ESP32S3_RGB();
    void begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void blink(uint8_t r, uint8_t g, uint8_t b, uint16_t interval);
    void pulse(uint8_t r, uint8_t g, uint8_t b);
};
```

---

**Document Status**: ✅ Ready for Library Development (พร้อมสำหรับการพัฒนา Library)  
**Last Updated**: 2026-06-28  
**Next Phase**: Library Implementation & Examples (ระยะถัดไป: พัฒนา Library และตัวอย่าง)
