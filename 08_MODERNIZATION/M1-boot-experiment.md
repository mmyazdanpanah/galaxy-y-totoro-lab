# M1 — Minimal Linux Boot Experiment

## Purpose

Prove the smallest reliable Linux boot path on the preserved GT-S5360 (totoro) before building a modern userspace.

Target:

    Samsung boot chain
        ↓
    Totoro-compatible 2.6.35 kernel
        ↓
    known Totoro ramdisk
        ↓
    /init
        ↓
    shell / diagnostics

The project does not need to reproduce the original Samsung boot image byte-for-byte. It needs a verified, Totoro-compatible boot path with the fewest new variables.

## Current evidence

### Kernel

Samsung OSS explicitly supports GT-S5360_GB:

    make bcm21553_totoro_05_defconfig
    make

Output:

    arch/arm/boot/zImage

### Boot geometry

CM9 Totoro BoardConfig.mk explicitly records:

    BOARD_KERNEL_BASE := 0x81600000
    BOARD_KERNEL_PAGESIZE := 4096
    BOARD_PAGE_SIZE := 0x00001000
    BOARD_KERNEL_CMDLINE :=

This agrees with independent kernel evidence:

    CONFIG_SDRAM_BASE_ADDR = 0x81600000
    zreladdr = SDRAM_BASE + 0x8000

Therefore 0x81600000 and 4096-byte pages are verified historical Totoro values.

Still unknown:

- kernel offset
- ramdisk offset
- tags offset
- exact historical mkbootimg invocation
- exact header from a preserved compatible image

Do not guess these values.

### Watson reuse path

Watson GB contains:

- a Totoro-specific Gingerbread ramdisk
- AIK unpack/repack tools
- a documented workflow that starts from a real Totoro boot.img, replaces zImage and ramdisk, then repacks

Watson's Git repository does not preserve the referenced boot.img because image files are ignored. Therefore Watson is a packaging/ramdisk reference, not the missing stock image.

### JPLC1 firmware

The preserved JPLC1 source archive is authoritative preservation material, but its inner HOME TAR contains no boot.img.

Do not continue searching that package for boot.img.

## M1 stages

### M1-A — Kernel build

Use:

    samsung-bcm21553
    gt-s5360_gb_opensource
    bcm21553_totoro_05_defconfig

Preferred order:

1. historical Linux ARM EABI 4.4.3 in a contained Linux environment;
2. if that host setup becomes the bottleneck, test a reproducible modern ARM cross-compiler;
3. patch source only for demonstrated compatibility failures.

Record:

    source commit
    config
    compiler/toolchain
    zImage size
    zImage SHA-256

### M1-B — Boot-image reconstruction

Do this entirely off-device.

Fastest reliable sequence:

1. inspect historical Totoro build files for the complete mkbootimg invocation;
2. search for an actual compatible Totoro boot image;
3. if an image is found, unpack it with Watson AIK and record every header parameter;
4. if no image is recoverable, construct from the CM9 Totoro configuration plus Watson's packaging workflow;
5. unpack the constructed image again and verify header, kernel, ramdisk and offsets.

A compatible historical community image is acceptable as a packaging reference. It must be clearly labeled community evidence, not stock Samsung evidence.

Success:

    verified boot image
    known base/pagesize
    known offsets
    known ramdisk
    reproducible repack

No phone.

### M1-C — Kernel-only substitution

Take the verified Totoro boot-image structure and change only:

    original/reference zImage
            ↓
    reproducible Samsung zImage

Keep:

    ramdisk
    base
    pagesize
    offsets
    cmdline
    board fields

unchanged unless evidence shows they must change.

Then unpack the result and compare the structure with the reference.

No phone.

### M1-D — Controlled boot

This is the first point where the phone is required.

Before any write:

1. preserve the original boot material or establish a reliable rollback source;
2. verify the test image offline;
3. prepare the recovery path;
4. explicitly decide whether the experiment is worth the device risk.

First target:

    bootloader
        ↓
    kernel
        ↓
    /init
        ↓
    diagnostic output

No PIT.
No repartition.
No EFS.
No modem.
No system.
No userdata.

## First Linux milestone

Do not start with Alpine.

First prove:

    kernel boots
        ↓
    init starts
        ↓
    /proc + /sys + /dev
        ↓
    shell
        ↓
    storage / framebuffer / USB diagnostics

Then introduce the minimal Linux userspace.

## Toolchain

Historical Samsung documentation specifies CodeSourcery G++ Lite 2009q3-68. The Android prebuilt repository provides ARM EABI 4.4.3 Linux-hosted binaries.

The exact old host environment is useful for reproducibility but must not become the project's main bottleneck.

Preferred strategy:

    historical toolchain + contained Linux
                  ↓
             if blocked
                  ↓
    modern reproducible cross-build
                  ↓
             source patch only if required

## Safety boundary

M1-A through M1-C are inspection/build work.

M1-D is the first device write and requires an explicit go/no-go decision.

Never use:

    --repartition

Never touch during M1:

    efs
    modem
    system
    userdata

## Decision rule

    Can the kernel build?
          ↓ yes
    Can a compatible Totoro boot structure be verified?
          ↓ yes
    Can only the kernel be substituted?
          ↓ yes
    Controlled boot test
          ↓
    boots?
      no → diagnose
      yes
       ↓
    minimal Linux userspace
       ↓
    M2 hardware bring-up

## Sources

- Samsung-OSS-Kernels/android_kernel_samsung_bcm21553
- percy-g2/android_device_totoro
- sonickles9/watson-kernel-totoro
- AndroidARMv6/CM11
- historical GT-S5360 community boot-image work
