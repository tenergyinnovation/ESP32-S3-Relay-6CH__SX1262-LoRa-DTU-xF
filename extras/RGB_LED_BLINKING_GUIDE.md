# RGB LED Blinking Control Guide
## คำแนะนำการควบคุมการกระพริบของ RGB LED

**Version**: 1.1 (Updated - Uses RGB LED instead of separate GPIO)  
**Date**: 2026-06-25  
**Language**: Thai & English  

---

## 📝 Overview / บทนำ

This guide explains how to use the updated `TickRedLED()` function to control the RGB LED blinking patterns on the ESP32-S3-POE-ETH-8DI-8DO Gateway. The function now works seamlessly with the `setRGBColor()` function to control the RGB LED (WS2812 NeoPixel).

**ไทย**: เอกสารนี้อธิบายวิธีการใช้ฟังก์ชั่น `TickRedLED()` ที่อัปเดตแล้ว เพื่อควบคุมการกระพริบของ RGB LED บนบอร์ด ESP32-S3-POE-ETH-8DI-8DO Gateway โดยฟังก์ชั่นนี้ทำงานร่วมกับ `setRGBColor()` เพื่อควบคุม RGB LED (WS2812 NeoPixel)

---

## 🔴 Hardware Information / ข้อมูลฮาร์ดแวร์

### RGB LED Specifications
- **GPIO Pin**: GPIO 38 (Data line)
- **Type**: WS2812 NeoPixel (RGB addressable LED)
- **Voltage**: 5V (power), 3.3V (data)
- **Number of LEDs**: 1
- **Protocol**: Single-wire digital communication
- **Color Range**: RGB (0-255 each channel)

### RGB LED Pin Configuration
```
GPIO 38 ──────[DATA]──────► WS2812 LED ─┐
                                        │
Power (5V) ───────────────► +5V ───────┤
                                        │
GND ──────────────────────► GND ────────┘
```

---

## 🛠️ Function Signature / ลายเซ็นของฟังก์ชั่น

```cpp
void TickRedLED(float seconds);
```

### Parameters / พารามิเตอร์

| Value | Behavior | Description |
|-------|----------|-------------|
| **0** | RGB LED OFF | Turn off the RGB LED (set to black/0,0,0) |
| **-1** | RGB LED ON (Red) | Turn on RGB LED permanently (red 255,0,0, no blinking) |
| **> 0** | Blinking (Red) | RGB LED blinks red at specified interval (seconds) |
| **< -1** | Invalid | Treated same as 0 (LED OFF) |

### ตารางพารามิเตอร์ (ไทย)

| ค่า | พฤติกรรม | คำอธิบาย |
|-----|---------|---------|
| **0** | RGB LED ดับ | ปิด RGB LED โดยสิ้นเชิง (เซ็ต 0,0,0) |
| **-1** | RGB LED สว่าง (แดง) | เปิด RGB LED ค้างไว้ตลอดเวลา (แดง 255,0,0, ไม่กระพริบ) |
| **> 0** | กระพริบ (แดง) | RGB LED กระพริบสีแดงตามเวลาที่กำหนด |
| **< -1** | ไม่ถูกต้อง | ถือว่า 0 (LED ดับ) |

---

## 📖 Usage Examples / ตัวอย่างการใช้งาน

### Example 1: Turn OFF RGB LED
```cpp
TickRedLED(0);
// Output: Serial Monitor shows "🔴 RGB LED (Red): OFF"
// RGB LED turns BLACK (0, 0, 0)
```

### Example 2: Turn ON RGB LED Permanently (Red)
```cpp
TickRedLED(-1);
// Output: Serial Monitor shows "🔴 RGB LED (Red): ON (continuous)"
// RGB LED displays RED (255, 0, 0) continuously
// LED stays ON until TickRedLED(0) is called
```

### Example 3: Slow Blinking (1 second interval)
```cpp
TickRedLED(1.0);
// Output: Serial Monitor shows "🔴 RGB LED (Red): Blinking every 1.0 seconds"
// RGB LED blinks: RED for 0.5s, BLACK for 0.5s, repeat
```

### Example 4: Medium Blinking (500ms interval)
```cpp
TickRedLED(0.5);
// Output: Serial Monitor shows "🔴 RGB LED (Red): Blinking every 0.5 seconds"
// RGB LED blinks: RED for 0.25s, BLACK for 0.25s, repeat
```

### Example 5: Fast Blinking (200ms interval)
```cpp
TickRedLED(0.2);
// Output: Serial Monitor shows "🔴 RGB LED (Red): Blinking every 0.2 seconds"
// RGB LED blinks rapidly: RED for 0.1s, BLACK for 0.1s, repeat
```

### Example 6: Very Slow Blinking (2 second interval)
```cpp
TickRedLED(2.0);
// Output: Serial Monitor shows "🔴 RGB LED (Red): Blinking every 2.0 seconds"
// RGB LED blinks slowly: RED for 1s, BLACK for 1s, repeat
```

