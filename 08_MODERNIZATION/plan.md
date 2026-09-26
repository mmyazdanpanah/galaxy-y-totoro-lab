# Galaxy Y Modernization Plan

## Objective

Turn the Samsung Galaxy Y GT-S5360 (totoro) into a useful, reproducible lightweight Linux system while preserving the original specimen.

Primary success target: M3 — Modern Totoro.

Working rule:

    reuse → verify → adapt → small reversible fix → new code

Optimize for the shortest reliable path. Do not spend time reconstructing obsolete infrastructure when a documented, compatible Totoro path already exists.

## Route

    preserved GT-S5360
          ↓
    reproduce/use proven Totoro kernel
          ↓
    reuse proven Totoro boot + ramdisk structure
          ↓
    controlled Linux boot
          ↓
    minimal Linux userspace
          ↓
    hardware bring-up
          ↓
    Modern Totoro
          ↓
    optional mainline audit

Mainline Linux is an optional later research goal, not the first implementation target.

## M0 — Preserve

Status: complete.

The stock specimen, Download Mode, PIT, partition evidence, firmware identity, and preservation boundary are frozen.

No firmware, bootloader, recovery, repartitioning, or EFS operation has been performed.

## M1 — Reproduce and boot

Status: active.

Phone is not required until M1-D.

### M1-A — Reproduce the Samsung kernel

Primary source:

- Samsung-OSS-Kernels/android_kernel_samsung_bcm21553
- branch gt-s5360_gb_opensource
- bcm21553_totoro_05_defconfig

Build with the historical ARM EABI 4.4.3-compatible environment.

Preferred execution order:

1. contained Linux environment with the historical Linux-hosted toolchain;
2. if that becomes disproportionately difficult, test a reproducible modern ARM cross-toolchain build;
3. patch source only when a concrete build error requires it.

Do not spend the project budget on reproducing the exact 2009 host environment if a simpler reproducible build produces a functionally equivalent kernel.

Success:

    arch/arm/boot/zImage
    ARM architecture
    recorded toolchain + source commit + config + hash

### M1-B — Reconstruct a known Totoro boot image

Do not guess Android boot parameters.

Current evidence converges on:

    BOARD_KERNEL_BASE   = 0x81600000
    BOARD_KERNEL_PAGESIZE = 4096
    BOARD_PAGE_SIZE     = 0x1000
    board                = totoro
    kernel cmdline       = empty in the CM9 BoardConfig

Independent evidence:

- Samsung kernel source: SDRAM base 0x81600000
- Watson kernel source: zreladdr = SDRAM base + 0x8000
- CM9 Totoro BoardConfig: base 0x81600000 and 4096-byte pages
- Watson GB: real Totoro boot-image unpack/repack workflow and Gingerbread ramdisk

The remaining boot-header offsets must come from an actual compatible image or a historical build invocation. Do not substitute generic Android defaults merely because they are common.

Fastest reliable evidence path:

1. inspect historical Totoro build configuration;
2. locate an actual compatible Totoro boot image if one is publicly recoverable;
3. extract its header/offsets with Watson AIK;
4. otherwise construct the image from the documented CM9 parameters and verify the result structurally before any device use.

The JPLC1 Samsung HOME package is not a boot-image source: the preserved package contains no boot.img. Do not spend further time searching that package for one.

No phone.

### M1-C — Kernel-only substitution

Use a known-compatible Totoro Gingerbread ramdisk, preferably Watson GB material, and replace only the kernel.

Goal:

    known Totoro ramdisk + reproducible zImage
        ↓
    test boot.img

Keep every other boot-image parameter unchanged from the verified reference.

No Alpine, new drivers, repartitioning, modem work, or unrelated kernel changes.

No phone.

### M1-D — Controlled boot

Phone required.

Only after:

1. original boot material is preserved or independently recoverable;
2. boot-image construction is verified;
3. rollback/recovery is prepared;
4. the test image differs only in the intended kernel variable.

First proof:

    bootloader → kernel → init → diagnostics

M1 safety boundary:

- no PIT write
- no repartition
- no EFS
- no modem
- no system
- no userdata

## M2 — Minimal Linux computer

Once the kernel boots, bring up hardware in dependency order:

1. CPU / RAM
2. init / proc / sys / dev
3. storage
4. framebuffer / display
5. touchscreen / buttons
6. USB
7. Wi-Fi / networking
8. audio
9. battery / charging
10. suspend / resume
11. Bluetooth
12. camera
13. modem

Camera and modem are optional.

Reuse existing Samsung/Watson/AndroidARMv6 drivers before writing new ones.

## M3 — Modern Totoro

Replace the Android userspace with a small maintainable Linux userspace.

Initial direction:

- BusyBox
- standard Linux tools
- SSH/network tools
- lightweight framebuffer-oriented interface
- Alpine userspace components where compatible
- postmarketOS ideas/infrastructure only where they reduce device-specific work

Do not make postmarketOS itself a hard dependency: the official Totoro port is not a booting solution and the old ARMv6 target is outside current postmarketOS support.

Avoid heavy desktop environments, large browsers, and unnecessary background services.

## M4 — Mainline audit

Only after M3 works, audit each subsystem against upstream Linux.

A hybrid kernel is a valid result. Mainline is valuable only when it provides a real maintenance or hardware benefit.

## Reuse-first evidence map

- Samsung OSS kernel → baseline and build reference
- Watson → proven Totoro GB boot/ramdisk and packaging workflow
- CM9 Totoro device tree → concrete historical boot configuration
- AndroidARMv6/CM11 → later hardware/userspace evidence
- other Totoro kernels/vendor trees → alternative implementation evidence
- XDA/YouTube → procedural/historical evidence only

See 10_RESEARCH/totoro-reuse-map.md.

## Explicitly out of scope initially

- Treble / GSI / DSU
- GKI
- forcing a modern Android release
- modern LineageOS as the first route
- bootloader replacement without demonstrated need
- repartitioning
- camera-first development
- modem-first development
- rewriting the entire kernel
- large experimental patch stacks
- reconstructing the original stock boot.img when a compatible historical Totoro boot path is sufficient

## Decision rule

    Existing working Totoro solution?
             ↓ yes
           reuse
             ↓ no
    small reversible fix?
        ↓ yes       ↓ no
       fix      proven alternative
                     ↓
                  adapt
                     ↓
                new code

At every step:

1. use the strongest existing evidence;
2. choose the simplest compatible path;
3. test one variable at a time;
4. record the result;
5. only then expand scope.
