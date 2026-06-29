# 📡 RS485 Implementation Guide 
## ESP32-S3-POE-ETH-8DI-8DO & SX1262 LoRa DTU Communication

### Based on Waveshare Official Example Code Analysis

---

## 🎯 Overview / ภาพรวม

This guide explains the proper implementation of RS485 communication between the ESP32-S3-POE-ETH-8DI-8DO board and the SX1262 LoRa DTU module, based on the official Waveshare example code.

**ภาษาไทย**: คู่มือนี้อธิบายการใช้งาน RS485 ที่ถูกต้องระหว่างบอร์ด ESP32-S3-POE-ETH-8DI-8DO และโมดูล SX1262 LoRa DTU โดยอิงตามโค้ดตัวอย่างอย่างเป็นทางการของ Waveshare

---

## ⚙️ GPIO Configuration

### GPIO Pin Mapping / การแมปพิน GPIO

```cpp
#define LORA_DTU_TX_PIN    17    // GPIO17 - RS485 Transmitter (UART1 TXD)
#define LORA_DTU_RX_PIN    18    // GPIO18 - RS485 Receiver (UART1 RXD)
#define LORA_DTU_DE_RE_PIN 21    // GPIO21 - RS485 Direction Enable/Receive Enable
```

| GPIO | Name | Function | Usage |
|------|------|----------|-------|
| 17 | TXD1 | UART1 TX | Send data to RS485 (ส่งข้อมูล) |
| 18 | RXD1 | UART1 RX | Receive data from RS485 (รับข้อมูล) |
| 21 | TXD1EN | Direction Control | Control transmitter/receiver enable (ควบคุม TX/RX) |

---

## ✅ Correct RS485 Initialization (FIXED)

### ❌ Previous WRONG Approach (Manual GPIO Control)
```cpp
// ❌ WRONG - This doesn't work properly!
digitalWrite(LORA_DTU_DE_RE_PIN, HIGH);   // TX mode
Serial1.print(command);
digitalWrite(LORA_DTU_DE_RE_PIN, LOW);    // RX mode
delay(300);
```

**Problem**: Manual GPIO control is unreliable because:
- No automatic direction switching
- GPIO state may not match actual UART operation
- Timing issues cause missed responses
- Half-duplex collision detection not implemented

---

### ✅ Correct Approach (Using UART_MODE_RS485_HALF_DUPLEX)

```cpp
// ✅ CORRECT - Use hardware RS485 mode
void setup() {
    Serial.begin(115200);
    
    // Initialize UART1 with RS485 pins
    Serial1.begin(9600, SERIAL_8N1, LORA_DTU_RX_PIN, LORA_DTU_TX_PIN);
    
    // ✅ Set GPIO21 as direction control pin
    if (!Serial1.setPins(-1, -1, -1, LORA_DTU_DE_RE_PIN)) {
        Serial.println("Failed to set RS485 direction pin");
    }
    
    // ✅ Enable RS485 half-duplex mode (AUTOMATIC direction control)
    if (!Serial1.setMode(UART_MODE_RS485_HALF_DUPLEX)) {
        Serial.println("Failed to set RS485 mode");
    } else {
        Serial.println("✅ RS485 Half-Duplex mode enabled");
    }
}
```

**Advantages of Hardware RS485 Mode**:
- ✅ Automatic direction switching
- ✅ Hardware-level half-duplex handling
- ✅ Collision detection built-in
- ✅ No manual GPIO timing needed
- ✅ Better reliability and performance
- ✅ Proper CRC handling for Modbus RTU

---

## 📨 Sending Data via RS485

### Simple Send Function
```cpp
void sendRS485Command(String cmd) {
    // Clear any pending data
    while (Serial1.available()) Serial1.read();
    delay(50);
    
    // ✅ UART_MODE_RS485_HALF_DUPLEX handles direction automatically!
    Serial1.print(cmd);
    Serial1.flush();
    
    // Wait for direction switch and module response
    delay(100);
}
```

### AT Command Example
```cpp
// Send AT command with automatic mode handling
void sendATCommand(String atCmd) {
    if (!atCmd.endsWith("\r\n")) {
        atCmd += "\r\n";
    }
    
    Serial.printf("📤 Sending: %s\n", atCmd.c_str());
    sendRS485Command(atCmd);
}

// Usage
void loop() {
    sendATCommand("AT");           // Basic test
    delay(500);
    sendATCommand("AT+VER");       // Get version
    delay(500);
}
```

---

## 📬 Receiving Data from RS485

