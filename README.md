# 🚀 ESP32-S3-Relay-6CH with SX1262-LoRa-DTU-xF

ตัวอย่าง firmware & library สำหรับบอร์ด **Waveshare ESP32-S3-Relay-6CH** ที่รวมกับโมดูล **SX1262-LoRa-DTU-xF** เพื่อสร้างระบบสื่อสารแบบ LoRa สำหรับการส่งข้อมูลระยะไกล พัฒนาโดย **Tenergy Innovation**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
![Platform: ESP32-S3](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-00599C)
![LoRa: SX1262](https://img.shields.io/badge/LoRa-SX1262-green)

---

## 📋 ลักษณะเด่น

### Hardware Support
- 🔌 **6 Relay Channels** - 10A @ 250VAC / 30VDC per channel, Optocoupler isolated
- 📡 **SX1262-LoRa-DTU-xF Module** - Long-range wireless communication (5+ km), 22dBm transmission power, -148dBm sensitivity
- 🔗 **RS485 Interface** - UART1 & UART2, Isolated with TVS protection (for LoRa DTU & Modbus RTU)
- 🌐 **WiFi 2.4GHz + Bluetooth 5.0** - Dual wireless connectivity
- 📡 **WiFi Modes**: AP Mode & Station Mode (STA)
- 💡 **RGB LED** - WS2812B NeoPixel (GPIO38)
- 🔊 **Buzzer** - Passive buzzer (GPIO21)
- 🎮 **40-pin Pico HAT Header** - Expandable via RTC, CAN, RS232, LoRa, sensors
- 🏭 **Industrial Grade** - Rail-mount enclosure, multiple isolation circuits

### Software Features
- 📝 **Interactive Serial Command Interface**
- 🎨 **RGB LED Control** - Red, Green, Blue, Yellow, Purple, Orange, White
- 🔄 **Status Query** - Read relay states and GPIO in real-time
- 🎵 **Buzzer Beep** - Configurable beep sequences
- � **LoRa DTU Control** - AT Commands, Stream Mode, Packet Mode, data transmission
- 📊 **Real-time LoRa Data Monitoring** - Display incoming/outgoing LoRa messages
- �🔐 **MQTT Support** - Cloud connectivity
- 🌐 **Web Configuration** - WiFi setup via web interface
- 📱 **Bluetooth Control** - Remote relay control via BLE
- 🌐 **OTA Firmware Update** - Over-the-air updates
- ⏰ **RTC Support** - Real-time clock for time-based control

---

## 🛠️ ติดตั้ง

### 1. Clone Repository
```bash
git clone https://github.com/yourusername/ESP32-S3-Relay-6CH_template.git
cd ESP32-S3-Relay-6CH_template
```

### 2. ติดตั้ง PlatformIO
```bash
# macOS / Linux
brew install platformio

# หรือใช้ VSCode Extension
# - Install "PlatformIO IDE" from VSCode Marketplace
```

### 3. Build & Upload
```bash
# Build firmware
platformio run

# Upload ไปที่บอร์ด
platformio run --target upload

# เปิด Serial Monitor
platformio device monitor
```

---

## 🚀 Quick Start Guide

### 1️⃣ ทดสอบ LED & Buzzer
```bash
LED red 3         # เปิดสีแดง 3 วินาที
LED green 2       # เปิดสีเขียว 2 วินาที
BUZZER 3          # Beep 3 ครั้ง
```

### 2️⃣ ทดสอบ Relay
```bash
OUTPUT 1 ON       # เปิด Relay 1
OUTPUT 1 OFF      # ปิด Relay 1
OUTPUT 1 ?        # อ่านสถานะ Relay 1
```

### 3️⃣ ทดสอบ LoRa DTU (Stream Mode)
```bash
# ทั้งสองเครื่อง: ตั้งค่า Mode = Stream
+++               # เข้าโหมด AT Command
AT+MODE=1         # Stream Mode
AT+EXIT           # ออกจาก AT Command

# เครื่องที่ 1 ส่งข้อมูล
sendLoRaData("Hello from Device 1");

# เครื่องที่ 2 จะรับข้อมูล: RECEIVED: "Hello from Device 1"
```

### 4️⃣ ส่งข้อมูล Sensor ผ่าน LoRa
```cpp
// ในไฟล์ main.cpp loop()
static unsigned long lastSend = 0;
if (millis() - lastSend > 10000) {  // ทุก 10 วินาที
    lastSend = millis();
    float temperature = 25.5;
    sendLoRaData("TEMP:" + String(temperature));
}
```

---

## 💻 คำสั่ง (Commands)

เมื่อเชื่อมต่อ Serial Monitor ที่ 115200 baud:

### 1️⃣ LED Control
```
LED <color> <seconds>

Colors: red, green, blue, yellow, purple, orange, white
Seconds: 0.1 to 10.0 (or -1 to turn OFF)

Examples:
  LED red 2         → แสดงสีแดง 2 วินาที
  LED green 5       → แสดงสีเขียว 5 วินาที
  LED blue -1       → ดับ LED ทันที
```

### 2️⃣ Buzzer Control
```
BUZZER <times>

Times: 1 to 10 beeps

Examples:
  BUZZER 1          → Beep 1 ครั้ง
  BUZZER 3          → Beep 3 ครั้ง
```

### 3️⃣ Digital Output (Relay)
```
OUTPUT <channel> <state>

Channel: 1-8 or DO1-DO8
State: ON/OFF (case-insensitive), 1/0, or ? (read status)

Examples:
  OUTPUT 1 ON       → เปิด relay DO1
  OUTPUT 1 on       → เปิด relay DO1 (lowercase ได้)
  OUTPUT DO5 OFF    → ปิด relay DO5
  OUTPUT 3 1        → เปิด relay DO3 (numeric format)
  OUTPUT 2 0        → ปิด relay DO2 (numeric format)
  OUTPUT 4 ?        → อ่านสถานะ relay DO4
```

### 4️⃣ Digital Input (Sensor Read)
```
INPUT <channel>

Channel: 1-8 or DI1-DI8

Examples:
  INPUT 1           → อ่านสถานะ DI1
  INPUT DI3         → อ่านสถานะ DI3
```

---

### 5️⃣ System
```
RESET               → รีเซ็ตบอร์ด
HELP                → แสดงรายการคำสั่ง
```

---

## 📡 LoRa DTU AT Commands

เมื่อเชื่อมต่อ Serial Monitor ที่ 115200 baud ให้ส่งคำสั่ง AT Command ไปยังโมดูล SX1262-LoRa-DTU-xF:

### เข้า/ออกโหมด AT Command
```
+++                 → เข้าโหมด AT Command (ไม่มี \r\n)
AT+EXIT             → ออกจากโหมด AT Command
AT+VER              → ดูเวอร์ชั่นของโมดูล
AT+RESTORE=1        → รีเซ็ตโมดูลเป็นค่าเริ่มต้น
```

### การตั้งค่าพื้นฐาน
```
AT+BAUD=9600        → ตั้ง Baud Rate = 9600 bps
AT+MODE=1           → โหมด Stream (1=Stream, 2=Packet, 3=Relay)
AT+PORT=2           → Interface (1=RS422, 2=RS485, 3=RS232)
AT+ADDR=0           → ตั้ง Address = 0
```

### การตั้งค่าช่องสัญญาณและกำลังส่ง
```
AT+TXCH=18          → ตั้ง Tx Channel = 18 (868MHz) หรือ 23 (433MHz)
AT+RXCH=18          → ตั้ง Rx Channel = 18 (868MHz) หรือ 23 (433MHz)
AT+SF=7             → Spreading Factor (7-12, ค่าต่ำ = เร็ว, ค่าสูง = ไกล)
AT+BW=0             → Bandwidth = 0 (125KHz)
AT+CR=1             → Code Rate = 1 (4/5)
AT+PWR=22           → ตั้ง Transmit Power = 22dBm (สูงสุด)
```

### อื่น ๆ
```
AT+RSSI=1           → เปิด RSSI Output (สัญญาณแรงที่รับ)
AT+LBT=0            → Listen Before Talk (0=Disable, 1=Enable)
```

---

## 📤 LoRa Data Transmission

### ส่งข้อมูลผ่าน Serial (อัตโนมัติไปยัง LoRa DTU)
```cpp
// ตัวอย่างการส่งข้อมูลเซนเซอร์
sendLoRaData("TEMP:25.5");        // ส่ง "TEMP:25.5" ผ่าน LoRa

// ส่ง JSON Data
String jsonData = "{\"sensor\":1,\"value\":42}";
sendLoRaData(jsonData);

// ส่งข้อมูลเป็นระยะ (ใน loop() ทุก 5 วินาที)
static unsigned long lastSend = 0;
if (millis() - lastSend > 5000) {
    lastSend = millis();
    sendLoRaData("HEARTBEAT");
}
```

### รับข้อมูลจาก LoRa DTU
```
ข้อมูลที่รับจากโมดูล LoRa DTU จะแสดงใน Serial Monitor โดยอัตโนมัติ
เช่น: RECEIVED: "Hello from Remote Device"
```

---

## 🔌 Relay Control Over LoRa

### ตัวอย่าง: ส่งคำสั่งควบคุม Relay ผ่าน LoRa
```
เครื่องฝั่งไกล (Remote) ส่ง AT Command:
  OUTPUT 1 ON         → เปิด Relay 1 บนเครื่องฝั่งใกล้ (Local)

หรือส่งผ่าน LoRa Data:
  sendLoRaData("DO:1:ON");      // ส่งคำสั่งเปิด Relay 1
  sendLoRaData("DO:2:OFF");     // ส่งคำสั่งปิด Relay 2
```

---

---

## 📦 โครงสร้าง Project

```
ESP32-S3-Relay-6CH_SX1262-LoRa-DTU-xF/
├── src/
│   └── main.cpp                 ← Main firmware with LoRa DTU support
├── lib/
│   ├── ESP32-S3-Relay-6CH/
│   │   ├── src/
│   │   │   ├── ESP32S3_Relay.h      ← Relay library header
│   │   │   └── ESP32S3_Relay.cpp    ← Relay implementation
│   │   └── examples/
│   │       ├── BasicRelayControl/
│   │       ├── LoRa_DTU_Stream_Mode/    ← LoRa Stream Mode example
│   │       └── LoRa_DTU_AT_Commands/    ← AT Command example
│   ├── Adafruit_NeoPixel/            ← RGB LED library
│   └── (other libraries)
├── include/
├── platformio.ini
├── README.md
├── LICENSE
├── CHANGELOG.md
├── CONTRIBUTING.md
└── extras/
    ├── PRD.md                           ← Product Requirements Doc
    ├── ai_prompt.md                     ← Development notes
    ├── LORA_DTU_RS485_GUIDE.md         ← LoRa DTU connection guide
    ├── QUICK_LORA_SEND_REFERENCE.md    ← Quick LoRa reference
    ├── RS485_LORA_DTU_IMPLEMENTATION.md ← Implementation guide
    └── (other docs & backups)
```

---

## 📡 RS485 Communication & LoRa DTU

### RS485 Interface (UART2 for LoRa DTU)
บอร์ดรองรับการสื่อสาร RS485 ผ่าน **UART2** สำหรับการเชื่อมต่อกับโมดูล SX1262-LoRa-DTU-xF:
- **TX**: GPIO17
- **RX**: GPIO18
- **RTS**: GPIO21
- **Baud**: 9600 (configurable)

ตัวอย่าง:
```cpp
// ส่งคำสั่ง AT Command ไป LoRa DTU
board.getRS485().write("AT+VER\r\n");

// รับข้อมูลกลับ
String response = board.getRS485().readUntil('\n', 100);

// ส่งข้อมูลผ่าน LoRa (Stream Mode)
board.getRS485().write("Hello from ESP32\r\n");
```

---

## 🔌 LoRa DTU Hardware Connection

### การเชื่อมต่อสาย RS485
```
┌──────────────────────────────────────────────────────────┐
│  SX1262-LoRa-DTU-xF      ESP32-S3-Relay-6CH             │
│  ─────────────────────────────────────────────────────── │
│  TA (RS485+) ──→ RS485 Module ──→ GPIO18 (UART2 RX)    │
│  TB (RS485-) ──→ RS485 Module ──→ GPIO17 (UART2 TX)    │
│  GND ─────────────────────────→ GND                    │
│  PWR (12V) ────────────────→ DC12V (แยก)              │
└──────────────────────────────────────────────────────────┘
```

### PIN Configuration
| Signal | GPIO | Direction | Function |
|--------|------|-----------|----------|
| **UART2 TX** | GPIO17 | OUT | Data send to LoRa DTU |
| **UART2 RX** | GPIO18 | IN | Data receive from LoRa DTU |
| **RTS** | GPIO21 | OUT | Request to Send (flow control) |

---

## 📡 RS485 Communication (UART1 for Modbus)
บอร์ดรองรับการสื่อสาร RS485 แบบ Modbus RTU ผ่าน **UART1** (สำหรับอุปกรณ์อื่น ๆ):
- **TX**: (configurable)
- **RX**: (configurable)
- **Baud**: 9600 (configurable)

## 🔌 Pinout Reference

| Component | GPIO | Function |
|-----------|------|----------|
| **RGB LED** | 38 | WS2812B NeoPixel |
| **Buzzer** | 46 | PWM (LEDC ch.0, 1kHz) |
| **LoRa DTU UART2 TX** | 17 | RS485 data send to LoRa DTU |
| **LoRa DTU UART2 RX** | 18 | RS485 data receive from LoRa DTU |
| **RS485 RTS** | 21 | Request to Send (flow control) |
| **DI1-DI8** | 4-11 | Digital Input |
| **DO Expander** | 41(SCL), 42(SDA) | TCA9554 I2C (addr 0x20) |
| **I2C SCL** | 41 | I2C Clock |
| **I2C SDA** | 42 | I2C Data |

---

## 🐛 Troubleshooting

### ❌ "LED red -1" ไม่ดับ
```
✅ Fixed in v1.0.0 - LEDOff() now properly stops animation
```

### ❌ "OUTPUT 5 ON" ติดทุกตัว
```
✅ Fixed in v1.0.0 - State synchronization issue resolved
```

### ❌ Serial Monitor ไม่ได้รับข้อมูล
```
1. ตรวจสอบ baud rate = 115200
2. ตรวจสอบ USB cable
3. กดปุ่ม RESET บนบอร์ด
4. ดู Device Manager / /dev/tty* สำหรับ port
```

### ❌ LoRa DTU ไม่รับส่งข้อมูล
```
1. ตรวจสอบสายต่อ RS485 ถูกต้องหรือไม่ (TX/RX ไม่สลับ)
2. ทั้งสองเครื่องต้องใช้ Frequency ช่องสัญญาณ และ Network ID เดียวกัน
   ตัวอย่าง: AT+TXCH=18 AT+RXCH=18 (ทั้งสองเครื่อง)
3. ทดสอบการเชื่อมต่อด้วย AT+VER เพื่อให้แน่ใจว่าโมดูลตอบสนอง
4. ตรวจสอบกำลัง Supply 12V ให้กับโมดูล LoRa DTU
5. ลองปรับ Spreading Factor (SF) ให้ต่ำ (SF=7) เพื่อความเร็ว หรือสูง (SF=12) เพื่อระยะไกล
```

### ❌ "+++" ไม่เข้าโหมด AT Command
```
1. ส่ง "+++" โดยไม่มี \r\n (Space character ก่อนหน้าจะ reset timeout)
2. รอ 1 วินาที ก่อนปิด/เปิด connectivity
3. ตรวจสอบ baud rate ของโมดูล = 9600 bps (default)
4. ลอง AT+EXIT เพื่อออก แล้วส่ง +++ อีกครั้ง
```

---

## 📚 Documentation

### ESP32-S3 & Relay Board
- 📖 [ESP32-S3 Official Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- 🌐 [Waveshare ESP32-S3-Relay-6CH Wiki](https://www.waveshare.com/wiki/ESP32-S3-Relay-6CH)
- 📱 [Waveshare ESP32-S3-Relay-6CH Product Page](https://www.waveshare.com/product/arduino/industrial-controller/esp32-s3-relay-6ch.htm)

### LoRa DTU Module
- 🌐 [Waveshare SX1262-LoRa-DTU-xF Wiki](https://www.waveshare.com/wiki/SX1262-LoRa-DTU-xF)
- 📱 [Waveshare SX1262-LoRa-DTU-xF Product Page](https://www.waveshare.com/product/iot-communication/long-range-wireless/nb-iot-lora/sx1262-lora-dtu-lf.htm)
- 📡 [LoRa Technology Overview](https://lora-alliance.org/)

### Protocols & Standards
- 📡 [Modbus RTU Protocol](https://en.wikipedia.org/wiki/Modbus)
- 📡 [RS485 Serial Communication](https://en.wikipedia.org/wiki/RS-485)

---

## 🤝 Contributing

ปรับปรุงโปรเจคนี้ได้เสมอ! 👋

ดูรายละเอียดใน [CONTRIBUTING.md](CONTRIBUTING.md)

---

## 📝 License

MIT License - ดู [LICENSE](LICENSE) สำหรับรายละเอียด

---

## 👨‍💻 Author

**Tenergy Innovation**
- 🌐 Website: [tenergy.co.th](https://tenergy.co.th)
- 📧 Email: info@tenergy.co.th
- 📍 Thailand

---

## ❓ FAQ

**Q: ต้องลงทะเบียน WiFi ไหม?**  
A: ไม่ต้อง - ใช้ได้ผ่าน Serial Command แบบ offline

**Q: สามารถขยายขึ้นไปอีก 8 relay ได้ไหม?**  
A: ได้ - เพิ่ม TCA9554 ตัวใหม่ที่ I2C address อื่น

**Q: LoRa DTU สามารถส่งข้อมูลไกลแค่ไหน?**  
A: ในสภาพแวดล้อมเปิดโล่ง (Line of Sight) ระยะประมาณ 5+ กิโลเมตร ขึ้นอยู่กับ Spreading Factor (SF), Antenna, และสภาพอากาศ

**Q: ต้องตั้งค่า Frequency Channel อะไรสำหรับประเทศไทย?**  
A: ประเทศไทยใช้ 868MHz (AT+TXCH=18 AT+RXCH=18) หรือตรวจสอบการ regulate ของประเทศของคุณ

**Q: Spreading Factor (SF) กี่เหมาะสม?**  
A: 
- SF7-9: เร็ว เหมาะสำหรับระยะใกล้/กลาง
- SF10-12: ช้า แต่ได้ระยะไกลและทนต่อการรบกวน

**Q: สามารถเชื่อมต่อหลายอุปกรณ์ LoRa พร้อมกันได้ไหม?**  
A: ได้ โปรโตคอล LoRa รองรับ Point-to-Multipoint ในโหมด Stream Mode

**Q: เทคนิคการโปรแกรมหรือใช้งาน?**  
A: เปิด Issues หรือสอบถามใน Discussion

---

**Happy Coding!** 🎉 If you found this helpful, give us a ⭐ on GitHub!
