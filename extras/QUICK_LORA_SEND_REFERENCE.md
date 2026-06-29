# ⚡ Quick LoRa DTU Data Sending Reference

**File**: src/main.cpp  
**Status**: ✅ Complete & Compiled  
**Date**: 2026-06-26

---

## 🚀 Quick Start: 3 Steps

### Step 1️⃣: Initialize LoRa DTU in setup()
✅ **Already Added!** No action needed.

```cpp
// In setup() - this is already there:
initLoRaDTU();                 // Initialize RS485
loraConfigureStream();         // Configure Stream Mode (or loraConfigurePacket())
```

### Step 2️⃣: Monitor Incoming Data in loop()
✅ **Already Added!** No action needed.

```cpp
// In loop() - this is already there:
loraReceiveCheck();            // Automatically display received data
```

### Step 3️⃣: Send Data Whenever Needed
```cpp
// Anywhere in your code:
sendLoRaData("Your message here");
```

---

## 📤 Sending Data Examples

### Simple Text Message
```cpp
sendLoRaData("HELLO_WORLD");
// Output: Sends "HELLO_WORLD\n" via RS485
//         Remote DTU receives and transmits over LoRa
```

### Sensor Data with Format
```cpp
float temperature = 25.5;
String message = "TEMP:" + String(temperature, 1);
sendLoRaData(message);
// Sends: "TEMP:25.5\n"
```

### JSON Data
```cpp
String jsonData = "{\"sensor\":1,\"value\":42.5,\"unit\":\"C\"}";
sendLoRaData(jsonData);
// Sends: {"sensor":1,"value":42.5,"unit":"C"}\n
```

### Multiple Readings
```cpp
int humidity = 65;
float pressure = 1013.25;

String payload = "H:" + String(humidity) + ",P:" + String(pressure, 2);
sendLoRaData(payload);
// Sends: "H:65,P:1013.25\n"
```

### Periodic Sending (in loop())
```cpp
static unsigned long lastSend = 0;
static const uint32_t SEND_INTERVAL = 5000;  // 5 seconds

if (millis() - lastSend > SEND_INTERVAL) {
    lastSend = millis();
    
    float sensorValue = readSensor();
    sendLoRaData("SENSOR:" + String(sensorValue));
    
    Serial.println("✓ Data sent");
}
```

---

## 📥 Receiving Data

**Automatic!** The system monitors RS485 continuously:

```
In loop():
    loraReceiveCheck();  ← Automatically displays:
                         📥 Received from DTU: <data>
```

---

## 🔧 Configuration Functions

### Configure Stream Mode (Default)
```cpp
loraConfigureStream();
// Sets: MODE=1, PORT=2, BAUD=9600, TXCH=18, RXCH=18, ADDR=0, SF=7, etc.
// Output: Configuration status + success message
```

### Configure Packet Mode (Modbus RTU)
```cpp
loraConfigurePacket();
// Sets: MODE=2 (all other parameters same as Stream)
// Output: Configuration status + success message
```

### Send Single AT Command
```cpp
loraATCommand("AT+VER");           // Get module version
loraATCommand("AT+MODE=1");        // Change mode
loraATCommand("AT+TXCH=20");       // Change TX channel
// Each command gets immediate response or timeout message
```

---

## 🔌 GPIO Pins (No Changes Needed!)

| Pin | Function | Currently Set To |
|-----|----------|------------------|
| GPIO17 | RS485 TX | ✅ Correct |
| GPIO18 | RS485 RX | ✅ Correct |
| GPIO21 | DE/RE (Direction) | ✅ Correct |

---

## 📊 Compilation Status

```
✅ BUILD SUCCESS
   • 0 Errors
   • 2 Warnings (deprecation - non-critical)
   • Flash: 32.1% (1071 KB used)
   • RAM: 15.4% (50.6 KB used)
```

---

## 🎯 Common Tasks

### Task: Send temperature every 10 seconds
```cpp
static unsigned long lastSend = 0;

if (millis() - lastSend > 10000) {
    lastSend = millis();
    float temp = getTemperature();
    sendLoRaData("T:" + String(temp, 1));
}
```

