# Changelog - ESP32-S3-Relay-6CH

ทุกการเปลี่ยนแปลงที่สำคัญของโปรเจค ESP32-S3-Relay-6CH จะถูกบันทึกไว้ในไฟล์นี้

รูปแบบของไฟล์นี้ขึ้นอยู่กับ [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)
และโปรเจคนี้ปฏิบัติตามหลักการ [Semantic Versioning](https://semver.org/spec/v2.0.0.html)

## [1.0.1] - 2026-06-29

### Fixed
- 🐛 Relay 3 & 4 GPIO swap: GPIO41 ↔ GPIO42 (now 3→GPIO41, 4→GPIO42)
- 🐛 Relay 5 & 6 incorrect pins: Changed GPIO40→GPIO45, GPIO39→GPIO46
- 🐛 LED timing: Equal ON/OFF duration (500ms each per cycle)
- 🐛 Relay toggle function: Fixed double-normalization in toggle()
- 🐛 ALL ON command: Now correctly activates all 6 relays
- 🐛 Channel validation: Enhanced _normalizeChannel() to properly validate inputs

### Changed
- 🔄 Updated RELAY_PINS array: {1, 2, 41, 42, 45, 46}
- 🔄 Updated relay documentation with correct GPIO mappings
- 🔄 Improved error handling and channel validation

---

## [1.0.0] - 2026-06-29

### Added
- ✨ ลำแรกของระบบ Relay Control Interface สำหรับ ESP32-S3-Relay-6CH
- ✨ รองรับคำสั่ง RELAY: ON/OFF, 1/0, status query (?)
- ✨ ฟังก์ชั่นควบคุม LED RGB (Red, Green, Blue, Yellow, Purple, Orange, White)
- ✨ ฟังก์ชั่นควบคุม Buzzer (beep sounds)
- ✨ ควบคุม 6 Relay Channels (Relay1-Relay6)
- ✨ RS485 Serial communication สำหรับการขยายเพิ่มเติม
- ✨ WiFi AP Mode configuration
- ✨ Bluetooth BLE support
- ✨ HELP menu แสดงคำสั่งทั้งหมด

### Fixed
- 🐛 แก้ไขปัญหา LED colors display
- 🐛 แก้ไขปัญหา Relay state synchronization

### Changed
- 🔄 อัปเดต board definition สำหรับ ESP32-S3-Relay-6CH
- 🔄 ปรับปรุง GPIO pin configuration
