
/***********************************************************************
 * Project      :     ESP32-S3-Relay-6CH with SX1262-LoRa-DTU-xF
 * Description  :     Interactive command interface with LoRa communication
 * Hardware     :     Waveshare ESP32-S3-Relay-6CH + SX1262-LoRa-DTU-xF
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     29/06/2026
 * Revision     :     2.0.0
 * 
 * Features:
 *   - 6 Relay Channel Control (10A @ 250VAC / 30VDC each)
 *   - RGB LED (WS2812B NeoPixel) Status Indicator
 *   - Passive Buzzer with PWM Control
 *   - RS485 Serial Communication (Isolated)
 *   - SX1262-LoRa-DTU-xF Integration (Long-range wireless 5+ km)
 *   - LoRa AT Commands & Stream Mode Support
 *   - System Status Monitoring
 * 
 * ***********************************************************************/

#include <Arduino.h>
#include "ESP32S3_Relay6CH.h"

/***********************************************************************
 * LoRa DTU GPIO DEFINITIONS (UART2)
 * Note: GPIO21 is used for Buzzer on ESP32-S3-Relay-6CH
 * RS485 direction control is handled automatically by UART2 hardware
 ***********************************************************************/
#define LORA_DTU_RX_PIN 18       // GPIO18 - UART2 RX for RS485
#define LORA_DTU_TX_PIN 17       // GPIO17 - UART2 TX for RS485
#define LORA_DTU_BAUD 9600       // LoRa DTU default baud rate
#define LORA_RX_BUFFER_SIZE 256  // Receive buffer size
#define LORA_RX_TIMEOUT 100      // ms - timeout for receiving
#define LORA_SEND_RETRY 3        // Retry count for sending

/***********************************************************************
 * LoRa DTU STATUS STRUCTURE
 ***********************************************************************/
struct LoRaStatus {
  bool isConnected;
  uint16_t packetsReceived;
  uint16_t packetsSent;
  uint16_t packetErrors;
  int16_t lastRSSI;
  uint32_t lastActivityTime;
  char lastErrorMsg[100];
  bool streamMode;  // true=Stream Mode, false=Packet Mode
};

/***********************************************************************
 * GLOBAL VARIABLES
 ***********************************************************************/
ESP32S3_Relay6CH board;

// LoRa DTU Status and Buffers
LoRaStatus loraStatus = {
  false,    // isConnected
  0,        // packetsReceived
  0,        // packetsSent
  0,        // packetErrors
  0,        // lastRSSI
  0,        // lastActivityTime
  "",       // lastErrorMsg
  true      // streamMode (default: Stream Mode)
};

char loraRxBuffer[LORA_RX_BUFFER_SIZE];
uint16_t loraRxIndex = 0;
uint32_t loraLastRxTime = 0;
bool atCommandModeActive = false;  // Track if module is in AT Command Mode
uint32_t atModeEnterTime = 0;      // Timestamp when AT mode was entered

// Serial input buffer
String inputBuffer = "";
const int MAX_COMMAND_LENGTH = 100;

// Timing variables
unsigned long lastSerialPromptTime = 0;
const unsigned long PROMPT_INTERVAL = 100;  // Show prompt every 100ms if idle

/***********************************************************************
 * FUNCTION PROTOTYPES
 ***********************************************************************/
void printWelcome(void);
void printHelpMenu(void);
void printPrompt(void);
void processCommand(String cmd);
void handleRelayCommand(String params);
void handleBuzzerCommand(String params);
void handleLEDCommand(String params);
void handleStatusCommand(String params);
void handleResetCommand(void);
void handleLoRaCommand(String params);
void printSeparator(const char* title = "");

/* LoRa DTU Functions */
void initLoRaDTU(void);                      // Initialize UART2 for LoRa DTU
void loraATCommand(String cmd);             // Send AT Command to DTU
void loraConfigureStream(void);             // Configure Stream Mode
void handleLoraATInput(void);               // Handle direct AT command input
void sendLoRaData(String data);             // Send data via LoRa

/***********************************************************************
 * SETUP FUNCTION
 ***********************************************************************/
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // Initialize board
    ESP32S3_Relay6CH::Status status = board.begin();
    
    if (status != ESP32S3_Relay6CH::STATUS_OK) {
        Serial.println("\n❌ BOARD INITIALIZATION FAILED!");
        Serial.print("Status: ");
        Serial.println((int)status);
        while (1) delay(1000);
    }
    
    // Initialize RS485
    if (!board.setupRS485(9600)) {
        Serial.println("\n⚠️  RS485 initialization skipped");
    }
    
    // Initialize LoRa DTU (RS485 UART2)
    delay(500);
    initLoRaDTU();
    delay(500);
    
    // Configure LoRa DTU for Stream Mode
    loraConfigureStream();
    delay(500);
    
    printWelcome();
}

/***********************************************************************
 * MAIN LOOP FUNCTION
 ***********************************************************************/
