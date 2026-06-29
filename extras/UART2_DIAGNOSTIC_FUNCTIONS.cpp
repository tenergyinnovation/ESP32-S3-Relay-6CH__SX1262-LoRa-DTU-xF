/***********************************************************************
 * DIAGNOSTIC MODE: UART2 RS485 Communication Test
 * 
 * PURPOSE: Test if UART2 is communicating with LoRa module
 * HOW TO USE:
 *   1. Replace the loraConfigureStream() call in setup() with this code
 *   2. Compile and upload
 *   3. Open Serial Monitor (115200 baud)
 *   4. Watch for test results
 * 
 * EXPECTED OUTPUT (if working):
 *   ✓ UART2 output working (data sent successfully)
 *   ✓ Listening for LoRa response...
 *   ✓ Received from module: [data here]
 * 
 * EXPECTED OUTPUT (if NOT working):
 *   ✓ UART2 output working (data sent successfully)
 *   ✗ No response on UART2 (Wiring problem likely!)
 * 
 ***********************************************************************/

void diagnosticLoRaUART2Test() {
  Serial.println("\n" "═══════════════════════════════════════");
  Serial.println("🔧 DIAGNOSTIC: UART2 RS485 Test Mode");
  Serial.println("═══════════════════════════════════════");
  
  // Test 1: Send raw data
  Serial.println("\n[TEST 1] Sending raw test string...");
  Serial.println("   Sending: \"HELLO\\r\\n\" to UART2");
  
  Serial2.print("HELLO\r\n");
  Serial2.flush();
  
  Serial.println("   ✓ Data sent to UART2 output");
  
  // Test 2: Try to enter AT mode
  Serial.println("\n[TEST 2] Attempting to enter AT Command Mode...");
  Serial.println("   Sending: \"+++\\r\\n\" to UART2");
  
  delay(100);
  Serial2.print("+++\r\n");
  Serial2.flush();
  
  Serial.println("   ✓ AT mode command sent");
  
  // Test 3: Listen for any response
  Serial.println("\n[TEST 3] Listening for LoRa module response...");
  Serial.println("   Waiting 2 seconds for data...");
  
  uint32_t testStartTime = millis();
  uint32_t bytesReceived = 0;
  String receivedData = "";
  
  while (millis() - testStartTime < 2000) {
    if (Serial2.available()) {
      char ch = Serial2.read();
      receivedData += ch;
      bytesReceived++;
      
      // Print hex representation for non-printable chars
      if (ch >= 32 && ch <= 126) {
        Serial.print(ch);
      } else {
        Serial.printf("[0x%02X]", ch);
      }
      
      delay(5);
    }
  }
  
  Serial.println();
  
  // Test 4: Display results
  Serial.println("\n[TEST RESULTS]");
  
  if (bytesReceived > 0) {
    Serial.println("   ✅ SUCCESS! Received data from module!");
    Serial.printf("   📊 Bytes received: %u\n", bytesReceived);
    Serial.println("   ✓ UART2 wiring appears to be working");
    Serial.println("   ✓ Module is responding");
    Serial.println("   → Next: Check AT command formatting");
  } else {
    Serial.println("   ❌ FAIL! No response from LoRa module");
    Serial.println("   ⚠️  This indicates a wiring or power problem:");
    Serial.println("   ");
    Serial.println("   CHECK THESE (in priority order):");
    Serial.println("   1. GPIO17 wire connected to module TA/TXD?");
    Serial.println("   2. GPIO18 wire connected to module RB/RXD?");
    Serial.println("   3. GND wire connected?");
    Serial.println("   4. Module power supply = 5V DC (not 3.3V)?");
    Serial.println("   5. Power supply has enough current (>100mA)?");
    Serial.println("   ");
    Serial.println("   USE MULTIMETER TO TEST:");
    Serial.println("   - Continuity: GPIO17→TA, GPIO18→RB, GND→GND");
    Serial.println("   - Voltage: Module VCC should be 4.8V-5.2V");
  }
  
  Serial.println("═══════════════════════════════════════");
  Serial.println("🔧 Diagnostic test complete\n");
}

