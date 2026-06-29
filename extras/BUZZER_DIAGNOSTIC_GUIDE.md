# 🔊 Buzzer Diagnostic & Troubleshooting Guide

## Issue Summary
Buzzer command executes successfully ("✅ Buzzer complete") but produces **no audible sound**.

## Root Cause Investigation Checklist

### 1️⃣ **Verify Code is Uploaded** ⚠️ CRITICAL FIRST STEP
After uploading, verify the code compiled and uploaded successfully:
```
✓ Build: Exit 0 (No errors)
✓ Upload: Exit 0 (No errors)
```

**If upload failed**: 
- Check USB cable connection
- Verify correct board selected (ESP32-S3-DevKit)
- Try uploading again

---

## 🔧 Hardware Verification

### Step 1: Check GPIO21 Connection
**Buzzer Pin**: GPIO21

**Wiring Requirements**:
```
┌──────────────────────────┐
│  ESP32-S3-Relay-6CH      │
├──────────────────────────┤
│ GPIO21 ──→ Buzzer(+)     │  ← Buzzer positive pin
│ GND    ──→ Buzzer(-)     │  ← Buzzer ground/negative pin
└──────────────────────────┘
```

**Check**:
- [ ] Buzzer pins securely connected (no loose wires)
- [ ] No bent or damaged pins
- [ ] Buzzer polarity correct (if buzzer has + and - markings)

---

### Step 2: Power Supply Check
Buzzer typically requires 3.3V - 5V DC:

**Test with Multimeter**:
```
Red probe   → GPIO21
Black probe → GND
```
Should show **3.3V** when buzzer is active

If showing 0V or incorrect voltage → Hardware issue

---

### Step 3: Physical Buzzer Inspection
- [ ] Buzzer is not burned out (black marks or damage)
- [ ] Buzzer model specification matches board requirements
- [ ] No solder bridges or cold solder joints
- [ ] Buzzer is **passive type** (PWM-controlled, not active)

---

## 📋 Software Testing Steps

### Test 1: Run BUZZER_TEST (Hardware Diagnostic)
```
📝 > BUZZER_TEST
🔊 Running buzzer hardware test...
   Continuous tone for 2 seconds...
✅ Buzzer test complete
```

**Expected**: Continuous beeping sound for ~2 seconds

**If NO sound**:
1. GPIO21 may be damaged
2. Buzzer not connected
3. Buzzer is defective
4. Power supply issue

---

### Test 2: Run BUZZER Command
```
📝 > BUZZER 3
🔊 Buzzing 3 time(s)...
✅ Buzzer complete
```

**Expected**: 3 distinct beeps with silences between them
- Beep: 100ms ON
- Silence: 100ms OFF
- Repeat 3 times

---

### Test 3: Serial Monitor Output
Monitor serial output while running buzzer:

**Correct Output**:
```
Board initialized successfully
✓ Relay: Ready
✓ RGB LED: Ready
✓ Buzzer: Ready (1000 Hz, PWM Channel 0)

📝 > BUZZER 2
🔊 Buzzing 2 time(s)...
✅ Buzzer complete
```

---

## 🔍 Detailed Troubleshooting

### Problem: No Sound at All

#### A. Check Board Initialization
Look for these messages in serial monitor on startup:
```
✓ Buzzer: Ready (1000 Hz, PWM Channel 0)
```

**If missing**: Buzzer initialization failed
- Check ESP32 power supply
- Verify no GPIO conflicts
- Check for short circuits

---

#### B. Verify PWM Pin Assignment
Current configuration:
```cpp
GPIO21 (PWM_CHANNEL 0, 1000 Hz frequency, 8-bit resolution)
```

**Check if conflicting**:
- GPIO21 used for other purposes?
- PWM Channel 0 already in use?
- Frequency too high/low for buzzer?

---

#### C. Test Direct GPIO Control
If buzzer still doesn't work, there may be a GPIO21 issue:

**Add this diagnostic code temporarily to main.cpp**:
```cpp
void testGPIO21() {
    Serial.println("\n🔧 Testing GPIO21 direct control...");
    
    pinMode(21, OUTPUT);
    
    // Test 1: Set HIGH
    digitalWrite(21, HIGH);
    Serial.println("✓ GPIO21 set to HIGH");
    delay(1000);
    
    // Test 2: Set LOW
    digitalWrite(21, LOW);
    Serial.println("✓ GPIO21 set to LOW");
    delay(1000);
    
    Serial.println("🔧 GPIO21 direct test complete\n");
}
```