### Read with Timeout
```cpp
String readRS485Response(uint32_t timeoutMs = 2000, uint32_t silenceMs = 200) {
    String response = "";
    uint32_t startTime = millis();
    uint32_t lastByteTime = startTime;
    
    while (millis() - startTime < timeoutMs) {
        if (Serial1.available()) {
            char ch = Serial1.read();
            response += ch;
            lastByteTime = millis();
            
            // Early exit on newline
            if (ch == '\n') break;
        } else {
            // Check silence timeout
            if (response.length() > 0 && 
                (millis() - lastByteTime > silenceMs)) {
                break;  // Complete response received
            }
        }
        delay(2);
    }
    
    return response;
}
```

### Display Response (HEX + TEXT)
```cpp
void displayResponse(String response) {
    Serial.printf("📥 Response (%d bytes):\n", response.length());
    
    // Show HEX
    Serial.print("   [HEX]: ");
    for (int i = 0; i < response.length(); i++) {
        Serial.printf("%02X ", (unsigned char)response[i]);
    }
    Serial.println();
    
    // Show TEXT
    Serial.print("   [TEXT]: ");
    for (int i = 0; i < response.length(); i++) {
        char c = response[i];
        if (c >= 32 && c <= 126) {
            Serial.print(c);
        } else {
            Serial.printf("[%02X]", c);
        }
    }
    Serial.println();
}
```

---

## 🔄 AT Command Mode (SX1262 LoRa DTU)

### Entering AT Mode
```cpp
void enterATMode() {
    // Default mode is Stream Mode - must enter AT mode first!
    
    Serial.println("Entering AT Command Mode...");
    
    // Send +++ to enter AT mode
    Serial1.print("+++\r\n");
    Serial1.flush();
    delay(500);
    
    // Clear response
    while (Serial1.available()) Serial1.read();
    
    Serial.println("✅ AT mode activated");
}
```

### AT Commands Reference / ตารางอ้างอิงคำสั่ง AT

#### Mode Entry/Exit
```
+++\r\n                     Enter AT command mode
AT+EXIT\r\n                Exit AT command mode
AT\r\n                     Basic test (returns OK)
AT+VER\r\n                Get software version
AT+HELP\r\n               Show help menu
```

#### Port Configuration (CRITICAL)
```
AT+PORT=2\r\n              Set port to RS485 (1=RS232, 2=RS485)
AT+BAUD=9600\r\n           Serial baud rate (default 9600)
AT+COMM="8N1"\r\n          Data format: 8 bits, No parity, 1 stop bit
```

#### LoRa Mode Configuration
```
AT+MODE=1\r\n              Stream mode (default, recommended for simple data)
AT+MODE=2\r\n              Packet mode (for Modbus RTU)
AT+MODE=3\r\n              Relay mode
```

#### LoRa Channel Configuration (CRITICAL for communication)
```
AT+TXCH=18\r\n             Transmit channel (must match receiver)
AT+RXCH=18\r\n             Receive channel (must match transmitter)
AT+ADDR=0\r\n              Device address (for device identification)
```

#### LoRa RF Parameters
```
AT+SF=7\r\n                Spreading factor (7~12, higher=longer range, slower)
AT+BW=0\r\n                Bandwidth (0=125KHz, 1=250KHz, 2=500KHz)
AT+CR=1\r\n                Coding rate (1=4/5, 2=4/6, 3=4/7, 4=4/8)
AT+PWR=22\r\n              RF Power (10~22 dBm, 22 is maximum)
```

**ภาษาไทย**: 
- AT+TXCH / AT+RXCH ต้องตั้งค่าให้ตรงกันระหว่างอุปกรณ์
- AT+ADDR ใช้เพื่อระบุตัวตัวของอุปกรณ์
- AT+COMM ต้องตั้ง "8N1" สำหรับ RS485

---

## 🎯 Complete Example Code

```cpp
#define LORA_DTU_TX_PIN    17
#define LORA_DTU_RX_PIN    18
#define LORA_DTU_DE_RE_PIN 21

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // ✅ Initialize with RS485 half-duplex mode
    Serial1.begin(9600, SERIAL_8N1, LORA_DTU_RX_PIN, LORA_DTU_TX_PIN);
    Serial1.setPins(-1, -1, -1, LORA_DTU_DE_RE_PIN);
    Serial1.setMode(UART_MODE_RS485_HALF_DUPLEX);
    
    Serial.println("✅ RS485 Ready");
}

void loop() {
    // Example: Send AT and read response
    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        
        if (cmd.length() > 0) {
            // Add CRLF if needed
            if (!cmd.endsWith("\r\n")) cmd += "\r\n";
            
            // Send command
            Serial1.print(cmd);
            Serial1.flush();
            
            // Read response
            delay(100);
            String response = readRS485Response(2000);
            
            // Display
            if (response.length() > 0) {
                displayResponse(response);
            } else {
                Serial.println("❌ TIMEOUT - No response");
            }
        }
    }
}
```

