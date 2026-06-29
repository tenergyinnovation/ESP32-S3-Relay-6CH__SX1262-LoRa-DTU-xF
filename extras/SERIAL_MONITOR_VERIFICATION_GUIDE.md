# 🔍 Serial Monitor Verification Guide

## Overview
After the latest firmware update, the Serial Monitor now displays enhanced AT command responses and detailed LoRa data reception information. This guide shows you exactly what to expect.

---

## 📺 Expected Serial Monitor Output During Boot

### Part 1: System Initialization (First 10-15 seconds)
```
[     0][I][esp32-hal-psram.c:96] PSRAM: Found 8 Quad SPI psram device
[     0][I][esp32-hal-psram.c:109] PSRAM: Speed: 80Mhz
[     0][I][esp32-hal-psram.c:120] PSRAM initialized, starting up ram disk
...
⏳ Waiting for WiFi connection...
✅ WiFi connected!
🌐 IP: 192.168.1.xxx
🕐 Time synchronized

⚙️  Configuring LoRa DTU - Stream Mode...
   [1] Entering AT Command Mode (sending +++)...
   [2] Sending configuration commands...
```

### Part 2: AT Command Configuration (CRITICAL SECTION)
```
⚙️  AT Command: AT+MODE=1
   ✅ Response: OK
⚙️  AT Command: AT+PORT=2
   ✅ Response: OK
⚙️  AT Command: AT+BAUD=9600
   ✅ Response: OK
⚙️  AT Command: AT+SF=7
   ✅ Response: OK
⚙️  AT Command: AT+BW=0
   ✅ Response: OK
⚙️  AT Command: AT+CR=1
   ✅ Response: OK
⚙️  AT Command: AT+PWR=22
   ✅ Response: OK
   [3] Exiting AT Command Mode...
⚙️  AT Command: AT+EXIT
   ✅ Response: OK
   [4] Verifying configuration...
⚙️  AT Command: AT+VER
   ✅ Response: SX1262 LoRa DTU xF V2.1.0
```

### Part 3: Configuration Complete
```
═══════════════════════════════════════
✅ Stream Mode configuration complete!
   Module is now ready to receive LoRa data in Stream Mode
═══════════════════════════════════════
```

---

## 🔧 Possible AT Command Response Issues

### Issue 1: TIMEOUT (Module Not Responding)
```
⚙️  AT Command: AT+MODE=1
   ⏱️  Response: TIMEOUT (No data received)
```
**Diagnosis:**
- ❌ RS485 connection broken (TX/RX wires disconnected)
- ❌ GPIO17/18 pins not wired correctly
- ❌ Module power supply issue
- ❌ UART2 baud rate mismatch

**Solutions:**
1. Check RS485 cable connections at module
2. Verify ESP32 power is stable (use multimeter on GND/3V3)
3. Check module power supply (should be 5V for SX1262-LoRa-DTU)
4. Try manual testing with AT+VER command

---

### Issue 2: ERROR Response
```
⚙️  AT Command: AT+MODE=1
   ❌ Response: ERROR
```
**Diagnosis:**
- ❌ Command not recognized by module
- ❌ Module not in AT Command Mode (forgot +++)
- ❌ Command parameters invalid

**Solutions:**
1. Ensure +++ is sent first to enter AT mode
2. Check command spelling (case-sensitive)
3. Review module AT command manual for valid parameters

---

### Issue 3: Corrupted/Garbled Response
```
⚙️  AT Command: AT+MODE=1
   📋 Response: ^^[ERR^^]
```
**Diagnosis:**
- ❌ Baud rate mismatch (UART2 not 9600 bps)
- ❌ Noise on RS485 line
- ❌ Cable too long or poor shielding

**Solutions:**
1. Verify UART2 baud rate: 9600 bps (check platformio.ini)
2. Shorten RS485 cable if possible
3. Add ferrite cores to RS485 lines
4. Check for proper termination resistances (120Ω at both ends)

---

## 📡 Expected Serial Monitor Output When LoRa Data Arrives

### When Module Receives Data (Stream Mode)
```
═══════════════════════════════════════
📡 LoRa RX: Data Received (42 bytes)
───────────────────────────────────────
   Text: Hello from LoRa Node #1 !
   [HEX](42 bytes): 48 65 6C 6C 6F 20 66 72 6F 6D 20 4C 6F 52 61 20 4E 6F 64 65 20 23 31 20 21
   Format: ASCII/Stream Data ✓
───────────────────────────────────────
   ✅ MQTT Published: loragreenergy/esp32-s3-poe/lora_rx
═══════════════════════════════════════
```

### When Module Receives Binary Data (Modbus RTU Mode - if enabled)
```
═══════════════════════════════════════
📡 LoRa RX: Data Received (12 bytes)
───────────────────────────────────────
   Text: [unprintable binary data]
   [HEX](12 bytes): 01 03 04 00 0A 00 02 39 70
   Format: Binary Data (Modbus RTU?) ⚠️
   ✅ Modbus Frame: VALID
───────────────────────────────────────
   ✅ MQTT Published: loragreenergy/esp32-s3-poe/lora_rx
═══════════════════════════════════════
```

