# ESP32-S3-Relay-6CH - Board Specifications & Features
## ข้อกำหนดทางเทคนิค บอร์ด ESP32-S3-Relay-6CH

---

## 📋 Overview / ภาพรวมของบอร์ด

**ESP32-S3-Relay-6CH** คือบอร์ดควบคุมรีเลย์ 6 ช่องสำหรับการใช้งานอุตสาหกรรม (Industrial Grade Relay Control Module) ที่ออกแบบโดย Waveshare โดยอยู่บนพื้นฐาน ESP32-S3 ที่มีความสามารถในการสื่อสารไร้สาย (WiFi, Bluetooth) และมีอินเตอร์เฟสสื่อสารแบบมีสาย (RS485) เหมาะสำหรับการควบคุมอุปกรณ์ไฟฟ้าต่างๆ ทั้งแรงดันสูง และการประยุกต์ใช้งาน IoT ขั้นสูง

---

## 🔧 Technical Specifications / ข้อกำหนดทางเทคนิค

### Processor / ตัวประมวลผล

| Parameter | Value |
|-----------|-------|
| **Chip** | ESP32-S3 Dual-Core |
| **Architecture** | Xtensa 32-bit LX7 |
| **Clock Speed** | 240 MHz (ปรับได้) |
| **Flash Memory** | 16 MB (QSPI) |
| **PSRAM** | 8 MB (DDR2) |
| **RTC Memory** | 8 KB internal SRAM |
| **ROM** | 384 KB |

### Wireless Connectivity / การเชื่อมต่อไร้สาย

| Feature | Specification |
|---------|--------------|
| **WiFi Standard** | IEEE 802.11 b/g/n (2.4 GHz) |
| **WiFi Modes** | AP (Access Point), STA (Station), P2P |
| **Bluetooth** | Bluetooth 5.0, BLE (Bluetooth Low Energy) |
| **Max TX Power** | +20 dBm |
| **Antenna** | External SMA connector (default: PCB antenna) |

### Relay Specifications / ข้อมูลรีเลย์

| Parameter | Specification |
|-----------|--------------|
| **Number of Channels** | 6 |
| **Contact Rating** | ≤ 10A @ 250VAC or ≤ 10A @ 30VDC |
| **Contact Form** | 1NO (Normally Open) + 1NC (Normally Closed) per channel |
| **Isolation Type** | Optocoupler isolation for each channel |
| **Response Time** | ~10ms (typical) |
| **Mechanical Life** | > 10,000,000 cycles |
| **Electrical Life** | > 100,000 cycles (resistive load) |

### RS485 Interface / อินเตอร์เฟส RS485

| Parameter | Specification |
|-----------|--------------|
| **Interface** | Isolated RS485 |
| **Connector** | Screw terminal (A, B, GND) |
| **Baud Rate** | 300 - 115,200 bps (configurable) |
| **Direction Control** | Hardware automatic (no manual control needed) |
| **Protection** | TVS diode, surge protection, ESD protection |
| **Matching Resistor** | 120Ω onboard (jumper selectable) |
| **Max Cable Length** | ~1200 m (depends on baud rate) |

### Power Supply / จ่ายไฟ

| Parameter | Specification |
|-----------|--------------|
| **Input Voltage** | 7V - 36V DC (wide range) |
| **USB Power** | 5V via USB Type-C |
| **Power Consumption (Idle)** | ~200mA |
| **Power Consumption (All Relays ON)** | ~1.5A (typical) |
| **Isolated Supply** | Yes (unibody design) |
| **Protection** | Reverse polarity protection, overvoltage protection |

### GPIO & Onboard Features / GPIO และฟีเจอร์ในตัว

| Feature | GPIO | Function |
|---------|------|----------|
| **Relay 1 Control** | GPIO1 | Relay #1 on/off control |
| **Relay 2 Control** | GPIO2 | Relay #2 on/off control |
| **Relay 3 Control** | GPIO42 | Relay #3 on/off control |
| **Relay 4 Control** | GPIO41 | Relay #4 on/off control |
| **Relay 5 Control** | GPIO45 | Relay #5 on/off control |
| **Relay 6 Control** | GPIO46 | Relay #6 on/off control |
| **RGB LED** | GPIO38 | WS2812B NeoPixel LED (1 color LED) |
| **Buzzer** | GPIO21 | Passive buzzer control |
| **RS485 RX** | GPIO5 | RS485 receive data |
| **RS485 TX** | GPIO4 | RS485 transmit data |
| **RESET Button** | - | Hardware reset |
| **BOOT Button** | - | Enter download mode |

### Physical Specifications / ลักษณะทางกายภาพ

| Parameter | Value |
|-----------|-------|
| **Dimensions** | 145 × 90 × 40 mm |
| **Enclosure** | Rail-mount ABS protective case |
| **Rail Mounting** | 35mm DIN rail compatible |
| **Weight** | ~200g (estimated) |
| **Connector Types** | Screw terminals (power, relays, RS485) |
| **Operating Temp** | -10°C ~ +50°C |
| **Storage Temp** | -20°C ~ +70°C |
| **Humidity** | 10% - 90% (non-condensing) |