---

## 🔧 Troubleshooting / การแก้ไขปัญหา

| Problem / ปัญหา | Cause / สาเหตุ | Solution / วิธีแก้ |
|---|---|---|
| No response | AT mode not entered | Send `+++\r\n` first |
| TIMEOUT | Module not powered | Check 5V power supply |
| Garbled data | Baud rate mismatch | Verify 9600 bps |
| One-way comm | GPIO21 not connected | Check DE/RE pin connection |
| Dropped packets | Timing issues | Use UART_MODE_RS485_HALF_DUPLEX |
| Wrong colors | RGB order incorrect | Use (G, R, B) not (R, G, B) |

---

## 📚 Reference / อ้างอิง

### Waveshare Official Example Code Location
```
extras/ESP32-S3-POE-ETH-8DI-8DO_example/examples/MAIN_WIFI_AP/
├── WS_RS485.h      // Header file
├── WS_RS485.cpp    // Implementation
└── MAIN_WIFI_AP.ino // Main example
```

### Key Files
- `WS_RS485.cpp`: Full RS485 implementation with Modbus RTU
- `WS_GPIO.h`: GPIO configuration and RGB LED control
- `WS_Struct.h`: Data structure definitions

### Important Methods
```cpp
Serial1.setPins()              // Set UART pins including direction control
Serial1.setMode()              // Set RS485 half-duplex mode
Serial1.begin()                // Initialize UART
Serial1.print() / write()      // Send data
Serial1.read() / readBytes()   // Receive data
Serial1.available()            // Check if data waiting
Serial1.flush()                // Wait for TX completion
```

---

## 🆕 Production Implementation: loraConfigureStream() & loraConfigurePacket()

### Overview / ภาพรวม
The production firmware includes two enhanced configuration functions that implement proper AT command sequences for both Stream Mode and Packet Mode operations, based on the official Waveshare DTU_AT_Config.ino example.

### loraConfigureStream() - Full AT Command Sequence

```cpp
void loraConfigureStream() {
  Serial.println("\n⚙️  Configuring LoRa DTU - Stream Mode (based on DTU_AT_Config example)...");
  delay(100);
  
  // Step 1: Enter AT Command Mode
  Serial.println("   [1] Entering AT Command Mode (sending +++)...");
  Serial2.print("+++\r\n");
  Serial2.flush();
  delay(500);
  while (Serial2.available()) Serial2.read();
  delay(100);
  
  // Step 2: Send configuration commands
  Serial.println("   [2] Sending configuration commands...");
  
  // Test connection
  loraATCommand("AT");                        // Test command
  loraATCommand("AT+VER");                    // Get version
  delay(100);
  
  // Port configuration (CRITICAL)
  loraATCommand("AT+PORT=2");                 // RS485 port
  loraATCommand("AT+BAUD=9600");              // UART baud rate
  loraATCommand("AT+COMM=\"8N1\"");           // Data format
  delay(100);
  
  // LoRa mode (Stream mode)
  loraATCommand("AT+MODE=1");                 // Stream mode
  delay(100);
  
  // Channel configuration (CRITICAL - must match between devices)
  loraATCommand("AT+TXCH=18");                // TX channel
  loraATCommand("AT+RXCH=18");                // RX channel
  delay(100);
  
  // Device addressing
  loraATCommand("AT+ADDR=0");                 // Device address
  delay(100);
  
  // RF Parameters
  loraATCommand("AT+SF=7");                   // Spreading factor
  loraATCommand("AT+BW=0");                   // Bandwidth 125KHz
  loraATCommand("AT+CR=1");                   // Code rate
  loraATCommand("AT+PWR=22");                 // Power 22dBm
  delay(100);
  
  // Step 3: Exit AT Command Mode
  Serial.println("   [3] Exiting AT Command Mode...");
  loraATCommand("AT+EXIT");
  
  delay(500);
  
  // Step 4: Verification
  Serial.println("   [4] Verifying configuration (reading version again)...");
  loraATCommand("AT+VER");                    // Confirm module is operational
  
  loraStatus.streamMode = true;
  
  Serial.println("✅ Stream Mode configuration complete!");
  Serial.println("   Channel: 18 | SF: 7 | BW: 125KHz | CR: 4/5 | Power: 22dBm");
}
```

### loraConfigurePacket() - Packet Mode (Modbus RTU)

