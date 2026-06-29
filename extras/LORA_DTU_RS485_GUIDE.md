# 📡 SX1262-LoRa-DTU-xF RS485 Integration Guide

## 📌 **สรุปโมดูล SX1262-LoRa-DTU-xF (ภาษาไทย)**

### **1. คำอธิบายโมดูล**
- **ฟังก์ชัน**: หน่วยส่งรับข้อมูล LoRa แบบไร้สาย สนับสนุน RS485, RS232, RS422
- **เทคโนโลยี**: LoRa Modulation (ส่งได้ไกล 5 กม.) ทนต่อการรบกวน
- **ความไว**: -148dBm (รับสัญญาณได้ดีแม้สัญญาณอ่อน)
- **กำลังส่ง**: 22dBm (ใหญ่สุด)
- **ความปลอดภัย**: รองรับ AES Encryption
- **ความเร็ว**: ปรับได้ 1200-115200 bps

### **2. อินเตอร์เฟส (Interface)**
| อินเตอร์เฟส | ลักษณะ | ความเร็วเริ่มต้น | หลายช่วงเวลา |
|-----------|--------|--------|--------|
| **RS485** | Half-duplex | 9600 bps | ได้ |
| **RS232** | Full-duplex | 9600 bps | ได้ |
| **RS422** | Full-duplex | 9600 bps | ได้ |

### **3. โหมดการทำงาน 3 แบบ**

#### **3.1 Stream Mode (โหมดกระแส)**
- ส่งข้อมูลตามที่ได้รับจากอินพุต
- เหมาะสำหรับ Modbus ASCII
- การสื่อสาร: Point-to-Point / Point-to-Multipoint
```
ตัวอย่าง: Device A ส่ง "Hello World" → Device B รับ "Hello World"
```

#### **3.2 Packet Mode (โหมดแพ็คเก็ต)**
- ระบุที่อยู่ปลายทาง (2 bytes) + ช่องสัญญาณ (1 byte) + ข้อมูล
- เหมาะสำหรับ Modbus RTU
- ส่งในรูปแบบ Hex
```
ตัวอย่าง: FF FE 12 "Hello World"
  FF FE = ที่อยู่ปลายทาง
  12    = ช่องสัญญาณ (18)
  "Hello World" = ข้อมูล
```

#### **3.3 Relay Mode (โหมดสัญญาณถ่ายทำ)**
- ใช้ DTU เป็นเรลย์เพื่อขยายระยะทาง
- First-level Relay: 1 เรลย์ ขยายระยะ 2x
- Second-level Relay: 2 เรลย์ ขยายระยะ 3x
- เรลย์ไม่ส่งข้อมูลของตัวเอง

### **4. พารามิเตอร์เริ่มต้น**
```
┌─────────────────────────────────────────────────────┐
│ Spreading Factor (SF):    7 (ปรับได้ 7-12)         │
│ Bandwidth (BW):           0 (125KHz)               │
│ Code Rate (CR):           1 (4/5)                  │
│ Transmit Power (PWR):     22 dBm (สูงสุด)         │
│ Network ID (NETID):       0                        │
│ Tx Channel (TXCH):        18 (868MHz) / 23 (433MHz)│
│ Rx Channel (RXCH):        18 (868MHz) / 23 (433MHz)│
│ Address:                  0                        │
│ Interface:                3 (RS232 default)       │
│ Baud Rate (BAUD):         9600 bps                │
│ Data Bits:                8                        │
│ Stop Bits:                1                        │
│ Parity:                   None (N)                │
│ Mode:                     1 (Stream Mode)         │
│ LBT (Listen Before Talk): 0 (Disabled)            │
└─────────────────────────────────────────────────────┘
```

