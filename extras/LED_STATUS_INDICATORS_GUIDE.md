# LED Status Indicators - Complete Understanding Guide 🔍

**Device**: ESP32-S3-POE-ETH-8DI-8DO  
**Analysis Date**: 2026-06-25  
**Status**: Complete LED behavior mapping

---

## 🎨 LED Color System Overview

The system uses **ONE RGB LED** (GPIO 38 - WS2812 NeoPixel) with different **blinking speeds** and **colors** to show different states.

### Color Meanings:
- 🔵 **BLUE** = Good/Success/Normal operation
- 🟡 **YELLOW** = Warning/Attempting connection
- 🔴 **RED** = Critical error/System will restart

### Blinking Patterns:
- **Slow (1.0 second)** = 0.5 second ON, 0.5 second OFF
- **Medium (0.5 second)** = 0.25 second ON, 0.25 second OFF
- **Fast (0.3 or 0.2 second)** = 0.15 second ON, 0.15 second OFF
- **Always ON (-1)** = Constant bright (no blinking)
- **OFF (0)** = No light

---

## 📊 LED Status in All System States

### Phase 1: System Startup (setup())

| Stage | LED Color | Blinking Speed | Function | Meaning |
|-------|-----------|----------------|----------|---------|
| 1️⃣ Initialization | 🔵 BLUE | Fast (0.1s) | `TickBlueLED(0.1)` | Setup starting |
| 2️⃣ WiFi Connected | 🔵 BLUE | Medium (0.5s) | `TickBlueLED(0.5)` | WiFi ready ✅ |
| 3️⃣ NTP Sync | (same) | (same) | (internal process) | Getting time |
| 4️⃣ MQTT Connect | (same) | (same) | (internal process) | Connecting to broker |
| 5️⃣ Watchdog Ready | 🔵 BLUE | Slow (1.0s) | `TickBlueLED(1.0)` | System ready 🎉 |

**Console Output Example**:
```
Device Name: esp32_tiny32
📨 Initializing MQTT Client...
Publish topic: loragreenergy/esp32_tiny32/uplink
Subscribe topic: loragreenergy/esp32_tiny32/cmd
✅ MQTT: connected to broker
🛡️  Initializing Watchdog Timer...
   ✓ WDT configured (Timeout: 180 seconds)
```

---

### Phase 2: Normal Operation (loop())

**Normal State**:
- LED: 🔵 BLUE (from previous successful state, still 1.0s blinking)
- Publishing: Every 10 seconds → "📤 MQTT Published to..."
- No issues detected

```
📤 MQTT Published to loragreenergy/esp32_tiny32/uplink: {"unit":"esp32_tiny32,0.1.0","wifi":"...","cnt":1234,"time":"2026-06-25T..."}
```

---

### Phase 3: WiFi Disconnection Detected (checkWiFiStatus)

**Timeline**:

#### Attempt 1-9: Trying to reconnect
```
Time: 5 seconds after WiFi drops
LED: 🟡 YELLOW | Blinking FAST (0.2s)
Buzzer: Silent
Serial: ⚠️ WiFi disconnected (attempt 1/10) - Status: 4
Status: Attempting to reconnect
```

**What's happening**:
- `checkWiFiStatus()` runs every 5 seconds
- `wifi_fail_count++` (incremented each check)
- `TickYellowLED(0.2)` shows fast yellow blinking
- System still tries to publish MQTT (but likely fails)

#### If WiFi Reconnects (before 10 failures):
```
Time: ~25 seconds after WiFi drops, then comes back
LED: 🔵 BLUE | Blinking SLOW (1.0s)
Buzzer: Silent
Serial: ✅ WiFi reconnected (was 5 failures)
Status: Back to normal ✅
```

**What happened**:
- WiFi connection restored
- `wifi_fail_count` reset to 0 → `TickBlueLED(1.0)` called
- System resumes normal operation
- LED returns to slow blue blinking

---

### Phase 4: WiFi Critical Failure (≥10 failures)

**Timeline**:
```
Time: ~50 seconds of WiFi being down continuously
LED: 🔴 RED | Always ON (no blinking) - constant bright red
Buzzer: 5 BEEPS 🔔🔔🔔🔔🔔
Serial: 🔴 CRITICAL: WiFi failed 10 times - Restarting system...
Status: SYSTEM RESTARTING
```

**What's happening**:
- `checkWiFiStatus()` detects 10th failure
- `TickRedLED(-1)` turns LED on permanently
- `buzzer_beep(5)` plays 5 alert beeps
- Waits 2 seconds to show error
- Calls `systemRestart(RESTART_REASON_WIFI)`

**Console Output Before Restart**:
```
⚠️  WiFi disconnected (attempt 9/10) - Status: 4
⚠️  WiFi disconnected (attempt 10/10) - Status: 4
🔴 CRITICAL: WiFi failed 10 times - Restarting system...

╔════════════════════════════════════════════════════════╗
║          🔄 SYSTEM RESTART INITIATED                   ║
╚════════════════════════════════════════════════════════╝
Reason: WiFi connection failed 10 times
Timestamp: 2026-06-25T14:35:22+07:00
Uptime: 287 seconds

Preparing for restart...
```