void loop() {
    // Update board (required for animations)
    board.loop();
    
    // Handle ALL serial input and LoRa AT commands
    handleLoraATInput();
    
    // ✅ Removed duplicate input processing - handleLoraATInput() now handles everything
}

/***********************************************************************
 * PRINT WELCOME MESSAGE
 ***********************************************************************/
void printWelcome(void) {
    Serial.println("\n\n");
    Serial.println("╔════════════════════════════════════════════════════════════╗");
    Serial.println("║                                                            ║");
    Serial.println("║      ESP32-S3-Relay-6CH + SX1262-LoRa-DTU-xF              ║");
    Serial.println("║                                                            ║");
    Serial.println("║  🔌 Control: 6 Relays, RGB LED, Buzzer, RS485            ║");
    Serial.println("║  📡 LoRa: Long-range wireless (5+ km, Stream Mode)       ║");
    Serial.println("║  📋 Type 'HELP' for command list                          ║");
    Serial.println("║                                                            ║");
    Serial.println("╚════════════════════════════════════════════════════════════╝");
    Serial.println();
    Serial.println("✅ Board initialized successfully!");
    Serial.println("✅ RS485 initialized at 9600 baud");
    if (loraStatus.isConnected) {
        Serial.println("✅ LoRa DTU initialized (UART2: GPIO17=TX, GPIO18=RX, Half-duplex mode)");
        Serial.println("✅ LoRa Stream Mode configured");
    } else {
        Serial.println("⚠️  LoRa DTU initialization skipped");
    }
    Serial.println();
    Serial.println(board.getBoardInfo().c_str());
    Serial.println();
    
    printPrompt();
}

/***********************************************************************
 * PRINT HELP MENU
 ***********************************************************************/
void printHelpMenu(void) {
    printSeparator("AVAILABLE COMMANDS");
    
    Serial.println("\n1️⃣  RELAY <channel> <state>");
    Serial.println("    Description: Control relay channel");
    Serial.println("    Channel:     1-6");
    Serial.println("    State:       ON/OFF, 1/0, TGL (toggle), or ? (read status)");
    Serial.println("    Examples:    RELAY 1 ON");
    Serial.println("                 RELAY 3 OFF");
    Serial.println("                 RELAY 5 ?       (read status)");
    Serial.println("                 RELAY 2 TGL     (toggle)\n");
    
    Serial.println("2️⃣  BUZZER <times>");
    Serial.println("    Description: Sound buzzer N times");
    Serial.println("    Example:     BUZZER 3");
    Serial.println("    Valid range: 1-10 times\n");
    
    Serial.println("3️⃣  LED <color> <seconds>");
    Serial.println("    Description: Control RGB LED color");
    Serial.println("    Colors:      red, green, blue, yellow, purple, orange, white");
    Serial.println("    Duration:    Float value (0.5-10.0) in seconds, or -1 to turn off");
    Serial.println("    Examples:    LED red 2.5");
    Serial.println("                 LED blue -1    (Turn off LED)\n");
    
    Serial.println("4️⃣  ALL <state>");
    Serial.println("    Description: Control all relay channels at once");
    Serial.println("    State:       ON, OFF");
    Serial.println("    Examples:    ALL ON         (Turn on all 6 relays)");
    Serial.println("                 ALL OFF        (Turn off all 6 relays)\n");
    
    Serial.println("5️⃣  STATUS");
    Serial.println("    Description: Display board and system information");
    Serial.println("    Information: Board info, relay states, ESP32 system stats\n");
    
    Serial.println("6️⃣  LORA <subcommand>");
    Serial.println("    Description: LoRa DTU (SX1262-LoRa-DTU-xF) module control");
    Serial.println("    Subcommands:");
    Serial.println("      - INIT       : Initialize LoRa DTU (UART2 RS485)");
    Serial.println("      - CONFIG     : Configure LoRa DTU Stream Mode");
    Serial.println("      - AT <cmd>   : Send AT command to module");
    Serial.println("      - SEND <msg> : Send message via LoRa");
    Serial.println("    Examples:");
    Serial.println("      LORA INIT");
    Serial.println("      LORA CONFIG");
    Serial.println("      LORA AT AT+VER       (Get firmware version)");
    Serial.println("      LORA AT AT+SF=9      (Set spreading factor)");
    Serial.println("      LORA SEND Hello      (Send 'Hello' via LoRa)\n");
    
    Serial.println("7️⃣  RESET");
    Serial.println("    Description: Reset ESP32-S3 board\n");
    
    Serial.println("8️⃣  BUZZER_TEST");
    Serial.println("    Description: Run buzzer hardware diagnostic test");
    Serial.println("    Output:      Continuous tone for 2 seconds\n");
    
    Serial.println("9️⃣  HELP");
    Serial.println("    Description: Show this help menu\n");
    
    Serial.println("📡 LoRa AT Command Mode:");
    Serial.println("    Type '+++' to enter AT Command mode");
    Serial.println("    Type 'AT+EXIT' to exit AT Command mode");
    Serial.println("    Type 'HELP' while in AT mode to see LoRa AT commands\n");
    
    printSeparator();
    Serial.println();
}

