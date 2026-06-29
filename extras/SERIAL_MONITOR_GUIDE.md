# Serial Monitor Guide - ESP32-S3-POE-ETH-8DI-8DO Gateway
## Serial Monitor คำแนะนำการใช้งาน

**Version**: 1.0  
**Date**: 2026-06-25  
**Language**: Thai & English  

---

## 📝 Overview / บทนำ

This guide explains how to use Serial Monitor to configure and debug the ESP32-S3-POE-ETH-8DI-8DO Gateway when entering Access Point (AP) mode.

**ไทย**: เอกสารนี้อธิบายวิธีการใช้ Serial Monitor เพื่อตั้งค่าและแก้ไขปัญหาของบอร์ด ESP32-S3-POE-ETH-8DI-8DO LoRa Gateway เมื่อเข้าสู่โหมด Access Point (AP)

---

## 🔌 What is HWCDC? / HWCDC คืออะไร?

### English Explanation:
**HWCDC (Hardware Communication Device Class)** is a feature on ESP32-S3 that enables USB-to-Serial communication directly through the USB Type-C port without requiring a separate USB-to-UART converter chip.

**Key Features:**
- ✅ Direct USB-to-Serial communication
- ✅ No external converter chip needed
- ✅ Works immediately after power-on
- ✅ Automatic driver installation on Windows/Mac/Linux
- ✅ Can send data while in AP mode

### คำอธิบายภาษาไทย:
**HWCDC (Hardware Communication Device Class)** คือฟีเจอร์ใน ESP32-S3 ที่ช่วยให้สื่อสารผ่าน USB Type-C โดยตรงโดยไม่ต้องใช้ชิป USB-to-UART ตัวแปลง

**ข้อดีของ HWCDC:**
- ✅ สื่อสาร USB-to-Serial โดยตรง
- ✅ ไม่ต้องใช้ชิปแปลงภายนอก
- ✅ ทำงานทันทีหลังเปิดไฟ
- ✅ ติดตั้งไดรเวอร์อัตโนมัติบน Windows/Mac/Linux
- ✅ สามารถส่งข้อมูลในขณะที่อยู่ในโหมด AP

---

## 🆚 HWCDC vs Traditional Serial / เปรียบเทียบ HWCDC กับ Serial แบบดั้งเดิม

### Comparison Table / ตารางเปรียบเทียบ

| Feature / ลักษณะ | Traditional UART | HWCDC (USB CDC) |
|------------------|-----------------|-----------------|
| **Converter Chip** | Requires (CH340, FT232, etc.) | Built-in (No extra chip) |
| **Power Source** | External power or USB | USB Power Only |
| **Setup Time** | Driver installation required | Plug & Play |
| **Reliability** | High but depends on converter | More reliable (direct USB) |
| **AP Mode Support** | Limited | Full support ✅ |
| **Latency** | ~10-50ms | ~2-10ms (Lower) |
| **Cost** | Higher (needs converter) | Lower (no extra chip) |

### ตารางเปรียบเทียบ (ไทย)

| ลักษณะ | UART แบบดั้งเดิม | HWCDC (USB CDC) |
|-------|-----------------|-----------------|
| **ชิปแปลง** | ต้องใช้ (CH340, FT232 ฯลฯ) | ในตัว (ไม่ต้อง) |
| **แหล่งพลังงาน** | พลังงานภายนอกหรือ USB | USB เท่านั้น |
| **เวลาติดตั้ง** | ต้องติดตั้งไดรเวอร์ | ใช้ได้ทันที |
| **ความเชื่อถือได้** | สูงแต่ขึ้นอยู่กับชิป | เชื่อถือได้มากขึ้น |
| **รองรับโหมด AP** | จำกัด | สนับสนุนทั้งหมด ✅ |
| **ความหน่วง** | ~10-50ms | ~2-10ms (น้อยกว่า) |
| **ต้นทุน** | สูงกว่า (ต้องชิป) | ต่ำกว่า (ไม่ต้องชิป) |

---

## 🚀 How to Use Serial Monitor / วิธีใช้ Serial Monitor

### Prerequisites / ข้อกำหนดเบื้องต้น:

