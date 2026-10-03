# Totoro Developer Baseline v0.1

Date: 2026-10-04

## Purpose

This baseline captures the first reproducible hardware and software state of the Samsung Galaxy Y GT-S5360 development environment.

It establishes the transition between remote development infrastructure and native UI experimentation.

## Capture Method

Primary development path:

```
Mac -> Totoro Wi-Fi hotspot -> SSHDroid -> root shell
```

USB ADB remains preserved as the emergency recovery path.

No firmware changes, boot changes, kernel changes, or ADB changes were performed.

## Captured Data

- uname information
- CPU information
- memory information
- Android properties
- mount state
- storage state
- framebuffer information
- graphics devices
- input devices
- running processes
- kernel filesystem information

## Hardware Baseline

Device:

Samsung Galaxy Y GT-S5360

Platform:

Broadcom BCM21553

Architecture:

ARMv6

Kernel:

Linux 2.6.35.7 armv6l

Android:

2.3.6 Gingerbread

## Display Discovery

Framebuffer discovery:

```
0 LCDfb
```

The device exposes an LCD framebuffer driver.

Next investigation target:

```
/dev/fb0
```

## Input Discovery

Detected input devices:

```
event0 accelerometer_sensor
event1 proximity_sensor
event2 magnetic_sensor
event3 sec_keypad
event4 sec_touchscreen
event5 max8986_ponkey
event6 bcm_headset
```

The touchscreen is available through the Linux input subsystem.

## Native UI Direction

Target architecture:

```
Application
     |
Minimal UI Toolkit
     |
Framebuffer Renderer
     |
/dev/fb0
     |
LCDfb
     |
Galaxy Y Display
```

## Next Engineering Tasks

1. Create totoro-tools diagnostic toolkit.
2. Add framebuffer ioctl probe.
3. Add input event monitor.
4. Investigate framebuffer ownership.
5. Begin minimal native renderer experiments.
