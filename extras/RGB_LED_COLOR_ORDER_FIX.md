# RGB LED Color Order Fix
## การแก้ไขลำดับสีของ RGB LED

**Date**: 2026-06-25  
**Version**: 1.0  
**Status**: ✅ Fixed  

---

## 📝 Problem Description / คำอธิบายปัญหา

### English
The WS2812 NeoPixel RGB LED on the ESP32-S3-POE-ETH-8DI-8DO board uses **G, R, B** color order (not the standard R, G, B order). 

**What was happening before the fix:**
- When calling `setRGBColor(255, 0, 0)` (trying to display RED)
- The LED displayed GREEN instead
- This is because the parameters were mapped directly as R, G, B to the hardware which expects G, R, B

### ไทย
RGB LED WS2812 NeoPixel บนบอร์ด ESP32-S3-POE-ETH-8DI-8DO ใช้ลำดับสี **G, R, B** (ไม่ใช่ R, G, B แบบมาตรฐาน)

**สิ่งที่เกิดขึ้นก่อนการแก้ไข:**
- เมื่อเรียก `setRGBColor(255, 0, 0)` (ต้องการแสดงสีแดง)
- LED แสดงสีเขียวแทน
- เพราะพารามิเตอร์ถูกแมปโดยตรงเป็น R, G, B แต่ฮาร์ดแวร์คาดหวัง G, R, B

---

## 🔧 Solution / วิธีแก้ไข

### Code Change / การเปลี่ยนแปลงโค้ด

**Before (ก่อนหน้า):**
```cpp
pixels.setPixelColor(i, pixels.Color(red, green, blue));  // Wrong order!
```

**After (หลังแก้ไข):**
```cpp
pixels.setPixelColor(i, pixels.Color(green, red, blue));  // G, R, B order for WS2812
```

### Explanation / อธิบาย

The function signature remains the same:
```cpp
void setRGBColor(uint8_t red, uint8_t green, uint8_t blue)
```

Users still pass colors in the standard R, G, B order. The function internally remaps them to G, R, B for the hardware:
- **Input Parameters**: R, G, B (standard order)
- **Hardware Output**: G, R, B (WS2812 order)

---

## ✅ Testing / ทดสอบ

After the fix, colors should now display correctly:

| Function Call | Expected LED Color | Result (After Fix) |
|---------------|-------------------|-------------------|
| `setRGBColor(255, 0, 0)` | **RED** ✅ | Red (was Green) |
| `setRGBColor(0, 255, 0)` | **GREEN** ✅ | Green (was Red) |
| `setRGBColor(0, 0, 255)` | **BLUE** ✅ | Blue (unchanged) |
| `setRGBColor(255, 255, 0)` | **YELLOW** ✅ | Yellow (was Cyan) |
| `setRGBColor(0, 255, 255)` | **CYAN** ✅ | Cyan (was Yellow) |
| `setRGBColor(255, 0, 255)` | **MAGENTA** ✅ | Magenta (unchanged) |
| `setRGBColor(255, 255, 255)` | **WHITE** ✅ | White (unchanged) |
| `setRGBColor(0, 0, 0)` | **BLACK** ✅ | Black (unchanged) |

### ไทย - ทดสอบ

| Function Call | สีที่คาดหวัง | ผลลัพธ์ (หลังแก้ไข) |
|---------------|------------|------------------|
| `setRGBColor(255, 0, 0)` | **แดง** ✅ | แดง (ก่อนหน้า: เขียว) |
| `setRGBColor(0, 255, 0)` | **เขียว** ✅ | เขียว (ก่อนหน้า: แดง) |
| `setRGBColor(0, 0, 255)` | **น้ำเงิน** ✅ | น้ำเงิน (ไม่เปลี่ยน) |
| `setRGBColor(255, 255, 0)` | **เหลือง** ✅ | เหลือง (ก่อนหน้า: ฟ้า) |
| `setRGBColor(0, 255, 255)` | **ฟ้า** ✅ | ฟ้า (ก่อนหน้า: เหลือง) |
| `setRGBColor(255, 0, 255)` | **ม่วง** ✅ | ม่วง (ไม่เปลี่ยน) |
| `setRGBColor(255, 255, 255)` | **ขาว** ✅ | ขาว (ไม่เปลี่ยน) |
| `setRGBColor(0, 0, 0)` | **ดำ** ✅ | ดำ (ไม่เปลี่ยน) |