1. **Hardware Setup / การตั้งค่าฮาร์ดแวร์:**
   - ESP32-S3-POE-ETH-8DI-8DO board connected via USB Type-C
   - USB Type-C cable (high quality recommended)
   - Computer with VS Code or Arduino IDE

2. **Software Setup / การตั้งค่าซอฟต์แวร์:**
   - Arduino IDE >= 1.8.19 OR VS Code + PlatformIO
   - ESP32 board manager >= 3.0.0
   - USB drivers installed (CH34XSER for macOS)

### Step-by-Step Guide / คำแนะนำแบบทีละขั้นตอน:

#### Option 1: Using VS Code + PlatformIO / ใช้ VS Code + PlatformIO

**Step 1: Connect Board**
```
1. Connect ESP32-S3 via USB Type-C to your computer
2. Open Terminal in VS Code
3. Run: platformio device monitor
```

**Step 2: Monitor Serial Output**
```
You should see:
- Firmware version info
- WiFi connection attempt
- If WiFi fails → AP mode activated
- Detailed setup instructions
```

**Step 3: Configure WiFi (via AP Mode)**
```
From Serial Monitor, you'll see:
- AP SSID: tiny32-XXXXXX
- AP Password: password
- IP Address: 192.168.4.1
```

#### Option 2: Using Arduino IDE / ใช้ Arduino IDE

**Step 1: Open Serial Monitor**
```
1. Connect board via USB Type-C
2. Select Tools → Port → COM3 (or /dev/cu.usbmodem...)
3. Select Tools → Serial Monitor
4. Set Baud Rate to 115200
```

**Step 2: View Output**
```
Serial Monitor will display startup information
and WiFi configuration details automatically
```

---

## 📊 Serial Monitor Output Examples / ตัวอย่างผลลัพธ์ Serial Monitor

### Example 1: Successful WiFi Connection / การเชื่อมต่อ WiFi สำเร็จ

```
***********************************************************************
* Project      :     ESP32-S3-POE-ETH-8DI-8DO-Gateway
* Description  :     IoT Gateway for LoRa Transmitter to MQTT Broker
* Hardware     :     ESP32-S3-POE-ETH-8DI-8DO
* Author       :     Uten Boonliam (Tenergy Innovation Co., Ltd.)
* Date         :     25/06/2026
* Revision     :     0.1.0
***********************************************************************

🔧 Hardware CDC (USB CDC On Boot) Initialization
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✓ USB CDC On Boot: ENABLED
✓ Baud Rate: 115200 bps
✓ Data Bits: 8
✓ Stop Bits: 1
✓ Parity: None
✓ Flow Control: None
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Device Name: tiny32-AABBCC

✅ Buzzer initialized successfully
✅ RGB LED initialized successfully

╔════════════════════════════════════════╗
║       WiFi Manager Initialization      ║
╚════════════════════════════════════════╝
⏳ Attempting WiFi connection (Timeout: 120 seconds)...

✅ WiFi Connected Successfully!
─────────────────────────────────────────────────────────────────
📬 SSID: MyHomeWiFi
📍 IP Address: 192.168.1.100
🔗 Signal Strength: -45 dBm
─────────────────────────────────────────────────────────────────

🛡️  Initializing Watchdog Timer...
   ✓ WDT configured (Timeout: 120 seconds)
```

### Example 2: AP Mode Activated / เข้าสู่โหมด AP

