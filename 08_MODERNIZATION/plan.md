# Galaxy Y Modernization Plan

## Goal

Make the Samsung Galaxy Y GT-S5360 (totoro) useful as a lightweight Linux computer while preserving a known-good Android recovery path.

The project now uses two independent implementation tracks. The first practical track does not replace the Android boot chain.

    PRESERVE
       ↓
    ANDROID-NATIVE LINUX USERSPACE  ← active
       ↓
    userspace Linux running

    NATIVE LINUX BOOT              ← research / deferred
       ↓
    reproducible kernel → boot image → controlled boot

Keeping these paths separate lets us obtain a useful Linux environment without coupling the first experiment to kernel, bootloader, or repartitioning work.

## Track A — Android-native Linux userspace (active)

### A0 — Read-only inventory

Before rooting, capture ro.build.display.id, Android release/model, complete /proc/cpuinfo, uname -a, mount, df -h /data, /proc/mtd, /proc/filesystems, /dev/loop*, /proc/devices, getprop, and /proc/cmdline.

Storage and filesystem route is selected from this evidence. Do not assume /data filesystem type, EFS path, or loop-device availability.

### A1 — Static ARMv6 userspace test

Before root, run a statically linked ARMv6-capable BusyBox from /data/local/tmp.

Minimum checks include BusyBox execution, uname, and a small floating-point expression through awk. The actual phone /proc/cpuinfo remains authoritative for CPU/VFP capability.

Record package/version provenance and checksum.

### A2 — Firmware-matched root gate

Root only after the phone's actual ro.build.display.id is matched to the selected root method/package and the package checksum/provenance is verified.

Stock recovery remains the preferred first recovery environment. Odin is not required for this first pass and remains a later recovery/firmware tool if needed.

### A3 — Preserve after root

Immediately re-check mount, /proc/mtd, getprop, and /proc/cmdline.

For EFS, inspect actual partition/mount evidence first. Any EFS backup must be read-only and hashed. Do not write or format EFS.

### A4 — Rootfs location

Preferred: /data/local/alpine

Fallback: loop-backed image on removable SD.

Do not use CWM partitioning or repartitioning as the main route.

### A5 — ARMv6-compatible rootfs

Select the Alpine ARMv6-compatible release from official Alpine release/download information at experiment time. Record exact release, architecture, checksum, and source.

If the selected userspace fails with an explicit compatibility error, stop and diagnose before trying an older release.

### A6 — Chroot

Build the minimum required environment for /dev, /dev/pts, /proc, and /sys, adapting commands to the actual Android 2.6.35 environment.

Maintain two small launchers: interactive shell and SSH daemon.

### A7 — Networking + SSH

Use Android's working network path. Install only the packages required for network diagnostics and SSH.

Intended endpoint: Mac → ADB forward → sshd → chroot.

### A8 — Milestone

The active track is complete at its first milestone when Android boots normally, the Linux rootfs enters reproducibly, networking works, sshd runs, and the Mac can connect.

Milestone name: **userspace Linux running**.

## Track B — Native Linux boot (research / deferred)

Preserved and valid:

- Samsung OSS gt-s5360_gb_opensource source baseline;
- bcm21553_totoro_05_defconfig build path;
- CPUFreq/AVS source-to-binary reconciliation;
- exact five-defconfig comparison;
- Totoro boot geometry and historical boot-image specimen;
- UART/SBL reconnaissance methodology;
- later mainline/hardware-enablement research.

Track B must not block Track A.

## Safety boundary

Never use repartitioning as a first solution.

Do not write PIT, EFS, or modem. Do not wipe system or userdata.

Do not combine the first experiment with CPU overclocking, AVS/voltage changes, kernel replacement, bootloader replacement, boot-image flashing, or a large patch stack.

## Working rule

    observe
      ↓
    preserve
      ↓
    smallest reversible step
      ↓
    test
      ↓
    record

If a command fails, stop at the failing boundary and diagnose from actual phone output. Do not guess storage paths, filesystem types, root packages, Alpine versions, or mount options.