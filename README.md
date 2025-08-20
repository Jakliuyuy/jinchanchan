# JCC 120fps Device Spoofer (Zygisk)

**Target app:** `com.tencent.jkchess` (金铲铲之战)  
**What it does:** For the target app only, spoof `android.os.Build` fields (MODEL/BRAND/MANUFACTURER/PRODUCT/DEVICE/BOARD/FINGERPRINT) to mimic a phone in the game's high-FPS whitelist (e.g., **nubia NX709S** or **ASUS_AI2201_B**). This often unlocks 120/144/165 fps options in settings if your display supports those refresh rates.

> ⚠️ Requirements
- Magisk 24.0+ with **Zygisk** enabled.
- Android 8+ (tested logic is generic; compile required).
- A 120Hz+ display; otherwise you won't *see* 120fps even if unlocked.
- Game versions can change checks; success isn't guaranteed.

## Build & Install
1. Install Android NDK (r26+ recommended) and CMake.
2. On a Linux/macOS shell:
   ```bash
   cd JCC120_Spoof_Zygisk
   bash build.sh
   ```
   This generates `out/jcc120spoof-release.zip`.
3. Flash the zip in Magisk Manager. Reboot.
4. (Optional) Edit `assets/config.json` to switch `profile` between `NX709S` and `ASUS_AI2201_B` and reflash.

## Scope
The module only activates for `com.tencent.jkchess` using Zygisk app specialization hooks. It does **not** change system-wide props.

## Notes
- 165fps generally needs ASUS ROG 6系等（`ASUS_AI2201_B`）白名单；120fps常见于红魔 `NX709S` 等。
- If the game additionally checks GPU/vendor libs, further spoofing may be required.
- SafetyNet/PlayIntegrity: modifying props inside app process usually doesn't affect system integrity, but proceed at your own risk.