```
⏳ Attempting WiFi connection (Timeout: 120 seconds)...

─────────────────────────────────────────────────────────────────────────────────
⚠️  WiFi Connection FAILED - Entering Access Point (AP) Mode
─────────────────────────────────────────────────────────────────────────────────

╔════════════════════════════════════════════════════════════════╗
║           📡 ACCESS POINT (AP) MODE ACTIVATED 📡              ║
╚════════════════════════════════════════════════════════════════╝

🔗 ACCESS POINT CONNECTION DETAILS:
─────────────────────────────────────────────────────────────────
  🏠 AP SSID (Network Name):  tiny32-AABBCC
  🔐 AP Password:             password
─────────────────────────────────────────────────────────────────

🌐 GATEWAY IP CONFIGURATION:
─────────────────────────────────────────────────────────────────
  📍 Gateway IP Address:      192.168.4.1
  🖥️  Subnet Mask:             255.255.255.0
  📋 IP Range (DHCP):         192.168.4.2 - 192.168.4.254
─────────────────────────────────────────────────────────────────

📱 DEVICE INFORMATION:
─────────────────────────────────────────────────────────────────
  🏷️  Device Name:             tiny32-AABBCC
  🔌 Firmware Version:         0.1.0
  💾 RAM Free:                "Check via Serial Menu"
─────────────────────────────────────────────────────────────────


╔════════════════════════════════════════════════════════════════╗
║                    📖 SETUP INSTRUCTIONS 📖                     ║
╚════════════════════════════════════════════════════════════════╝

🇹🇭 THAI INSTRUCTIONS (คำแนะนำภาษาไทย):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
1. เปิดการตั้งค่า WiFi บนอุปกรณ์ของคุณ (สมาร์ทโฟน/แท็บเล็ต)
   ค้นหา WiFi ชื่อ: "tiny32-AABBCC"
   ใส่รหัสผ่าน: "password"
   แล้วเชื่อมต่อ

2. เปิดเบราวเซอร์ (Chrome, Safari, Firefox) และไปที่:
   URL: http://192.168.4.1

3. เลือก WiFi network ที่คุณต้องการเชื่อมต่อ
4. ใส่รหัสผ่าน WiFi และกด Save
5. บอร์ดจะรีบูตและเชื่อมต่อกับ WiFi โดยอัตโนมัติ
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🇬🇧 ENGLISH INSTRUCTIONS:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
1. Open WiFi Settings on your device (Smartphone/Tablet)
   Look for WiFi network: "tiny32-AABBCC"
   Enter password: "password"
   and connect

2. Open a web browser (Chrome, Safari, Firefox) and go to:
   URL: http://192.168.4.1

3. Select the WiFi network you want to connect to
4. Enter WiFi password and click Save
5. Board will reboot and automatically connect to WiFi
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✅ CONFIGURATION TIMELINE:
┌─────────────────────────────────────────────────────────────┐
│ Step                              │ Expected Time            │
├─────────────────────────────────────────────────────────────┤
│ 1. Connect to AP                  │ < 10 seconds             │
│ 2. Open 192.168.4.1 in browser   │ < 5 seconds              │
│ 3. Complete WiFi setup            │ < 30 seconds             │
│ 4. Board reboots & connects      │ < 30 seconds             │
│ ────────────────────────────────────────────────────────────│
│ Total Expected Time               │ < 2 minutes              │
└────────────────────────────────────────────────────────────┘

⚠️  IMPORTANT NOTES:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
• Keep Serial Monitor open to monitor connection progress
• Board will automatically restart after 120 seconds
• If you see this message again, WiFi connection failed
• Check WiFi signal strength and credentials
• Factory reset via Serial Menu (Option 4) if needed
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

⏳ Restarting board in 120 seconds (automatic restart)...
   If WiFi configuration is completed, the board will connect.
   Otherwise, board will restart and enter AP mode again.

   ⏱️  110 seconds remaining...
   ⏱️  100 seconds remaining...
   ...
```

---

## 🎯 Key Information Displayed in AP Mode / ข้อมูลสำคัญที่แสดงในโหมด AP

When the board enters AP mode, Serial Monitor displays:

### 1. **AP Connection Details / ข้อมูลการเชื่อมต่อ AP**
```
🏠 AP SSID (Network Name):  tiny32-AABBCC
🔐 AP Password:             password
```
- **Use these to connect your phone/tablet to the gateway**
- **ใช้ข้อมูลนี้เพื่อเชื่อมต่อสมาร์ทโฟน/แท็บเล็ตกับเกตเวย์**

### 2. **Gateway IP Address / ที่อยู่ IP ของเกตเวย์**
```
📍 Gateway IP Address:      192.168.4.1
🖥️  Subnet Mask:             255.255.255.0
```
- **Open this address in web browser to configure WiFi**
- **เปิดที่อยู่นี้ในเบราวเซอร์เพื่อตั้งค่า WiFi**

### 3. **Device Information / ข้อมูลอุปกรณ์**
```
🏷️  Device Name:             tiny32-AABBCC
🔌 Firmware Version:         0.1.0
```
- **Identifies the board for tracking and debugging**
- **ใช้ระบุบอร์ดสำหรับการติดตามและแก้ไขปัญหา**

