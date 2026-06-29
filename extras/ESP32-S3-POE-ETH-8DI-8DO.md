# ESP32-S3-POE-ETH-8DI-8DO บอร์ดควบคุม Industrial

## 📋 บทนำ

บอร์ด **ESP32-S3-POE-ETH-8DI-8DO** เป็นบอร์ดควบคุมอุตสาหกรรมที่ทรงพลังจาก Waveshare ออกแบบมาสำหรับการควบคุมและจัดการอุปกรณ์ IoT ขนาดใหญ่ บอร์ดนี้รวมเอา:
- **8 ช่อง Digital Input (DI)** - รับสัญญาณดิจิตัล
- **8 ช่อง Digital Output (DO)** - ควบคุมอุปกรณ์เอาต์พุต
- **Ethernet พร้อม PoE** - เชื่อมต่อเครือข่ายแบบ Power over Ethernet
- **RS485 และ CAN** - สำหรับการสื่อสารในสภาวะอุตสาหกรรม

---

## 🔧 ข้อมูลจำเพาะด้านฮาร์ดแวร์

### โปรเซสเซอร์หลัก
| รายการ | ข้อมูล |
|--------|--------|
| **ไมโครคอนโทรลเลอร์** | ESP32-S3 (WROOM-1U-N16R8) |
| **ประมวลผล** | Xtensa 32-bit LX7 Dual-Core |
| **ความเร็ว** | ขึ้นถึง 240 MHz |
| **หน่วยความจำ Flash** | 16 MB |
| **หน่วยความจำ PSRAM** | 8 MB |

### การเชื่อมต่อไร้สาย
| รายการ | ข้อมูล |
|--------|--------|
| **WiFi** | 2.4GHz (802.11 b/g/n) |
| **Bluetooth** | 5.0 (Low Energy) |
| **เสาอากาศ** | SMA Female Connector |

### พอร์ตและอินเตอร์เฟส
| อินเตอร์เฟส | ข้อมูล |
|------------|--------|
| **USB** | Type-C (5V Power + Communication) |
| **Ethernet** | PoE Port (10/100 Mbps) |
| **RS485** | Isolated (Screw Terminal) |
| **CAN** | Isolated (Screw Terminal) |
| **TF Card** | External Storage |

---

## 🔌 ระบบดิจิตัล Input/Output

### Digital Output (DO) - 8 ช่อง
- **แรงดันอินพุต**: 5V ~ 40V
- **ประเภท**: Open-Drain Output
- **ความสามารถ**: 500 mA per channel
- **ฉนวนไฟฟ้า**: Optocoupler Isolation
- **ลักษณะเฉพาะ**: 
  - ใช้ Darlington Transistor
  - มีไดโอดป้องกัน Flyback
  - สามารถขับ Relay โดยตรง

### Digital Input (DI) - 8 ช่อง
- **แรงดันอินพุต**: 5V ~ 36V
- **ประเภท**:
  - Passive Input (สัญญาณปกติ)
  - Active Input (PNP หรือ NPN)
- **ฉนวนไฟฟ้า**: Bi-directional Optocoupler Isolation
- **ลักษณะเฉพาะ**:
  - ป้องกันสัญญาณรบกวนจากวงจรภายนอก
  - สามารถสำรวจสถานะ DI ผ่านโปรแกรม

---

## 🌐 การเชื่อมต่อ Ethernet

### โมดูล Ethernet
- **ชิป**: W5500
- **ความเร็ว**: 10/100 Mbps
- **มาตรฐาน**: IEEE 802.3af (PoE)
- **วิธีเชื่อมต่อ**: SPI Interface

### PoE (Power over Ethernet)
- **แหล่งพลังงาน**: ได้รับพลังงานและสัญญาณผ่านสายเดียว
- **มาตรฐาน**: IEEE 802.3af compliant
- **ข้อดี**:
  - ลดความซับซ้อนในการวางสาย
  - เหมาะสำหรับติดตั้งในห้องเครื่องหรือจุดสูง

### การป้องกัน
- **TVS Diode** - ป้องกันแรงดันสูง
- **Surge Protection** - ป้องกันจากการเพิ่มขึ้นของกระแสน้อย
- **ESD Protection** - ป้องกันการปล่อยไฟฟ้าแบบ Static

---

## 📡 การสื่อสารอื่นๆ