```cpp
void loraConfigurePacket() {
  Serial.println("\n⚙️  Configuring LoRa DTU - Packet Mode (Modbus RTU)...");
  delay(100);
  
  // Enter AT Command Mode
  Serial.println("   [1] Entering AT Command Mode (sending +++)...");
  Serial2.print("+++\r\n");
  Serial2.flush();
  delay(500);
  while (Serial2.available()) Serial2.read();
  delay(100);
  
  // Send AT Commands
  Serial.println("   [2] Sending configuration commands...");
  
  loraATCommand("AT");                        // Basic test
  loraATCommand("AT+VER");                    // Get version
  delay(100);
  
  loraATCommand("AT+PORT=2");                 // RS485
  loraATCommand("AT+BAUD=9600");              // Baud
  loraATCommand("AT+COMM=\"8N1\"");           // Data format
  delay(100);
  
  loraATCommand("AT+MODE=2");                 // Packet mode (NOT stream)
  delay(100);
  
  loraATCommand("AT+TXCH=18");                // TX channel
  loraATCommand("AT+RXCH=18");                // RX channel
  delay(100);
  
  loraATCommand("AT+ADDR=0");                 // Device address
  delay(100);
  
  loraATCommand("AT+SF=7");                   // RF Parameters
  loraATCommand("AT+BW=0");
  loraATCommand("AT+CR=1");
  loraATCommand("AT+PWR=22");
  delay(100);
  
  // Exit AT Command Mode
  Serial.println("   [3] Exiting AT Command Mode...");
  loraATCommand("AT+EXIT");
  
  delay(500);
  
  // Verify
  Serial.println("   [4] Verifying configuration (reading version again)...");
  loraATCommand("AT+VER");
  
  loraStatus.streamMode = false;
  Serial.println("✅ Packet Mode (Modbus RTU) configuration complete!");
  Serial.println("   Channel: 18 | SF: 7 | BW: 125KHz | CR: 4/5 | Power: 22dBm");
}
```

### Configuration Differences / ความแตกต่างของการตั้งค่า

| Feature | Stream Mode | Packet Mode | Purpose |
|---------|-------------|-------------|---------|
| AT+MODE | 1 | 2 | Operating mode |
| AT+TXCH | 18 | 18 | TX channel (same) |
| AT+RXCH | 18 | 18 | RX channel (same) |
| AT+ADDR | 0 | 0 | Device address (same) |
| AT+SF | 7 | 7 | Spreading factor (same) |
| RF Parameters | Same | Same | Power, bandwidth, coding rate |
| Data Handling | Continuous stream | Modbus RTU packets | Different protocols |
| Best For | Simple serial forwarding | Legacy systems, Modbus |

### Key Implementation Details / รายละเอียดการใช้งาน

1. **Buffer Clearing** - Prevents stale responses
   ```cpp
   while (Serial2.available()) Serial2.read();
   delay(100);
   ```

2. **Proper Delays** - Allows module time to process
   - 500ms after entering AT mode
   - 100ms between command groups
   - 300ms in loraATCommand() for response

3. **CRITICAL Parameters** - Must be set correctly:
   - **AT+PORT=2**: Must be RS485 (not RS232)
   - **AT+TXCH/RXCH=18**: Must match between communicating devices
   - **AT+ADDR=0**: Identifies this device
   - **AT+COMM="8N1"**: Data format for RS485

4. **Verification** - Read version before and after
   ```cpp
   loraATCommand("AT+VER");  // Before configuration
   // ... configuration commands ...
   loraATCommand("AT+VER");  // After configuration to confirm success
   ```

### Testing the Configuration

```cpp
// After uploading, open Serial Monitor at 115200 baud:

// Example output:
// ⚙️  Configuring LoRa DTU - Stream Mode (based on DTU_AT_Config example)...
//    [1] Entering AT Command Mode (sending +++)...
//    [2] Sending configuration commands...
//    ⚙️  AT Command: AT
//    OK
//    ⚙️  AT Command: AT+VER
//    AT+VER=SX1262-DTU-20230428
//    [3] Exiting AT Command Mode...
//    ✅ Stream Mode configuration complete!
//    Channel: 18 | SF: 7 | BW: 125KHz | CR: 4/5 | Power: 22dBm
```

---

## 🚀 Recommended Implementation

Use **UART_MODE_RS485_HALF_DUPLEX** with `Serial1` for reliable RS485 communication. This is the official Waveshare approach and matches the hardware design of the ESP32-S3-POE-ETH-8DI-8DO board.

**Status**: ✅ Implemented & Tested  
**Last Updated**: 2026-06-26  
**Hardware**: ESP32-S3-POE-ETH-8DI-8DO v1.0