---

## 🔄 Blink Interval Explanation / อธิบายช่วงเวลาการกระพริบ

The `seconds` parameter defines the **total cycle time** for blinking:

```
seconds = 1.0  (1 second total cycle)
├── ON (RED)  for  0.5 seconds  ┐
└── OFF (BLACK) for 0.5 seconds ┘ (repeats)

seconds = 0.5  (500ms total cycle)
├── ON (RED)  for  0.25 seconds ┐
└── OFF (BLACK) for 0.25 seconds ┘ (repeats)

seconds = 2.0  (2 second total cycle)
├── ON (RED)  for  1.0 second   ┐
└── OFF (BLACK) for 1.0 second  ┘ (repeats)
```

---

## 🎨 Customizing Blink Colors / การปรับแต่งสีการกระพริบ

The `TickRedLED()` function defaults to blinking between red (255,0,0) and black (0,0,0). You can customize the colors by modifying the global variables:

### Default Colors (จำเป็นต้องแก้ไขโค้ด)
```cpp
// Current default colors (in main.cpp):
uint8_t blinkRedColor[3] = {255, 0, 0};  // Red color (ON state)
uint8_t blinkGreenColor[3] = {0, 0, 0};  // Black color (OFF state)
```

### To Change Colors / เปลี่ยนสี
Modify these variables before calling `TickRedLED()`:

```cpp
// Example: Blink between GREEN and BLACK
void setup() {
  // ... other initialization ...
  
  // Change blink colors to green
  blinkRedColor[0] = 0;     // Red = 0
  blinkRedColor[1] = 255;   // Green = 255 (max)
  blinkRedColor[2] = 0;     // Blue = 0
  
  // Now call TickRedLED() to blink green
  TickRedLED(0.5);
}
```

### Color Examples / ตัวอย่างสี

| Color | RGB Values | Code |
|-------|-----------|------|
| **Red** | (255, 0, 0) | `blinkRedColor[0]=255; blinkRedColor[1]=0; blinkRedColor[2]=0;` |
| **Green** | (0, 255, 0) | `blinkRedColor[0]=0; blinkRedColor[1]=255; blinkRedColor[2]=0;` |
| **Blue** | (0, 0, 255) | `blinkRedColor[0]=0; blinkRedColor[1]=0; blinkRedColor[2]=255;` |
| **Yellow** | (255, 255, 0) | `blinkRedColor[0]=255; blinkRedColor[1]=255; blinkRedColor[2]=0;` |
| **Cyan** | (0, 255, 255) | `blinkRedColor[0]=0; blinkRedColor[1]=255; blinkRedColor[2]=255;` |
| **Magenta** | (255, 0, 255) | `blinkRedColor[0]=255; blinkRedColor[1]=0; blinkRedColor[2]=255;` |
| **White** | (255, 255, 255) | `blinkRedColor[0]=255; blinkRedColor[1]=255; blinkRedColor[2]=255;` |
| **Orange** | (255, 165, 0) | `blinkRedColor[0]=255; blinkRedColor[1]=165; blinkRedColor[2]=0;` |
| **Purple** | (128, 0, 255) | `blinkRedColor[0]=128; blinkRedColor[1]=0; blinkRedColor[2]=255;` |

---

## 💡 System State Indication / การบ่งชี้สถานะของระบบ

### Recommended LED Patterns for Different States

| System State | LED Pattern | Code |
|--------------|------------|------|
| **Startup/Boot** | Fast Blinking Red (0.2s) | `TickRedLED(0.2);` |
| **WiFi Connecting** | Medium Blinking Red (0.5s) | `TickRedLED(0.5);` |
| **WiFi Connected** | Slow Blinking Red (2.0s) | `TickRedLED(2.0);` |
| **MQTT Connected** | Red Always ON | `TickRedLED(-1);` |
| **Error/Alert** | Very Fast Blinking (0.1s) | `TickRedLED(0.1);` |
| **System Idle** | OFF | `TickRedLED(0);` |
| **Data Processing** | Medium Blinking (0.5s) | `TickRedLED(0.5);` |

---

## 📝 Implementation in setup() / การนำไปใช้ในฟังก์ชั่น setup()

```cpp
void setup() {
  // ... other initialization code ...
  
  // Show startup status with fast blinking
  TickRedLED(0.2);
  delay(2000);
  
  // Indicate WiFi connection attempt with medium blinking
  TickRedLED(0.5);
}
```

---

## 📝 Implementation in loop() / การนำไปใช้ในฟังก์ชั่น loop()

```cpp
void loop() {
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    // WiFi connected - indicate with slow blinking
    static bool wifiWasConnected = false;
    if (!wifiWasConnected) {
      TickRedLED(2.0);  // Change to slow blinking
      wifiWasConnected = true;
    }
  } else {
    // WiFi disconnected - indicate with medium blinking
    static bool wifiWasConnected = true;
    if (wifiWasConnected) {
      TickRedLED(0.5);  // Change to medium blinking
      wifiWasConnected = false;
    }
  }
  
  // Check MQTT connection status
  if (client.connected()) {
    // MQTT connected - keep LED on (red)
    TickRedLED(-1);
  }
  
  // ... rest of loop code ...
}
```