/***********************************************************************
 * PRINT COMMAND PROMPT
 ***********************************************************************/
void printPrompt(void) {
    Serial.print("📝 > ");
}

/***********************************************************************
 * PRINT SEPARATOR
 ***********************************************************************/
void printSeparator(const char* title) {
    if (title != NULL && strlen(title) > 0) {
        Serial.print("┌─── ");
        Serial.print(title);
        Serial.println(" ───────────────────────────────────┐");
    } else {
        Serial.println("└─────────────────────────────────────────────────────────┘");
    }
}

/***********************************************************************
 * PROCESS INCOMING COMMAND
 ***********************************************************************/
void processCommand(String cmd) {
    // Convert to uppercase
    cmd.toUpperCase();
    
    // Trim whitespace
    cmd.trim();
    
    if (cmd.length() == 0) {
        return;
    }
    
    Serial.println();
    
    // Parse command
    int spaceIndex = cmd.indexOf(' ');
    String commandType = (spaceIndex > 0) ? cmd.substring(0, spaceIndex) : cmd;
    String params = (spaceIndex > 0) ? cmd.substring(spaceIndex + 1) : "";
    
    // Execute command
    if (commandType == "RELAY") {
        handleRelayCommand(params);
    } else if (commandType == "BUZZER") {
        handleBuzzerCommand(params);
    } else if (commandType == "LED") {
        handleLEDCommand(params);
    } else if (commandType == "ALL") {
        // ALL ON/OFF control
        params.toUpperCase();
        if (params == "ON") {
            Serial.println("🔴 Turning ALL relays ON...");
            board.allRelaysOn();
            Serial.println("✅ All relays activated");
        } else if (params == "OFF") {
            Serial.println("⚫ Turning ALL relays OFF...");
            board.allRelaysOff();
            Serial.println("✅ All relays deactivated");
        } else {
            Serial.println("❌ Invalid parameter for ALL command");
            Serial.println("   Usage: ALL <ON|OFF>");
        }
    } else if (commandType == "STATUS") {
        handleStatusCommand(params);
    } else if (commandType == "RESET") {
        handleResetCommand();
    } else if (commandType == "LORA") {
        handleLoRaCommand(params);
    } else if (commandType == "BUZZER_TEST") {
        Serial.println("🔊 Running buzzer hardware test...");
        Serial.println("   Continuous tone for 2 seconds...");
        board.buzzer_test();
        Serial.println("✅ Buzzer test complete");
    } else if (commandType == "HELP") {
        printHelpMenu();
    } else {
        Serial.print("❌ Unknown command: '");
        Serial.print(commandType);
        Serial.println("' - Type 'HELP' for available commands");
    }
    
    Serial.println();
}

/***********************************************************************
 * HANDLE BUZZER COMMAND
 * Format: BUZZER <times>
 ***********************************************************************/
void handleBuzzerCommand(String params) {
    params.trim();
    
    if (params.length() == 0) {
        Serial.println("❌ Missing parameter: BUZZER <times>");
        Serial.println("   Example: BUZZER 3");
        return;
    }
    
    int times = params.toInt();
    
    if (times < 1 || times > 10) {
        Serial.print("❌ Invalid times: ");
        Serial.print(times);
        Serial.println(" (valid range: 1-10)");
        return;
    }
    
    Serial.print("🔊 Buzzing ");
    Serial.print(times);
    Serial.println(" time(s)...");
    
    board.buzzer_beep(times);
    
    Serial.println("✅ Buzzer complete");
}

/***********************************************************************
 * HANDLE LED COMMAND
 * Format: LED <color> <seconds>
 ***********************************************************************/
void handleLEDCommand(String params) {
    params.trim();
    
    // Parse color and duration
    int spaceIndex = params.indexOf(' ');
    if (spaceIndex < 0) {
        Serial.println("❌ Missing parameters: LED <color> <seconds>");
        Serial.println("   Example: LED red 2.5");
        return;
    }
    
    String color = params.substring(0, spaceIndex);
    String durationStr = params.substring(spaceIndex + 1);
    
    color.toUpperCase();
    float duration = durationStr.toFloat();
    
    // Validate duration
    if (duration < -1 || (duration > 0 && duration < 0.1)) {
        Serial.print("❌ Invalid duration: ");
        Serial.println(duration);
        Serial.println("   Use: -1 (OFF), or 0.1-10.0 (seconds)");
        return;
    }
    
    // Turn off LED
    if (duration == -1) {
        Serial.print("💡 Turning OFF LED...");
        board.LEDOff();
        Serial.println(" ✅");
        return;
    }
    
    // Set LED color
    Serial.print("💡 LED: ");
    Serial.print(color);
    Serial.print(" for ");
    Serial.print(duration);
    Serial.println(" seconds...");
    
    if (color == "RED") {
        board.TickRedLED(duration);
    } else if (color == "GREEN") {
        board.TickGreenLED(duration);
    } else if (color == "BLUE") {
        board.TickBlueLED(duration);
    } else if (color == "YELLOW") {
        board.TickYellowLED(duration);
    } else if (color == "PURPLE") {
        board.TickPurpleLED(duration);
    } else if (color == "ORANGE") {
        board.TickOrangeLED(duration);
    } else if (color == "WHITE") {
        board.TickWhiteLED(duration);
    } else {
        Serial.print("❌ Unknown color: ");
        Serial.println(color);
        Serial.println("   Valid colors: red, green, blue, yellow, purple, orange, white");
        return;
    }
    
    Serial.println("✅ LED animation started");
}

