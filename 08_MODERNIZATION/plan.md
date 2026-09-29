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

Before rooting, capture firmware/build properties, complete `/proc/cpuinfo`, kernel version, mounts, `df -h /data`, `/proc/mtd`, `/proc/filesystems`, loop nodes, `/proc/devices`, `getprop`, and `/proc/cmdline`. Preserve the full output.

**Physical handset inventory captured 2026-09-29:** GT-S5360/totoro, Android 2.3.6 `GINGERBREAD.JPLC1`, PDA `S5360JPLC1`, CSC `S5360OJPLC1`, baseband `S5360XXLK3`; ARMv6 6TEJ BCM21553 ThunderbirdEDN31 with VFP/EDSP; Linux `2.6.35.7` (GCC 4.4.3, PREEMPT). Android ABI property is `armeabi`. `/data` is Samsung RFS with 163 MB free at capture; removable SD is VFAT. Loop block major 7 and `/dev/block/loop0`–`loop7` exist, but nodes are root-owned mode 0600 and `/dev/loop*` is absent. `/data/local/tmp` exists and was empty; write/execute permission remains to be tested. Unprivileged `/proc/mtd` showed only a header and `/proc/cmdline` was denied, so these must not be interpreted as absent; inspect read-only after root. Full evidence and caveats are in `android-chroot-linux.md`.

Storage and filesystem route is selected from handset evidence. Do not assume /data filesystem type, EFS path, or loop-device usability.

### A1 — Static ARMv6 userspace test

Before root, run a statically linked BusyBox from `/data/local/tmp`. Verify the candidate's ARM architecture/minimum CPU requirements, static-link status, provenance, and checksum; an `armhf` label alone does not establish ARMv6 compatibility. Minimum checks include BusyBox execution, `uname`, and a small floating-point expression through `awk`. The actual phone `/proc/cpuinfo` remains authoritative for CPU/VFP capability.

### A2 — Firmware-matched root gate

Root only after the phone's actual `ro.build.display.id` is matched to the selected root method/package and the package checksum/provenance is verified.

Stock recovery remains the preferred first recovery environment. Odin is not required for this first pass and remains a later recovery/firmware tool if needed.

### A3 — Preserve after root

Immediately re-check `mount`, `/proc/mtd`, `getprop`, and `/proc/cmdline`.

For EFS, inspect actual partition/mount evidence first. Any EFS backup must be read-only and hashed. Do not write or format EFS.

### A4 — Rootfs location

Preferred candidate: `/data/local/alpine`, subject to RFS behavior, free space, and permissions.

Fallback candidate: loop-backed image on removable SD, subject to root access, device-node permissions, SD capacity, and mount validation.

Do not use CWM partitioning or repartitioning as the main route.

### A5 — ARMv6-compatible rootfs

Select an Alpine rootfs only after confirming the selected release's ARMv6 CPU compatibility from official release/package information. Record exact release, architecture, checksum, and source.

If the selected userspace fails with an explicit compatibility error, stop and diagnose before trying an older release.

### A6 — Chroot

Build the minimum required environment for `/dev`, `/dev/pts`, `/proc`, and `/sys`, adapting commands to the actual Android 2.6.35 environment.

Maintain two small launchers: interactive shell and SSH daemon.

### A7 — Networking + SSH

Use Android's working network path. Install only the packages required for network diagnostics and SSH.

Intended endpoint: Mac → ADB forward → sshd → chroot.

### A8 — Milestone

The active track reaches its first milestone when Android boots normally, the Linux rootfs enters reproducibly, networking works, sshd runs, and the Mac can connect.

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


## Current checkpoint — 2026-09-29

Track A has completed the pre-root observation and preservation gates and has now reached a verified historical root-artifact checkpoint. The exact `update.zip` candidate is:

- 2,260,360 bytes
- MD5 `eac189609fd71de6bf053e7ff2636d7e`
- SHA-1 `89108755e3cf1d6c298e60fc963881dacb3d313d`
- SHA-256 `3e4ebe31b908ea3a8750347f875f91493f550edd1cd2a3006293c45a41592a27`
- ZIP integrity test passed.

Before any recovery installation, perform a complete offline updater-script/update-binary audit and confirm the GT-S5360/JPLC1 assertions. The artifact verification does not by itself authorize flashing.
