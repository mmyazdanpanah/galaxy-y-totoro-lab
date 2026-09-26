# M1 — Minimal Linux Boot Experiment

## Purpose

Prove the smallest possible Linux boot path on the preserved GT-S5360 (totoro) before attempting a modern userspace.

Target:

    Samsung boot chain
        ↓
    Totoro-compatible 2.6.35 kernel
        ↓
    minimal initramfs
        ↓
    /init
        ↓
    shell / diagnostic output

This is a laboratory experiment. It does not require ADB, Treble, GSI, GKI, repartitioning, or a modern Android ROM.

## Evidence that fixes the starting point

### Samsung kernel

The Samsung OSS kernel tree explicitly supports GT-S5360_GB and documents:

    make bcm21553_totoro_05_defconfig
    make

with output:

    arch/arm/boot/zImage

The kernel is Linux 2.6.35.

Historical Totoro kernel work independently uses the same BCM21553/Totoro source family and confirms a reusable custom-kernel build path.

### Boot image format

Historical GT-S5360 work documents the Samsung-specific boot image construction:

    zImage
    + ramdisk.gz
    + base 0x81600000
    + kernelMD5
    → boot.img

This is important: the first experiment should preserve the proven Samsung boot-image format rather than inventing a new loader.

### Live specimen PIT

The preserved live PIT identifies the real device partition as:

    Partition: boot
    Image: boot.img
    Blocks: 20

The PIT is specimen evidence and must remain the authority for any later write operation.

## M1 stages

### M1-A — Reproduce the Samsung kernel build

Use the Samsung OSS gt-s5360_gb_opensource tree and the exact:

    bcm21553_totoro_05_defconfig

Do not modify the configuration initially.

Success:

    zImage exists
    architecture = ARM
    kernel build completes reproducibly

### M1-B — Reproduce the boot image without touching the phone

Use a known Galaxy Y stock boot image only as a packaging reference.

Verify:

- boot header
- kernel offset/base
- ramdisk format
- kernel MD5 requirement
- command-line/board information
- resulting image structure

Do not flash anything during this stage.

### M1-C — Replace only the kernel

Construct:

    stock-compatible ramdisk
        +
    newly built Totoro zImage
        →
    test boot.img

Initially keep the ramdisk unchanged.

This isolates the kernel variable.

Do not introduce Alpine, BusyBox, display changes, or driver changes simultaneously.

### M1-D — First boot test

Only after M1-A through M1-C succeed and the original boot image has been independently preserved should the project perform a controlled boot test.

The first test should change only the boot partition.

No PIT.
No repartition.
No modem.
No EFS.
No system/userdata changes.

The recovery path must be prepared before the first write.

## First Linux milestone

The first useful Linux proof does not need a graphical interface.

Preferred progression:

    kernel boots
        ↓
    init starts
        ↓
    /proc + /sys + /dev available
        ↓
    shell
        ↓
    framebuffer / USB / storage diagnostics

Only after this works should we build the real minimal Linux userspace.

## Toolchain

Historical Samsung documentation specifies CodeSourcery G++ Lite 2009q3-68 for the original tree. Later BCM21553 community work documents ARM EABI 4.6.

Because this is a modern Apple Silicon host, do not modify the kernel source merely to accommodate the host compiler.

Preferred order:

1. reproduce with a contained Linux build environment;
2. use the historically compatible ARM EABI toolchain;
3. if the old toolchain is impractical, use a reproducible container/CI build;
4. patch the kernel only when a build failure demonstrates a real compatibility requirement.

## Safety boundary

M1-A through M1-C are build/inspection work.

M1-D is the first device write and requires an explicit go/no-go decision after the original boot image and rollback path are preserved.

Never use:

    --repartition

for this experiment.

Never touch:

    efs
    modem
    system
    userdata

during M1.

## Decision rule

    Can Samsung kernel build?
          |
        YES
          ↓
    Can boot image be reproduced?
          |
        YES
          ↓
    Can kernel be substituted without changing ramdisk?
          |
        YES
          ↓
    Controlled boot test
          |
       boots? ── NO → diagnose kernel/boot parameters
          |
         YES
          ↓
    minimal initramfs
          ↓
    M2 hardware bring-up

## Sources

- Samsung-OSS-Kernels/android_kernel_samsung_bcm21553
- CyanogenMod-ARMv6/android_kernel_samsung_bcm21553-common
- sonickles9/watson-kernel-totoro
- zecn/eve_kernel
- Historical GT-S5360 boot-image construction documentation
