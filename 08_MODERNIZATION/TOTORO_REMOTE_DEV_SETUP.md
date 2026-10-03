# Totoro Remote Development Setup

## Purpose

This document records the current remote development access path for the Samsung Galaxy Y GT-S5360 (Totoro).

The goal is to avoid dependence on the USB cable for daily development while preserving USB ADB as the emergency recovery channel.

## Current Architecture

```
Mac
 |
 | Wi-Fi
 v
Totoro Wi-Fi Hotspot
 |
 v
SSHDroid (root shell)
 |
 v
Android 2.3.6 / Linux 2.6.35.7 ARMv6
```

Recovery path:

```
Mac
 |
 | USB
 v
ADB
 |
 v
Totoro shell/root
```

## Verified Components

- Samsung Galaxy Y GT-S5360 (totoro)
- Android 2.3.6
- Linux kernel 2.6.35.7
- Root access through Superuser
- SSHDroid installed
- Android Terminal Emulator installed
- Wi-Fi SSH root login verified
- USB ADB recovery access verified

## Totoro Developer Baseline v0.1

A reproducible system capture has been created:

```
99_SANDBOX/00_LOGS/totoro-developer-baseline-v0.1/
```

Captured information includes:

- kernel information
- CPU information
- memory state
- Android properties
- filesystem mounts
- storage state
- framebuffer information
- graphics devices
- input devices
- running processes

Hardware discovery highlights:

```
Kernel:
Linux 2.6.35.7 armv6l

Framebuffer:
0 LCDfb
```

The device exposes a framebuffer driver for future native UI investigation.

Input devices discovered:

```
event0 accelerometer_sensor
event1 proximity_sensor
event2 magnetic_sensor
event3 sec_keypad
event4 sec_touchscreen
event5 max8986_ponkey
event6 bcm_headset
```

The touchscreen input path is available through the Linux input subsystem.

## SSH Connection

Modern macOS OpenSSH requires enabling legacy algorithms because SSHDroid uses older SSH implementations.

Recommended Mac SSH config:

```
Host totoro
    HostName 172.30.39.16
    User root
    KexAlgorithms +diffie-hellman-group1-sha1
    HostKeyAlgorithms +ssh-rsa
    PubkeyAcceptedAlgorithms +ssh-rsa
```

Connect:

```
ssh totoro
```

## USB ADB Recovery

USB ADB remains the safest recovery mechanism.

Check connection:

```
adb devices -l
```

Open shell:

```
adb shell
```

## Operational Rule

Do not remove the USB recovery path until SSH startup persistence and recovery procedures are verified.

Daily workflow:

1. Connect to Totoro over SSH.
2. Use USB ADB only for recovery, installation, or debugging when SSH is unavailable.
3. Record experiments before changing system-critical components.

## Next Steps

- Configure SSHDroid startup persistence.
- Create Totoro management scripts.
- Build totoro-tools diagnostic toolkit.
- Probe framebuffer metadata through /dev/fb0.
- Continue native UI/framebuffer development.
