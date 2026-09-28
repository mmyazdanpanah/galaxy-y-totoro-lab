# M1 — Minimal Linux Boot Experiment

## Purpose

M1 is the implementation stage of the simple project loop:

    BACKUP → IMPLEMENTATION → FIX → DONE / RESTORE BACKUP

The goal is only to prove a small, reproducible Totoro Linux boot path. Do not solve the whole phone at once.

## Current gate status

M1 is **not yet cleared for the first phone boot**.

The research/preparation work is now close to the implementation boundary. A first-party Samsung OSS source baseline has been verified, and the CPUFreq/defconfig archaeology step is complete enough to proceed to reproducible-build work.

The remaining gates are:

1. reproducible Samsung kernel build;
2. direct zImage comparison against the preserved kernel;
3. exact-enough boot-image reconstruction;
4. offline unpack/repack verification;
5. rollback/recovery verification.

The preserved six-entry CPUFreq-shaped object remains an important provenance clue, but it is not identical to the two-state CPUFreq table in the public Samsung OSS source. No voltage/frequency modification should be made for the first boot.

## Current evidence

### Kernel

Samsung OSS explicitly supports GT-S5360_GB:

    make bcm21553_totoro_05_defconfig
    make

Output:

    arch/arm/boot/zImage

Samsung's public kernel repository documents the GT-S5360 build target, the historical CodeSourcery ARM EABI toolchain path, and the expected `arch/arm/boot/zImage` output. The source branch used for this baseline is `gt-s5360_gb_opensource`, HEAD `179772dd`.

### Samsung OSS source baseline

The Samsung BCM21553 OSS tree explicitly contains five Totoro configurations: `bcm21553_totoro_02B0_defconfig`, `02B1`, `03`, `04`, and `05`, plus `board-totoro.c`, `cpu-bcm21553.c`, `cpufreq_bcm21553.c`, and `cpuidle_bcm21553.c`.

The `05` defconfig enables historical Broadcom framebuffer/display and multimedia paths including `CONFIG_FB_BCM`, `CONFIG_FB_BCM_215XX`, `CONFIG_BCM_DSS`, `CONFIG_BCM215XX_DSS`, and `CONFIG_BRCM_V3D`. It also enables Broadcom MMC support and legacy flash/filesystem support. The board source explicitly documents SDIO/eMMC/SD controller roles and the OneNAND/SDHC2 pin-mux constraint.

This source is the primary historical reference for the rebuild. It does not prove modern mainline support or the exact stock bootloader behavior.

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

### CPUFreq / AVS source-to-binary result

The public Samsung OSS source defines an 8-byte `{cpu_freq MHz, cpu_voltage uV}` record and a two-state platform table:

    312 MHz / 1200000 uV
    832 MHz / 1360000 uV

AVS can alter the voltages of those two states.

The preserved kernel instead contains the previously identified six-record sequence:

    156   1160000
    312   1200000
    468   1200000
    624   1220000
    832   1300000
    1124  1320000

The record shape is compatible, but the contents are not the public Samsung OSS table. The six-state object's provenance must therefore be established by zImage comparison; `1124` must not be assumed to be a stock MHz value.

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

The original stock Samsung mkbootimg command, exact stock cmdline, and exact stock board field are not claimed. The Samsung source baseline strengthens the `0x81600000` / `0x81608000` reconstruction but does not close the packaging question.

## Next implementation experiment

The next task is a controlled source-to-binary build experiment rather than additional broad archaeology:

    Samsung OSS 179772dd
          +
    bcm21553_totoro_05_defconfig
          +
    historical toolchain
          ↓
    reproducible zImage
          ↓
    compare with preserved kernel
          ↓
    identify CPUFreq/config provenance
          ↓
    reconstruct boot image offline

Do not modify CPUFreq or AVS values during this build.

## Implementation sequence

The first hardware candidate should be deliberately conservative:

    known Totoro boot structure
          +
    reproducible Samsung zImage
          ↓
    test boot image
          ↓
    offline unpack/recheck
          ↓
    rollback verification
          ↓
    controlled phone test

Do **not** combine the first boot with:

- CPU overclocking
- AVS/voltage changes
- repartitioning
- modem changes
- EFS changes
- a new userspace
- a large kernel patch stack

The first successful boot should prove only that the reconstructed boot path works.

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

The first phone experiment must remain reversible.

## External historical evidence

The Watson Totoro kernel repository documents a BCM21553 Totoro kernel with Gingerbread/ICS/KitKat ramdisk variants and an experimental DVFS-disabled configuration. It is historical comparison material, not the baseline for the first experiment. citeturn0search0
