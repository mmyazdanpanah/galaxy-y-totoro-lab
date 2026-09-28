# Galaxy Y Modernization Plan

## Goal

Turn the Samsung Galaxy Y GT-S5360 (totoro) into a useful, lightweight Linux system while preserving a known-good recovery path.

Keep the project simple:

    BACKUP → IMPLEMENTATION → FIX → DONE / RESTORE BACKUP

Do not create complexity before the phone requires it.

## 1. BACKUP

Status: active.

First make the physical phone safely recoverable.

Already preserved:

- stock device identity and firmware state
- Download Mode
- live PIT
- JPLC1 firmware source archive
- historical Totoro boot-image specimen
- kernel/ramdisk evidence
- stock Recovery Mode

Stock Recovery is confirmed working and can:

- browse the SD card
- open folders
- select with the physical Home button
- expose an "apply update from sdcard" path

The SD update mechanism is a possible recovery/experiment path, but we do not need to explore it further unless it helps the backup or implementation.

Backup priority:

1. obtain the simplest reliable backup of the phone's original boot/recovery state;
2. preserve the backup with hashes and provenance;
3. verify that the rollback path is usable;
4. stop backup work once we have a sufficient recovery path.

Do not modify the phone during backup.

No repartition.
No PIT write.
No EFS.
No modem.
No system.
No userdata wipe.

## 1.5. RESEARCH / RECONNAISSANCE

Status: CPUFreq/defconfig archaeology complete; reproducible-build preparation is next.

The 2026-09-29 deep source audit independently corroborated the existing Totoro boot geometry and LZMA path and added a concrete historical BCM21553 driver inventory. A first-party Samsung OSS tree was then verified at `gt-s5360_gb_opensource` / `179772dd`, with five Totoro defconfigs plus `board-totoro.c`, CPU/CP initialization, CPUFreq/CPUidle code, and the historical display/V3D configuration. It also established that current postmarketOS no longer provides ARMv6/armhf package and cross-compiler infrastructure.

Before the first phone boot, perform a Totoro-specific low-risk UART/SBL reconnaissance where physically and electrically safe. The purpose is diagnostic visibility, not bootloader replacement. Use the historical Samsung Broadcom workflow as methodology only: identify UART, capture early output, determine whether boot interruption/environment access exists, and map the kernel-loading path.

The Samsung source baseline remains the primary historical kernel reference. The exact five-defconfig comparison and CPUFreq source-to-binary reconciliation are now complete. The preserved six-state CPUFreq-shaped object is not identical to the public Samsung OSS two-state table, so its provenance should be resolved by reproducible zImage comparison rather than speculative interpretation.

Keep the following historical alternatives documented but inactive unless the primary boot path requires them:

- Watson MTD kernel support for Gingerbread.
- Merruk Totoro build/compression tooling.
- Historical Totoro device-tree material.
- BCM21553 GPU/video driver archaeology (`v3d`, `hx170dec`, `h6270enc`).

Do not infer that any later Samsung Broadcom bootloader technique is directly compatible with Totoro.

## 2. IMPLEMENTATION

After backup is sufficient, make the smallest useful change.

First implementation target:

    Samsung/Totoro boot chain
          ↓
    reproducible Totoro kernel
          ↓
    known-compatible boot structure
          ↓
    controlled boot
          ↓
    diagnostics

Current technical evidence already gives us:

- Samsung BCM21553 Totoro kernel source at `gt-s5360_gb_opensource` / `179772dd`
- `bcm21553_totoro_05_defconfig`
- Totoro-specific `board-totoro.c`, `cpu-bcm21553.c`, `cpufreq_bcm21553.c`, and `cpuidle_bcm21553.c`
- kernel address 0x81608000 (independently corroborated by historical physical-device research)
- base 0x81600000
- page size 4096
- verified historical Totoro boot image
- verified raw-LZMA → newc CPIO ramdisk
- independent historical confirmation that the BCM21553/Totoro kernel uses the Linux LZMA decompressor path
- working historical Totoro ramdisk material

Use existing working material first.

Do not rebuild obsolete infrastructure just because it is interesting.

Do not begin with Alpine, postmarketOS, mainline Linux, new drivers, repartitioning, modem work, or a large patch stack.

The first implementation should change as little as possible.

## 3. FIX

If the implementation does not boot or something is broken:

    observe
      ↓
    identify the actual failure
      ↓
    make the smallest reversible fix
      ↓
    test again

One variable at a time where practical.

Prefer:

    reuse → small fix → proven alternative → new code

Do not invent a larger architecture to solve a small failure.

If the problem cannot be fixed simply, stop and choose the next proven alternative deliberately.

## 4. DONE / RESTORE

If the implementation works:

- verify it
- record the working image/config/hash
- preserve the result
- move to the next small capability

If it fails and cannot be fixed safely:

- restore the known-good backup
- verify the phone returns to baseline
- stop there or choose a new implementation path

A failed experiment is acceptable if the backup works.

## First useful milestone

The first meaningful success is not "modern Linux desktop".

It is:

    kernel boots
        ↓
    init starts
        ↓
    /proc /sys /dev
        ↓
    shell / diagnostics

Then add hardware and userspace incrementally.

## Later, only after the basic loop works

M2 — make it a useful Linux computer:

- storage
- display/framebuffer
- touchscreen/buttons
- USB
- Wi-Fi/networking
- audio
- battery/charging
- suspend/resume
- Bluetooth
- camera
- modem

Camera and modem remain optional.

M3 — make the userspace maintainable and useful.

Userspace note: current postmarketOS has dropped ARMv6/armhf package and cross-compiler support. Do not plan M3 around current postmarketOS binaries. Prefer a reproducible ARMv6-capable userspace assembled and maintained specifically for this project, or another historically compatible Linux userspace.

M4 — optional mainline Linux audit.

These are outcomes, not separate projects.

## Safety boundary

Never use:

    --repartition

Do not write:

    PIT
    EFS
    modem

Do not wipe:

    system
    userdata

unless a later experiment explicitly requires it and the backup/recovery path has already been verified.

## Working rule

At every step ask:

    Can we do this simply?
        ↓ yes
      do it
        ↓ no
    Is there a proven alternative?
        ↓ yes
      use it
        ↓ no
    only then invent something new
