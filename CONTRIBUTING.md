# Guide to Contributing

ยินดีต้อนรับที่จะมีส่วนร่วมในโปรเจค **ESP32-S3-Relay-6CH_template**! 🎉

## วิธีการมีส่วนร่วม

### 1. สำหรับผู้ที่เพิ่งเริ่ม
- Fork โปรเจคนี้
- Clone ไปยังเครื่องของคุณ
- สร้าง branch ใหม่: `git checkout -b feature/your-feature-name`

### 2. การทำงาน
- ฟังก์ชั่นใหม่ต้องมี documentation (comments)
- ทำให้ code readable และ maintainable
- Test ด้วย serial monitor ก่อนส่ง PR

### 3. การส่ง Pull Request
- อธิบายการเปลี่ยนแปลงอย่างชัดเจน
- Commit message ควรเป็นภาษาอังกฤษหรือไทยที่เข้าใจได้ง่าย
- ให้เพื่อนๆ review code ก่อนที่ merge

## ประเภทของ Contribution

### 🐛 Bug Fixes
- ระบุสิ่งที่เสีย
- ให้รูป serial output
- แสดงวิธีแก้ไข

### ✨ New Features
- อธิบายฟังก์ชั่นใหม่
- ให้ code example
- เพิ่ม test case

### 📝 Documentation
- ปรับปรุง README
- เพิ่ม comments ในโค้ด
- อัปเดต CHANGELOG

## Code Style

```cpp
// ✅ ตัวอย่างที่ดี
void doSomething(int value) {
    // อธิบายว่าฟังก์ชั่นทำอะไร
    if (value < 0) return;
    // ทำสิ่งที่ต้องการ
}

// ❌ ไม่ดี
void ds(int v){if(v<0)return;}
```

## Questions?
- เปิด Issue เพื่อถามคำถาม
- Discord / Email: tenergy@example.com

---
**ขอบคุณ!** 🙏