### **5. AT Commands ที่สำคัญ**
```bash
+++\r\n                     # เข้าโหมด AT Command
AT+EXIT\r\n                # ออกจากโหมด AT Command
AT+VER\r\n                 # ดูเวอร์ชั่น
AT+SF=7\r\n                # ตั้ง Spreading Factor = 7
AT+BW=0\r\n                # ตั้ง Bandwidth = 125KHz
AT+CR=1\r\n                # ตั้ง Code Rate = 4/5
AT+PWR=22\r\n              # ตั้ง Power = 22dBm
AT+MODE=1\r\n              # ตั้ง Mode = Stream (1=Stream, 2=Packet, 3=Relay)
AT+PORT=2\r\n              # ตั้ง Interface = RS485 (1=RS422, 2=RS485, 3=RS232)
AT+BAUD=9600\r\n           # ตั้ง Baud Rate = 9600 bps
AT+ADDR=0\r\n              # ตั้ง Address = 0
AT+TXCH=18\r\n             # ตั้ง Tx Channel = 18 (868MHz)
AT+RXCH=18\r\n             # ตั้ง Rx Channel = 18 (868MHz)
AT+RSSI=1\r\n              # เปิด RSSI Output
AT+RESTORE=1\r\n           # รีเซ็ตเป็นค่าเริ่มต้น
```

---

## 🔌 **การเชื่อมต่อฮาร์ดแวร์กับ ESP32-S3**

### **การต่อสาย RS485**
```
┌─────────────────────────────────────────────────────┐
│  SX1262-LoRa-DTU-xF        ESP32-S3-POE-ETH-8DI-8DO│
│  ────────────────────────────────────────────────── │
│  TA (RS485+) ──RS485 Module──> GPIO4 (UART2 RX)   │
│  TB (RS485-) ──RS485 Module──> GPIO5 (UART2 TX)   │
│  GND ───────────────────────> GND                 │
│  PWR (12V) ────────────────> DC12V (แยก)         │
└─────────────────────────────────────────────────────┘

หมายเหตุ: ต้องใช้ RS485 to TTL/UART Module เช่น:
- DSD TECH RS485 Module
- HW-0050 RS485 Serial Port Module
```

### **การกำหนด UART2**
- UART0: Serial Monitor (USB CDC)
- UART1: GPS หรือ Device อื่น
- UART2: RS485 LoRa DTU (RX=GPIO5, TX=GPIO4)

---

## 💻 **โค้ด ESP32-S3 สำหรับ RS485 LoRa DTU**

### **ส่วนที่ 1: Include และ Define**
```cpp
// เพิ่มใน Global Section
#define LORA_DTU_RX_PIN 5   // GPIO5 - UART2 RX
#define LORA_DTU_TX_PIN 4   // GPIO4 - UART2 TX
#define LORA_DTU_BAUD 9600  // LoRa DTU Baud Rate

// Buffer สำหรับเก็บข้อมูล
#define LORA_RX_BUFFER_SIZE 512
char loraRxBuffer[LORA_RX_BUFFER_SIZE];
uint16_t loraRxIndex = 0;
uint32_t loraLastRxTime = 0;
const uint32_t LORA_RX_TIMEOUT = 100; // ms

// LoRa DTU Status
struct {
  bool isConnected;
  uint16_t packetsReceived;
  uint16_t packetsSent;
  int16_t lastRSSI;
  char lastErrorMsg[100];
  uint32_t lastActivityTime;
} loraStatus = {false, 0, 0, 0, "", 0};
```

