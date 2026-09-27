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

### Verified real Totoro boot image

A real historical GT-S5360 boot image was recovered from the community repository `thedeadfish59/cmx11-totoro` and preserved locally as:

    /Volumes/TotoroBuild/totoro-real-boot.img

This is community evidence, not stock Samsung firmware evidence.

The image was independently identified as:

    Android bootimg
    kernel
    ramdisk
    page size: 4096

Its Android boot header was decoded directly from the preserved image:

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

Derived boot geometry:

    base            0x81600000
    pagesize        0x1000
    kernel_offset   0x00008000
    ramdisk_offset  0x00A00000
    tags_offset     0x00000100
    second_offset   0x00900000

The payload alignment was also checked against the header:

    kernel payload offset    0x1000
    ramdisk payload offset   0x2b4000
    second payload offset    0x4c0000
    expected image end       0x4c0000
    actual image size        0x4c0000

The extracted kernel is a valid ARM Linux zImage:

    size: 2,828,168 bytes
    SHA-256:
    251531c44c2763f2b58d36b02941d3b202a3987800a66f7990dabc9d6e59aefd

The image contains no preserved cmdline or board string in its header. A simple string scan of the extracted kernel did not recover a useful Linux version/compiler identity, so the exact kernel source lineage is not claimed from the binary alone.

### Verified ramdisk extraction chain

The real boot image was manually extracted using the verified header offsets:

    boot.img
        ↓
    kernel payload
        ↓
    raw LZMA ramdisk
        ↓
    LZMA decompression
        ↓
    SVR4/newc CPIO archive
        ↓
    Totoro/CM ramdisk files

Ramdisk:

    compressed size       2,142,869 bytes
    SHA-256               1a2c4711ebaa185b549c1fe61b43cf0c0d4570173eab389c531656cb801da7e4
    decompressed size     6,060,544 bytes
    CPIO magic            070701

This proves the historical image uses a raw LZMA-compressed newc CPIO ramdisk rather than an assumed format.

### CM/Totoro ramdisk findings

The real ramdisk contains explicit GT-S5360/Totoro board integration, including:

    init.cm.rc
    init.gt-s5360board.rc
    init.gt-s5360board.*
    fstab.gt-s5360board
    ueventd.gt-s5360board.rc
    recovery.rc
    adbd
    busybox

Its `default.prop` contains:

    ro.secure=0
    ro.allow.mock.location=1
    ro.debuggable=1
    persist.sys.usb.config=adb

`init.cm.rc` explicitly identifies CyanogenMod-derived behavior, including:

    import /init.superuser.rc
    /system/etc/terminfo
    /system/bin/sysinit
    /cache/dalvik-cache
    /data/.ssh
    interactive/ondemand CPU governor controls
    ADB-over-network property handling

The real `fstab.gt-s5360board` records the historical Android storage topology:

    /dev/block/mtdblock8   → /system   yaffs2
    /dev/block/mtdblock9   → /cache    yaffs2
    /dev/block/mtdblock10  → /data     yaffs2
    bcm_sdhc.3/mmc1       → sdcard
    /dev/block/mmcblk0p2  → /sd-ext   ext4 (recoveryonly)
    boot                  → /boot     mtd
    /dev/block/zram0      → swap

This is direct evidence from a real Totoro boot image, not an inferred layout.

The real ramdisk is not byte-identical to Watson's Gingerbread ramdisk. In particular, Watson contains a broader set of board-specific init fragments, while the recovered real image contains its own smaller board-init set and its own `fstab.gt-s5360board`.

Therefore:

    real ramdisk ≠ Watson GB ramdisk byte-for-byte

Watson remains a valuable historical Totoro packaging and ramdisk reference, but its ramdisk must not be substituted blindly.

### Boot geometry convergence

Three independent evidence paths converge on the same Totoro boot geometry.

Samsung OSS:

    CONFIG_SDRAM_BASE_ADDR = 0x81600000
    zreladdr-y = SDRAM_BASE_ADDR + 0x8000
    zreladdr = 0x81608000

CM9 Totoro BoardConfig.mk:

    BOARD_KERNEL_BASE := 0x81600000
    BOARD_KERNEL_PAGESIZE := 4096
    BOARD_PAGE_SIZE := 0x00001000

Real historical boot image:

    kernel_addr = 0x81608000
    pagesize    = 4096

Watson also independently contains:

    zreladdr-y := $(CONFIG_SDRAM_BASE_ADDR)+0x8000

This is now verified historical Totoro evidence rather than a guessed mkbootimg configuration.

Still not claimed from the sources:

- an original stock Samsung mkbootimg command
- an exact stock Samsung cmdline
- an exact stock Samsung board field
- the exact source lineage of the recovered community kernel

### Broader research cross-check

A separate research pass independently points to the same reusable Totoro ecosystem: Samsung BCM21553 kernel sources, Watson packaging work, CM9/CM11 device trees, and AndroidARMv6 hardware support.

These sources are useful as research leads for M2/M3, but they remain secondary to evidence reproduced in this project.

The research also reinforces the current strategy of treating postmarketOS/other modern userspaces as later alternatives rather than assuming an existing ready-made Totoro Linux port.

For M2, preserve hardware/driver leads as a source inventory first; do not promote unverified hardware claims into project facts until they are tied to source code, device-tree/config evidence, or direct testing.

### Watson reuse path

Watson GB contains:

- a Totoro-specific Gingerbread ramdisk
- AIK unpack/repack tools
- a documented workflow that starts from a real Totoro boot.img, replaces zImage and ramdisk, then repacks

Watson's Git repository does not preserve the referenced boot.img because image files are ignored. Therefore Watson is a packaging/ramdisk reference, not the missing stock image.

Its AIK wrapper was tested against the recovered real boot image, but the bundled magic/script combination rejected the image as an unsupported format. The bundled `unpackbootimg` binaries are Linux i386 ELF executables, so the failure is tooling/host related and does not invalidate the recovered image.

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

Current evidence changes the shortest reliable sequence:

1. preserve the recovered real Totoro boot image as the packaging specimen;
2. record its verified header geometry and raw-LZMA→CPIO ramdisk chain;
3. use Samsung/CM/Watson evidence to validate the kernel geometry;
4. reproduce the boot-image structure offline;
5. unpack the constructed image again and verify header, kernel, ramdisk and offsets.

Do not reconstruct stock Samsung boot.img byte-for-byte unless necessary.

Do not guess missing Android boot parameters.

A compatible historical community image is acceptable as a packaging reference. It must be clearly labeled community evidence, not stock Samsung evidence.

Success:

    verified boot image
    known base/pagesize
    known offsets
    known ramdisk format
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
- thedeadfish59/cmx11-totoro (community boot-image specimen)
- AndroidARMv6/CM11
- historical GT-S5360 community boot-image work
