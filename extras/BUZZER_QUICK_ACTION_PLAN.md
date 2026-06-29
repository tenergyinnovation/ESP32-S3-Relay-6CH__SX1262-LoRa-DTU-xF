# ⚡ Buzzer Issue - Quick Action Plan

## 🎯 Your Situation
- ✅ BUZZER command shows "✅ Buzzer complete"
- ❌ No sound is heard
- 🤔 Unclear if it's code or hardware issue

---

## 🚀 IMMEDIATE ACTIONS (Do These FIRST)

### Step 1: Re-Upload Code (5 minutes)
1. In VS Code, open PlatformIO panel
2. Click **Project Tasks → Clean**
3. Click **Build** (wait for completion)
4. If Build passes, click **Upload**
5. Wait for "Upload complete" message

**Why**: Ensures latest buzzer fix is on your board

---

### Step 2: Run Hardware Diagnostic (1 minute)
Open Serial Monitor (115200 baud) and type:
```
BUZZER_TEST
```

**What to expect**:
- ✅ If you hear a **continuous beeping for 2 seconds** → Code is fine, proceed to Step 3
- ❌ If you hear **nothing** → Hardware issue, proceed to Step 4

---

### Step 3: Verify Buzzer Pin Connection (2 minutes)
If BUZZER_TEST produces sound:

Use a **multimeter** to check GPIO21 voltage:
1. Set multimeter to **DC Voltage** mode
2. Red probe → GPIO21 pin
3. Black probe → GND
4. Run BUZZER_TEST again
5. During beeping, multimeter should show **3.3V**

**Results**:
- ✅ Shows 3.3V → Buzzer connection is good, problem might be buzzer hardware
- ❌ Shows 0V or nothing → GPIO21 connection issue

---

### Step 4: Check Physical Connections (3 minutes)
If BUZZER_TEST produces NO sound:

**Inspect These**:
```
1. ✓ Buzzer wires not loose or bent
2. ✓ Buzzer pin 1 (usually red/+)  → GPIO21
3. ✓ Buzzer pin 2 (usually black/-) → GND
4. ✓ No visible burnt/damaged components
5. ✓ No debris or corrosion on connectors
```

**If found issue**: 
- Reseat connections firmly
- Clean connectors gently with eraser
- Re-upload code
- Test again

---

## 🔧 DETAILED TROUBLESHOOTING

If quick actions didn't work, follow this order:

### Problem A: BUZZER_TEST produces NO sound
**Most likely**: Hardware issue with buzzer or GPIO21 connection

**Tests to run**:
1. Measure GPIO21 voltage during BUZZER_TEST (should be 3.3V)
2. Inspect buzzer for physical damage
3. Try alternative buzzer if available
4. Check for GPIO21 conflicts in code

📖 **Details**: See [BUZZER_DIAGNOSTIC_GUIDE.md](./BUZZER_DIAGNOSTIC_GUIDE.md) - Section "Advanced Testing"

---

### Problem B: BUZZER_TEST works but BUZZER command doesn't
**Most likely**: Issue with `buzzer_beep()` function

**Solution**:
- Verify latest code is uploaded
- Check loop() is being called regularly (add debug output)
- Verify PWM frequency correct (1000 Hz default)

---

### Problem C: Sound is wrong (not typical beep)
**Most likely**: Frequency or timing issue

**Adjust in [ESP32S3_Relay6CH.h](../lib/ESP32-S3-Relay-6CH/src/ESP32S3_Relay6CH.h)**:
```cpp
// Line ~45: Change frequency
static constexpr uint16_t DEFAULT_FREQ = 2000;  // Try 500-4000 Hz
```

---

## 📝 CRITICAL CHECKLIST

Before claiming "buzzer doesn't work":
- [ ] Code recompiled and uploaded successfully (Build: Exit 0, Upload: Exit 0)
- [ ] Serial monitor shows "✓ Buzzer: Ready" at startup
- [ ] Ran BUZZER_TEST command
- [ ] Checked GPIO21 voltage with multimeter during BUZZER_TEST
- [ ] Inspected physical buzzer connections
- [ ] Tested alternative buzzer (if available)

---

## 💡 KEY TECHNICAL INFO

**Current Buzzer Implementation**:
- Pin: GPIO21
- PWM Channel: 0
- Frequency: 1000 Hz
- Beep duration: 100ms ON, 100ms OFF
- Control: Direct PWM with `ledcWrite()`

**Why this setup**:
- Direct PWM = instant response, no delays
- 1000 Hz = standard buzzer frequency
- LEDC = Hardware PWM (accurate timing)

---

## 🎯 Next Steps Based on Results

**If BUZZER_TEST works but BUZZER doesn't**:
- → Look at `buzzer_beep()` in [ESP32S3_Relay6CH.cpp](../lib/ESP32-S3-Relay-6CH/src/ESP32S3_Relay6CH.cpp) line 635

**If BUZZER_TEST doesn't work at all**:
- → Check GPIO21 connections
- → Verify buzzer hardware not defective
- → Check power supply voltage

**If multimeter shows 0V on GPIO21**:
- → GPIO21 may be damaged
- → Check for short circuits
- → Try alternative GPIO pin

---

## 📞 Getting Help

**Provide this information when asking for help**:
1. Result of BUZZER_TEST (sound/no sound)
2. GPIO21 voltage reading from multimeter
3. Serial monitor output during startup
4. Photo of buzzer connections
5. Buzzer model/specification

---

**Status**: Ready for Testing  
**Date**: 29 June 2026  
**Last Updated**: Latest
