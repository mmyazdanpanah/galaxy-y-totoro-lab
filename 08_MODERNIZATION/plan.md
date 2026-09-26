# Galaxy Y Modernization Plan

## Objective

Turn the Samsung Galaxy Y GT-S5360 (totoro) into a useful, reproducible lightweight Linux system while preserving the original specimen.

Primary success target: M3 — Modern Totoro.

The rule is:

    reuse → adapt → small reversible fix → new code

Do not modernize the phone by replacing working parts unnecessarily.

## Route

    preserved GT-S5360
          ↓
    reproduce Samsung kernel
          ↓
    reuse proven Totoro boot/ramdisk work
          ↓
    controlled Linux boot
          ↓
    minimal Linux computer
          ↓
    Alpine / postmarketOS-style userspace
          ↓
    useful hardware + lightweight interface
          ↓
    mainline audit
          ↓
    selective upstreaming, only if useful

A fully mainline kernel is an optional later research goal, not the first target.

## M0 — Preserve

Status: complete.

The stock specimen, Download Mode, PIT, partition evidence, firmware identity, and preservation boundary are frozen.

No firmware, bootloader, recovery, repartitioning, or EFS operation has been performed.

## M1 — Reproduce and boot

Status: M1-A preparation / build environment. Phone not required yet.

### M1-A — Reproduce the Samsung kernel

Use:

- Samsung OSS android_kernel_samsung_bcm21553
- branch gt-s5360_gb_opensource
- bcm21553_totoro_05_defconfig
- historical ARM EABI 4.4.3 / CodeSourcery-compatible environment

The exact historical Android prebuilt repository contains Darwin and Linux ARM EABI 4.4.3 toolchains. The Darwin compiler is i386 and cannot execute on current macOS; use a contained Linux environment for the Linux-hosted toolchain rather than modifying the kernel to fit a modern compiler.

Success:

    arch/arm/boot/zImage
    ARM architecture
    reproducible build record + hash

### M1-B — Reconstruct a known Totoro boot image

Do not invent the first boot format.

Reuse historical Totoro material, especially:

- known working ramdisks
- Watson kernel/ramdisk material
- existing boot-image tools and documentation
- independent Totoro kernel/recovery projects

Verify the actual header, offsets, ramdisk, command line and any checksum/MD5 requirements before constructing an image.

The previously assumed zImage + ramdisk.gz + base + kernelMD5 recipe is historical evidence, not yet a fully verified project recipe.

No phone.

### M1-C — Kernel-only substitution

Start with a known-compatible Totoro ramdisk and replace only the kernel.

Goal:

    known ramdisk + reproducible zImage → test boot.img

Do not introduce Alpine, new drivers, repartitioning, or other variables simultaneously.

No phone.

### M1-D — Controlled boot

Phone required.

Only after:

1. original boot image is preserved,
2. rollback is prepared,
3. boot-image construction is verified,
4. the test image changes only what is necessary.

M1 safety boundary:

- no PIT write
- no repartition
- no EFS
- no modem
- no system
- no userdata

First proof:

    bootloader → kernel → init → diagnostics/shell

## M2 — Linux computer

Once the kernel boots, bring up only what is needed for a useful computer:

1. CPU / RAM
2. storage
3. USB
4. framebuffer / display
5. touchscreen / buttons
6. networking
7. Wi-Fi
8. audio
9. battery / charging
10. suspend / resume
11. Bluetooth
12. camera
13. modem

Camera and modem are optional for M2.

Use existing Samsung/community drivers before writing new ones.

## M3 — Modern Totoro

Replace the prototype userspace with a small maintainable Linux userspace:

- Alpine Linux as the lightweight base
- postmarketOS infrastructure where it reduces device-specific work
- existing Totoro kernel/drivers initially
- BusyBox and standard Linux tools
- SSH/network tools
- lightweight framebuffer-oriented interface

Avoid modern desktop environments, heavy browsers, large background services, and unnecessary graphics stacks.

M3 is the primary project success condition.

## M4 — Mainline audit

Only after M3 works, compare each subsystem with upstream Linux.

Ask:

- what already works upstream?
- what is missing?
- can an existing Totoro/BCM21553 driver be adapted?
- is the upstream route simpler to maintain?
- does it provide a real capability or maintenance benefit?

A hybrid kernel is a valid result.

## Reuse-first evidence map

The historical Totoro ecosystem is now treated as a parts library:

- Samsung OSS kernel → baseline and build reference
- Watson kernel → boot/ramdisk and packaging reference
- Eve kernel → alternative kernel lineage and toolchain reference
- CM9/Android device trees → board/device configuration reference
- AndroidARMv6/CM11 → later hardware/userspace evidence
- Samsung/community vendor trees → hardware integration reference
- XDA/YouTube → procedural and historical evidence, not authoritative source code

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

## Decision rule

    Existing working Totoro solution?
             ↓ yes
          reuse it
             ↓ no
       small reversible fix?
          ↓ yes       ↓ no
         fix      proven alternative
                       ↓
                    new code

At every step:

1. reuse existing work;
2. make the smallest reversible change;
3. test;
4. record the result;
5. only then expand scope.
