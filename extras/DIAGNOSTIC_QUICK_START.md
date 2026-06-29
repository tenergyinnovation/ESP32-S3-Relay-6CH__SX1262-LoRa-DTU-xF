# 🔧 Quick Start: Diagnostic Testing

## Situation
Your Serial Monitor shows all AT commands timing out - module not responding.

## What This Means
❌ UART2 is not receiving data from the LoRa module
❌ Most likely causes:
- GPIO17/18 not connected correctly
- Module power supply is 3.3V instead of 5V
- Loose wiring

---

## 🎯 Immediate Action Plan (Choose ONE)

### Option A: Visual Inspection + Multimeter (NO CODE CHANGES) ⏱️ 10 min

**Do This First (Fastest):**

1. **Power Off Everything**
2. **Check wiring physically:**
   - [ ] Find your breadboard/wiring
   - [ ] Trace wire from ESP32 GPIO17 → Does it go to module TA/TXD?
   - [ ] Trace wire from ESP32 GPIO18 → Does it go to module RB/RXD?
   - [ ] Trace GND wire → Connected?
   - [ ] Any wires loose or barely inserted?

3. **Use Multimeter (Continuity Mode - Beep)**
   - [ ] GPIO17 to TA/TXD: Should beep ✓
   - [ ] GPIO18 to RB/RXD: Should beep ✓
   - [ ] GND to GND: Should beep ✓

4. **Check Power Supply**
   - Set multimeter to Voltage (V—)
   - Probe module VCC pin
   - **Should read 4.8V - 5.2V**
   - If reads 3.3V → **PROBLEM FOUND! Need 5V supply**
   - If reads 0V → Module not powered

**If you find the problem:** Fix wiring/power, restart ESP32, check Serial Monitor again

---

### Option B: Add Diagnostic Code (CODE CHANGE) ⏱️ 15 min

**Use This If Option A Didn't Help:**

**Step 1: Add the diagnostic functions to your code**
- Copy file: `UART2_DIAGNOSTIC_FUNCTIONS.cpp`
- Paste content into [src/main.cpp](../src/main.cpp) right before `setup()` function
- Around line 750

**Step 2: Modify setup() function**
- Find this line (around line 1230):
  ```cpp
  loraConfigureStream();
  ```
- **Replace with:**
  ```cpp
  diagnosticLoRaUART2Test();
  // loraConfigureStream();  // Disabled for testing
  ```

**Step 3: Compile and Upload**
```bash
cd "/Users/utenboonliam/Documents/Tenergy_project/2026/32_ESP32-S3-POE-ETH-8DI-8DO WAVESHARE /source_code/ESP32-S3-POE-ETH-8DI-8DO-Gateway"
pio run --target upload
```

**Step 4: Watch Serial Monitor (115200 baud)**

**Expected Good Output:**
```
═══════════════════════════════════════
🔧 DIAGNOSTIC: UART2 RS485 Test Mode
═══════════════════════════════════════

[TEST 1] Sending raw test string...
   Sending: "HELLO\r\n" to UART2
   ✓ Data sent to UART2 output

[TEST 2] Attempting to enter AT Command Mode...
   Sending: "+++\r\n" to UART2
   ✓ AT mode command sent

[TEST 3] Listening for LoRa module response...
   Waiting 2 seconds for data...
OK
[TEST RESULTS]
   ✅ SUCCESS! Received data from module!
   📊 Bytes received: 3
   ✓ UART2 wiring appears to be working
   ✓ Module is responding
   → Next: Check AT command formatting
═══════════════════════════════════════
```

**Expected Bad Output (Wiring Problem):**
```
[TEST 3] Listening for LoRa module response...
   Waiting 2 seconds for data...

[TEST RESULTS]
   ❌ FAIL! No response from LoRa module
   ⚠️  This indicates a wiring or power problem:
   
   CHECK THESE (in priority order):
   1. GPIO17 wire connected to module TA/TXD?
   2. GPIO18 wire connected to module RB/RXD?
   3. GND wire connected?
   4. Module power supply = 5V DC (not 3.3V)?
   5. Power supply has enough current (>100mA)?
```