---

### Phase 5: MQTT Disconnection (when WiFi is OK)

**Key Difference**: MQTT check only runs if WiFi is connected!

#### Attempt 1-9: Trying to reconnect
```
Time: 5 seconds after MQTT drops (WiFi still OK)
LED: 🟡 YELLOW | Blinking MEDIUM (0.3s)
Buzzer: Silent
Serial: ⚠️ MQTT disconnected (attempt 1/10) - State: X
Status: Attempting MQTT reconnect
WiFi: Still connected (no change to WiFi LED)
```

**What's happening**:
- `checkMQTTStatus()` detects `client.connected() == false`
- `mqtt_fail_count++` incremented
- `TickYellowLED(0.3)` shows medium yellow blinking
- Different speed (0.3s) than WiFi (0.2s) for differentiation

#### If MQTT Reconnects (before 10 failures):
```
Time: ~25 seconds after MQTT drops, then comes back
LED: 🔵 BLUE | Blinking MEDIUM (0.5s)
Buzzer: Silent
Serial: ✅ MQTT reconnected (was 3 failures)
Status: Back to normal ✅
```

**What happened**:
- MQTT connection restored
- `mqtt_fail_count` reset to 0 → `TickBlueLED(0.5)` called
- System resumes publishing data
- **BUT**: LED stays at 0.5s (not 1.0s) until WiFi is also checked

---

### Phase 6: MQTT Critical Failure (≥10 failures)

**Timeline**:
```
Time: ~50 seconds of MQTT being down
LED: 🔴 RED | Always ON (no blinking) - constant bright red
Buzzer: 5 BEEPS 🔔🔔🔔🔔🔔
Serial: 🔴 CRITICAL: MQTT failed 10 times - Restarting system...
Status: SYSTEM RESTARTING
```

**Same as WiFi critical failure, but different reason!**

**Console Output Before Restart**:
```
⚠️ MQTT disconnected (attempt 10/10) - State: 5
🔴 CRITICAL: MQTT failed 10 times - Restarting system...

╔════════════════════════════════════════════════════════╗
║          🔄 SYSTEM RESTART INITIATED                   ║
╚════════════════════════════════════════════════════════╝
Reason: MQTT connection failed 10 times
Timestamp: 2026-06-25T14:40:15+07:00
Uptime: 450 seconds

Preparing for restart...
```

---

### Phase 7: Both WiFi AND MQTT Down

**Scenario**: WiFi fails first (MQTT depends on WiFi)

```
Time: WiFi goes down
LED: 🟡 YELLOW | Fast (0.2s)
Status: WiFi attempting

Time: WiFi still down after 5 seconds
LED: 🟡 YELLOW | Fast (0.2s) [unchanged]
Serial: checkMQTTStatus() detects WiFi down
Action: mqtt_fail_count = 0 (reset - WiFi problem, not MQTT)
Status: Can't test MQTT while WiFi is down

Result:
- Only WiFi counter increments
- If WiFi fails 10 times → Restart with "WiFi failed 10 times"
- MQTT counter stays at 0 (gets reset each check when WiFi is down)
```

**Key Point**: MQTT failures don't count while WiFi is down!

---

## 🎯 Quick Reference: What Each LED Pattern Means

| LED Pattern | Meaning | Time to Issue |
|-------------|---------|---------------|
| 🔵 Blue - Super Fast (0.1s) | Setup starting | Immediate |
| 🔵 Blue - Medium (0.5s) | WiFi connected | ~2-5 seconds |
| 🔵 Blue - Slow (1.0s) | System ready | ~10 seconds total |
| 🟡 Yellow - Fast (0.2s) | WiFi unstable | When WiFi down |
| 🟡 Yellow - Medium (0.3s) | MQTT unstable | When MQTT down |
| 🔴 Red - Always ON | CRITICAL ERROR | ~50 seconds of continuous failure |
| OFF | LED in setup/disabled | Setup phase or error |

---

## 📋 State Transition Diagram

```
┌────────────────────────────────────────────────────────────┐
│               LED STATE TRANSITIONS                        │
├────────────────────────────────────────────────────────────┤
│                                                            │
│  [Power On]                                               │
│       ↓                                                    │
│  🔵 Blue 0.1s (Setup start)                              │
│       ↓                                                    │
│  🔵 Blue 0.5s (WiFi ready)                               │
│       ↓                                                    │
│  🔵 Blue 1.0s (System ready) ← NORMAL STATE              │
│       ↓                                                    │
│    ┌──────────────┬──────────────┐                        │
│    ↓ (WiFi OK)   ↓ (WiFi Down)  ↓ (MQTT Down)           │
│    [Normal]  🟡 Yellow 0.2s  🟡 Yellow 0.3s             │
│                  (count: 1-9)     (count: 1-9)           │
│                  ↓ if reconnects  ↓ if reconnects        │
│                  🔵 Blue 1.0s  🔵 Blue 0.5s             │
│                  ↓              ↓                        │
│              [Normal]      [Normal, Blue 0.5s]           │
│              ↓ OR          ↓ OR                           │
│              (count ≥ 10) (count ≥ 10)                   │
│              ↓              ↓                            │
│         🔴 Red Always ON                                 │
│         Buzzer: 5 beeps                                  │
│         Action: systemRestart()                          │
│              ↓                                            │
│        [System Restarts]                                 │
│              ↓                                            │
│        [Back to: 🔵 Blue 0.1s]                          │
│                                                            │
└────────────────────────────────────────────────────────────┘
```

