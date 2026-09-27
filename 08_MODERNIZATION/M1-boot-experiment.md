# M1 — Minimal Linux Boot Experiment

## Purpose

M1 is the implementation stage of the simple project loop:

    BACKUP → IMPLEMENTATION → FIX → DONE / RESTORE BACKUP

The goal is only to prove a small, reproducible Totoro Linux boot path. Do not solve the whole phone at once.

## Current evidence

### Kernel

Samsung OSS explicitly supports GT-S5360_GB:

    make bcm21553_totoro_05_defconfig
    make

Output:

    arch/arm/boot/zImage

### Verified real Totoro boot image

A real historical GT-S5360 boot image was recovered from the community repository thedeadfish59/cmx11-totoro and preserved locally as:

    /Volumes/TotoroBuild/totoro-real-boot.img

This is community evidence, not stock Samsung firmware evidence.

Decoded header:

    magic           ANDROID!
    kernel_size     0x002b2788 = 2,828,168
    kernel_addr     0x81608000
    ramdisk_size    0x0020b295 = 2,142,869
    ramdisk_addr    0x82600000
    second_size     0
    second_addr     0x82500000
    tags_addr       0x81600100
    page_size       0x1000 = 4096
    dt_size         0
    cmdline         empty
    board           empty

Derived:

    base            0x81600000
    kernel_offset   0x00008000
    ramdisk_offset  0x00A00000
    tags_offset     0x00000100
    second_offset   0x00900000

Payloads were checked against the header:

    kernel payload offset    0x1000
    ramdisk payload offset   0x2b4000
    second offset            0x4c0000
    actual image size        0x4c0000

Kernel:

    size: 2,828,168 bytes
    SHA-256:
    251531c44c2763f2b58d36b02941d3b202a3987800a66f7990dabc9d6e59aefd

### Ramdisk

The real image uses:

    boot.img
      ↓
    kernel + raw LZMA ramdisk
      ↓
    LZMA
      ↓
    SVR4/newc CPIO
      ↓
    Totoro/CM files

Ramdisk:

    compressed size       2,142,869 bytes
    SHA-256               1a2c4711ebaa185b549c1fe61b43cf0c0d4570173eab389c531656cb801da7e4
    decompressed size     6,060,544 bytes
    CPIO magic            070701

The ramdisk contains explicit GT-S5360/Totoro files including:

    init.cm.rc
    init.gt-s5360board.rc
    init.gt-s5360board.*
    fstab.gt-s5360board
    ueventd.gt-s5360board.rc
    recovery.rc
    adbd
    busybox

Its historical Android storage map includes:

    /system   → mtdblock8
    /cache    → mtdblock9
    /data     → mtdblock10
    sdcard    → bcm_sdhc.3/mmc1
    /sd-ext   → mmcblk0p2
    /boot     → mtd
    swap      → zram0

The real ramdisk is not byte-identical to Watson GB. Watson remains useful as a historical Totoro packaging/ramdisk reference; do not substitute it blindly.

### Boot geometry

Independent evidence converges:

    Samsung OSS:
    CONFIG_SDRAM_BASE_ADDR = 0x81600000
    zreladdr = 0x81608000

    CM9 Totoro:
    BOARD_KERNEL_BASE = 0x81600000
    BOARD_KERNEL_PAGESIZE = 4096
    BOARD_PAGE_SIZE = 0x1000

    real boot image:
    kernel_addr = 0x81608000
    page_size = 4096

The original stock Samsung mkbootimg command, exact stock cmdline, and exact stock board field are not claimed.

### Stock Recovery

The physical GT-S5360 has now been confirmed in stock Recovery Mode.

Observed:

    Android system recovery
    reboot system now
    apply update from sdcard
    wipe data/factory reset

Confirmed experimentally:

- SD card is visible
- folders can be opened
- the physical Home button selects an item
- the SD update path is available

Historical research indicates that Galaxy Y stock recovery was used with update.zip and with temporary CWM ZIP packages. This is useful as a possible recovery/implementation route, but it is not required for the current plan.

Do not use wipe data/factory reset.

## Implementation

Keep the first implementation small:

    known Totoro boot structure
          +
    reproducible Samsung zImage
          ↓
    test boot image
          ↓
    controlled phone test

Before writing the phone:

1. complete the required backup;
2. build the kernel;
3. construct the boot image offline;
4. unpack/recheck the constructed image;
5. confirm the rollback path.

Do not change unrelated variables.

Do not begin with a new Linux userspace.

## Fix

If the phone does not boot or a component fails:

    observe
      ↓
    diagnose
      ↓
    smallest reversible fix
      ↓
    retest

If the fix becomes complicated, stop and use a proven alternative rather than building a new system around the problem.

## Done / Restore

Success:

    kernel → init → diagnostics

Then preserve the working image and continue incrementally.

Failure:

    restore known-good backup
      ↓
    verify baseline
      ↓
    stop or choose another proven path

## Safety

No repartition.
No PIT write.
No EFS.
No modem.
No system write.
No userdata wipe.

The first phone experiment should remain reversible.

## Sources

- Samsung-OSS-Kernels/android_kernel_samsung_bcm21553
- percy-g2/android_device_totoro
- sonickles9/watson-kernel-totoro
- thedeadfish59/cmx11-totoro
- AndroidARMv6/CM11
- historical GT-S5360 community boot-image work