**Test**: Measure GPIO21 voltage with multimeter while running test
- Should show 3.3V during HIGH
- Should show 0V during LOW

---

### Problem: Intermittent Sound

#### Possible Causes:
1. **Loose wire connection** → Secure connections
2. **Partial short circuit** → Check for bent pins
3. **Power supply fluctuation** → Use stable 5V supply
4. **GPIO conflict** → Verify GPIO21 not used elsewhere

---

### Problem: Wrong Sound Frequency

#### Adjust Buzzer Frequency
Edit [ESP32S3_Relay6CH.h](../lib/ESP32-S3-Relay-6CH/src/ESP32S3_Relay6CH.h) line ~45:

```cpp
// Original
static constexpr uint16_t DEFAULT_FREQ = 1000;  // Hz

// Try different frequencies
static constexpr uint16_t DEFAULT_FREQ = 2000;  // Higher pitch
static constexpr uint16_t DEFAULT_FREQ = 500;   // Lower pitch
```

---

## 🧪 Advanced Testing

### Test 1: Check LEDC PWM Setup
Add to main.cpp for verification:
```cpp
void checkLEDC() {
    Serial.println("\n📊 LEDC PWM Status:");
    Serial.print("  Channel 0: ");
    Serial.println(ledcRead(0) > 0 ? "ACTIVE" : "INACTIVE");
    Serial.println();
}
```

---

### Test 2: Measure Duty Cycle
With oscilloscope on GPIO21:
- Should see square wave at 1000 Hz during beep
- Duty cycle: ~100% ON during buzzer_beep()
- Should drop to 0% during silence periods

---

## 📋 Pinout Reference

### ESP32-S3-Relay-6CH Pin Configuration
```
GPIO21  → Buzzer (PWM Channel 0)
GPIO38  → RGB LED (NeoPixel)
GPIO1   → Relay 1
GPIO2   → Relay 2
GPIO41  → Relay 3
GPIO42  → Relay 4
GPIO45  → Relay 5
GPIO46  → Relay 6
GPIO4   → RS485 TX
GPIO5   → RS485 RX
GND     → Ground (Connect to buzzer negative)
3V3     → Power (Can be used for auxiliary circuits)
```

---

## ✅ Solution Steps (In Order)

1. **Recompile and Upload**
   - Clean build: `PlatformIO → Project Tasks → Clean`
   - Rebuild: `PlatformIO → Build`
   - Upload: `PlatformIO → Upload`

2. **Run BUZZER_TEST**
   - If sound → Problem resolved ✅
   - If no sound → Proceed to step 3

3. **Check Physical Connections**
   - Inspect GPIO21 wiring
   - Measure voltage on GPIO21 (should be ~3.3V during test)
   - Reseat buzzer connector

4. **Check for GPIO Conflicts**
   - Verify GPIO21 not used elsewhere
   - Check for overlapping PWM channels
   - Run diagnostic tests above

5. **Test Alternative Buzzer**
   - If available, try different buzzer model
   - Verify buzzer specification matches board

6. **Contact Support**
   - If steps 1-5 don't resolve issue
   - Provide: Serial monitor output, testing results, GPIO voltage readings

---

## 🎯 Expected Behavior After Fix

### BUZZER Command Output
```
📝 > BUZZER 3
🔊 Buzzing 3 time(s)...
✅ Buzzer complete
```
🔊 **Sound**: Three distinct 100ms beeps with 100ms silence

### BUZZER_TEST Output
```
📝 > BUZZER_TEST
🔊 Running buzzer hardware test...
   Continuous tone for 2 seconds...
✅ Buzzer test complete
```
🔊 **Sound**: Continuous beeping for ~2 seconds

---

## 📞 Support Information

**If issue persists after all steps**:

1. Run `STATUS` command and note output
2. Take oscilloscope reading of GPIO21 during buzzer operation
3. Check power supply voltage (should be stable 3.3V or 5V)
4. Verify board purchased from authorized distributor
5. Document findings and contact Tenergy Innovation support

---

**Version**: 1.0.0  
**Date**: 29 June 2026  
**Board**: ESP32-S3-Relay-6CH  
**Status**: Diagnostic & Troubleshooting