### **ส่วนที่ 2: ฟังก์ชันเริ่มต้น**
```cpp
void initLoRaDTU() {
  Serial.println("\n📡 Initializing LoRa DTU (RS485)...");
  
  // Initialize UART2 for LoRa DTU
  Serial2.begin(LORA_DTU_BAUD, SERIAL_8N1, LORA_DTU_RX_PIN, LORA_DTU_TX_PIN);
  
  if (!Serial2) {
    Serial.println("❌ UART2 initialization failed!");
    return;
  }
  
  Serial.println("✓ UART2 initialized (9600 bps)");
  
  // ทำความสะอาด buffer
  delay(100);
  while (Serial2.available()) {
    Serial2.read();
  }
  
  loraStatus.isConnected = true;
  loraStatus.lastActivityTime = millis();
  
  Serial.println("✓ LoRa DTU ready!");
  Serial.println("  Interface: RS485");
  Serial.println("  Mode: Stream (default)");
  Serial.println("  Baud: 9600 bps");
}

void loraReceiveCheck() {
  if (!loraStatus.isConnected) return;
  
  // อ่านข้อมูลจาก UART2
  while (Serial2.available() > 0) {
    char ch = Serial2.read();
    
    // ตรวจสอบขนาด buffer
    if (loraRxIndex >= LORA_RX_BUFFER_SIZE - 1) {
      loraRxIndex = 0; // รีเซ็ตหากเต็ม
      Serial.println("⚠️  LoRa RX buffer overflow!");
    }
    
    loraRxBuffer[loraRxIndex++] = ch;
    loraLastRxTime = millis();
    
    // ตรวจสอบการสิ้นสุดข้อมูล (newline หรือ timeout)
    if (ch == '\n' || (millis() - loraLastRxTime > LORA_RX_TIMEOUT)) {
      loraRxBuffer[loraRxIndex] = '\0';
      
      // ประมวลผลข้อมูลที่ได้
      processLoRaData(loraRxBuffer, loraRxIndex);
      
      // รีเซ็ต buffer
      loraRxIndex = 0;
      loraLastRxTime = 0;
    }
  }
}

void processLoRaData(char *data, uint16_t len) {
  if (len == 0) return;
  
  // ลบ whitespace และ newline
  String cleanData = String(data);
  cleanData.trim();
  
  if (cleanData.length() == 0) return;
  
  Serial.printf("📡 LoRa Received (%d bytes): %s\n", len, cleanData.c_str());
  
  loraStatus.packetsReceived++;
  loraStatus.lastActivityTime = millis();
  
  // ส่งข้อมูลไป MQTT
  if (client.connected()) {
    String topic = "loragreenergy/" + unitName + "/lora_rx";
    String payload = "{\"data\":\"" + cleanData + "\",\"time\":\"" + isoTimestamp() + "\",\"rssi\":" + String(loraStatus.lastRSSI) + "}";
    
    if (client.publish(topic.c_str(), payload.c_str())) {
      Serial.println("✓ Sent to MQTT");
    }
  }
}

void loraATCommand(String cmd) {
  if (!loraStatus.isConnected) {
    Serial.println("❌ LoRa DTU not connected!");
    return;
  }
  
  Serial.printf("⚙️  Sending AT Command: %s\n", cmd.c_str());
  
  // ส่ง command ไป DTU
  Serial2.print(cmd);
  if (!cmd.endsWith("\r\n")) {
    Serial2.print("\r\n");
  }
  
  delay(100);
  
  // อ่านการตอบกลับ
  String response = "";
  uint32_t startTime = millis();
  while (millis() - startTime < 1000) {
    if (Serial2.available()) {
      char ch = Serial2.read();
      response += ch;
    }
  }
  
  if (response.length() > 0) {
    Serial.printf("✓ Response: %s\n", response.c_str());
  }
}

void loraConfigureStream() {
  Serial.println("\n⚙️  Configuring LoRa DTU for Stream Mode...");
  
  // เข้าโหมด AT Command
  delay(100);
  Serial2.print("+++\r\n");
  delay(500);
  
  // ตั้งค่า Stream Mode
  loraATCommand("AT+MODE=1\r\n");     // Mode 1 = Stream
  loraATCommand("AT+PORT=2\r\n");     // Port 2 = RS485
  loraATCommand("AT+BAUD=9600\r\n");  // Baud 9600
  loraATCommand("AT+SF=7\r\n");       // SF=7
  loraATCommand("AT+BW=0\r\n");       // BW=125KHz
  loraATCommand("AT+PWR=22\r\n");     // Power=22dBm
  
  // ออกจากโหมด AT
  loraATCommand("AT+EXIT\r\n");
  
  delay(500);
  Serial.println("✓ Configuration complete!");
}

void loraSendData(String data) {
  if (!loraStatus.isConnected) {
    Serial.println("❌ LoRa DTU not connected!");
    return;
  }
  
  Serial.printf("📤 Sending via LoRa: %s\n", data.c_str());
  
  Serial2.print(data);
  if (!data.endsWith("\r\n")) {
    Serial2.print("\r\n");
  }
  
  loraStatus.packetsSent++;
  loraStatus.lastActivityTime = millis();
}
```

