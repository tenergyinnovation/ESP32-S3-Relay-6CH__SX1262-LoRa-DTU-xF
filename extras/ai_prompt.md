- [Waveshare Wiki](https://www.waveshare.com/wiki/ESP32-S3-Relay-6CH)
- [Product Page](https://www.waveshare.com/product/arduino/industrial-controller/

ลิ้งด้านบนคือเว็บไซต์ของบอร์ด ESP32-S3-Relay-6CH และลิงค์วิกิของบอร์ดนี้ ซึ่งมีข้อมูลเกี่ยวกับสเปค, การใช้งาน, และตัวอย่างโค้ดสำหรับการพัฒนาโปรเจคต่าง ๆ บนบอร์ดนี้


https://www.waveshare.com/wiki/SX1262-LoRa-DTU-xF
https://www.waveshare.com/product/iot-communication/long-range-wireless/nb-iot-lora/sx1262-lora-dtu-lf.htm

และนี่คือเว็บไซต์และลิงค์วิกิของโมดูล SX1262-LoRa-DTU-xF ซึ่งเป็นโมดูลสำหรับการสื่อสารแบบ LoRa

โดยโปรเจ็คนี้จะใช้บอร์ด ESP32-S3-Relay-6CH ร่วมกับโมดูล SX1262-LoRa-DTU-xF เพื่อสร้างระบบสื่อสารแบบ LoRa สำหรับการส่งข้อมูลระยะไกล  ช่วยปรับปรุงไฟล์ README.md และไฟล์อื่น ๆ ให้มีข้อมูลเกี่ยวกับบอร์ดและโมดูลนี้ ซึ่งเป็นไฟล์ .md ที่ใช้สำหรับการอธิบายโปรเจ็คและการใช้งานของบอร์ดและโมดูลนี้

โดยตอนนี้ยังไม่ต้องเข้าไปแก้ไข source code ของโปรเจ็ค แต่ให้เน้นไปที่การปรับปรุงไฟล์ README.md และไฟล์อื่น ๆ ที่เกี่ยวข้องกับการอธิบายโปรเจ็คและการใช้งานของบอร์ดและโมดูลนี้


ให้คุณเข้ามาปรับปรุงไฟล์ main.cpp โดยให้เพิ่มส่วนของการเชื่อมต่อกับโมดูล SX1262-LoRa-DTU-xF 
โดยให้คุณเข้าไปดูโค้ดตัวอย่างในการติดต่อกับโมดูล SX1262-LoRa-DTU-xF จาก tmp/ESP32-S3-POE-ETH-8DI-8DO_SX1262-LoRa-DTU-xF.cpp โดยเป็นแนวทางจากบอร์ดอีกตัว ซึ่งมีขา ในการเชื่อมต่อ RS485 ไม่เหมือนกัน (ESP32-S3-Relay-6CH ใช้ขา 17,18 แต่ ESP32-S3-POE-ETH-8DI-8DO ใช้ขา 16,17,21 ต้องพึงระวังในการปรับปรุงโค้ดให้ถูกต้องตามขาของบอร์ด ESP32-S3-Relay-6CH) โดยให้คุณดูที่ฟังก์ชั่น initLoRaDTU();, loraConfigureStream();, loraATCommand(String cmd); แล้วนำโค้ดตัวอย่างเหล่านี้มาปรับปรุงในไฟล์ main.cpp เพื่อให้สามารถเชื่อมต่อและใช้งานโมดูล SX1262-LoRa-DTU-xF ได้อย่างถูกต้อง
โดยดูที่ฟังก์ชั่น  

  initLoRaDTU();
  loraConfigureStream(); 
  loraATCommand(String cmd); 

  แล้วนำโค้ดตัวอย่างเหล่านี้มาปรับปรุงในไฟล์ main.cpp เพื่อให้สามารถเชื่อมต่อและใช้งานโมดูล SX1262-LoRa-DTU-xF ได้อย่างถูกต้อง

  โดยก่อนที่คุณจะปรับปรุงไฟล์ main.cpp คุณควรทำความเข้าใจเกี่ยวกับการใช้งานของโมดูล SX1262-LoRa-DTU-xF และวิธีการสื่อสารกับมันผ่านคำสั่ง AT Command โดยดูรายละเอียดคำสั่ง AT Command ที่อยู่ในไฟล์ extras/AT_COMMAND.md ซึ่งมีคำสั่งต่าง ๆ สำหรับการตั้งค่าและควบคุมโมดูล SX1262-LoRa-DTU-xF

ให้ปรับปรุงโค้ด main.cpp โดยในฟังก์ชั่น loop() ให้ให้ฟังก์ชั้น handleLoraATInput(); เพื่อรอรับคำสั่งจาก User โดยตรง แล้วส่งค่า Response จากโมดูล SX1262-LoRa-DTU-xF กลับไปยัง User ผ่าน Serial Monitor โดยให้คุณดูตัวอย่างโค้ดใน extras/main_org.cpp เพื่อปรับปรุงโค้ดใน main.cpp ให้สามารถทำงานได้อย่างถูกต้อง

ให้เพิ่มคำสั่ง reset เข้าไปใน main.cpp เพื่อให้ ESP32S3 ทำการ reset ตัวเอง


จาก main.cpp ช่วยสร้างเป็นตัวอย่างโค้ดสำหรับการใช้งานโมดูล SX1262-LoRa-DTU-xF โดยสร้างไว้ใน lib/ESP32-S3-Relay-6CH/examples/LoRa_DTU_Example/LoRa_DTU_Example.ino โดยให้โค้ดตัวอย่างนี้สามารถทำงานได้อย่างถูกต้อง และสามารถสื่อสารกับโมดูล SX1262-LoRa-DTU-xF ผ่านคำสั่ง AT Command ได้อย่างสมบูรณ์

#include <Arduino.h>
#include "ESP32S3_Relay6CH.h"

/***********************************************************************
 * LoRa DTU GPIO DEFINITIONS
 ***********************************************************************/
#define LORA_DTU_RX_PIN 18       // GPIO18 - RS485 RX (UART2)
#define LORA_DTU_TX_PIN 17       // GPIO17 - RS485 TX (UART2)
#define LORA_DTU_DE_RE_PIN 21    // GPIO21 - RS485 Direction control (DE/RE)
#define LORA_DTU_BAUD 9600       // LoRa DTU default baud rate
#define LORA_RX_BUFFER_SIZE 256  // Receive buffer size
#define LORA_RX_TIMEOUT 100      // ms - timeout for receiving
#define LORA_SEND_RETRY 3        // Retry count for sending

จากโค้ดข้างบน ยังคงผิดอยุ่ ขอเน้นย้ำว่า ESP32-S3-Relay-6CH ใช้ขา 17,18 ส่วนขา 21 ที่เป็น LORA_DTU_DE_RE_PIN ไม่ได้ใช้ กรุณาแก้ไขโค้ดให้ถูกต้อง

และเพื่อให้แต่ใจ กรุณาตรวจสอบ pinout จากไฟล์นี้

ฉันกำลังจะทดลองโปรแกรมให้กับบอร์ด ESP32-S3-Relay-6CH โดยใช้โมดูล SX1262-LoRa-DTU-xF เพื่อทดสอบการสื่อสารแบบ LoRa ผ่านคำสั่ง AT Command จากนั้นฉันจะรายงานผลการทดสอบและปัญหาที่พบให้คุณทราบ เพื่อให้คุณสามารถปรับปรุงโค้ดและแก้ไขปัญหาได้อย่างถูกต้อง


📝 > 
📤 Sending: at (Auto-entering AT Command Mode first)
⚙️  AT Command: at
   📥 Raw: OK
   ✅ Response: OK

📝 > 
📤 Sending: at
⚙️  AT Command: at
   📥 Raw: OK
   ✅ Response: OK

📝 > t
❌ Unknown LoRa AT command: a
   Type 'HELP' for available LoRa AT commands or 'AT+EXIT' to exit AT mode



❌ Unknown command: 'T' - Type 'HELP' for available commands

📝 > a
❌ Unknown LoRa AT command: t
   Type 'HELP' for available LoRa AT commands or 'AT+EXIT' to exit AT mode



❌ Unknown command: 'A' - Type 'HELP' for available commands

📝 > at
❌ Unknown LoRa AT command: aatttt
   Type 'HELP' for available LoRa AT commands or 'AT+EXIT' to exit AT mode



❌ Unknown command: 'AT' - Type 'HELP' for available commands

📝 > 
📤 Sending: at
⚙️  AT Command: at
   📥 Raw: OK
   ✅ Response: OK

📝 > 

ฉันพบว่าการตอบสนองต่อการพิมพ์ช้ามาก และไม่แสดงว่าฉันพิมพ์อะไรไป กรุณาตรวจสอบโค้ดใน main.cpp