---

## 💡 How to Diagnose Problems Using LED

### Problem 1: LED is OFF
**Possible causes**:
- ❌ RGB LED not initialized properly
- ❌ GPIO 38 not configured
- ❌ Adafruit_NeoPixel library issue
- ✅ Check: Does Serial show "LED RGB Initialized"?

### Problem 2: LED is solid 🔵 BLUE, not blinking
**Possible causes**:
- ❌ LED function called with 0 (OFF)
- ❌ Ticker not working properly
- ✅ Check: Serial should show "Blinking..." messages

### Problem 3: LED stuck 🟡 YELLOW, never reconnects
**Possible causes**:
- ❌ WiFi/MQTT broker really is down
- ❌ Wrong SSID/password configured
- ❌ Network timeout too short
- ✅ Check: Serial shows why connection is failing
- ✅ Solution: Wait for WiFi/MQTT to come back or fix configuration

### Problem 4: LED suddenly turns 🔴 RED + buzzer beeps
**DO NOT IGNORE THIS!**
- ⚠️ System will restart in 2 seconds
- ✅ Check Serial for restart reason:
  - "WiFi connection failed 10 times" → WiFi problem
  - "MQTT connection failed 10 times" → MQTT broker problem
- 🔧 Solution:
  - WiFi: Check router, WiFi signal strength (RSSI in logs)
  - MQTT: Check if broker (159.65.11.12:1883) is running

---

## 🔍 Understanding the Code Logic

### checkWiFiStatus() - Every 5 seconds
```cpp
IF (5 seconds have NOT passed):
    RETURN  // Don't check yet
    
IF (WiFi is connected):
    IF (had previous failures):
        Reset wifi_fail_count = 0
        TickBlueLED(1.0)  // Show success
ELSE (WiFi is disconnected):
    wifi_fail_count++
    IF (wifi_fail_count < 10):
        TickYellowLED(0.2)  // Show warning
    ELSE (wifi_fail_count >= 10):
        TickRedLED(-1)  // Show critical
        buzzer_beep(5)
        systemRestart("WiFi failed 10 times")
```

### checkMQTTStatus() - Every 5 seconds
```cpp
IF (5 seconds have NOT passed):
    RETURN  // Don't check yet

IF (WiFi is NOT connected):
    mqtt_fail_count = 0  // Reset (can't test MQTT if WiFi is down)
    RETURN

IF (MQTT is connected):
    IF (had previous failures):
        Reset mqtt_fail_count = 0
        TickBlueLED(0.5)  // Show success
ELSE (MQTT is disconnected):
    mqtt_fail_count++
    IF (mqtt_fail_count < 10):
        TickYellowLED(0.3)  // Show warning (different speed)
    ELSE (mqtt_fail_count >= 10):
        TickRedLED(-1)  // Show critical
        buzzer_beep(5)
        systemRestart("MQTT failed 10 times")
```

---

## 📝 Summary Table: All LED States

| Stage | WiFi Status | MQTT Status | LED Color | Pattern | Duration | Next Check |
|-------|-------------|-------------|-----------|---------|----------|-----------|
| Setup | Connecting | N/A | 🔵 BLUE | 0.1s | 1-3s | Next |
| Setup | Connected | N/A | 🔵 BLUE | 0.5s | 3-5s | Next |
| Setup | Connected | N/A | 🔵 BLUE | 1.0s | Until loop | Next |
| Normal | Connected | Connected | 🔵 BLUE | 1.0s | Continuous | Check (5s) |
| Warning | Attempting | N/A | 🟡 YELLOW | 0.2s | Until reconnect | Check (5s) |
| Warning | N/A | Attempting | 🟡 YELLOW | 0.3s | Until reconnect | Check (5s) |
| Critical | Failed 10x | N/A | 🔴 RED | Always ON | 2s | Restart |
| Critical | N/A | Failed 10x | 🔴 RED | Always ON | 2s | Restart |

---

## ✅ Verification Checklist

After understanding the LED system, verify these behaviors:

- [ ] LED shows 🔵 Blue after power on (setup phase)
- [ ] LED shows 🔵 Blue slow after 10 seconds (ready)
- [ ] Can manually disconnect WiFi → LED turns 🟡 Yellow
- [ ] WiFi reconnects within 30 seconds → LED returns 🔵 Blue
- [ ] Leaving WiFi off for 60 seconds → LED turns 🔴 Red + 5 beeps + restart
- [ ] Check Serial shows "WiFi failed 10 times" before restart
- [ ] After restart, sequence repeats from 🔵 Blue 0.1s

---

**Status**: ✅ Complete LED understanding documented  
**Confidence**: HIGH - LED system is clear and functional  
**Next Step**: Deploy and observe LED behavior in real environment
