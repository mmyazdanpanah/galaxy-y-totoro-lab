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

Expected shell:

```
# whoami
root

# id
uid=0(root) gid=0(root)
```

## USB ADB Recovery

USB ADB remains the safest recovery mechanism.

Check connection:

```
adb devices -l
```

Expected:

```
0123456789ABCDEF device usb:...
```

Open shell:

```
adb shell
```

Root test:

```
su
id
```

Expected:

```
uid=0(root) gid=0(root)
```

## Operational Rule

Do not remove the USB recovery path until SSH startup persistence and recovery procedures are verified.

Daily workflow:

1. Connect to Totoro over SSH.
2. Use USB ADB only for recovery, installation, or debugging when SSH is unavailable.
3. Record experiments before changing system-critical components.

## Next Steps

- Configure SSHDroid startup persistence.
- Create Totoro management scripts on Mac.
- Stabilize Wi-Fi addressing.
- Return to native UI/framebuffer development.
