#!/usr/bin/env bash
set -euo pipefail

MOD_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT="$MOD_DIR/out"
rm -rf "$OUT"
mkdir -p "$OUT/zygisk/arm64-v8a" "$OUT/zygisk/armeabi-v7a" "$OUT/assets"

# Build for arm64 (requires ANDROID_NDK and cmake in PATH)
if [ -z "${ANDROID_NDK:-}" ]; then
  echo "Please export ANDROID_NDK to your NDK path"
  exit 1
fi

# Configure and build arm64
BUILD_DIR="$MOD_DIR/build-arm64"
rm -rf "$BUILD_DIR"; mkdir -p "$BUILD_DIR"
cmake -S "$MOD_DIR/zygisk/src/main" -B "$BUILD_DIR" -DANDROID_ABI=arm64-v8a   -DANDROID_PLATFORM=android-21 -DANDROID_STL=c++_shared   -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake"   -DANDROID_NDK="$ANDROID_NDK"
cmake --build "$BUILD_DIR" --config Release
cp "$BUILD_DIR/libzygisk-jcc120spoof.so" "$OUT/zygisk/arm64-v8a/zygisk-jcc120spoof.so"

# Copy module files
cp "$MOD_DIR/module.prop" "$OUT/"
cp "$MOD_DIR/post-fs-data.sh" "$OUT/"
cp "$MOD_DIR/service.sh" "$OUT/"
cp "$MOD_DIR/customize.sh" "$OUT/"
cp -r "$MOD_DIR/assets" "$OUT/"

# Package
cd "$OUT"
zip -r "jcc120spoof-release.zip" .
echo "Built: $OUT/jcc120spoof-release.zip"