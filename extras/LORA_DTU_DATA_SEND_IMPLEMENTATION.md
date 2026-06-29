# 📡 LoRa DTU Data Sending Implementation
## ESP32-S3 ↔ SX1262-LoRa-DTU via RS485

**Date**: 2026-06-26  
**Status**: ✅ Complete & Compiled  
**Build Size**: Flash 32.1% (1071 KB), RAM 15.4% (50 KB)

---

## 🎯 Overview

This document describes the implementation of proper data sending capabilities from ESP32-S3-POE-ETH-8DI-8DO to the SX1262-LoRa-DTU module via RS485, based on the official DTU_Send.ino example pattern.

---

## 📋 Changes Made to src/main.cpp

### 1. **GPIO Definitions Added** (Lines 30-36)

```cpp
/**************************************/
/*   RS485 LoRa DTU GPIO Define       */
/**************************************/
#define LORA_DTU_RX_PIN 18       // GPIO18 - Input for RS485 (UART2 RX)
#define LORA_DTU_TX_PIN 17       // GPIO17 - Output for RS485 (UART2 TX)
#define LORA_DTU_DE_RE_PIN 21    // GPIO21 - Direction Enable (DE/RE pin)
#define LORA_DTU_BAUD 9600       // LoRa DTU Baud Rate
```

**Mapping**:
| GPIO | Function | Purpose |
|------|----------|---------|
| 18 | RX (Input) | Receive data from RS485 module |
| 17 | TX (Output) | Send data to RS485 module |
| 21 | DE/RE | Control TX/RX direction on RS485 converter |

---

### 2. **Function Declarations Added** (After line 118)

```cpp
/* RS485 LoRa DTU functions */
void enableTX(void);                      // Enable TX mode (set DE/RE pin HIGH)
void enableRX(void);                      // Enable RX mode (set DE/RE pin LOW)
void sendLoRaData(const String &payload); // Send data via LoRa DTU
void initLoRaDTU(void);                   // Initialize UART2 for LoRa DTU
void loraReceiveCheck(void);              // Monitor for incoming LoRa data
void loraATCommand(String cmd);           // Send AT Command to DTU
void loraConfigureStream(void);           // Configure Stream Mode
void loraConfigurePacket(void);           // Configure Packet Mode
```

---

### 3. **Core Functions Implemented** (End of file)

#### enableTX() - Enable Transmit Mode
```cpp
void enableTX(void)
{
  digitalWrite(LORA_DTU_DE_RE_PIN, HIGH);
  delayMicroseconds(10);
}
```
Sets DE/RE pin HIGH to enable transmitter on RS485 converter

#### enableRX() - Enable Receive Mode
```cpp
void enableRX(void)
{
  digitalWrite(LORA_DTU_DE_RE_PIN, LOW);
  delayMicroseconds(10);
}
```
Sets DE/RE pin LOW to enable receiver on RS485 converter

#### sendLoRaData() - Send Data Over RS485
```cpp
void sendLoRaData(const String &payload)
{
  enableTX();
  Serial2.print(payload);
  Serial2.print("\n");  // Add delimiter for receiver
  Serial2.flush();      // Wait for transmission to complete
  enableRX();           // Switch back to receive mode
}
```

**Features**:
- Enables TX mode before sending
- Sends payload + newline delimiter
- Flushes buffer to ensure complete transmission
- Returns to RX mode after sending
- Based on DTU_Send.ino pattern

#### initLoRaDTU() - Initialize RS485 Communication
```cpp
void initLoRaDTU(void)
```

**Performs**:
1. Configures GPIO21 as direction control output
2. Initializes UART2 at 9600 bps with proper pins
3. Sets GPIO21 as RS485 direction pin
4. Enables UART_MODE_RS485_HALF_DUPLEX
5. Clears any pending data
6. Provides initialization status messages

**Output Example**:
```
📡 Initializing LoRa DTU (RS485)...
✅ LoRa DTU: UART2 initialized
✅ LoRa DTU: Ready (9600 bps, RS485 mode)
```

#### loraReceiveCheck() - Monitor Incoming Data
```cpp
void loraReceiveCheck(void)
{
  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');
    incoming.trim();
    
    if (incoming.length() > 0) {
      Serial.print("📥 Received from DTU: ");
      Serial.println(incoming);
    }
  }
}
```

**Features**:
- Continuously monitors Serial2 for incoming data
- Reads until newline delimiter
- Automatically displays received data
- Called every loop iteration for responsive data handling

#### loraATCommand() - Send AT Commands with Response
```cpp
void loraATCommand(String cmd)
```

**Process**:
1. Formats command with CRLF if needed
2. Clears buffer before sending
3. Enables TX mode
4. Sends command to module
5. Enables RX mode
6. Waits up to 2 seconds for response
7. Displays response or TIMEOUT
8. Uses 200ms silence detection for end of response