### RS485 (Isolated)
- **ประเภท**: Industrial Serial Communication
- **พอร์ต**: Screw Terminal
- **ความต้านทาน**: 120Ω Matching Resistor (Jumper Selectable)
- **ฉนวนไฟฟ้า**: Isolated
- **การควบคุม**: Automatic Direction Control

### CAN Bus (Isolated)
- **ประเภท**: Controller Area Network
- **พอร์ต**: Screw Terminal
- **ความต้านทาน**: 120Ω Matching Resistor (Jumper Selectable)
- **ฉนวนไฟฟ้า**: Isolated
- **การควบคุม**: Hardware Automatic Control

---

## ⚡ ระบบจ่ายไฟ

### แหล่งพลังงาน
- **พอร์ต USB Type-C**: 5V
- **Screw Terminal**: 7V ~ 36V (Wide Voltage Input)
- **PoE Ethernet**: IEEE 802.3af compliant

### ป้องกัน
- **Power Isolation** - ฉนวนไฟฟ้าระหว่างแหล่งพลังงาน
- **Stable Isolated Voltage** - ไม่ต้องมีแหล่งพลังงานเพิ่มเติมสำหรับ Isolated Terminal

---

## 🎛️ ตัวบ่งชี้และส่วนควบคุม

### LED Indicators
| ตัวบ่งชี้ | สี | ฟังก์ชัน |
|---------|-----|---------|
| **PWR** | Red | ไฟสัญญาณ USB เชื่อมต่อและตรวจพบแรงดัน |
| **TXD** | Green | ข้อมูลส่ง (RS485/CAN) |
| **RXD** | Blue | ข้อมูลรับ (RS485/CAN) |
| **RGB** | RGB | WS2812 Programmable LED |

### ปุ่มควบคุม
| ปุ่ม | ฟังก์ชัน |
|-------|---------|
| **RESET** | รีเซ็ตบอร์ด |
| **BOOT** | เข้าโหมดการโหลด Firmware |

### ส่วนประกอบอื่น
- **Buzzer** - สัญญาณเสียง (Programmable)
- **RTC Battery Holder** - ถ่านสำรอง RTC (1220 Size, 3V~3.3V Rechargeable)

---

## 🛡️ มาตรการปลอดภัยของ Relay

### 1. การจับคู่โหลด
- ตรวจสอบให้แน่ใจว่าแรงดันและกระแสของ Relay ตรงกับโหลด
- ไม่ใช้ยาใจเกินความสามารถที่กำหนด
- สำหรับโหลดแบบ Inductive (มอเตอร์, คอยล์, หลอดไฟ) ให้เลือก Relay ที่มีอัตรากระแสเพียงพอ

### 2. การป้องกัน Short Circuit
- ติดตั้ง Fuse หรือ Circuit Breaker
- ตรวจสอบการต่อสาย ไม่ให้มี Short Circuit
- เลือก Fuse ที่มีอัตรากระแสเหมาะสม

### 3. การป้องกัน Arc & Switching
- ใช้ RC Snubber หรือ Varistor
- ลดการเสื่อมสภาพของ Contact
- ป้องกันการเชื่อมติดของ Contact

### 4. สภาวะแวดล้อม
- ไม่ใช้ในสภาวะชื้น สูง อบอุ่น หรือติดไฟได้
- ติดตั้งให้แน่นอน หลีกเลี่ยงการสั่นสะเทือน
- เก็บจากสนามแม่เหล็กที่แรงระหว่างการปิด Relay

### 5. ตัดไฟก่อนบำรุงรักษา
- **CRITICAL**: ตัดไฟสำหรับการบำรุงรักษาหรือเปลี่ยน Relay
- ตรวจสอบและรีเซ็ตสถานะ Relay หลังจากเปิดไฟ

---

## 💻 สภาวะแวดล้อมการพัฒนา

### เครื่องมือที่รองรับ
- **Arduino IDE** (แนะนำ) - ง่ายต่อการเรียนรู้
- **VS Code** - พัฒนาขั้นสูง
- **Thonny** - สำหรับ MicroPython
- **PlatformIO** - ในที่นี้ใช้สำหรับโปรเจ็คนี้

