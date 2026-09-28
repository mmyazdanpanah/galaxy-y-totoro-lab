# Android-Native Linux Chroot Track

## Purpose

This is the practical, low-risk modernization path for the preserved Samsung Galaxy Y GT-S5360 (totoro).

The phone continues to boot its known-good Android 2.3.6 system normally. Linux is introduced as a userspace environment inside Android rather than by replacing the boot image or kernel.

Target:

    stock Android boots normally
          ↓
    root access
          ↓
    ARMv6-compatible static BusyBox
          ↓
    Linux root filesystem
          ↓
    chroot
          ↓
    network + SSH
          ↓
    userspace Linux running

This track is deliberately separate from the native-boot/mainline investigation. Native boot remains preserved as research and may be resumed later.

## Safety model

The first pass is observation-first and read-only until the rooting gate is explicitly cleared.

Do not flash a new boot image, replace the stock kernel, repartition the phone, write PIT, write/erase/format EFS, modify modem/radio partitions, wipe system/userdata, or combine the first userspace experiment with CPU overclocking or AVS changes.

Preserve the known-good Android baseline and recovery path throughout.

## Phase 0 — read-only phone inventory

Run these commands from the Android shell before making changes:

    echo '=== BUILD ==='
    getprop ro.build.display.id
    getprop ro.build.version.release
    getprop ro.product.model
    echo
    echo '=== CPU ==='
    cat /proc/cpuinfo
    echo
    echo '=== KERNEL ==='
    uname -a
    echo
    echo '=== MOUNTS ==='
    mount
    echo
    echo '=== DATA SPACE ==='
    df -h /data
    echo
    echo '=== MTD ==='
    cat /proc/mtd
    echo
    echo '=== FILESYSTEMS ==='
    cat /proc/filesystems
    echo
    echo '=== LOOP ==='
    ls -l /dev/loop* 2>&1 || true
    echo
    echo '=== DEVICES ==='
    cat /proc/devices

Use this output to select the actual storage strategy. Do not assume that /data is ext*, RFS, or another filesystem until the phone reports it.

Also preserve:

    getprop > getprop-before-root.txt
    cat /proc/cmdline > proc-cmdline-before-root.txt

Store the complete Phase 0 output in the experiment record before proceeding.

## Phase 1 — prove ARMv6 userspace before rooting

Use a statically linked BusyBox binary from an ARMv6-capable Alpine armhf package. Do not install it system-wide.

Copy it to a temporary location such as /data/local/tmp/busybox and run it without root:

    /data/local/tmp/busybox --help
    /data/local/tmp/busybox uname -a
    /data/local/tmp/busybox awk 'BEGIN { print 1.5 * 2.5 }'

The floating-point expression is an early compatibility probe. The phone's actual /proc/cpuinfo remains authoritative for VFP/ARM feature reporting.

Record package/version provenance and checksum. If execution fails, diagnose the exact error before rooting.

## Phase 2 — firmware-matched rooting gate

Before applying any root package:

1. read ro.build.display.id from the actual phone;
2. identify the exact firmware/build target expected by the rooting method;
3. verify root package provenance;
4. verify its checksum;
5. keep the stock recovery path available.

Do not use a random historical root ZIP merely because it is labeled for GT-S5360.

Odin is not part of the first pass. It remains a later recovery/firmware tool if a separate experiment requires it.

## Phase 3 — preservation after root

Immediately after successful root access, re-check mount, /proc/mtd, getprop, and /proc/cmdline without modifying them.

For EFS preservation, first use the actual partition/mount information reported by the phone. Do not assume a path such as /efs is correct. Any EFS backup must be read-only, copied to external storage, and hashed.

## Phase 4 — choose the root filesystem location

Preferred path A:

    /data/local/alpine

Fallback path B:

    loop-backed Linux image on removable SD storage

Use the path supported by the Phase 0 filesystem, free-space, permission, and device evidence.

Do not make CWM partitioning or repartitioning part of the main path.

## Phase 5 — unpack a minimal Linux root filesystem

Start with an ARMv6-compatible Alpine armhf minirootfs selected from official Alpine release/download information available at experiment time.

Record the exact release, architecture, checksum, and source. Do not hard-code an unverified future release in this document.

If the selected userspace produces an explicit compatibility failure such as Illegal instruction or Function not implemented, stop and diagnose before evaluating an older release.

The initial rootfs should contain only what is needed for chroot, networking, package management, and SSH.

## Phase 6 — chroot

Bind or mount only the interfaces required by the Linux userspace, initially /dev, /dev/pts, /proc, and /sys as supported by the actual Android 2.6.35 environment.

Keep two small launchers:

- an interactive shell launcher;
- an SSH daemon launcher.

Do not rely on fragile all-purpose Android-shell parameter expansion for both roles.

## Phase 7 — networking and SSH

First prove that the chroot can use the Android-provided network path. Then install the minimum package set required for SSH.

Intended endpoint:

    Mac
      ↓
    ADB port forward, e.g. tcp:2222
      ↓
    sshd in the Linux userspace
      ↓
    chrooted Linux environment

Use SSH keys where practical.

## Completion criterion

This track reaches its first milestone when Android still boots normally, the Linux rootfs can be entered reproducibly with chroot, /proc /sys /dev and required networking are functional, package management works sufficiently, sshd starts reliably, and the Mac can connect.

Milestone name: **userspace Linux running**.

## Separate native-boot track

The Samsung OSS kernel rebuild, boot-image reconstruction, CPUFreq provenance work, UART/SBL research, and possible mainline kernel work remain valid research, but they are not prerequisites for this chroot milestone.

Keeping the tracks separate prevents a userspace experiment from becoming coupled to boot-chain changes.

## Experiment rule

    observe
      ↓
    preserve
      ↓
    smallest reversible step
      ↓
    test
      ↓
    record

If an error appears, stop at the failing boundary and diagnose from actual phone output. Do not guess a storage path, filesystem type, root package, Alpine release, or mount option.