/***********************************************************************
 * HANDLE RELAY COMMAND
 * Format: RELAY <channel> <state>
 * state: ON, OFF, 1, 0, TGL (toggle), or ? (to read status)
 ***********************************************************************/
void handleRelayCommand(String params) {
    params.trim();
    
    // Parse channel and state
    int spaceIndex = params.indexOf(' ');
    if (spaceIndex < 0) {
        Serial.println("❌ Missing parameters: RELAY <channel> <state>");
        Serial.println("   Example: RELAY 1 ON");
        Serial.println("   Example: RELAY 3 ?");
        return;
    }
    
    String channelStr = params.substring(0, spaceIndex);
    String state = params.substring(spaceIndex + 1);
    
    state.trim();
    state.toUpperCase();
    
    // Parse channel number
    int channel = channelStr.toInt();
    
    // Validate channel
    if (channel < 1 || channel > 6) {
        Serial.print("❌ Invalid channel: ");
        Serial.print(channel);
        Serial.println(" (valid range: 1-6)");
        return;
    }
    
    // Handle READ status request
    if (state == "?") {
        bool status = board.getRelayState(channel);
        Serial.print("🔌 Relay CH");
        Serial.print(channel);
        Serial.print(" Status: ");
        Serial.println(status ? "🟢 ON" : "🔴 OFF");
        return;
    }
    
    // Convert alternate formats to ON/OFF
    if (state == "1") {
        state = "ON";
    } else if (state == "0") {
        state = "OFF";
    }
    
    // Validate state
    if (state != "ON" && state != "OFF" && state != "TGL") {
        Serial.print("❌ Invalid state: ");
        Serial.println(state);
        Serial.println("   Valid states: ON, OFF, TGL (toggle), 1, 0, or ? (read status)");
        return;
    }
    
    // Control relay
    Serial.print("🔌 Relay CH");
    Serial.print(channel);
    Serial.print(" -> ");
    Serial.print(state);
    Serial.println("...");
    
    if (state == "ON") {
        board.relayOn(channel);
    } else if (state == "OFF") {
        board.relayOff(channel);
    } else if (state == "TGL") {
        board.toggleRelay(channel);
        // Show new state after toggle
        delay(50);
        bool newState = board.getRelayState(channel);
        Serial.print("   New state: ");
        Serial.println(newState ? "🟢 ON" : "🔴 OFF");
    }
    
    board.buzzer_beep(1);  // Feedback beep
    Serial.println("✅ Relay changed");
}

/***********************************************************************
 * HANDLE STATUS COMMAND
 * Displays board and ESP32S3 system information
 ***********************************************************************/
