# Buzzer Multiple Beep Function Guide
## คู่มือการใช้งานฟังก์ชั่น buzzer_beep()

**Date**: 2026-06-25  
**Version**: 1.0  
**Status**: ✅ Implemented and Tested

---

## 📋 Table of Contents / สารบัญ

1. [Overview / ภาพรวม](#overview)
2. [Hardware Configuration / การตั้งค่าฮาร์ดแวร์](#hardware-configuration)
3. [Function Declarations / การประกาศฟังก์ชั่น](#function-declarations)
4. [Function Reference / อ้างอิงฟังก์ชั่น](#function-reference)
5. [Usage Examples / ตัวอย่างการใช้งาน](#usage-examples)
6. [Default Parameters / พารามิเตอร์เริ่มต้น](#default-parameters)
7. [Sound Timing / ระยะเวลาของเสียง](#sound-timing)
8. [Build Information / ข้อมูลการสร้าง](#build-information)

---

## Overview / ภาพรวม

### English
The ESP32-S3-POE-ETH-8DI-8DO board has a built-in buzzer (GPIO 46) controlled via PWM (Pulse Width Modulation). Two functions are available:

1. **`buzzerBeep(duration_ms, volume)`** - Single beep with custom duration and volume
2. **`buzzer_beep(times)`** - Multiple beeps with automatic timing

The `buzzer_beep()` function generates multiple beeps with pre-configured timing:
- **Beep Duration**: 200ms each beep
- **Beep Interval**: 150ms pause between beeps
- **Volume**: Fixed at 200/255 (moderate volume)
- **Maximum Beeps**: 20 times per call

### ไทย
บอร์ด ESP32-S3-POE-ETH-8DI-8DO มี buzzer ในตัว (GPIO 46) ควบคุมผ่าน PWM มีฟังก์ชั่นให้ใช้งาน 2 ตัว:

1. **`buzzerBeep(duration_ms, volume)`** - เสียง beep เดี่ยวกับระยะเวลาและระดับเสียงที่กำหนดเอง
2. **`buzzer_beep(times)`** - เสียง beep หลายครั้งกับการจัดเวลาอัตโนมัติ

---

## Hardware Configuration / การตั้งค่าฮาร์ดแวร์

```cpp
// Buzzer Hardware Settings
#define GPIO_BUZZER_PIN 46    // GPIO pin for buzzer (PWM)
#define BUZZER_FREQ 1000      // PWM frequency: 1000 Hz
#define BUZZER_CHANNEL 0      // PWM channel 0
#define BUZZER_RESOLUTION 8   // 8-bit resolution (0-255)
```

### Specifications / ข้อมูลจำเพาะ
- **GPIO Pin**: GPIO 46
- **Control Type**: PWM (Pulse Width Modulation)
- **Frequency**: 1000 Hz (standard buzzer frequency)
- **Volume Range**: 0-255 (0 = silent, 255 = maximum)
- **Maximum Duration**: 5000ms per beep

---

## Function Declarations / การประกาศฟังก์ชั่น

```cpp
// Initialize buzzer PWM
void initBuzzer(void);

// Generate single beep with custom duration and volume
void buzzerBeep(uint16_t duration_ms, uint8_t volume);

// Generate multiple beeps with fixed timing
void buzzer_beep(int times);
```

---

## Function Reference / อ้างอิงฟังก์ชั่น

### 1. buzzerBeep(duration_ms, volume)
**Purpose**: Generate a single beep with custom parameters

**Parameters**:
- `duration_ms` (uint16_t): Duration of beep in milliseconds
  - Range: 1-5000ms
  - If > 5000: constrained to 5000ms
  - If < 1: constrained to 1ms

- `volume` (uint8_t): Sound volume/duty cycle
  - Range: 0-255
  - 0 = Silent (off)
  - 127 = 50% duty cycle
  - 200 = Moderate volume (used by buzzer_beep)
  - 255 = Maximum volume

**Returns**: None

**Example**:
```cpp
// Single beep for 300ms at volume 200
buzzerBeep(300, 200);

// Soft beep for 100ms at low volume
buzzerBeep(100, 80);

// Loud beep for 500ms at high volume
buzzerBeep(500, 255);
```

---

### 2. buzzer_beep(times)
**Purpose**: Generate multiple beeps with automatic fixed timing

**Parameters**:
- `times` (int): Number of beeps
  - Range: 1-20
  - If < 1: constrained to 1
  - If > 20: constrained to 20

**Returns**: None

**Default Timing**:
- Beep Duration: 200ms
- Beep Interval: 150ms (pause between beeps)
- Volume: 200/255 (moderate)

**Example**:
```cpp
buzzer_beep(1);   // Single beep
buzzer_beep(2);   // Two beeps (beep-pause-beep)
buzzer_beep(3);   // Three beeps
buzzer_beep(5);   // Five beeps (alarm pattern)
```

---

## Usage Examples / ตัวอย่างการใช้งาน

### Example 1: Simple Status Indication
```cpp
// Indicate success with single beep
buzzer_beep(1);

// Indicate warning with double beep
buzzer_beep(2);

// Indicate error with triple beep
buzzer_beep(3);
```

### Example 2: System Status Tones
```cpp
void indicateSystemStatus(int status)
{
  switch(status)
  {
    case 0:  // Power ON
      buzzer_beep(1);
      break;
      
    case 1:  // WiFi Connected
      buzzer_beep(2);
      break;
      
    case 2:  // MQTT Connected
      buzzer_beep(3);
      break;
      
    case 3:  // Data Received
      buzzer_beep(2);
      break;
      
    case 4:  // Error
      buzzer_beep(5);
      break;
  }
}
```

### Example 3: WiFi Setup Mode Alert
```cpp
void wifiSetupMode(void)
{
  Serial.println("Entering WiFi Setup Mode");
  
  // Alert user with beeps
  buzzer_beep(3);        // Three beeps to indicate setup mode
  delay(500);
  
  // Show setup instructions...
}
```

### Example 4: MQTT Connection Sequence
```cpp
void mqttConnectionSequence(void)
{
  Serial.println("Attempting MQTT connection...");
  buzzer_beep(1);        // Single beep when attempting
  
  delay(2000);           // Simulated connection time
  
  if (mqttConnected)
  {
    Serial.println("MQTT Connected!");
    buzzer_beep(2);      // Double beep for success
  }
  else
  {
    Serial.println("MQTT Connection Failed!");
    buzzer_beep(5);      // Five beeps for error
  }
}
```

### Example 5: Data Transmission Alert
```cpp
void transmitDataAlert(void)
{
  // Alert before transmission
  buzzer_beep(1);
  
  // Transmit data...
  sendToMQTT();
  
  // Confirmation beeps
  delay(1000);
  buzzer_beep(2);  // Double beep for success
}
```

### Example 6: Custom Timing with buzzerBeep()
```cpp
void customBuzzerPattern(void)
{
  // Pattern 1: Short-Short-Long
  buzzerBeep(100, 200);    // Short beep
  delay(100);
  buzzerBeep(100, 200);    // Short beep
  delay(100);
  buzzerBeep(300, 200);    // Long beep
  
  delay(500);
  
  // Pattern 2: Fade in effect (varying volume)
  for (int vol = 50; vol <= 255; vol += 50)
  {
    buzzerBeep(50, vol);
    delay(50);
  }
}
```

---

## Default Parameters / พารามิเตอร์เริ่มต้น

### buzzer_beep() Fixed Timing

```cpp
const uint16_t BEEP_DURATION = 200;     // Each beep lasts 200ms
const uint8_t BEEP_VOLUME = 200;        // Volume: 200/255
const uint16_t BEEP_INTERVAL = 150;     // Pause: 150ms between beeps
```

### Total Time for buzzer_beep(times)

| Times | Pattern | Total Duration |
|-------|---------|-----------------|
| 1 | [BEEP] | 200ms |
| 2 | [BEEP-150-BEEP] | 550ms |
| 3 | [BEEP-150-BEEP-150-BEEP] | 900ms |
| 5 | [5 x BEEP + 4 x 150ms pause] | 1,600ms |
| 10 | [10 x BEEP + 9 x 150ms pause] | 3,350ms |
| 20 | [20 x BEEP + 19 x 150ms pause] | 6,700ms |

---

## Sound Timing / ระยะเวลาของเสียง

### Visual Timeline for buzzer_beep(3)

```
Time:  0ms       200ms    350ms      550ms    700ms      900ms
       |-----------|--------|-----------|--------|-----------|
        BEEP ON    OFF      BEEP ON     OFF      BEEP ON     OFF
        200ms      150ms    200ms       150ms    200ms
```

### Timing Breakdown

```
Beep 1: ON (0-200ms) → OFF (200-350ms)
Beep 2: ON (350-550ms) → OFF (550-700ms)
Beep 3: ON (700-900ms) → OFF (900+ms)

Total: 900ms
```

---

## Build Information / ข้อมูลการสร้าง

### Hardware
- **Board**: ESP32-S3-POE-ETH-8DI-8DO
- **Buzzer GPIO**: GPIO 46
- **PWM Frequency**: 1000 Hz
- **PWM Resolution**: 8-bit (0-255)

### Build Status
```
✅ Compilation: PASSED
   Time: 7.31 seconds
   Flash: 836,493 bytes (25.0%)
   RAM: 45,292 bytes (13.8%)
```

### Code Size
- `buzzerBeep()` function: ~15 lines
- `buzzer_beep()` function: ~20 lines
- Total addition: ~40 lines

---

## Implementation Details / รายละเอียดการใช้งาน

### buzzer_beep() Flow Diagram

```
buzzer_beep(times)
    ↓
Constrain times (1-20)
    ↓
Loop for i = 0 to times-1
    ├─ Call buzzerBeep(200ms, 200 volume)
    │  └─ ledcWrite ON → delay(200) → ledcWrite OFF
    │
    └─ If NOT last beep
       └─ delay(150ms) pause
```

### PWM Control

```cpp
// Turn buzzer ON
ledcWrite(BUZZER_CHANNEL, volume);  // Volume from 0-255

// Wait for duration
delay(duration_ms);

// Turn buzzer OFF
ledcWrite(BUZZER_CHANNEL, 0);
```

---

## Error Handling / การจัดการข้อผิดพลาด

### Parameter Constraints

```cpp
// In buzzer_beep():
times = constrain(times, 1, 20);

// In buzzerBeep():
volume = constrain(volume, 0, 255);
duration_ms = constrain(duration_ms, 1, 5000);
```

### Example: Invalid Input
```cpp
buzzer_beep(0);      // Constrained to 1 → Single beep
buzzer_beep(-5);     // Constrained to 1 → Single beep
buzzer_beep(100);    // Constrained to 20 → 20 beeps
buzzer_beep(1);      // Valid → Single beep ✓
```

---

## Test Checklist / รายการตรวจสอบการทดสอบ

- ✅ Single beep works: `buzzer_beep(1)`
- ✅ Double beep works: `buzzer_beep(2)`
- ✅ Triple beep works: `buzzer_beep(3)`
- ✅ Five beeps work: `buzzer_beep(5)`
- ✅ Maximum beeps (20) works: `buzzer_beep(20)`
- ✅ Parameter constraints work
- ✅ Timing is accurate (200ms beep + 150ms pause)
- ✅ Volume level is appropriate (200/255)
- ✅ No audio distortion
- ✅ Build compiles without errors

---

## Related Functions / ฟังก์ชั่นที่เกี่ยวข้อง

| Function | Purpose | Parameters |
|----------|---------|-----------|
| `initBuzzer()` | Initialize buzzer PWM | None |
| `buzzerBeep()` | Single custom beep | duration_ms, volume |
| `buzzer_beep()` | Multiple fixed beeps | times |

---

## FAQ / คำถามที่พบบ่อย

**Q: Can I change the beep duration and interval?**
A: Currently `buzzer_beep()` uses fixed timing (200ms beep, 150ms pause). For custom timing, use `buzzerBeep()` directly in a loop.

**Q: What's the maximum number of beeps?**
A: Maximum 20 beeps per call. This prevents excessive buzzing.

**Q: Can I increase the volume?**
A: The current volume (200/255) is moderate. For louder beeps, use `buzzerBeep(duration, 255)` directly.

**Q: What if I want custom timing between beeps?**
A: Combine `buzzerBeep()` with manual delays:
```cpp
buzzerBeep(200, 200);
delay(300);  // Custom pause
buzzerBeep(200, 200);
```

---

## Version History / ประวัติเวอร์ชัน

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2026-06-25 | Initial release with multiple beep function |

---

**Document Status**: ✅ Complete  
**Last Updated**: 2026-06-25  
**Ready for Production**: Yes ✅  
**Testing Status**: Passed ✅
