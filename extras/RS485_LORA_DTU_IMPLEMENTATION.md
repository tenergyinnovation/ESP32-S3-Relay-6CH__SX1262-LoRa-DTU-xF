# 📡 RS485 LoRa DTU Modbus RTU Implementation

## Overview
This document provides complete code for RS485 communication with SX1262-LoRa-DTU-xF using Modbus RTU protocol.

---

## Part 1: Global Variables & Definitions

Add these to the **Global Section** (after #include statements, before setup):

```cpp
/**************************************/
/*      RS485 LoRa DTU Define         */
/**************************************/
#define LORA_DTU_RX_PIN 18       // GPIO18 - Input for RS485 (UART2 RX)
#define LORA_DTU_TX_PIN 17       // GPIO17 - Output for RS485 (UART2 TX)
#define LORA_DTU_BAUD 9600       // LoRa DTU Baud Rate
#define LORA_RX_BUFFER_SIZE 256  // Receive buffer size
#define LORA_RX_TIMEOUT 100      // ms - timeout for completing reception
#define LORA_SEND_RETRY 3        // Retry count for sending

// LoRa DTU Communication Status
struct LoRaStatus {
  bool isConnected;
  uint16_t packetsReceived;
  uint16_t packetsSent;
  uint16_t packetErrors;
  int16_t lastRSSI;
  uint32_t lastActivityTime;
  char lastErrorMsg[100];
  bool streamMode;  // true=Stream Mode, false=Packet Mode
};

// Global LoRa Status Variable
LoRaStatus loraStatus = {
  false,    // isConnected
  0,        // packetsReceived
  0,        // packetsSent
  0,        // packetErrors
  0,        // lastRSSI
  0,        // lastActivityTime
  "",       // lastErrorMsg
  true      // streamMode (default: Stream Mode)
};

// LoRa RX Buffer
char loraRxBuffer[LORA_RX_BUFFER_SIZE];
uint16_t loraRxIndex = 0;
uint32_t loraLastRxTime = 0;

// Modbus RTU Helper Variables
struct ModbusFrame {
  uint8_t slaveID;
  uint8_t functionCode;
  uint16_t startAddress;
  uint16_t quantity;
  uint16_t crc;
};

// Add function declarations to header section:
void initLoRaDTU(void);                                      // Initialize UART2 for LoRa DTU
void loraReceiveCheck(void);                                 // Check for incoming LoRa data
void processLoRaData(char *data, uint16_t len);             // Process received LoRa data
void loraSendData(String data);                             // Send data via LoRa
void loraATCommand(String cmd);                             // Send AT Command to DTU
void loraConfigureStream(void);                             // Configure Stream Mode
void loraConfigurePacket(void);                             // Configure Packet Mode
void loraDisplayStatus(void);                               // Display LoRa status
uint16_t calculateCRC16(uint8_t *buffer, uint16_t len);    // Calculate Modbus CRC
bool validateModbusFrame(uint8_t *frame, uint16_t len);    // Validate Modbus frame
void sendModbusRead(uint8_t slaveID, uint16_t address, uint16_t count);  // Send Modbus Read
void processModbusResponse(uint8_t *data, uint16_t len);    // Process Modbus response
```

---

## Part 2: Core LoRa DTU Functions

Add these functions **before the setup() function**:

```cpp
/***********************************************************************
 * FUNCTION:    initLoRaDTU
 * DESCRIPTION: Initialize UART2 for RS485 LoRa DTU communication
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void initLoRaDTU() {
  Serial.println("\n📡 Initializing LoRa DTU (RS485)...");
  
  // Initialize UART2 with correct pin configuration
  // UART2: RX=GPIO5 (RO pin), TX=GPIO4 (DI pin)
  Serial2.begin(LORA_DTU_BAUD, SERIAL_8N1, LORA_DTU_RX_PIN, LORA_DTU_TX_PIN);
  
  if (!Serial2) {
    Serial.println("❌ LoRa DTU: UART2 initialization failed!");
    strcpy(loraStatus.lastErrorMsg, "UART2 init failed");
    return;
  }
  
  Serial.println("✓ LoRa DTU: UART2 initialized");
  
  // Clear any pending data in buffer
  delay(100);
  while (Serial2.available()) {
    Serial2.read();
  }
  loraRxIndex = 0;
  
  loraStatus.isConnected = true;
  loraStatus.lastActivityTime = millis();
  
  Serial.println("✓ LoRa DTU: Ready (9600 bps)");
  Serial.println("  Mode: Stream (default)");
  Serial.println("  Protocol: RTU/Stream");
}

/***********************************************************************
 * FUNCTION:    loraReceiveCheck
 * DESCRIPTION: Check for data from LoRa DTU and process it
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void loraReceiveCheck() {
  if (!loraStatus.isConnected) return;
  
  // Read available bytes from UART2
  while (Serial2.available() > 0) {
    char ch = Serial2.read();
    
    // Check buffer overflow
    if (loraRxIndex >= LORA_RX_BUFFER_SIZE - 1) {
      Serial.println("⚠️  LoRa RX: Buffer overflow!");
      loraRxIndex = 0;
      loraStatus.packetErrors++;
      strcpy(loraStatus.lastErrorMsg, "Buffer overflow");
      return;
    }
    
    loraRxBuffer[loraRxIndex++] = ch;
    loraLastRxTime = millis();
    
    // Check for end of data (newline or carriage return)
    if (ch == '\n' || ch == '\r') {
      // Process complete frame
      if (loraRxIndex > 1) {
        loraRxBuffer[loraRxIndex] = '\0';
        processLoRaData(loraRxBuffer, loraRxIndex);
      }
      
      // Reset for next frame
      loraRxIndex = 0;
      loraLastRxTime = 0;
    }
  }
  
  // Check for timeout (incomplete frame after 100ms silence)
  if (loraRxIndex > 0 && (millis() - loraLastRxTime > LORA_RX_TIMEOUT)) {
    Serial.printf("⚠️  LoRa RX: Timeout (incomplete frame %d bytes)\n", loraRxIndex);
    loraRxIndex = 0;
    loraStatus.packetErrors++;
  }
}

/***********************************************************************
 * FUNCTION:    processLoRaData
 * DESCRIPTION: Process received data from LoRa DTU
 * PARAMETERS:  data - pointer to data buffer
 *              len  - length of data
 * RETURNED:    none
 ***********************************************************************/
void processLoRaData(char *data, uint16_t len) {
  if (len == 0 || data == NULL) return;
  
  // Trim whitespace
  String cleanData = String(data);
  cleanData.trim();
  
  if (cleanData.length() == 0) return;
  
  Serial.printf("📡 LoRa RX: %s (%d bytes)\n", cleanData.c_str(), len);
  
  loraStatus.packetsReceived++;
  loraStatus.lastActivityTime = millis();
  
  // Check if this is Modbus RTU format (binary) or text
  if (data[0] >= 32 && data[0] <= 126) {
    // Printable ASCII - likely Stream Mode
    Serial.println("   Mode: Stream (ASCII data)");
  } else {
    // Binary data - likely Modbus RTU
    Serial.println("   Mode: Modbus RTU (binary data)");
    // Validate Modbus frame
    if (validateModbusFrame((uint8_t *)data, len)) {
      processModbusResponse((uint8_t *)data, len);
    }
  }
  
  // Send to MQTT if connected
  if (client.connected()) {
    String topic = "loragreenergy/" + unitName + "/lora_rx";
    String payload = "{\"data\":\"" + cleanData + "\",\"time\":\"" + isoTimestamp() + "\",\"bytes\":" + String(len) + "}";
    
    if (client.publish(topic.c_str(), payload.c_str())) {
      Serial.printf("✓ Published to MQTT: %s\n", topic.c_str());
    }
  }
}

/***********************************************************************
 * FUNCTION:    loraSendData
 * DESCRIPTION: Send data via LoRa DTU
 * PARAMETERS:  data - string data to send
 * RETURNED:    none
 ***********************************************************************/
void loraSendData(String data) {
  if (!loraStatus.isConnected) {
    Serial.println("❌ LoRa DTU: Not connected!");
    return;
  }
  
  if (data.length() == 0) {
    Serial.println("❌ LoRa DTU: Empty data!");
    return;
  }
  
  // Add CRLF if not present
  if (!data.endsWith("\r\n")) {
    data += "\r\n";
  }
  
  Serial.printf("📤 LoRa TX: %s", data.c_str());
  
  // Send data via UART2
  Serial2.print(data);
  Serial2.flush();
  
  loraStatus.packetsSent++;
  loraStatus.lastActivityTime = millis();
}

/***********************************************************************
 * FUNCTION:    loraATCommand
 * DESCRIPTION: Send AT command to LoRa DTU and get response
 * PARAMETERS:  cmd - AT command string
 * RETURNED:    none
 ***********************************************************************/
void loraATCommand(String cmd) {
  if (!loraStatus.isConnected) {
    Serial.println("❌ LoRa DTU: Not connected!");
    return;
  }
  
  // Ensure command ends with CRLF
  if (!cmd.endsWith("\r\n")) {
    cmd += "\r\n";
  }
  
  Serial.printf("⚙️  AT Command: %s", cmd.c_str());
  
  Serial2.print(cmd);
  Serial2.flush();
  
  delay(200); // Wait for response
  
  // Read response
  String response = "";
  uint32_t startTime = millis();
  while (millis() - startTime < 1000 && Serial2.available()) {
    char ch = Serial2.read();
    response += ch;
    delay(5);
  }
  
  if (response.length() > 0) {
    response.trim();
    Serial.printf("✓ Response: %s\n", response.c_str());
  } else {
    Serial.println("⚠️  No response");
  }
}

/***********************************************************************
 * FUNCTION:    loraConfigureStream
 * DESCRIPTION: Configure LoRa DTU for Stream Mode
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void loraConfigureStream() {
  Serial.println("\n⚙️  Configuring LoRa DTU - Stream Mode...");
  
  // Wait before sending AT commands
  delay(100);
  
  // Enter AT command mode
  Serial2.print("+++\r\n");
  delay(500);
  
  // Configure parameters
  loraATCommand("AT+MODE=1");     // Mode 1 = Stream
  loraATCommand("AT+PORT=2");     // Port 2 = RS485
  loraATCommand("AT+BAUD=9600");  // 9600 bps
  loraATCommand("AT+SF=7");       // Spreading Factor = 7
  loraATCommand("AT+BW=0");       // Bandwidth = 125KHz
  loraATCommand("AT+CR=1");       // Code Rate = 4/5
  loraATCommand("AT+PWR=22");     // Power = 22dBm (max)
  loraATCommand("AT+LBT=0");      // LBT = disabled
  
  // Exit AT command mode
  loraATCommand("AT+EXIT");
  
  delay(500);
  loraStatus.streamMode = true;
  
  Serial.println("✓ Stream Mode configured!");
}

/***********************************************************************
 * FUNCTION:    loraConfigurePacket
 * DESCRIPTION: Configure LoRa DTU for Packet Mode (Modbus RTU)
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void loraConfigurePacket() {
  Serial.println("\n⚙️  Configuring LoRa DTU - Packet Mode (Modbus RTU)...");
  
  delay(100);
  Serial2.print("+++\r\n");
  delay(500);
  
  loraATCommand("AT+MODE=2");     // Mode 2 = Packet
  loraATCommand("AT+PORT=2");     // Port 2 = RS485
  loraATCommand("AT+BAUD=9600");  // 9600 bps
  loraATCommand("AT+SF=7");       // Spreading Factor = 7
  loraATCommand("AT+BW=0");       // Bandwidth = 125KHz
  loraATCommand("AT+CR=1");       // Code Rate = 4/5
  loraATCommand("AT+PWR=22");     // Power = 22dBm
  
  loraATCommand("AT+EXIT");
  
  delay(500);
  loraStatus.streamMode = false;
  
  Serial.println("✓ Packet Mode (Modbus RTU) configured!");
}

/***********************************************************************
 * FUNCTION:    loraDisplayStatus
 * DESCRIPTION: Display current LoRa DTU status
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void loraDisplayStatus() {
  Serial.println("\n📊 === LoRa DTU Status ===");
  Serial.printf("   Connected: %s\n", loraStatus.isConnected ? "Yes" : "No");
  Serial.printf("   Mode: %s\n", loraStatus.streamMode ? "Stream" : "Packet (Modbus RTU)");
  Serial.printf("   RX Packets: %u\n", loraStatus.packetsReceived);
  Serial.printf("   TX Packets: %u\n", loraStatus.packetsSent);
  Serial.printf("   Errors: %u\n", loraStatus.packetErrors);
  Serial.printf("   Last RSSI: %d dBm\n", loraStatus.lastRSSI);
  Serial.printf("   Idle Time: %u ms\n", (uint32_t)(millis() - loraStatus.lastActivityTime));
  if (strlen(loraStatus.lastErrorMsg) > 0) {
    Serial.printf("   Last Error: %s\n", loraStatus.lastErrorMsg);
  }
  Serial.println("   ========================");
}

/***********************************************************************
 * FUNCTION:    calculateCRC16
 * DESCRIPTION: Calculate Modbus RTU CRC-16
 * PARAMETERS:  buffer - data buffer
 *              len    - data length
 * RETURNED:    CRC value
 ***********************************************************************/
uint16_t calculateCRC16(uint8_t *buffer, uint16_t len) {
  uint16_t crc = 0xFFFF;
  
  for (uint16_t i = 0; i < len; i++) {
    crc ^= buffer[i];
    
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc = crc >> 1;
      }
    }
  }
  
  return crc;
}

/***********************************************************************
 * FUNCTION:    validateModbusFrame
 * DESCRIPTION: Validate Modbus RTU frame
 * PARAMETERS:  frame - frame buffer
 *              len   - frame length
 * RETURNED:    true if valid, false otherwise
 ***********************************************************************/
bool validateModbusFrame(uint8_t *frame, uint16_t len) {
  if (len < 8) {
    Serial.println("❌ Modbus: Frame too short");
    return false;
  }
  
  // Extract CRC from last 2 bytes
  uint16_t receivedCRC = (frame[len-1] << 8) | frame[len-2];
  
  // Calculate CRC
  uint16_t calculatedCRC = calculateCRC16(frame, len - 2);
  
  if (receivedCRC != calculatedCRC) {
    Serial.printf("❌ Modbus: CRC mismatch (Received: 0x%04X, Calculated: 0x%04X)\n", receivedCRC, calculatedCRC);
    return false;
  }
  
  Serial.println("✓ Modbus: Frame valid");
  return true;
}

/***********************************************************************
 * FUNCTION:    sendModbusRead
 * DESCRIPTION: Send Modbus Read Holding Registers command
 * PARAMETERS:  slaveID - Modbus slave ID
 *              address - starting register address
 *              count   - number of registers to read
 * RETURNED:    none
 ***********************************************************************/
void sendModbusRead(uint8_t slaveID, uint16_t address, uint16_t count) {
  if (!loraStatus.isConnected) {
    Serial.println("❌ LoRa DTU: Not connected!");
    return;
  }
  
  // Build Modbus frame: [Slave ID][Function Code][Address H][Address L][Count H][Count L][CRC L][CRC H]
  uint8_t frame[8];
  frame[0] = slaveID;           // Slave ID
  frame[1] = 0x03;              // Function Code 3 (Read Holding Registers)
  frame[2] = (address >> 8);    // Start Address High
  frame[3] = (address & 0xFF);  // Start Address Low
  frame[4] = (count >> 8);      // Quantity High
  frame[5] = (count & 0xFF);    // Quantity Low
  
  // Calculate CRC
  uint16_t crc = calculateCRC16(frame, 6);
  frame[6] = (crc & 0xFF);      // CRC Low
  frame[7] = (crc >> 8);        // CRC High
  
  // Send frame
  Serial.printf("📤 Modbus: Read command (Slave=%d, Addr=0x%04X, Count=%d)\n", slaveID, address, count);
  
  for (int i = 0; i < 8; i++) {
    Serial2.write(frame[i]);
  }
  Serial2.flush();
  
  loraStatus.packetsSent++;
  loraStatus.lastActivityTime = millis();
}

/***********************************************************************
 * FUNCTION:    processModbusResponse
 * DESCRIPTION: Process Modbus response data
 * PARAMETERS:  data - response data buffer
 *              len  - data length
 * RETURNED:    none
 ***********************************************************************/
void processModbusResponse(uint8_t *data, uint16_t len) {
  if (len < 5) {
    Serial.println("❌ Modbus: Response too short");
    return;
  }
  
  uint8_t slaveID = data[0];
  uint8_t functionCode = data[1];
  uint8_t byteCount = data[2];
  
  Serial.printf("✓ Modbus Response: Slave=%d, FC=%d, Bytes=%d\n", slaveID, functionCode, byteCount);
  
  // Extract register values
  if (functionCode == 0x03) {
    // Read Holding Registers response
    uint16_t registerCount = byteCount / 2;
    Serial.printf("  Register Values (%d):\n", registerCount);
    
    for (uint16_t i = 0; i < registerCount; i++) {
      uint16_t value = (data[3 + i*2] << 8) | data[4 + i*2];
      Serial.printf("    Register %d: 0x%04X (%u)\n", i, value, value);
    }
  }
}
```

---

## Part 3: Integration with setup()

Add this line in **setup()** function after initializing WiFi and MQTT:

```cpp
  // Initialize LoRa DTU (RS485)
  initLoRaDTU();
  loraConfigureStream();  // Configure for Stream Mode
  // OR for Modbus RTU:
  // loraConfigurePacket();  // Configure for Packet Mode (Modbus RTU)
```

---

## Part 4: Integration with loop()

Add this code in **loop()** function:

```cpp
  // Check for LoRa DTU data (call frequently)
  loraReceiveCheck();
  
  // Display LoRa status periodically (every 60 seconds)
  static uint32_t lastLoraStatus = 0;
  if (millis() - lastLoraStatus > 60000) {
    loraDisplayStatus();
    lastLoraStatus = millis();
  }
```

---

## Part 5: Example Usage in Serial Menu

Add these commands to your serial menu handler:

```cpp
// Add to handleSerialMenu() or command processor:

case '1':  // Send test message via LoRa
  Serial.println("\n📤 Sending test message via LoRa...");
  loraSendData("Hello from ESP32-S3!");
  break;

case '2':  // Display LoRa status
  loraDisplayStatus();
  break;

case '3':  // Reconfigure to Stream Mode
  loraConfigureStream();
  break;

case '4':  // Reconfigure to Packet Mode (Modbus RTU)
  loraConfigurePacket();
  break;

case '5':  // Send Modbus Read command (Slave 1, Register 0, Count 10)
  Serial.println("\n📤 Sending Modbus Read command...");
  sendModbusRead(0x01, 0x0000, 10);
  break;

case '6':  // Send AT command
  Serial.println("Enter AT command (e.g., AT+VER):");
  String atCmd = "";
  while (Serial.available() == 0) delay(10);
  atCmd = Serial.readStringUntil('\n');
  loraATCommand(atCmd);
  break;
```

---

## Part 6: Data Flow Diagram

```
┌──────────────────────────────────────────────────────────┐
│           LoRa Device (e.g., ESP32-S3-Transmitter)      │
└─────────────────────┬──────────────────────────────────┘
                      │
                   LoRa RF
                      │
┌─────────────────────▼──────────────────────────────────┐
│     SX1262-LoRa-DTU-xF (Receiver)                      │
│     - Receives LoRa signal                              │
│     - Converts to RS485 (Stream or Packet)             │
└─────────────────────┬──────────────────────────────────┘
                      │
                   RS485 (9600 bps)
                      │
        ┌─────────────┴─────────────┐
        │                           │
    TA (A+)                      TB (B-)
        │                           │
        ▼                           ▼
   GPIO4 (TX)                  GPIO5 (RX)
        │                           │
        └─────────────┬─────────────┘
                      │
        ┌─────────────▼──────────────┐
        │    ESP32-S3-POE-ETH       │
        │    (UART2 Receiver)        │
        │    - Process Data          │
        │    - Send to MQTT Broker   │
        │    - Send to Google Sheets │
        └───────────────────────────┘
```

---

## Testing Checklist

- [ ] UART2 initializes correctly (9600 bps)
- [ ] LoRa DTU connected and detected
- [ ] Stream Mode: Can receive text messages
- [ ] Packet Mode: Can receive Modbus RTU frames
- [ ] CRC validation works correctly
- [ ] Data published to MQTT broker
- [ ] AT commands processed correctly
- [ ] Status displayed periodically
- [ ] Error handling and recovery works

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| No data received | Check RS485 wiring (TX↔RX), baud rate (9600) |
| CRC errors | Verify Modbus frame format, check for corruption |
| Buffer overflow | Reduce data size or increase LORA_RX_BUFFER_SIZE |
| AT commands not working | Ensure 100ms delay before sending +++ |
| Data not in MQTT | Check WiFi/MQTT connection, topic name |
| Intermittent failures | Add delay between commands, check power supply |

---

## Next Steps

1. Copy all code to src/main.cpp
2. Build and test: `pio run`
3. Upload: `pio run -t upload`
4. Monitor: `pio device monitor -b 115200`
5. Test commands (1-6 in serial menu)

✅ System ready for full LoRa DTU integration!