void handleStatusCommand(String params) {
    params.trim();
    params.toUpperCase();
    
    Serial.println();
    Serial.println("═══════════════════════════════════════════════════════════════");
    Serial.println("              📊 BOARD & SYSTEM STATUS");
    Serial.println("═══════════════════════════════════════════════════════════════");
    Serial.println();
    
    // 1. Board Information
    Serial.println("🔌 BOARD INFORMATION");
    Serial.println("───────────────────────────────────────────────────────────────");
    Serial.println(board.getBoardInfo().c_str());
    Serial.println();
    
    // 2. Relay Status
    Serial.println("🔴 RELAY CHANNELS");
    Serial.println("───────────────────────────────────────────────────────────────");
    for (int i = 1; i <= 6; i++) {
        bool status = board.getRelayState(i);
        Serial.printf("   Relay CH%d: %s %s\n", i, status ? "🟢 ON " : "🔴 OFF", 
                      status ? "(10A @ 250VAC/30VDC)" : "");
    }
    Serial.println();
    
    // 3. Temperature
    Serial.println("🌡️  TEMPERATURE");
    Serial.println("───────────────────────────────────────────────────────────────");
    float tempC = temperatureRead();
    Serial.printf("   Current: %.1f°C\n", tempC);
    Serial.println();
    
    // 4. MAC Address
    Serial.println("📱 MAC ADDRESS");
    Serial.println("───────────────────────────────────────────────────────────────");
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    Serial.printf("   WiFi (STA): %02X:%02X:%02X:%02X:%02X:%02X\n", 
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println();
    
    // 5. Memory Information
    Serial.println("💾 MEMORY (RAM)");
    Serial.println("───────────────────────────────────────────────────────────────");
    uint32_t heapSize = ESP.getHeapSize();
    uint32_t heapFree = ESP.getFreeHeap();
    uint32_t heapUsed = heapSize - heapFree;
    Serial.printf("   Total Size: %u bytes (%.2f KB)\n", heapSize, heapSize / 1024.0);
    Serial.printf("   Used: %u bytes (%.2f KB)\n", heapUsed, heapUsed / 1024.0);
    Serial.printf("   Free: %u bytes (%.2f KB)\n", heapFree, heapFree / 1024.0);
    Serial.printf("   Usage: %.1f%%\n", (heapUsed * 100.0) / heapSize);
    Serial.println();
    
    // 6. Flash Information
    Serial.println("💿 FLASH");
    Serial.println("───────────────────────────────────────────────────────────────");
    uint32_t flashSize = ESP.getFlashChipSize();
    uint32_t sketchSize = ESP.getSketchSize();
    uint32_t flashFree = flashSize - sketchSize;
    Serial.printf("   Total Size: %u bytes (%.2f MB)\n", flashSize, flashSize / 1024.0 / 1024.0);
    Serial.printf("   Sketch Size: %u bytes (%.2f KB)\n", sketchSize, sketchSize / 1024.0);
    Serial.printf("   Free: %u bytes (%.2f MB)\n", flashFree, flashFree / 1024.0 / 1024.0);
    Serial.println();
    
    // 7. Chip Information
    Serial.println("🖥️  CHIP INFORMATION");
    Serial.println("───────────────────────────────────────────────────────────────");
    Serial.printf("   Model: %s\n", ESP.getChipModel());
    Serial.printf("   Revision: %d\n", ESP.getChipRevision());
    Serial.printf("   Cores: %d\n", ESP.getChipCores());
    Serial.printf("   CPU Frequency: %u MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("   SDK Version: %s\n", ESP.getSdkVersion());
    Serial.println();
    
    // 8. Uptime
    Serial.println("⏱️  UPTIME");
    Serial.println("───────────────────────────────────────────────────────────────");
    uint32_t uptime = millis();
    uint32_t days = uptime / (24 * 60 * 60 * 1000);
    uint32_t hours = (uptime % (24 * 60 * 60 * 1000)) / (60 * 60 * 1000);
    uint32_t minutes = (uptime % (60 * 60 * 1000)) / (60 * 1000);
    uint32_t seconds = (uptime % (60 * 1000)) / 1000;
    Serial.printf("   %d days, %02d:%02d:%02d (Total: %u ms)\n", days, hours, minutes, seconds, uptime);
    Serial.println();
    
    Serial.println("═══════════════════════════════════════════════════════════════");
    Serial.println();
}

/***********************************************************************
 * HANDLE RESET COMMAND
 * Software reset of ESP32-S3
 ***********************************************************************/
void handleResetCommand(void) {
    Serial.println("🔄 Resetting ESP32-S3 board...");
    Serial.println("   Turning off all relays...");
    board.allRelaysOff();
    
    Serial.println("   Saving state...");
    Serial.flush();
    
    delay(500);
    
    // Perform software reset
    ESP.restart();
}

/***********************************************************************
 * HANDLE LORA COMMAND
 * Format: LORA <subcommand> [parameters]
 * Subcommands: INIT, CONFIG, AT <command>, SEND <message>
 ***********************************************************************/
void handleLoRaCommand(String params) {
    params.trim();
    params.toUpperCase();
    
    // Parse subcommand
    int spaceIndex = params.indexOf(' ');
    String subCmd = (spaceIndex > 0) ? params.substring(0, spaceIndex) : params;
    String subParams = (spaceIndex > 0) ? params.substring(spaceIndex + 1) : "";
    
    if (subCmd == "INIT") {
        Serial.println("🔄 Re-initializing LoRa DTU...");
        initLoRaDTU();
    } else if (subCmd == "CONFIG") {
        Serial.println("🔧 Reconfiguring LoRa DTU for Stream Mode...");
        loraConfigureStream();
    } else if (subCmd == "AT") {
        if (subParams.length() == 0) {
            Serial.println("❌ Missing AT command");
            Serial.println("   Example: LORA AT AT+VER");
            return;
        }
        Serial.println();
        loraATCommand(subParams);
    } else if (subCmd == "SEND") {
        if (subParams.length() == 0) {
            Serial.println("❌ Missing message to send");
            Serial.println("   Example: LORA SEND Hello World");
            return;
        }
        Serial.println();
        sendLoRaData(subParams);
    } else if (subCmd == "STATUS") {
        Serial.println("\n📡 LoRa DTU Status:");
        Serial.println("───────────────────────────────────────");
        Serial.print("   Connection:     ");
        Serial.println(loraStatus.isConnected ? "🟢 Connected" : "🔴 Disconnected");
        Serial.print("   Stream Mode:    ");
        Serial.println(loraStatus.streamMode ? "✅ Enabled" : "❌ Disabled");
        Serial.print("   Packets Sent:   ");
        Serial.println(loraStatus.packetsSent);
        Serial.print("   Packets Recv:   ");
        Serial.println(loraStatus.packetsReceived);
        Serial.print("   Errors:         ");
        Serial.println(loraStatus.packetErrors);
        if (loraStatus.lastErrorMsg[0] != '\0') {
            Serial.print("   Last Error:     ");
            Serial.println(loraStatus.lastErrorMsg);
        }
        Serial.println();
    } else {
        Serial.println("❌ Unknown LoRa subcommand: " + subCmd);
        Serial.println("   Valid subcommands: INIT, CONFIG, AT <cmd>, SEND <msg>, STATUS");
    }
}

/***********************************************************************
 * INITIALIZE LoRa DTU (UART2 for RS485)
 ***********************************************************************/
void initLoRaDTU() {
  Serial.println("\n📡 Initializing LoRa DTU (UART2 RS485)...");
  
  // Configure UART2 pins for RS485
  Serial.printf("   Configuring UART2: TX=GPIO%d, RX=GPIO%d\n", LORA_DTU_TX_PIN, LORA_DTU_RX_PIN);
  
  // Initialize UART2 with explicit pin mapping (RS485 with automatic direction control)
  Serial2.begin(LORA_DTU_BAUD, SERIAL_8N1, LORA_DTU_RX_PIN, LORA_DTU_TX_PIN);
  
  // Enable RS485 half-duplex mode (automatic direction control by hardware)
  if (!Serial2.setMode(UART_MODE_RS485_HALF_DUPLEX)) {
    Serial.println("⚠️  RS485 half-duplex mode warning!");
  }
  
  if (!Serial2) {
    Serial.println("❌ LoRa DTU: UART2 initialization failed!");
    strcpy(loraStatus.lastErrorMsg, "UART2 init failed");
    return;
  }
  
  Serial.println("   ✓ UART2 initialized (9600 bps, 8N1, Half-duplex mode)");
  Serial.println("   ✓ GPIO17 (TX): Ready to send");
  Serial.println("   ✓ GPIO18 (RX): Ready to receive");
  Serial.println("   ✓ RS485 direction: Automatic hardware control");
  
  delay(100);
  while (Serial2.available()) Serial2.read();
  loraRxIndex = 0;
  
  loraStatus.isConnected = true;
  loraStatus.lastActivityTime = millis();
  
  Serial.println("✅ LoRa DTU initialization complete!\n");
}

/***********************************************************************
 * SEND AT COMMAND TO LoRa DTU
 ***********************************************************************/
void loraATCommand(String cmd) {
  if (!loraStatus.isConnected) {
    Serial.println("❌ LoRa DTU: Not connected!");
    return;
  }
  
  if (!cmd.endsWith("\r\n")) cmd += "\r\n";
  
  // Clear any pending data in RX buffer
  while (Serial2.available()) {
    Serial2.read();
  }
  delay(10);  // Reduced from 50ms for faster response
  
  // Send command
  Serial2.print(cmd);
  Serial2.flush();
  
  // Wait for response (RS485 half-duplex needs time to switch)
  delay(100);  // Reduced from 300ms
  
  String response = "";
  uint32_t startTime = millis();
  uint32_t lastByteTime = startTime;
  
  // Read response with timeout logic
  while (millis() - startTime < 1500) {  // Overall timeout: 1.5 seconds (reduced from 2s)
    if (Serial2.available()) {
      char ch = Serial2.read();
      response += ch;
      lastByteTime = millis();
      
      // If we got a complete line, exit early
      if (ch == '\n') {
        delay(50);  // Reduced from 100ms
        while (Serial2.available()) {
          response += (char)Serial2.read();
        }
        break;
      }
      delay(1);  // Reduced from 2ms
    } else {
      // No data available - check if we've been idle too long
      if (response.length() > 0 && (millis() - lastByteTime > 100)) {  // Reduced from 200ms
        break;  // Response is complete
      }
    }
  }
  
  // Print response
  if (response.length() > 0) {
    response.trim();
    
    // Log response with diagnostics
    Serial.printf("   📥 Raw: ");
    for (int i = 0; i < response.length() && i < 64; i++) {
      char ch = response[i];
      if (ch >= 32 && ch <= 126) {
        Serial.print(ch);
      } else if (ch == '\r') {
        Serial.print("\\r");
      } else if (ch == '\n') {
        Serial.print("\\n");
      } else {
        Serial.printf("[0x%02X]", ch);
      }
    }
    Serial.println();
    
    // Check response type
    if (response.indexOf("OK") != -1) {
      Serial.printf("   ✅ Response: %s\n", response.c_str());
      loraStatus.lastActivityTime = millis();
    } else if (response.indexOf("ERROR") != -1) {
      Serial.printf("   ❌ Response: %s\n", response.c_str());
      loraStatus.packetErrors++;
    } else if (response.indexOf("SX1262") != -1 || response.indexOf("xF") != -1) {
      Serial.printf("   ✅ Device ID: %s\n", response.c_str());
    } else {
      Serial.printf("   📋 Response: %s\n", response.c_str());
    }
  } else {
    Serial.println("   ⏱️  Response: TIMEOUT (No data received)");
    Serial.println("      🔍 Diagnostics:");
    Serial.println("      • Check RS485 wiring (GPIO17=TX, GPIO18=RX)");
    Serial.println("      • Verify module power supply (12V)");
    Serial.println("      • Confirm LoRa module is powered on");
    loraStatus.packetErrors++;
    strcpy(loraStatus.lastErrorMsg, "AT Command timeout");
  }
}

/***********************************************************************
 * CONFIGURE LoRa DTU FOR STREAM MODE
 ***********************************************************************/
void loraConfigureStream() {
  Serial.println("\n⚙️  Configuring LoRa DTU - Stream Mode...");
  delay(100);
  
  // Enter AT Command Mode
  Serial.println("   [1] Entering AT Command Mode (sending +++)...");
  Serial2.print("+++\r\n");
  Serial2.flush();
  delay(500);
  
  // Clear any residual data
  while (Serial2.available()) Serial2.read();
  delay(100);
  
  // Send configuration commands
  Serial.println("   [2] Sending configuration commands...");
  
  loraATCommand("AT");                     // Basic test
  delay(100);
  
  loraATCommand("AT+VER");                 // Get version
  delay(100);
  
  loraATCommand("AT+PORT=2");              // Set RS485 interface
  delay(100);
  
  loraATCommand("AT+BAUD=9600");           // Set baud rate
  loraATCommand("AT+COMM=\"8N1\"");        // Set data format
  delay(100);
  
  loraATCommand("AT+MODE=1");              // Stream Mode
  delay(100);
  
  loraATCommand("AT+TXCH=18");             // Transmit channel 18 (868MHz)
  loraATCommand("AT+RXCH=18");             // Receive channel 18
  delay(100);
  
  loraATCommand("AT+ADDR=0");              // Device address
  delay(100);
  
  loraATCommand("AT+SF=7");                // Spreading Factor 7
  loraATCommand("AT+BW=0");                // Bandwidth 125KHz
  loraATCommand("AT+CR=1");                // Code Rate 4/5
  loraATCommand("AT+PWR=22");              // Power 22dBm (max)
  delay(100);
  
  // Exit AT Command Mode
  Serial.println("   [3] Exiting AT Command Mode...");
  loraATCommand("AT+EXIT");
  
  delay(500);
  
  // Verify configuration
  Serial.println("   [4] Verifying configuration...");
  loraATCommand("AT+VER");
  
  loraStatus.streamMode = true;
  
  Serial.println("\n✅ Stream Mode configuration complete!");
  Serial.println("   Channel: 18 (868MHz) | SF: 7 | BW: 125KHz | Power: 22dBm");
  Serial.println("   Module is ready to send/receive LoRa data\n");
}

/***********************************************************************
 * SEND DATA VIA LoRa
 ***********************************************************************/
void sendLoRaData(String data) {
  if (!loraStatus.isConnected) {
    Serial.println("❌ LoRa DTU: Not connected!");
    return;
  }
  
  if (data.length() == 0) {
    Serial.println("❌ Empty message!");
    return;
  }
  
  Serial.printf("📤 Sending LoRa data: '%s' (%d bytes)\n", data.c_str(), data.length());
  
  // Add line terminator
  if (!data.endsWith("\n")) {
    data += "\n";
  }
  
  // Send data via UART2 (RS485 to LoRa DTU)
  Serial2.print(data);
  Serial2.flush();
  
  loraStatus.packetsSent++;
  loraStatus.lastActivityTime = millis();
  
  Serial.println("✅ Data sent!");
}

/***********************************************************************
 * HANDLE LoRa AT INPUT FROM SERIAL MONITOR
 * Allows user to send AT commands interactively
 ***********************************************************************/
void handleLoraATInput(void) {
  static String loraInputBuffer = "";

  // Process all available serial data
  while (Serial.available() > 0) {
    char inputChar = Serial.read();
    
    // ✅ ECHO character immediately for user feedback
    if (inputChar == '\r' || inputChar == '\n') {
      // Don't echo carriage return, but show newline for formatting
      Serial.println();
    } else if (inputChar == '\b' || inputChar == 127) {
      // Backspace: show visual feedback
      if (loraInputBuffer.length() > 0) {
        Serial.print("\b \b");  // Move cursor back, erase, move back again
        loraInputBuffer.remove(loraInputBuffer.length() - 1);
      }
    } else if (inputChar >= 32 && inputChar < 127) {
      // Printable characters: echo immediately
      Serial.write(inputChar);
      
      if (loraInputBuffer.length() < MAX_COMMAND_LENGTH) {
        loraInputBuffer += inputChar;
      }
    }
    
    // Process command when Enter is pressed
    if (inputChar == '\r' || inputChar == '\n') {
      if (loraInputBuffer.length() > 0) {
        String upperCmd = loraInputBuffer;
        upperCmd.toUpperCase();

        // Check for HELP command
        if (upperCmd == "HELP") {
          if (atCommandModeActive) {
            // Show LoRa AT commands when in AT mode
            Serial.println("\n📋 SX1262-LoRa-DTU-xF AT Commands Reference:");
            Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
            Serial.println("Basic Commands:");
            Serial.println("  +++              Enter AT Command Mode");
            Serial.println("  AT+EXIT          Exit AT Command Mode");
            Serial.println("  AT+VER           Get Firmware Version");
            Serial.println("  AT+HELP          Show Help");
            Serial.println("  ATE=1/0          Enable/Disable Echo");
            Serial.println("\nConfiguration Commands:");
            Serial.println("  AT+MODE=1        Stream Mode (continuous data)");
            Serial.println("  AT+MODE=2        Packet Mode (modbus RTU)");
            Serial.println("  AT+MODE=3        Relay Mode (range extension)");
            Serial.println("  AT+PORT=2        Select RS485 Port (2=RS485)");
            Serial.println("  AT+BAUD=9600     Set Baud Rate (9600-115200)");
            Serial.println("  AT+SF=7          Set Spreading Factor (7-12)");
            Serial.println("  AT+BW=0          Set Bandwidth (0=125K, 1=250K, 2=500K)");
            Serial.println("  AT+CR=1          Set Coding Rate (1-4)");
            Serial.println("  AT+PWR=22        Set TX Power (10-22 dBm)");
            Serial.println("  AT+TXCH=18       Set TX Channel (0-80)");
            Serial.println("  AT+RXCH=18       Set RX Channel (0-80)");
            Serial.println("  AT+ADDR=0        Set Device Address (0-65535)");
            Serial.println("  AT+NETID=0       Set Network ID (0-255)");
            Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
          } else {
            // Show regular board commands when not in AT mode
            printHelpMenu();
          }
        }
        // Check for +++ command (Enter AT Mode)
        else if (upperCmd == "+++") {
          Serial.println("📤 Entering AT Command Mode...");
          loraATCommand("+++");
          atCommandModeActive = true;
          atModeEnterTime = millis();
        }
        // Check for AT+EXIT command (Exit AT Mode)
        else if (upperCmd == "AT+EXIT") {
          Serial.println("📤 Exiting AT Command Mode...");
          loraATCommand("AT+EXIT");
          atCommandModeActive = false;
        }
        // Handle AT commands
        else if (upperCmd.startsWith("AT")) {
          Serial.printf("⚙️  AT Command: %s\n", loraInputBuffer.c_str());
          
          // If not in AT mode, enter it first
          if (!atCommandModeActive) {
            Serial.println("   (Auto-entering AT Command Mode first)");
            Serial2.print("+++\r\n");
            delay(500);
            atCommandModeActive = true;
            atModeEnterTime = millis();
          }
          
          // Send the command
          loraATCommand(loraInputBuffer);
        }
        // Check if it's a regular command (not in AT mode)
        else if (!atCommandModeActive && 
                 (upperCmd.startsWith("RELAY") || 
                  upperCmd.startsWith("BUZZER") || 
                  upperCmd.startsWith("LED") || 
                  upperCmd.startsWith("ALL") || 
                  upperCmd.startsWith("STATUS") || 
                  upperCmd.startsWith("LORA") || 
                  upperCmd == "RESET" ||
                  upperCmd == "BUZZER_TEST" ||
                  upperCmd == "HELP")) {
          // Process as regular command
          processCommand(loraInputBuffer);
        }
        else if (loraInputBuffer.length() > 0 && atCommandModeActive) {
          // In AT mode but not a recognized command
          Serial.printf("❌ Unknown LoRa AT command: %s\n", loraInputBuffer.c_str());
          Serial.println("   Type 'HELP' for available LoRa AT commands or 'AT+EXIT' to exit AT mode");
        }
        else if (loraInputBuffer.length() > 0 && !atCommandModeActive) {
          // Regular mode but unknown command
          Serial.printf("❌ Unknown command: '%s'\n", loraInputBuffer.c_str());
          Serial.println("   Type 'HELP' for available commands");
        }
        
        loraInputBuffer = "";
        printPrompt();
      }
    }
  }
}