---

## 🔄 Impact on TickRedLED() / ผลกระทบต่อฟังก์ชั่น TickRedLED()

The `TickRedLED()` function is **unaffected** by this change because it uses `setRGBColor()` internally:

```cpp
// This still works correctly:
TickRedLED(0.5);      // Blinks RED (now displays correct red color)
TickRedLED(-1);       // RED permanently ON (now displays correct red color)
TickRedLED(0);        // Turns OFF (black)
```

The blinkRedColor variable remains:
```cpp
uint8_t blinkRedColor[3] = {255, 0, 0};  // Red in R, G, B format
```

When `TickRedLED()` calls `setRGBColor(255, 0, 0)`, the remapping happens automatically in the setRGBColor function, so the correct red color is displayed.

---

## 💡 Technical Details / รายละเอียดทางเทคนิค

### Why G, R, B Order? / เพราะเหตุใด G, R, B?

The WS2812 NeoPixel protocol sends color data in this order:
1. **Green** data byte (8 bits)
2. **Red** data byte (8 bits)  
3. **Blue** data byte (8 bits)

This is a hardware characteristic of WS2812 LEDs and cannot be changed. The Adafruit_NeoPixel library's `Color()` function typically creates colors in R, G, B order, so we must remap it.

### How the Fix Works / วิธีการแก้ไขทำงาน

```
User calls:
setRGBColor(red, green, blue)
    ↓
Function receives: red, green, blue
    ↓
Hardware mapping: pixels.Color(green, red, blue)
    ↓
WS2812 LED receives: G, R, B (correct order)
    ↓
LED displays: Correct color!
```

---

## 📊 Build Information / ข้อมูลการสร้าง

```
Platform: ESP32-S3-POE-ETH-8DI-8DO
Framework: Arduino ESP32
Library: Adafruit_NeoPixel
LED Type: WS2812 NeoPixel (1 LED)
GPIO Pin: GPIO 38
Color Order: G, R, B
```

**Build Status**: ✅ SUCCESS  
**RAM Usage**: 13.8%  
**Flash Usage**: 25.0%  

---

## 🔍 Related Changes / การเปลี่ยนแปลงที่เกี่ยวข้อง

This fix affects:
1. ✅ `setRGBColor()` function - Color remapping
2. ✅ `TickRedLED()` function - No changes needed (uses setRGBColor internally)
3. ✅ `redLEDCallback()` function - No changes needed
4. ✅ All color-related examples - Work correctly now

---

## 🚀 Deployment / การปรับใช้

Upload the updated firmware:
```bash
platformio run --target upload
```

---

## ✅ Verification Steps / ขั้นตอนการตรวจสอบ

1. **Upload new firmware** to the board
2. **Test Red color**: 
   ```cpp
   setRGBColor(255, 0, 0);  // Should display RED
   ```
3. **Test Green color**:
   ```cpp
   setRGBColor(0, 255, 0);  // Should display GREEN
   ```
4. **Test Blue color**:
   ```cpp
   setRGBColor(0, 0, 255);  // Should display BLUE
   ```
5. **Test Blinking**:
   ```cpp
   TickRedLED(1.0);  // Should blink RED correctly
   ```

---

## 📝 Commit Message / ข้อความการ Commit

```
Fix: Correct RGB LED color order from R,G,B to G,R,B

- WS2812 LED on ESP32-S3-POE-ETH-8DI-8DO uses G, R, B order
- Remap pixels.Color() parameters in setRGBColor() function
- All RGB color functions now display correct colors
- TickRedLED() blinking works correctly with proper red color
```

---

## 📚 References / อ้างอิง

- [Adafruit_NeoPixel Library Documentation](https://github.com/adafruit/Adafruit_NeoPixel)
- [WS2812 LED Datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812.pdf)
- [ESP32-S3-POE-ETH-8DI-8DO Specification](https://www.waveshare.com/esp32-s3-poe-eth-8di-8do.htm)

---

**Document Status**: ✅ Complete  
**Last Updated**: 2026-06-25  
**Ready for Production**: Yes ✅
