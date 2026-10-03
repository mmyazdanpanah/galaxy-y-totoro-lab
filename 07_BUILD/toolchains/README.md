# Totoro ARM Toolchains

## Purpose

This directory contains reproducible build inputs for the Samsung Galaxy Y GT-S5360 (totoro) ARMv6 modernization experiments.

The first recovered compiler is the Android Open Source Project prebuilt GCC 4.8 toolchain.

## GCC 4.8 toolchain

Source:

```
https://android.googlesource.com/platform/prebuilts/gcc/darwin-x86/arm/arm-linux-androideabi-4.8
```

Recovered revision:

```
264394c
[darwin-x86] ARM toolchain refresh
```

Important:

- Do not modify the recovered GCC repository.
- The original repository history was preserved because later commits removed the binary payloads.
- The checked-out revision contains the compiler binaries and target tools.

Host:

```
darwin-x86_64 (macOS host)
```

Verified compiler:

```
arm-linux-androideabi-gcc-4.8 (GCC) 4.8
```

## Sysroot

The recovered GCC toolchain does not contain a complete Android userspace sysroot. The missing pieces include headers and runtime libraries such as:

```
errno.h
libc.so
libdl.so
Android platform headers
```

Planned location:

```
07_BUILD/toolchains/android-10-sysroot
```

Preferred sources:

1. Historical Android NDK platform sysroot (for example NDK r8e android-10 arch-arm).
2. If unavailable, reconstruct the minimum required headers from matching AOSP historical sources:

```
bionic/libc/include/
bionic/libc/kernel/common/
system/core/include/
```

Target compatibility is Android 2.3.x / ARMv6-era userspace.

## Example build command

After the sysroot is installed:

```bash
export TOOLCHAIN=$PWD/07_BUILD/toolchains/arm-linux-androideabi-4.8
export SYSROOT=$PWD/07_BUILD/toolchains/android-10-sysroot

$TOOLCHAIN/bin/arm-linux-androideabi-gcc \
  --sysroot=$SYSROOT \
  -march=armv6 \
  -mtune=arm1136jf-s \
  -mfloat-abi=soft \
  -I08_MODERNIZATION/ui/uapi \
  -o 08_MODERNIZATION/ui/totoro-native-ui \
  08_MODERNIZATION/ui/totoro-native-ui.c
```

## Current gate

The compiler is verified. The next missing build dependency is the compatible Android sysroot.

Pipeline:

```
sysroot recovery
    ↓
compile native UI
    ↓
scp to Totoro
    ↓
execute on device
    ↓
verify framebuffer
    ↓
verify EVIOCGRAB
    ↓
verify touchscreen
    ↓
record runtime evidence
```