---

### Option C: Advanced Byte-Level Test (MORE DETAILED) ⏱️ 20 min

**Use This If Option B Shows the Problem But You Need More Details:**

**Step 1:** Same as Option B, but in Step 2, use:
```cpp
advancedUART2DebugTest();
// loraConfigureStream();  // Disabled for testing
```

**Expected Output:**
```
🔬 ADVANCED: Byte-Level UART2 Test

[Test 1] Sending: +++
   ✓ Sent to UART2
   Receiving: OK
   ✓ Module responded!

[Test 2] Sending: AT\r\n
   ✓ Sent to UART2
   Receiving: OK

[Test 3] Sending: AT+VER\r\n
   ✓ Sent to UART2
   Receiving: SX1262 LoRa DTU xF V2.1.0
   ✓ Got version string!
```

---

### Option D: GPIO Voltage Test (For Multimeter Correlation) ⏱️ 10 min

**Use This If You Have a Multimeter and Want to See Pin Voltages:**

**Step 1:** Same as Option B, but in Step 2, use:
```cpp
testGPIOVoltages();
// loraConfigureStream();  // Disabled for testing
```

**Expected Output:**
```
⚡ GPIO17/18 Voltage Test

Reading GPIO pin levels (use multimeter in parallel):
GPIO17 (TX) and GPIO18 (RX) should show changing voltages

[1] GPIO17: HIGH (3.3V)  |  GPIO18: LOW (0V)
[2] GPIO17: LOW (0V)     |  GPIO18: LOW (0V)
[3] GPIO17: HIGH (3.3V)  |  GPIO18: HIGH (3.3V)
...
```

**While this runs:**
- Touch multimeter probes to GPIO17 and GPIO18
- Should see voltage changing between 0V and 3.3V
- If always 0V → GPIO not configured correctly
- If always 3.3V → GPIO configured but not toggling

---

## 📋 Decision Tree

```
START: All AT commands timeout

├─ Have multimeter nearby?
│  ├─ YES → Try Option A first (10 min) → Check continuity & voltage
│  └─ NO → Go to Option B
│
└─ Multimeter shows problem fixed?
   ├─ YES → Restart ESP32, check Serial Monitor
   ├─ NO → Try Option B (run diagnostic code)
   │  └─ Did diagnostic show SUCCESS?
   │     ├─ YES → Problem was transient, try loraConfigureStream()
   │     └─ NO → Go to Option C (advanced test) OR contact support
   └─ Option B shows FAIL → You have wiring/power problem
      └─ Use printed troubleshooting checklist to fix
```

---

## 🔄 After Finding & Fixing Problem

**When you've fixed the issue:**

1. Edit [src/main.cpp](../src/main.cpp) around line 1230
2. **Comment OUT** the diagnostic call:
   ```cpp
   // diagnosticLoRaUART2Test();
   ```
3. **Uncomment** the normal function:
   ```cpp
   loraConfigureStream();
   ```
4. Compile and upload
5. Check Serial Monitor for successful AT command responses
6. Verify status report appears every 60 seconds

---

## ✅ Success Checklist

After fixing, you should see in Serial Monitor:

- [ ] LoRa DTU initialization message
- [ ] 9 AT commands with responses (✅ OK or ✅ Response: [data])
- [ ] Configuration complete message
- [ ] Status report every 60 seconds
- [ ] When LoRa data arrives: "📡 LoRa RX: Data Received" box

---

## 🆘 Stuck? Do This

1. Run Option A (Multimeter check) - usually finds the problem
2. If multimeter can't help, run Option B (Diagnostic code)
3. Screenshot the Serial Monitor output
4. Tell me:
   - ✅ What does multimeter show? (voltage, continuity)
   - ✅ What does diagnostic code show? (SUCCESS or FAIL?)
   - ✅ What are your GPIO17 and GPIO18 wired to?
   - ✅ What power supply voltage are you using? (3.3V or 5V?)

I'll help you fix it! 🔧

---

**Last Update:** 2025-01-15
**Status:** Ready for troubleshooting
