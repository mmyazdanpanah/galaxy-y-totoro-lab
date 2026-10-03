# Totoro Connection Stack

## Milestone: USB ADB → Local Terminal → Wi-Fi SSH

Date: 2026-10-03

## Achievement

Totoro (Samsung Galaxy Y GT-S5360, Android 2.3.6) now has a multi-layer control architecture.

The device was successfully moved from a USB-only development workflow into a network-controlled embedded device workflow while preserving USB ADB as the recovery channel.

## Verified Hardware/Software State

- Device: Samsung Galaxy Y GT-S5360 (Totoro)
- Android: 2.3.6
- Kernel: Linux 2.6.35.7
- Architecture: ARMv6
- Root: Superuser verified

## Control Layers

### Layer 1 — USB ADB (Recovery Channel)

Status: VERIFIED

Purpose:
- Initial development access
- APK installation
- Emergency recovery
- Low-level debugging

Rule:

Do not replace or destabilize this channel until alternatives are proven.

## Layer 2 — Local Terminal

Application:

- Android Terminal Emulator (jackpal.androidterm)

Status: VERIFIED

Installation:

```
adb install Term.apk
```

Notes:

The Galaxy Y required selecting the Android 2.3 compatible input method:

```
Input method → Word-based
```

Terminal type:

```
xterm
```

Root verification:

```
su
id
```

Expected:

```
uid=0(root)
```

## Layer 3 — Wi-Fi SSH Remote Control

Application:

- SSHDroid 2.1.2

Status: VERIFIED

Connection:

```
ssh totoro
```

Mac SSH configuration:

```
Host totoro
    HostName 172.30.39.16
    User root
    KexAlgorithms +diffie-hellman-group1-sha1
    HostKeyAlgorithms +ssh-rsa
    PubkeyAcceptedAlgorithms +ssh-rsa
```

Reason:

Modern OpenSSH disables old SSH algorithms required by the Gingerbread-era SSH server.

Verified remote shell:

```
whoami
root

id
uid=0(root) gid=0(root)

uname -a
Linux localhost 2.6.35.7 #1 PREEMPT Fri Mar 16 15:40:13 KST 2012 armv6l GNU/Linux
```

## Current Architecture

```
                 Wi-Fi
Mac  <======================>  Totoro
 |                              |
 | SSH root shell               | SSHDroid
 |                              |
 | USB ADB -------------------- |
 | recovery/development         |
 |
 | Android Terminal Emulator
 | local rescue shell
```

## Next Steps

1. Configure SSHDroid persistence after reboot.
2. Change default SSH password.
3. Stabilize Totoro network identity (hostname/static IP).
4. Install BusyBox toolkit.
5. Continue Linux userspace and pocket-PC development.

## Engineering Principle

USB ADB is now the bootstrap and recovery path.

Wi-Fi SSH is the daily management path.

The local terminal is the physical fallback path.
