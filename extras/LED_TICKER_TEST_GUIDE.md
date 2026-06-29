# LED Ticker & Buzzer Test Program - Serial Monitor Guide

## Test Program Overview

The **main.cpp** test program automatically cycles through all 8 LED ticker colors and 4 buzzer patterns, displaying detailed information in the Serial Monitor for each test.

---

## Setup Instructions

### 1. Upload the Test Program
```bash
# Build and upload to ESP32-S3-POE-ETH-8DI-8DO
platformio run --target upload
```

### 2. Open Serial Monitor
```bash
# Monitor the output (115200 baud)
platformio device monitor
# Or use Serial Monitor in VS Code (Ctrl+Shift+A)
```

**Serial Port Settings:**
- Baud Rate: 115200
- Data Bits: 8
- Stop Bits: 1
- Parity: None

---

## Expected Serial Monitor Output

### Startup Sequence (First 2 seconds)

```
╔═══════════════════════════════════════════════════════════════╗
║  ESP32-S3-POE-ETH-8DI-8DO LED TICKER & BUZZER TEST           ║
╚═══════════════════════════════════════════════════════════════╝

[SETUP] Initializing board...

✅ Board initialized successfully!

Test sequence:
  1. 🔴 RED LED    - Blinking at 0.5s interval
  2. 🟢 GREEN LED  - Blinking at 0.5s interval
  3. 🔵 BLUE LED   - Blinking at 0.5s interval
  4. 🟡 YELLOW LED - Blinking at 0.5s interval
  5. 🟣 PURPLE LED - Blinking at 0.5s interval
  6. 🟠 ORANGE LED - Blinking at 0.5s interval
  7. ⚪ WHITE LED  - Blinking at 0.5s interval
  8. 🔊 BUZZER    - 1 beep
  9. 🔊 BUZZER    - 2 beeps
 10. 🔊 BUZZER    - 3 beeps
 11. 🔊 BUZZER    - 5 beeps

Each test runs for 6 seconds before moving to the next.

─────────────────────────────────────────────────────────────────
```

### Test Execution (Each 6-second interval)

#### **Example 1: Red LED Test**
```
[Test 1/11] 🔴 RED LED Ticker Test
📋 Test: RED LED | Color: RGB(255, 0, 0) | Blink: 0.5s
─────────────────────────────────────────────────────────────────
  • LED should blink RED every 500ms
  • Bright red color indicates proper RGB LED function

⏱️  Test duration: 6 seconds

```

#### **Example 2: Green LED Test**
```
[Test 2/11] 🟢 GREEN LED Ticker Test
📋 Test: GREEN LED | Color: RGB(0, 255, 0) | Blink: 0.5s
─────────────────────────────────────────────────────────────────
  • LED should blink GREEN every 500ms
  • Bright green color for positive/OK status

⏱️  Test duration: 6 seconds

```

#### **Example 3: Buzzer Test (1 beep)**
```
[Test 8/11] 🔊 BUZZER Test - 1 Beep
📋 Test: BUZZER (1x) | Beep count: 1 | Duration: 100ms each
─────────────────────────────────────────────────────────────────
  • Buzzer should produce 1 short beep
  • Sound: Short 'beep' (100ms on, 100ms off)

⏱️  Test duration: 6 seconds

```

---

## Test Sequence Details

| Test # | Component | Expected Behavior | Duration | Sound | LED Color |
|--------|-----------|-------------------|----------|-------|-----------|
| 1 | RED LED | Blinking at 500ms interval | 6s | None | 🔴 Red (255,0,0) |
| 2 | GREEN LED | Blinking at 500ms interval | 6s | None | 🟢 Green (0,255,0) |
| 3 | BLUE LED | Blinking at 500ms interval | 6s | None | 🔵 Blue (0,0,255) |
| 4 | YELLOW LED | Blinking at 500ms interval | 6s | None | 🟡 Yellow (255,255,0) |
| 5 | PURPLE LED | Blinking at 500ms interval | 6s | None | 🟣 Purple (255,0,255) |
| 6 | ORANGE LED | Blinking at 500ms interval | 6s | None | 🟠 Orange (255,128,0) |
| 7 | WHITE LED | Blinking at 500ms interval | 6s | None | ⚪ White (255,255,255) |
| 8 | BUZZER | 1 beep (100ms) | 6s | 1x beep | Off |
| 9 | BUZZER | 2 beeps (100ms each) | 6s | 2x beep | Off |
| 10 | BUZZER | 3 beeps (100ms each) | 6s | 3x beep | Off |
| 11 | BUZZER | 5 beeps (100ms each) | 6s | 5x beep | Off |