### 4. **Setup Instructions / คำแนะนำการตั้งค่า**
- Thai language instructions (ภาษาไทย)
- English instructions
- Step-by-step WiFi configuration guide

---

## 🔧 Troubleshooting / การแก้ไขปัญหา

### Problem 1: No Serial Monitor Output
**Issue**: Serial Monitor shows nothing after connecting board

**Solutions / วิธีแก้:**
1. ✅ Check baud rate is set to **115200**
2. ✅ Verify **USB Type-C cable** is properly connected
3. ✅ Try different USB port on computer
4. ✅ Update **CH34XSER driver** for macOS
5. ✅ Check board is powered (Red LED should be ON)
6. ✅ Reset board by pressing **BOOT + RESET** buttons together

### Problem 2: Cannot Connect to AP
**Issue**: Cannot find AP SSID in WiFi list

**Solutions / วิธีแก้:**
1. ✅ Board might still be connecting - wait 30 seconds
2. ✅ Check Serial Monitor for device name (tiny32-XXXXXX)
3. ✅ Move closer to board (2G WiFi has limited range)
4. ✅ Try on different device (phone, tablet, laptop)
5. ✅ Restart board and try again

### Problem 3: Cannot Open 192.168.4.1
**Issue**: Web page shows "Cannot connect to server"

**Solutions / วิธีแก้:**
1. ✅ Verify connected to correct AP SSID
2. ✅ Check IP is actually **192.168.4.1** (not 192.168.1.1)
3. ✅ Disable VPN/Proxy if using
4. ✅ Use incognito/private browsing mode
5. ✅ Try from different device

### Problem 4: WiFi Connection Keeps Failing
**Issue**: Board enters AP mode repeatedly

**Solutions / วิธีแก้:**
1. ✅ Verify WiFi credentials are correct
2. ✅ Check WiFi signal strength (RSSI)
3. ✅ Try WiFi network without special characters in password
4. ✅ Factory reset WiFi (Option 4 in Serial Menu)
5. ✅ Move board closer to router

---

## 📋 Serial Menu Commands / คำสั่ง Serial Menu

When board is running, you can send commands:

```
╔════════════════════════════════════════╗
║           COMMAND MENU                 ║
╠════════════════════════════════════════╣
║ 1. Show IP Address                     ║
║ 2. Show SSID                           ║
║ 3. Buzzer Beep                         ║
║ 4. Reset WiFi (Auto Reboot)            ║
║ 0. Reset Board                         ║
║ X. Exit Menu                           ║
╚════════════════════════════════════════╝
```

**How to use / วิธีใช้:**
1. Open Serial Monitor
2. Type command number (1, 2, 3, 4, 0, or X)
3. Press Enter
4. View result in Serial Monitor

---

## ✅ Best Practices / วิธีปฏิบัติที่ดี

1. **Keep Serial Monitor Open**
   - Monitor connection progress
   - Get real-time debug information
   - Troubleshoot issues quickly

2. **Use Quality USB Cable**
   - Use certified USB Type-C cable
   - Avoid cheap/damaged cables
   - Check cable regularly

3. **Check WiFi Signal**
   - Board should be within 5 meters of router
   - Avoid obstacles between board and router
   - Check signal strength in Serial Monitor

4. **Monitor Firmware Version**
   - Check version displayed at startup
   - Update firmware if bugs are found
   - Keep track of changes

5. **Document Your Configuration**
   - Note the Device Name (tiny32-XXXXXX)
   - Save WiFi SSID and IP address
   - Keep Serial Monitor logs for reference

---

## 📞 Support & Additional Resources / การสนับสนุนและทรัพยากร

- **Waveshare Wiki**: https://www.waveshare.com/wiki/ESP32-S3-POE-ETH-8DI-8DO
- **Arduino ESP32 Docs**: https://docs.espressif.com/projects/arduino-esp32/
- **CH34XSER Driver (macOS)**: https://github.com/WCHSoftware/ch34x_install

---

**Last Updated**: 2026-06-25  
**Version**: 1.0  
**Status**: ✅ Ready for Use