/***********************************************************************
 * ADVANCED UART2 BYTE LEVEL TEST
 * Uncomment and call this if you want to see every byte sent/received
 ***********************************************************************/

void advancedUART2DebugTest() {
  Serial.println("\n" "═══════════════════════════════════════");
  Serial.println("🔬 ADVANCED: Byte-Level UART2 Test");
  Serial.println("═══════════════════════════════════════");
  
  // Flush any pending data
  while (Serial2.available()) {
    Serial2.read();
  }
  delay(100);
  
  // Test sequence
  const char *tests[] = {"+++", "AT\r\n", "AT+VER\r\n"};
  
  for (int i = 0; i < 3; i++) {
    Serial.printf("\n[Test %d] Sending: ", i + 1);
    
    // Print command in readable format
    for (int j = 0; tests[i][j] != '\0'; j++) {
      char ch = tests[i][j];
      if (ch == '\r') Serial.print("\\r");
      else if (ch == '\n') Serial.print("\\n");
      else Serial.print(ch);
    }
    Serial.println();
    
    // Send command
    Serial2.print(tests[i]);
    Serial2.flush();
    Serial.println("   ✓ Sent to UART2");
    
    // Wait for response
    delay(300);
    String response = "";
    uint32_t rxStartTime = millis();
    
    Serial.print("   Receiving: ");
    while (millis() - rxStartTime < 500 && Serial2.available()) {
      char ch = Serial2.read();
      response += ch;
      
      if (ch >= 32 && ch <= 126) {
        Serial.print(ch);
      } else if (ch == '\r') {
        Serial.print("\\r");
      } else if (ch == '\n') {
        Serial.print("\\n");
      } else {
        Serial.printf("[0x%02X]", ch);
      }
      delay(2);
    }
    
    if (response.length() == 0) {
      Serial.print("(no response)");
    }
    Serial.println();
  }
  
  Serial.println("\n═══════════════════════════════════════");
}

/***********************************************************************
 * GPIO PIN VOLTAGE TEST
 * Call this to verify GPIO17/18 are toggling
 ***********************************************************************/

void testGPIOVoltages() {
  Serial.println("\n" "═══════════════════════════════════════");
  Serial.println("⚡ GPIO17/18 Voltage Test");
  Serial.println("═══════════════════════════════════════");
  
  Serial.println("\nReading GPIO pin levels (use multimeter in parallel):");
  Serial.println("GPIO17 (TX) and GPIO18 (RX) should show changing voltages");
  Serial.println();
  
  for (int i = 0; i < 10; i++) {
    int gpio17_level = digitalRead(17);
    int gpio18_level = digitalRead(18);
    
    Serial.printf("[%d] GPIO17: %s  |  GPIO18: %s\n", 
                  i + 1,
                  gpio17_level ? "HIGH (3.3V)" : "LOW (0V)",
                  gpio18_level ? "HIGH (3.3V)" : "LOW (0V)");
    
    delay(200);
  }
  
  Serial.println("\n✓ Test complete. Watch multimeter probes on GPIO17/18.");
  Serial.println("  Both should toggle between 0V and 3.3V.");
  Serial.println("═══════════════════════════════════════\n");
}

/***********************************************************************
 * HOW TO USE THESE TESTS IN SETUP():
 * 
 * Replace this:
 *   initLoRaDTU();
 *   loraConfigureStream();
 * 
 * With one of these:
 * 
 *   Option 1 (Quick test - 2 seconds):
 *     initLoRaDTU();
 *     diagnosticLoRaUART2Test();
 *     // Don't call loraConfigureStream() yet
 * 
 *   Option 2 (Advanced test - 5 seconds):
 *     initLoRaDTU();
 *     advancedUART2DebugTest();
 *     // Don't call loraConfigureStream() yet
 * 
 *   Option 3 (GPIO voltage test):
 *     initLoRaDTU();
 *     testGPIOVoltages();
 *     // Don't call loraConfigureStream() yet
 * 
 * 
 * AFTER TESTING:
 * Once you find and fix the problem, comment out the test and
 * uncomment the normal loraConfigureStream() call.
 ***********************************************************************/