### When Reception Times Out (No Data Received)
```
⚠️  LoRa RX: Timeout (incomplete frame 15 bytes)
```
**Diagnosis:**
- Module received partial data but frame ended without newline
- Sender didn't include terminator

---

## 📊 LoRa DTU Status Report

The system displays status every 60 seconds:
```
📊 === LoRa DTU Status ===
   Connected: Yes
   Mode: Stream
   RX Packets: 5
   TX Packets: 2
   Errors: 0
   Idle Time: 3246 ms
   ========================
```

**Status Fields Explained:**
| Field | Meaning | Healthy Value |
|-------|---------|---------------|
| Connected | Module initialized | Yes |
| Mode | Operating mode | Stream or Packet |
| RX Packets | Data received count | Increases when data arrives |
| TX Packets | Data sent count | Increases if using loraSendData() |
| Errors | Communication errors | Should be 0 or low |
| Idle Time | Milliseconds since activity | Should reset when data arrives |

---

## 🧪 How to Test

### Test 1: Verify AT Command Responses
1. Open Serial Monitor at 115200 baud
2. Power on ESP32-S3
3. Wait for device to boot
4. Look for 8 "⚙️ AT Command:" lines followed by "✅ Response: OK"
5. **Expected Time:** 15-20 seconds after power-on

**✅ SUCCESS:** All 8 AT commands show "✅ Response: OK"
**❌ FAILURE:** See TIMEOUT or ERROR sections above

---

### Test 2: Send Test LoRa Message
1. Have a LoRa transmitter ready (another SX1262 module or similar)
2. Configure to same settings: SF=7, BW=125KHz, 9600 bps
3. Send test message: `"Test Message from LoRa Node"`
4. **Expected Time:** Message should appear within 1 second

**✅ SUCCESS:** See "📡 LoRa RX: Data Received (X bytes)" box
**❌ FAILURE:** No output = Check LoRa TX module and RF cables

---

### Test 3: Verify MQTT Publishing
1. Connect to MQTT broker: 159.65.11.12:1883
2. Subscribe to topic: `loragreenergy/+/lora_rx`
3. Send test LoRa message (Test 2)
4. Watch MQTT broker for incoming message

**✅ SUCCESS:** JSON message appears: `{"data":"Test Message from LoRa Node","time":"2025-01-15T12:34:56Z","bytes":25}`
**❌ FAILURE:** Check MQTT connection status (should say "✅ MQTT Published")

---

## 🔴 Common Problems & Solutions

| Symptom | Cause | Solution |
|---------|-------|----------|
| No "⚙️ AT Command" lines appear | ESP32 not detecting LoRa module | Check GPIO17/18 wiring |
| Only TIMEOUT responses | RS485 cable disconnected | Recheck TA/TB connections |
| "Format: Binary Data" instead of "ASCII" | Data has non-printable characters | May be Modbus RTU - normal if enabled |
| MQTT not published | WiFi or MQTT disconnected | Check WiFi connection first |
| Module appears to hang during config | Baud rate mismatch | Verify platformio.ini UART2 config |

---

## 📋 Complete Test Checklist

- [ ] ESP32 powers on (LED blinks)
- [ ] WiFi connects within 10 seconds
- [ ] LoRa DTU configuration starts (see "Configuring LoRa DTU...")
- [ ] All 8 AT commands receive "✅ Response: OK"
- [ ] "✅ Stream Mode configuration complete!" appears
- [ ] Wait 60 seconds for status report
- [ ] Status shows: Connected=Yes, Mode=Stream, Errors=0
- [ ] Send test LoRa message from another device
- [ ] "📡 LoRa RX: Data Received" box appears
- [ ] MQTT broker receives the message
- [ ] Message format is valid JSON

If all items are checked ✅, your system is working correctly!

---

## 🎯 Next Steps

If all tests pass:
1. **Stream Mode Verified:** Ready to receive real LoRa data
2. **Optional - Enable Modbus RTU:** Change setup() to call `loraConfigurePacket()` instead of `loraConfigureStream()`
3. **Optional - Test Multiple Nodes:** Connect 2-3 LoRa transmitters to verify reception from each
4. **Production:** Deploy to final location and monitor MQTT topic for data flow

---

## 📞 Troubleshooting Support

**For detailed debugging, check:**
- [INTEGRATION_COMPLETE.md](INTEGRATION_COMPLETE.md) - Features and integration points
- [RS485_LORA_DTU_IMPLEMENTATION.md](RS485_LORA_DTU_IMPLEMENTATION.md) - Code implementation details
- [LORA_DTU_RS485_GUIDE.md](LORA_DTU_RS485_GUIDE.md) - Module specifications (Thai)
- Module datasheet for AT command reference

---

**Last Updated:** 2025-01-15
**Firmware Version:** Latest (with enhanced Serial output)