### **ส่วนที่ 3: การใช้งาน Setup และ Loop**
```cpp
// ใน setup()
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // ... other initialization code ...
  
  // LoRa DTU initialization
  initLoRaDTU();
  loraConfigureStream();  // Optional: ตั้งค่าเพิ่มเติม
  
  // ... continue with WiFi, MQTT setup ...
}

// ใน loop()
void loop() {
  // ... existing code ...
  
  // ตรวจสอบข้อมูลจาก LoRa DTU ทุก loop
  loraReceiveCheck();
  
  // Display LoRa status periodically
  static uint32_t lastStatusDisplay = 0;
  if (millis() - lastStatusDisplay > 30000) { // ทุก 30 วินาที
    Serial.printf("📊 LoRa Status: RX=%d, TX=%d, Activity=%dms ago\n", 
      loraStatus.packetsReceived,
      loraStatus.packetsSent,
      (uint32_t)(millis() - loraStatus.lastActivityTime));
    lastStatusDisplay = millis();
  }
  
  delay(10); // ให้ระบบ CPU หยุดพัก
}
```

---

## 📡 **ตัวอย่าง Stream Mode (Stream Transfer)**

### **ส่วน 1: Point-to-Point Communication**
```
Device A (ESP32-S3)              Device B (LoRa DTU)
┌──────────────────┐            ┌──────────────────┐
│ "Hello World" ──────────────→ │ "Hello World"    │
│                  │            │                  │
│ ← ─ ─ ─"Reply"─ ─ ─ ─ ─ ─ │ "Reply"          │
└──────────────────┘            └──────────────────┘

ตั้งค่าเหมือนกัน:
- Mode: 1 (Stream)
- Channel: 18 (868MHz)
- SF=7, BW=125KHz
```

### **ส่วน 2: Broadcast Communication**
```
Device A (ส่ง)   Device B (Relay, ADDR=0xFFFF)   Device C (รับ)
                      ↓
All devices on same channel can receive
```

---

## 📊 **ตารางเปรียบเทียบ: Stream vs Packet Mode**

| ลักษณะ | Stream Mode | Packet Mode |
|--------|-------------|-------------|
| **ที่อยู่** | ไม่ระบุ | ระบุใน 3 bytes แรก |
| **ข้อมูล** | ส่งตามที่รับ | Format Hex |
| **Modbus** | ASCII | RTU |
| **ความซับซ้อน** | ง่าย | ปานกลาง |
| **การใช้** | IoT Gateways | PLC/Sensors |

---

## 🛠️ **Troubleshooting**

### **ปัญหา: ไม่สามารถรับข้อมูล**
```
✓ ตรวจสอบ Baud Rate (9600 bps)
✓ ตรวจสอบการต่อสาย RS485
✓ ตรวจสอบ Channel ของทั้ง TX/RX
✓ ตรวจสอบ Power supply (12V)
```

### **ปัญหา: ข้อมูลขาดหรือผิดพลาด**
```
✓ ลด Bandwidth (BW=0 ← BW=1)
✓ เพิ่ม Spreading Factor (SF=7 → SF=9)
✓ เปิด LBT (AT+LBT=1)
✓ ตรวจสอบการรบกวน RF
```

### **ปัญหา: ระยะสั้น**
```
✓ เพิ่ม Power (PWR=22 สูงสุด)
✓ เพิ่ม Spreading Factor (SF=12)
✓ ลด Bandwidth (BW=0)
✓ ใช้ Relay Mode
```

---

## 📚 **ลิงก์อ้างอิง**
- [Waveshare SX1262-LoRa-DTU-xF Wiki](https://www.waveshare.com/wiki/SX1262-LoRa-DTU-xF)
- [SX1262 Datasheet](https://files.waveshare.com/upload/e/e1/DS_SX1261-2_V1.2.pdf)
- [LoRa Range Calculator](https://www.rfwireless-world.com/calculators/LoRaWAN-Range-calculator.html)

---

✅ **Next Step**: Copy โค้ด Section ที่ 1-3 ไปยัง `src/main.cpp` และ test LoRa communication!
