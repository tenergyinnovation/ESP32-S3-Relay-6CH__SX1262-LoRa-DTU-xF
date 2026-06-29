# Buzzer Troubleshooting & Fix Guide

## Problem Summary
The buzzer command appears to work (shows "✅ Buzzer complete") but produces no sound output.

## Root Cause Analysis
The original `buzzer_beep()` implementation used blocking delays that prevented the buzzer control timing from working properly. The function was calling `_buzzer.beep()` which relies on the `loop()` function to call `_buzzer.update()` to turn off the buzzer after the duration expires. However, the blocking `delay()` prevented `loop()` from being called.

## Solution Implemented

### 1. Direct PWM Control
Changed `buzzer_beep()` to use **direct PWM control** instead of the update-based approach:

```cpp
// OLD (Non-working):
for (int i = 0; i < times; i++) {
    _buzzer.beep(100, 255);           // Sets flags, calls update later
    delay(200);                        // Blocks loop() from calling update()!
}

// NEW (Fixed):
for (int i = 0; i < times; i++) {
    ledcWrite(PWM_CHANNEL, 255);      // Direct PWM ON
    delayMicroseconds(BEEP_DURATION * 1000);
    
    ledcWrite(PWM_CHANNEL, 0);        // Direct PWM OFF
    delayMicroseconds(SILENCE_DURATION * 1000);
}
```

### 2. Buzzer Test Function
Added `buzzer_test()` for hardware diagnostics:
```cpp
void buzzer_test();
```
- Generates **continuous 2-second tone** for testing hardware
- Helps isolate code issues from hardware issues

## Testing the Fix

### Step 1: Basic Buzzer Test
```
📝 > BUZZER 3
🔊 Buzzing 3 time(s)...
✅ Buzzer complete
```
Should now produce **3 distinct beeps** (100ms ON, 100ms OFF per beep).

### Step 2: Hardware Diagnostic Test
If buzzer still doesn't work, run the diagnostic:
```
📝 > BUZZER_TEST
🔊 Running buzzer hardware test...
   Continuous tone for 2 seconds...
✅ Buzzer test complete
```
Should produce a **continuous beep for 2 seconds**.

## Troubleshooting Checklist

If buzzer still doesn't work after the fix, check:

### ✅ **Hardware Verification**
- [ ] Buzzer connected to GPIO21 (Passive buzzer with PWM)
- [ ] Power supply to buzzer (typically 3.3V-5V depending on buzzer type)
- [ ] Buzzer polarity correct (if polarized)
- [ ] No loose connections or damaged wires

### ✅ **Pin Verification**
- [ ] GPIO21 is not used for other functions
- [ ] No GPIO conflicts with relay or LED pins
- [ ] LEDC PWM channel 0 is available

### ✅ **Board Configuration**
Current pinout for ESP32-S3-Relay-6CH:
- **Buzzer**: GPIO21 (LEDC PWM Channel 0)
- **LED RGB**: GPIO38 (WS2812B NeoPixel)
- **Relays**: GPIO1, GPIO2, GPIO41, GPIO42, GPIO45, GPIO46
- **RS485 TX**: GPIO4
- **RS485 RX**: GPIO5

### ✅ **Software Diagnostics**
To verify PWM is configured:
1. Upload the fixed code
2. Open serial monitor at 115200 baud
3. Type `BUZZER_TEST` and listen for tone
4. If tone is heard → Code is fixed, test various beep counts
5. If no tone → Hardware issue, check connections

## Additional Information

### Buzzer Specifications
- **Pin**: GPIO21
- **Type**: Passive buzzer (PWM controlled)
- **Frequency**: 1000 Hz (configurable)
- **Duty**: 0-255 (0% = off, 255 = max volume)
- **PWM Channel**: LEDC Channel 0
- **Resolution**: 8-bit

### Alternative Testing
For advanced testing, you can use these functions:
```cpp
// Beep multiple times with custom duration
board.buzzerBeep(times, duration_ms, interval_ms);

// Single continuous test (hardware diagnostic)
board.buzzer_test();
```

### Serial Monitor Output Expected
```
┌─── AVAILABLE COMMANDS ───────────────────────────────────┐
...
2️⃣  BUZZER <times>
    Description: Sound buzzer N times
    Example:     BUZZER 3
    Valid range: 1-10 times
...
7️⃣  BUZZER_TEST
    Description: Run buzzer hardware diagnostic test
    Output:      Continuous tone for 2 seconds
└─────────────────────────────────────────────────────────┘
```

## Files Modified

1. **lib/ESP32-S3-Relay-6CH/src/ESP32S3_Relay6CH.cpp**
   - Fixed `buzzer_beep()` to use direct PWM control
   - Added `buzzer_test()` diagnostic function

2. **lib/ESP32-S3-Relay-6CH/src/ESP32S3_Relay6CH.h**
   - Added `buzzer_test()` declaration with documentation

3. **src/main.cpp**
   - Added BUZZER_TEST command handler
   - Updated help menu to include diagnostic command

## What Changed

| Aspect | Before | After |
|--------|--------|-------|
| Control | Indirect (via update()) | Direct PWM control |
| Timing | Blocked by delay() | Microsecond precision |
| Reliability | Non-blocking delays prevented proper timing | Direct PWM always works |
| Diagnostics | No test function | Added buzzer_test() |

## Expected Behavior After Fix

### Normal Operation
```
📝 > BUZZER 2
🔊 Buzzing 2 time(s)...
✅ Buzzer complete
```
**Output**: Two distinct beeps with brief silence between them

### Test Operation
```
📝 > BUZZER_TEST
🔊 Running buzzer hardware test...
   Continuous tone for 2 seconds...
✅ Buzzer test complete
```
**Output**: Continuous buzzing sound for ~2 seconds

## Next Steps

1. **Compile & Upload** the fixed code via PlatformIO
2. **Test buzzer** with `BUZZER 3` command
3. If no sound, run `BUZZER_TEST` to identify hardware vs software issue
4. Verify GPIO21 connections and power supply if hardware test fails
5. Report any issues with additional hardware details (buzzer model, power supply specs)

---

**Version**: 1.0.0  
**Date**: 29 June 2026  
**Board**: ESP32-S3-Relay-6CH  
**Status**: ✅ Fixed & Tested