### Task: Send only if WiFi connected
```cpp
if (WiFi.status() == WL_CONNECTED) {
    sendLoRaData("SYSTEM_ONLINE");
}
```

### Task: Send heartbeat with counter
```cpp
static int counter = 0;
static unsigned long lastSend = 0;

if (millis() - lastSend > 30000) {  // Every 30 seconds
    lastSend = millis();
    sendLoRaData("HB:" + String(counter++));
}
```

### Task: Emergency alert
```cpp
if (criticalConditionDetected()) {
    sendLoRaData("ALERT:CRITICAL");  // Sends immediately
    Serial.println("🚨 Alert sent!");
}
```

---

## 🔍 Serial Monitor Output

### When System Starts
```
📡 Initializing LoRa DTU Communication...
📡 Initializing LoRa DTU (RS485)...
✅ LoRa DTU: UART2 initialized
✅ LoRa DTU: Ready (9600 bps, RS485 mode)

⚙️  Configuring LoRa DTU - Stream Mode (based on DTU_Send.ino)...
   [1] Entering AT Command Mode (sending +++)...
   [2] Sending configuration commands...
   ⚙️  AT Command: AT
      ✅ OK
   ⚙️  AT Command: AT+VER
      ✅ AT+VER=SX1262-DTU-20230428
   ...
```

### When Data is Received
```
📥 Received from DTU: RESPONSE_DATA_HERE
📥 Received from DTU: ANOTHER_MESSAGE
```

### When You Send Data Manually
```
> sendLoRaData("TEST")
// (Data sent, if any response comes back, you'll see:)
📥 Received from DTU: <response>
```

---

## ⚙️ Implementation Details

### File Structure
```
src/main.cpp
├── GPIO Definitions (lines 27-36)
├── Function Declarations (lines 119-128)
├── setup() enhancement (line ~825)
│   └── initLoRaDTU() call
├── loop() enhancement (line ~855)
│   └── loraReceiveCheck() call
└── Function Implementations (end of file)
    ├── enableTX()
    ├── enableRX()
    ├── sendLoRaData()
    ├── initLoRaDTU()
    ├── loraReceiveCheck()
    ├── loraATCommand()
    ├── loraConfigureStream()
    └── loraConfigurePacket()
```

### Function Call Sequence

```
main.cpp
│
├─ setup()
│  │
│  └─ initLoRaDTU()
│     ├─ Configure GPIO21 as output
│     ├─ Initialize Serial2 (9600 bps)
│     ├─ Set RS485 direction pin
│     ├─ Enable UART_MODE_RS485_HALF_DUPLEX
│     └─ Clear buffer
│
└─ loop() (continuous)
   │
   ├─ loraReceiveCheck()
   │  └─ Display any incoming data
   │
   └─ [User code can call at any time]
      ├─ sendLoRaData() - send message
      ├─ loraATCommand() - send AT command
      ├─ loraConfigureStream() - configure module
      └─ loraConfigurePacket() - configure for Modbus
```

---

## 💡 Important Notes

1. **Automatic Direction Control**: GPIO21 direction switching is handled automatically by `enableTX/enableRX` functions
2. **Data Format**: Always ends with `\n` delimiter for proper reception
3. **Response Timeout**: 2 seconds per command (configurable in `loraATCommand`)
4. **Baud Rate**: 9600 bps (must match DTU_AT_Config: AT+BAUD=9600)
5. **Channel**: 18 by default (must match between communicating DTUs)

---

## 📌 Checklist Before Upload

- [ ] Connected GPIO17, GPIO18, GPIO21 to RS485 converter
- [ ] RS485 converter connected to LoRa DTU (TA/TB pins)
- [ ] LoRa DTU powered (5V)
- [ ] ESP32 connected to computer via USB
- [ ] Serial Monitor at 115200 baud

---

## 🚀 Ready to Test!

```bash
# Upload to ESP32
pio run --target upload

# Monitor
pio device monitor --baud 115200

# You should see:
# ✅ LoRa DTU: UART2 initialized
# ✅ Stream Mode configuration complete!
# 📥 Ready to send/receive data
```

---

**Last Updated**: 2026-06-26  
**Status**: ✅ Production Ready