### การตั้งค่า Arduino IDE
1. ติดตั้ง Arduino IDE จากเว็บไซต์อย่างเป็นทางการ
2. ติดตั้ง ESP32 Board Manager (v3.0.0 หรือสูงกว่า)
3. ติดตั้ง Libraries ที่จำเป็น:
   - ArduinoJson (v6.21.4)
   - PubSubClient (v2.8.0)
   - NTPClient (v3.2.1)

### ตั้งค่าพารามิเตอร์โปรเจ็ค
```
Board: ESP32-S3-DevKitC-1
Flash: 16MB
PSRAM: OPI PSRAM
CPU Freq: 240MHz
Upload Speed: 921600
```

---

## 📚 ตัวอย่าง Demo และการใช้งาน

### Demo 1: MAIN_WIFI_AP
**วัตถุประสงค์**: ควบคุม 8 Relay ผ่าน WiFi, Bluetooth, Ethernet, RS485

**ลักษณะ**:
- WiFi AP Mode (หรือ STA Mode)
- Web Page Interface
- Serial Communication via Bluetooth
- RS485 Modbus Protocol
- Status via RGB LED

**โค้ด Key Function**:
- `Relay_Analysis()` - วิเคราะห์คำสั่งและควบคุม Relay
- `WIFI_Init()` - ตั้งค่า Access Point และ Web Server
- `Bluetooth_SendData()` - ส่งข้อมูลผ่าน Bluetooth
- `DIN_Init()` - เริ่มต้น Digital Input

### Demo 2: MAIN_WIFI_STA
**วัตถุประสงค์**: เชื่อมต่อ WiFi แบบ Station Mode

**ลักษณะ**:
- STA Mode (เชื่อมเครือข่าย WiFi ที่มีอยู่)
- เหมาะสำหรับใช้ในเครือข่ายภายในสำนัก
- Control ผ่าน Mobile Phone ในเครือข่ายเดียวกัน

### Demo 3: MAIN_MQTT_ALL
**วัตถุประสงค์**: รวมการควบคุมแบบ Hybrid (Local + Cloud)

**ลักษณะ**:
- RS485 Interface Control
- Bluetooth Control
- Web Page Control (Local Network)
- Waveshare Cloud Control (Remote)
- MQTT Protocol

---

## 🎯 การใช้งาน GPIO

### ตัวอย่างการควบคุม Output
```cpp
// ตั้งค่า Pin 
pinMode(GPIO_PIN_CH1, OUTPUT);  // Channel 1

// เปิด Output
digitalWrite(GPIO_PIN_CH1, HIGH);

// ปิด Output
digitalWrite(GPIO_PIN_CH1, LOW);

// Toggle
digitalToggle(GPIO_PIN_CH1);
```

### ตัวอย่างการอ่าน Input
```cpp
// ตั้งค่า Pin
pinMode(GPIO_PIN_DIN1, INPUT);

// อ่านสถานะ
int state = digitalRead(GPIO_PIN_DIN1);
```

### PWM Buzzer
```cpp
// ตั้งค่า PWM
ledcSetup(0, Frequency, Resolution);
ledcAttachPin(GPIO_PIN_Buzzer, 0);

// ควบคุม Volume (0-255)
ledcWrite(0, dutyCycle);
```

---

## 📦 เครื่องมือและทรัพยากรที่จำเป็น

### Hardware
- ESP32-S3-POE-ETH-8DI-8DO x1
- USB Cable (Type-A to Type-C) x1
- สายเครือข่าย Ethernet RJ45
- อุปกรณ์ที่ต้องควบคุม (Relay, Motor, Sensor)

### Software
- Arduino IDE หรือ PlatformIO
- CH34XSER Driver (สำหรับ macOS)
- Libraries: NTPClient, WiFi, Bluetooth, Ethernet

### Optional Components
- Modbus RTU Relay
- USB to RS485 Converter
- CAN Adapter
- PoE Injector/Switch

---

## ⚙️ คำสั่งควบคุมพื้นฐาน

### Control via Bluetooth/WiFi/RS485
| คำสั่ง | ความหมาย | ตัวอย่าง |
|--------|---------|---------|
| `0x31` - `0x38` | Toggle Channel 1-8 | `0x31` = Toggle CH1 |
| `0x39` | ปิดทั้งหมด (All OFF) | `0x39` |
| `0x30` | เปิดทั้งหมด (All ON) | `0x30` |