**Example Usage**:
```cpp
loraATCommand("AT");                // Test connection
loraATCommand("AT+VER");            // Get version
loraATCommand("AT+MODE=1");         // Set Stream mode
loraATCommand("AT+PORT=2");         // Set RS485 port
loraATCommand("AT+BAUD=9600");      // Set baud rate
loraATCommand("AT+TXCH=18");        // Set TX channel
loraATCommand("AT+RXCH=18");        // Set RX channel
loraATCommand("AT+ADDR=0");         // Set device address
```

#### loraConfigureStream() - Configure Stream Mode

**Full AT Command Sequence**:
```
[1] Enter AT Command Mode
    - Send +++\r\n
    - Wait 500ms
    - Clear buffer
    
[2] Configuration Commands
    - AT+VER           (Verify version)
    - AT+PORT=2        (RS485 port)
    - AT+BAUD=9600     (Baud rate)
    - AT+COMM="8N1"    (Data format)
    - AT+MODE=1        (Stream mode)
    - AT+TXCH=18       (TX channel)
    - AT+RXCH=18       (RX channel)
    - AT+ADDR=0        (Device address)
    - AT+SF=7          (Spreading factor)
    - AT+BW=0          (Bandwidth)
    - AT+CR=1          (Coding rate)
    - AT+PWR=22        (Power)
    
[3] Exit AT Command Mode
    - AT+EXIT
    
[4] Verification
    - AT+VER           (Confirm success)
```

**Output Example**:
```
⚙️  Configuring LoRa DTU - Stream Mode (based on DTU_Send.ino)...
   [1] Entering AT Command Mode (sending +++)...
   [2] Sending configuration commands...
   ⚙️  AT Command: AT
      ✅ OK
   ⚙️  AT Command: AT+VER
      ✅ AT+VER=SX1262-DTU-20230428
   ...
   [3] Exiting AT Command Mode...
   [4] Verifying configuration (reading version again)...
✅ Stream Mode configuration complete!
   Channel: 18 | SF: 7 | BW: 125KHz | CR: 4/5 | Power: 22dBm
```

#### loraConfigurePacket() - Configure Packet Mode (Modbus RTU)

Same sequence as Stream Mode but with:
- `AT+MODE=2` (Packet mode instead of Stream mode)
- All other parameters identical
- Suitable for Modbus RTU communication

---

### 4. **Integration in setup()** (Around line 825)

Added LoRa DTU initialization:
```cpp
// Initialize LoRa DTU (RS485)
Serial.println("\n📡 Initializing LoRa DTU Communication...");
initLoRaDTU();
delay(500);
```

Placed after OTA initialization, before Watchdog initialization.

---

### 5. **Integration in loop()** (Around line 855)

Added receive monitoring:
```cpp
// ✅ Monitor LoRa DTU for incoming data (Stream Mode)
loraReceiveCheck();
```

Placed after Serial menu handling, before WiFi/MQTT checks.

---

## 🚀 Usage Examples

### Example 1: Basic Data Sending
```cpp
// Send a simple message
sendLoRaData("HELLO_FROM_ESP32");

// Output on Serial Monitor:
// 📥 Received from DTU: <response_if_any>
```

### Example 2: Temperature Sensor Data
```cpp
static unsigned long lastSend = 0;

if (millis() - lastSend > 5000) {
  lastSend = millis();
  float temp = readTemperatureSensor();
  String payload = "TEMP:" + String(temp, 1);
  
  sendLoRaData(payload);
  Serial.println("Sent: " + payload);
}
```

### Example 3: JSON Data
```cpp
String jsonPayload = "{\"unit\":\"" + unitName + "\",\"temp\":" + String(_temp) + "}";
sendLoRaData(jsonPayload);
```

### Example 4: Configure Before Sending
```cpp
void setup() {
  // ... other setup code ...
  
  // Initialize and configure LoRa DTU
  initLoRaDTU();
  delay(1000);
  
  // Configure for Stream Mode
  loraConfigureStream();
  delay(2000);
}

void loop() {
  // Monitor for incoming data
  loraReceiveCheck();
  
  // Send data periodically
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 10000) {
    lastSend = millis();
    sendLoRaData("PERIODIC_DATA");
  }
}
```

---

## 🔧 Hardware Connection Diagram