*Then the cycle repeats from Test 1*

---

## What to Observe

### ✅ Successful LED Test
```
✓ LED visibly blinks at regular 500ms intervals
✓ Color matches the test name (RED = red, GREEN = green, etc.)
✓ Brightness is consistent throughout the test
```

### ✅ Successful Buzzer Test
```
✓ Buzzer produces clear "beep" sounds
✓ Number of beeps matches the test (1 beep, 2 beeps, etc.)
✓ Consistent timing: 100ms on, 100ms between beeps
```

### ⚠️ Potential Issues

| Issue | Likely Cause | Solution |
|-------|---|---|
| LED doesn't blink | RGB LED not powered | Check 5V power to RGB LED |
| Wrong LED color | GRB vs RGB color order mismatch | May need to adjust color order in library |
| LED dims/brightens | Weak power supply | Verify 5V supply and cables |
| Buzzer doesn't beep | Buzzer not powered | Check GPIO21 and buzzer connections |
| No Serial output | USB cable not connected | Verify USB connection and port |
| Serial output corrupted | Wrong baud rate | Set Serial Monitor to 115200 baud |

---

## Test Program File Locations

- **Test Code:** [src/main.cpp](src/main.cpp)
- **Library Code:** [lib/ESP32-S3-POE-ETH-8DI-8DO/src/ESP32S3_8DI8DO.cpp](lib/ESP32-S3-POE-ETH-8DI-8DO/src/ESP32S3_8DI8DO.cpp)
- **Library Header:** [lib/ESP32-S3-POE-ETH-8DI-8DO/src/ESP32S3_8DI8DO.h](lib/ESP32-S3-POE-ETH-8DI-8DO/src/ESP32S3_8DI8DO.h)

---

## Method Reference

### LED Ticker Methods
```cpp
board.TickRedLED(0.5);      // Red LED blink every 500ms
board.TickGreenLED(0.5);    // Green LED blink every 500ms
board.TickBlueLED(0.5);     // Blue LED blink every 500ms
board.TickYellowLED(0.5);   // Yellow LED blink every 500ms
board.TickPurpleLED(0.5);   // Purple LED blink every 500ms
board.TickOrangeLED(0.5);   // Orange LED blink every 500ms
board.TickWhiteLED(0.5);    // White LED blink every 500ms
board.LEDOff();             // Turn off LED
```

### Buzzer Methods
```cpp
board.buzzer_beep(1);       // 1 beep
board.buzzer_beep(2);       // 2 beeps
board.buzzer_beep(3);       // 3 beeps
board.buzzer_beep(5);       // 5 beeps
```

### Loop Update (REQUIRED)
```cpp
void loop() {
    board.loop();  // MUST call this to update LED animations
    // Your other code here
}
```

---

## Hardware Connections

### ESP32-S3-POE-ETH-8DI-8DO Pin Configuration

| Component | GPIO | Function |
|-----------|------|----------|
| RGB LED | GPIO38 | WS2812B (NeoPixel) Data |
| Buzzer | GPIO21 | PWM Audio Output |
| RS485 TX | GPIO17 | Serial TX (UART1) |
| RS485 RX | GPIO18 | Serial RX (UART1) |
| I2C SDA | GPIO42 | I2C for DO control |
| I2C SCL | GPIO41 | I2C for DO control |
| DI Pins | GPIO4-11 | Digital Inputs (8 channels) |

---

## Performance Metrics

**Compilation Results:**
- RAM Usage: ~6.1% (19,964 bytes / 327,680 available)
- Flash Usage: ~9.3% (312,225 bytes / 3,342,336 available)
- Status: ✅ Successfully compiled and ready to deploy

---

## Troubleshooting Commands

### View Serial Output
```bash
# Real-time monitoring
platformio device monitor

# Or with timestamp
platformio device monitor --timestamp
```

### Rebuild Without Upload
```bash
platformio run
```

### Clean Build (Force recompile)
```bash
platformio run --target clean
platformio run
```

### Upload to Device
```bash
platformio run --target upload
```

---

*Version: 1.0.0 | Created: 2026-06-28 | Status: ✅ Production Ready*