### Protocol RS485
- Modbus RTU Protocol
- Baud Rate: Configurable
- Enable 120Ω Resistor via Jumper if needed

---

## 🔍 Troubleshooting

### ปัญหา: ไม่สามารถอัปโหลด Firmware
**แนวทาง**:
1. กด BOOT ยาวๆ + RESET พร้อมกัน
2. ปล่อย RESET ก่อน แล้วปล่อย BOOT
3. บอร์ดจะเข้า Download Mode

### ปัญหา: RS485 ไม่ตอบสนอง
**แนวทาง**:
- ตรวจสอบ Jumper 120Ω ที่ Terminating Resistor
- บอร์ด RS485 ต้องมี Matching Resistor เช่นกัน

### ปัญหา: ไม่มี Output หลังการอัปโหลด
**แนวทาง**:
- สำหรับ Direct USB Output: ใช้ `printf()` สำหรับหรือเปิด USB CDC On Boot
- สำหรับ UART to USB: `printf()` และ `Serial` ทั้งสองใช้ได้

### ปัญหา: I2C TCA9554PWR ไม่ตอบสนอง
**แนวทาง**:
- ตรวจสอบที่อยู่ I2C: `0x20`
- ใช้ I2C Scanner เพื่อทดสอบ

---

## 🚀 การพัฒนาเพิ่มเติม

### การทำให้ Firmware โปรแกรมด้วยตัวเอง
1. ปรับปรุง Demo Code ตามความต้องการ
2. เพิ่มตรรกะการจัดการ Edge Case
3. Test ในสภาวะงานจริง

### Integration กับระบบสมัยใหม่
- **Home Assistant**: ควบคุมผ่าน Modbus RTU
- **MQTT Cloud**: ส่งข้อมูลไปยัง Waveshare Cloud
- **Mobile App**: ผ่าน RESTful API

---

## 📞 ทรัพยากรและสนับสนุน

### เว็บไซต์ที่เกี่ยวข้อง
- **Product Page**: https://www.waveshare.com/esp32-s3-poe-eth-8di-8do.htm
- **Wiki**: https://www.waveshare.com/wiki/ESP32-S3-POE-ETH-8DI-8DO
- **Arduino Docs**: https://docs.espressif.com/projects/arduino-esp32/

### การติดต่อสนับสนุน
- **Technical Support**: https://service.waveshare.com/
- **Working Time**: 9 AM - 6 PM GMT+8 (Monday to Friday)
- **Response Time**: 1-2 วันทำการ

---

## 📋 ตารางเปรียบเทียบผลิตภัณฑ์

| รุ่น | DI | DO | Relay | RS485 | CAN | Network | PoE | RTC |
|------|----|----|-------|-------|-----|---------|-----|-----|
| ESP32-S3-POE-ETH-8DI-8DO | 8 | 8 | - | ✓ | ✓ | ✓ | ✓ | ✓ |
| ESP32-S3-POE-ETH-8DI-8RO | 8 | - | 8 | ✓ | - | ✓ | ✓ | ✓ |
| ESP32-S3-ETH-8DI-8RO | 8 | - | 8 | ✓ | - | ✓ | - | ✓ |
| ESP32-S3-Relay-6CH | - | - | 6 | ✓ | - | - | - | - |

---

## 🔐 ข้อมูลด้านความปลอดภัย

### การป้องกันต่างๆ
- **Optocoupler Isolation** - ฉนวนไฟฟ้า DI/DO
- **TVS Diode** - ป้องกัน Surge Voltage
- **Digital Isolation** - ป้องกันสัญญาณรบกวน
- **Power Isolation** - ฉนวนไฟฟ้าแหล่งพลังงาน

### ข้อเตือน
- ⚠️ ตัดไฟก่อนการบำรุงรักษา
- ⚠️ ไม่ถอดรายละเอียดภายในเครื่อง
- ⚠️ ใช้เฉพาะในสภาวะที่ปลอดภัย
- ⚠️ ติดตั้งโดยผู้เชี่ยวชาญเท่านั้น

---

**เอกสารนี้จัดทำเมื่อ**: 2026-06-24  
**เวอร์ชัน**: 1.0  
**ภาษา**: Thai (ไทย)

*สำหรับข้อมูลอัปเดตและสารสนเทศเพิ่มเติม โปรดเยี่ยมชม Waveshare Wiki*