---

## 🎯 Key Features / ฟีเจอร์หลัก

### 1. **Dual Wireless Connectivity**
- ✅ WiFi 2.4GHz with AP/STA modes
- ✅ Bluetooth 5.0 & BLE for mobile apps
- ✅ Both can operate simultaneously

### 2. **Industrial-Grade Protection**
- ✅ Optocoupler isolation on every relay channel
- ✅ Digital isolation for signal protection
- ✅ Power supply isolation (unibody design)
- ✅ TVS diodes for surge suppression
- ✅ Multiple protection circuit layers

### 3. **Rich Onboard Resources**
- ✅ WS2812B RGB LED (programmable colors)
- ✅ Passive buzzer (PWM-controlled)
- ✅ Power indicator LED
- ✅ RS485 TX/RX indicator LEDs

### 4. **Expandability**
- ✅ 40-pin Pico HAT compatible header
- ✅ Can add: RTC, CAN, RS232, LoRa, sensors
- ✅ Custom extension support

### 5. **Development Flexibility**
- ✅ Arduino IDE support
- ✅ MicroPython support
- ✅ PlatformIO support
- ✅ Comprehensive SDK & examples

---

## 📡 Communication Protocols / โปรโตคอลการสื่อสาร

### Supported Protocols

1. **WiFi**
   - HTTP/HTTPS (web server)
   - MQTT (IoT cloud)
   - Custom TCP/UDP

2. **Bluetooth/BLE**
   - GATT protocol
   - Custom services & characteristics
   - Mobile app control

3. **RS485**
   - Modbus RTU
   - Custom serial protocols
   - Full duplex communication

4. **Web Interface**
   - RESTful APIs
   - WebSocket support
   - Real-time data

---

## 🔌 Pinout Reference / ข้อมูลพิน

### Relay Control Pins

| Relay | GPIO | Active Level |
|-------|------|--------------|
| Relay 1 | GPIO1 | HIGH |
| Relay 2 | GPIO2 | HIGH |
| Relay 3 | GPIO42 | HIGH |
| Relay 4 | GPIO41 | HIGH |
| Relay 5 | GPIO45 | HIGH |
| Relay 6 | GPIO46 | HIGH |

### Serial Port Configuration

| Function | GPIO | UART |
|----------|------|------|
| RS485 RX | GPIO5 | UART2 |
| RS485 TX | GPIO4 | UART2 |
| Baud Rate | - | 9600 (default) |

### Status Indicators

| LED | Purpose | Color |
|-----|---------|-------|
| PWR | Power indicator | Red (always on with power) |
| TXD | RS485 transmit | Green (blinks during TX) |
| RXD | RS485 receive | Blue (blinks during RX) |
| RGB | Status/Custom | Programmable (1 NeoPixel) |

---

## 💡 Use Cases / กรณีการใช้งาน

1. **Industrial Automation**
   - Motor control via wireless commands
   - Equipment interlocking systems
   - Pump/Valve remote operation

2. **Smart Home/Building**
   - Lighting control
   - Power outlet management
   - HVAC system control

3. **Remote Monitoring**
   - Equipment status reporting
   - Alert notifications via MQTT/Telegram
   - Data logging to cloud

4. **IoT Applications**
   - Sensor data collection + relay control
   - Machine learning edge inference
   - Multi-device coordination

5. **Time-Based Automation**
   - Scheduled relay switching
   - Timer-based operations
   - Cron-like task scheduling

---

## 🔒 Safety Features / ฟีเจอร์ความปลอดภัย

1. **Electrical Safety**
   - Optocoupler isolation prevents high-voltage interference
   - TVS diodes protect against surge voltages
   - Reverse polarity protection on power input

2. **Thermal Safety**
   - Operating temperature monitoring
   - Thermal shutdown protection
   - Proper heat dissipation design

3. **Software Safety**
   - Watchdog timer for automatic recovery
   - Error handling & logging
   - Graceful degradation on sensor failure

4. **Data Security**
   - HTTPS support for web communication
   - WPA2 encryption for WiFi
   - Optional API key authentication

---

## 📚 Additional Resources / ทรัพยากรเพิ่มเติม

- **Official Wiki**: https://www.waveshare.com/wiki/ESP32-S3-Relay-6CH
- **Product Page**: https://www.waveshare.com/product/arduino/industrial-controller/esp32-s3-relay-6ch.htm
- **Demo Code**: https://github.com/waveshareteam
- **Schematic**: Available on Waveshare website
- **Datasheet**: Available on product page

---

## 🎓 Learning Path / เส้นทางการเรียนรู้

### Beginner
1. Upload basic relay control example
2. Test relay switching via serial commands
3. Configure WiFi and LED colors

### Intermediate
4. Integrate MQTT broker connectivity
5. Build web-based control interface
6. Add Bluetooth mobile app control

### Advanced
7. Multi-device coordination
8. Time-based automation & scheduling
9. Cloud integration & data analytics
10. Custom expansion via Pico HAT