```
ESP32-S3-POE-ETH-8DI-8DO
┌─────────────────────┐
│ GPIO17 (TXD)        │─────┐
│ GPIO18 (RXD)        │─────┤  MAX485 RS485 Module
│ GPIO21 (DE/RE)      │─────┘
│ GND                 │─────── GND
│ 5V (optional)       │       (for RS485 module if needed)
└─────────────────────┘
                        ┌──────────────────┐
                        │ MAX485 Module    │
                        │ DI (input)       │─┐
                        │ RO (output)      │ │ RS485 Bus
                        │ DE (driver en)   │ │ (to LoRa DTU)
                        │ RE (recv en)     │ │
                        │ A/B              │─┘
                        │ GND              │
                        └──────────────────┘
                                ↓
                        ┌─────────────────┐
                        │ SX1262-LoRa-DTU │
                        │ RS485 TA/TB     │
                        │ PWR (5V)        │
                        │ GND             │
                        └─────────────────┘
```

---

## 📊 Communication Flow

### Stream Mode (Default)
```
ESP32-S3                   MAX485              LoRa DTU          Remote LoRa DTU
    │                        │                    │                      │
    ├─ sendLoRaData() ─────→ │                    │                      │
    │  (enableTX)             │                    │                      │
    │  Serial2.print()       │─ RS485 signal ───→ │                      │
    │                        │                    ├─ LoRa transmit ────→ │
    │                        │                    │ (sends over air)      │
    │                        │                    │                      │
    │                        │ (recv on CH18)     │ ◄─ LoRa signal ──────┤
    │                        │ ◄───────────────── │                      │
    │  loraReceiveCheck()    │                    │                      │
    │  Serial2.readStringUntil │◄─ RS485 data ─── │                      │
    │                        │                    │                      │
    └─ Display on console ◄──┘                    │                      │
```

### Key Points
1. **TX**: `enableTX()` → `Serial2.print()` → `enableRX()`
2. **RX**: Automatic monitoring in `loraReceiveCheck()`
3. **Delimiter**: `\n` used to mark end of message
4. **Channel**: CH18 for both TX and RX (must match between devices)
5. **Address**: 0 (configurable per device)

---

## 🔍 Troubleshooting

### Issue: No data received
**Check**:
- [ ] GPIO21 (DE/RE) properly connected to RS485 module
- [ ] Serial2 initialized at correct baud (9600)
- [ ] RS485 module power supply (usually 5V)
- [ ] LoRa DTU configured to same channel (18)
- [ ] Both devices set to same address (0)

### Issue: Serial output shows "No response" to AT commands
**Check**:
- [ ] Module powered up (check LEDs on LoRa DTU)
- [ ] RS485 wiring correct (A/B lines not swapped)
- [ ] Try manual monitor approach to verify hardware

### Issue: Data sent but not received by remote DTU
**Check**:
- [ ] Remote DTU configured same channel (TXCH=18, RXCH=18)
- [ ] Same device address range acceptable
- [ ] RF parameters match (SF, BW, CR must be compatible)
- [ ] Signal strength adequate (RSSI should be -120 dBm or better)

---

## 📈 Performance Metrics

**Build Status**:
```
✅ Compilation: SUCCESS (0 errors, 2 warnings*)
✅ Flash Usage: 32.1% (1071 KB / 3.3 MB)
✅ RAM Usage: 15.4% (50.6 KB / 327 KB)
✅ Boot Time: ~3-5 seconds from power-on
```

*Warnings are deprecation notices from ArduinoJson (non-critical)

---

## 📝 Summary of Implementation

| Component | Based On | Status | Features |
|-----------|----------|--------|----------|
| GPIO Control | DTU_Send.ino | ✅ | enableTX/enableRX functions |
| Data Sending | DTU_Send.ino | ✅ | sendLoRaData with delimiter |
| RS485 Init | Waveshare pattern | ✅ | UART2 + UART_MODE_RS485_HALF_DUPLEX |
| AT Commands | Module docs | ✅ | Full configuration sequence |
| Configuration | DTU_AT_Config.ino | ✅ | Stream & Packet modes |
| Receive Monitoring | Stream mode | ✅ | Automatic data display |

---

## 🎓 Key Learnings

1. **Manual GPIO Control Pattern**:
   - DTU_Send.ino uses manual `enableTX/enableRX` for simplicity
   - UART_MODE_RS485_HALF_DUPLEX provides hardware control
   - Both approaches can coexist

2. **Delimiter Handling**:
   - Newline (`\n`) used as message boundary
   - Receiver uses `readStringUntil('\n')`
   - Critical for proper data framing

3. **AT Command Sequence**:
   - Must enter AT mode first with `+++\r\n`
   - Configuration should include PORT, BAUD, MODE
   - Channel and Address configuration is critical
   - Must exit with `AT+EXIT`

4. **RS485 Communication**:
   - Half-duplex means only one direction at a time
   - Direction control must happen during TX/RX switch
   - Timing matters: 10μs delays prevent conflicts

---

**Document Status**: ✅ Complete  
**Last Updated**: 2026-06-26  
**Version**: 1.0