---

## 🔧 Advanced: Changing Colors Dynamically / ปรับเปลี่ยนสีแบบไดนามิก

```cpp
// Create a function to change blink colors and restart blinking
void setBlinkColor(uint8_t red, uint8_t green, uint8_t blue, float blinkInterval) {
  // Update color variables
  blinkRedColor[0] = red;
  blinkRedColor[1] = green;
  blinkRedColor[2] = blue;
  
  // Restart blinking with new color
  if(blinkInterval > 0) {
    TickRedLED(blinkInterval);
  }
}

// Usage in loop():
void loop() {
  // Blink Green at 1Hz when WiFi connected
  if (WiFi.status() == WL_CONNECTED) {
    setBlinkColor(0, 255, 0, 1.0);  // Green, 1 second blink
  }
  
  // Blink Blue at 0.5Hz for MQTT connected
  if (client.connected()) {
    setBlinkColor(0, 0, 255, 0.5);  // Blue, 500ms blink
  }
}
```

---

## 🔍 Troubleshooting / การแก้ไขปัญหา

### Problem 1: RGB LED not blinking
**Issue**: LED stays OFF despite calling `TickRedLED(0.5)`

**Solutions**:
1. ✅ Check GPIO 38 is connected to RGB LED data pin
2. ✅ Verify RGB LED is powered (5V supply)
3. ✅ Check Serial Monitor for initialization message
4. ✅ Verify `initRGB()` was called in `setup()`
5. ✅ Try `TickRedLED(-1)` to turn ON permanently

### Problem 2: LED always stays ON
**Issue**: LED doesn't respond to `TickRedLED(0.5)` calls

**Solutions**:
1. ✅ Check if `TickRedLED(-1)` was last called
2. ✅ Try `TickRedLED(0)` to turn OFF
3. ✅ Power cycle the board
4. ✅ Check Ticker library is properly included

### Problem 3: LED displays wrong color
**Issue**: LED blinks in different color or not red

**Solutions**:
1. ✅ Verify `blinkRedColor[]` array is set to (255, 0, 0)
2. ✅ Check if color was changed in code
3. ✅ Try default colors: `setRGBColor(255, 0, 0)`

### Problem 4: Blink rate is incorrect
**Issue**: LED blinks faster or slower than expected

**Solutions**:
1. ✅ Verify Ticker library is properly included
2. ✅ Check Serial Monitor for actual blink rate message
3. ✅ Try different values (0.2, 0.5, 1.0, 2.0)
4. ✅ High CPU load might affect timing

---

## 🔗 Related Functions / ฟังก์ชั่นที่เกี่ยวข้อง

- `setRGBColor(uint8_t red, uint8_t green, uint8_t blue)` - Set RGB LED to specific color (direct control)
- `initRGB()` - Initialize RGB LED
- `redLEDCallback()` - Internal callback for Ticker (auto-called)

---

## 📊 Global Variables / ตัวแปร Global

```cpp
// These variables are managed by TickRedLED() function
Ticker redLEDTicker;              // Ticker object for timing
bool redLEDBlinking;              // Is LED currently blinking?
uint8_t blinkRedColor[3];         // RGB color when LED is ON
uint8_t blinkGreenColor[3];       // RGB color when LED is OFF (usually black)
```

---

## ✅ Verification Checklist / รายการตรวจสอบ

- [ ] RGB LED GPIO 38 is connected properly
- [ ] RGB LED is powered (5V)
- [ ] Serial Monitor shows initialization message
- [ ] TickRedLED(0) turns OFF LED
- [ ] TickRedLED(-1) turns ON LED permanently (red)
- [ ] TickRedLED(0.5) blinks LED every 500ms
- [ ] TickRedLED(1.0) blinks LED every 1 second
- [ ] Blink patterns match expected behavior
- [ ] Color can be changed using `blinkRedColor[]`
- [ ] No compilation errors in platformio
- [ ] LED responds to commands in main loop

---

## 🔄 Difference from Previous Version / ความแตกต่างจากเวอร์ชั่นก่อนหน้า

| Feature | Previous Version | Current Version |
|---------|-----------------|-----------------|
| **LED Type** | Separate GPIO (Digital) | RGB LED (WS2812) |
| **GPIO Used** | GPIO 47 | GPIO 38 (shared with RGB) |
| **Color** | Single (On/Off only) | Full RGB (customizable) |
| **Integration** | Standalone | Works with setRGBColor() |
| **Blinking** | GPIO toggle | RGB color toggle |
| **Flexibility** | Limited | Full RGB color control |

---

**Document Status**: ✅ Complete  
**Last Updated**: 2026-06-25  
**Ready for Production**: Yes ✅
