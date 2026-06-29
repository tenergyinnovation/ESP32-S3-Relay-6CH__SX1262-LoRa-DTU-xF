# Relay Pin Mapping Fix & Verification Guide

## Issue Report
- **Relay 3 & 4**: Positions swapped (เสลับตำแหน่ง)
- **Relay 5 & 6**: Non-functional (ไม่ทำงาน)

## GPIO Pin Mapping - CORRECTED

### Old (Incorrect) Mapping
```
Relay 1 → GPIO1
Relay 2 → GPIO2
Relay 3 → GPIO42  ❌ SWAPPED
Relay 4 → GPIO41  ❌ SWAPPED
Relay 5 → GPIO40  ❌ WRONG
Relay 6 → GPIO39  ❌ WRONG
```

### New (Fixed) Mapping
```
Relay 1 → GPIO1
Relay 2 → GPIO2
Relay 3 → GPIO41  ✅ CORRECTED
Relay 4 → GPIO42  ✅ CORRECTED
Relay 5 → GPIO45  ✅ CORRECTED
Relay 6 → GPIO46  ✅ CORRECTED
```

**File Updated**: [lib/ESP32-S3-Relay-6CH/src/ESP32S3_Relay6CH.h](../lib/ESP32-S3-Relay-6CH/src/ESP32S3_Relay6CH.h)

**Changes Made**:
- Swapped GPIO41 and GPIO42 (Relay 3 & 4): `{1, 2, 41, 42, ...}`
- Corrected Relay 5 & 6 pins: Changed from GPIO40/39 to GPIO45/46
- Final RELAY_PINS array: `{1, 2, 41, 42, 45, 46}`
- Updated comments to reflect correct mapping

## Testing Instructions

### Step 1: Recompile and Upload
```bash
pio run -t upload
```

### Step 2: Test Individual Relays

**Via Serial Monitor** (115200 baud):

```
# Test Relay 1
RELAY 1 ON
→ Should activate Relay 1 (GPIO1)
RELAY 1 OFF

# Test Relay 3 (was swapped with Relay 4)
RELAY 3 ON
→ Should now activate Relay 3 (GPIO41) - NOT Relay 4
RELAY 3 OFF

# Test Relay 4 (was swapped with Relay 3)
RELAY 4 ON
→ Should now activate Relay 4 (GPIO42) - NOT Relay 3
RELAY 4 OFF

# Test Relay 5 (GPIO45)
RELAY 5 ON
→ Should activate Relay 5 (GPIO45)
RELAY 5 OFF

# Test Relay 6 (GPIO46)
RELAY 6 ON
→ Should activate Relay 6 (GPIO46)
RELAY 6 OFF

# Test ALL ON/OFF
ALL ON
→ All 6 relays should activate in order
ALL OFF
→ All 6 relays should deactivate
```

### Step 3: Verify with STATUS Command
```
STATUS
→ Check board info and relay state flags
```

## Expected Results After Fix

| Relay | GPIO | Command | Expected Behavior |
|-------|------|---------|-------------------|
| 1 | GPIO1 | RELAY 1 ON | ✅ Relay 1 activates |
| 2 | GPIO2 | RELAY 2 ON | ✅ Relay 2 activates |
| 3 | GPIO41 | RELAY 3 ON | ✅ Relay 3 activates (previously swapped) |
| 4 | GPIO42 | RELAY 4 ON | ✅ Relay 4 activates (previously swapped) |
| 5 | GPIO45 | RELAY 5 ON | ✅ Relay 5 activates (corrected from GPIO40) |
| 6 | GPIO46 | RELAY 6 ON | ✅ Relay 6 activates (corrected from GPIO39) |

## Troubleshooting

### If Relay 5 & 6 Still Don't Work

Possible causes:
1. **GPIO45/GPIO46 Hardware Issue**
   - Check physical solder connections on board
   - Test continuity with multimeter: ESP32-S3 pin → Relay input

2. **Pin Conflict**
   - GPIO45/46 might be shared with SD card interface (SDIO_D2/D3)
   - Verify board doesn't use SD card mode

### Debug Steps for Relay 5 & 6 (if still not working)

1. **Test GPIO directly** (add to main.cpp temporary test):
```cpp
void testGPIODirect() {
    pinMode(45, OUTPUT);
    pinMode(46, OUTPUT);
    digitalWrite(45, HIGH);  // Should activate Relay 5
    delay(1000);
    digitalWrite(45, LOW);
    digitalWrite(46, HIGH);  // Should activate Relay 6
    delay(1000);
    digitalWrite(46, LOW);
}
```

2. **Verify Hardware Connections**:
   - Check physical solder connections on PCB
   - Test continuity: ESP32-S3 GPIO45 → Relay 5 input
   - Test continuity: ESP32-S3 GPIO46 → Relay 6 input

3. **Check GPIO45/46 Status**:
   - GPIO45 is SDIO_D2 (shared with SD card if used)
   - GPIO46 is SDIO_D3 (shared with SD card if used)
   - Verify board doesn't use SD card mode

## Summary of Changes

**Modified File**: `lib/ESP32-S3-Relay-6CH/src/ESP32S3_Relay6CH.h`

**Line ~32** - GPIO Pin Array:
```cpp
// OLD (with errors):
static constexpr uint8_t RELAY_PINS[6] = {1, 2, 42, 41, 40, 39};

// NEW (corrected):
static constexpr uint8_t RELAY_PINS[6] = {1, 2, 41, 42, 45, 46};
```

**Line ~31** - Comment Documentation:
```cpp
// OLD:
// - Relay 1: GPIO1  | Relay 4: GPIO41
// - Relay 2: GPIO2  | Relay 5: GPIO40
// - Relay 3: GPIO42 | Relay 6: GPIO39

// NEW:
// - Relay 1: GPIO1  | Relay 4: GPIO42
// - Relay 2: GPIO2  | Relay 5: GPIO45
// - Relay 3: GPIO41 | Relay 6: GPIO46
```

---

**Status**: ✅ Pin swap fix applied | 🔄 Awaiting hardware verification of Relay 5 & 6

**Next Action**: Test with corrected firmware and report results for Relay 5 & 6
