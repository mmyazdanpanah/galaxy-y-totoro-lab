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
    ls -l /dev/loop* /dev/block/loop* 2>&1
    echo
    echo '=== DEVICES ==='
    cat /proc/devices

Also capture, where readable:

    getprop > getprop-before-root.txt
    cat /proc/cmdline > proc-cmdline-before-root.txt
    cat /proc/version

Store the complete Phase 0 output in the experiment record before proceeding. Use the output to select the actual storage strategy. Do not assume that /data is ext*, RFS, or another filesystem until the phone reports it.

### Confirmed handset inventory — 2026-09-29

The read-only inventory was captured from the physical phone through an interactive ADB shell:

- Model/product: GT-S5360; board/device: totoro.
- Android: 2.3.6, build display ID `GINGERBREAD.JPLC1`.
- PDA: `S5360JPLC1`; CSC: `S5360OJPLC1`; baseband: `S5360XXLK3`.
- Build fingerprint: `samsung/GT-S5360/GT-S5360:2.3.6/GINGERBREAD/JPLC1:user/release-keys`.
- CPU: ARMv6-compatible processor rev 5 (v6l), architecture 6TEJ, BCM21553 ThunderbirdEDN31.
- CPU features reported: `swp half thumb fastmult vfp edsp java`; VFP is present.
- Kernel: `2.6.35.7`, GCC 4.4.3, build `#1 PREEMPT`, dated Fri Mar 16 15:40:13 KST 2012. This was read from `/proc/version`; `uname -a` returned permission denied for the shell user.
- Android ABI property: `armeabi`. This is the reported Android application ABI and does not negate the independently observed VFP CPU feature. Actual executable compatibility must be tested.
- `/data`: `/dev/stl11`, mounted as Samsung RFS, read-write, with 189 MB total and 163 MB free at capture.
- `/system`: `/dev/stl9`, RFS, read-only. `/cache`: `/dev/stl10`, RFS, read-write.
- Removable SD: `/dev/block/vold/179:1`, VFAT, mounted at `/mnt/sdcard`.
- `/proc/filesystems` lists ext2, ext3, cramfs, vfat, jffs2, yaffs/yaffs2, RFS, and j4fs among supported filesystems.
- `/proc/devices` lists loop block major 7, as well as MTD, BML, STL, MMC, and device-mapper block majors.
- Device nodes `/dev/block/loop0` through `/dev/block/loop7` exist, owned by root:root with mode 0600. No `/dev/loop*` nodes were listed. Their presence confirms device nodes, not that the unprivileged shell can use loop devices.
- `/data/local/tmp` exists and was empty at inspection. It is the candidate location for the temporary pre-root BusyBox test; write/execute access still needs to be tested explicitly.
- The unprivileged `cat /proc/mtd` output contained only the header, and `cat /proc/cmdline` returned permission denied. Do not infer that MTD is absent: the observed Android mounts use Samsung STL devices, and privileged inspection is still pending.
- ADB device serial `0123456789ABCDEF` was listed as `device`; interactive `adb shell` succeeded with UID/GID 2000 (`shell`).

The handset reported `ro.secure=1`, `ro.debuggable=0`, and build type `user` with `release-keys`; treat it as a production build, not a permissive engineering build.

### Interpretation and constraints

The evidence confirms ARMv6 plus VFP, but does not itself prove that a particular armhf binary will execute. Verify the selected BusyBox's architecture, minimum CPU requirements, static-link status, provenance, and checksum, then test it on-device before rooting.

RFS on `/data` and 163 MB free make a large rootfs there uncertain. Keep `/data/local/alpine` as a candidate only after checking write/execute behavior and realistic size. The kernel exposes loop support and root-owned loop nodes under `/dev/block`, but loop-backed storage remains conditional on post-root permissions, available SD capacity, and safe mount behavior. Do not create a loop image or repartition at this stage.

The first attempted inventory ran on the Mac and returned Darwin-side errors; it was not phone evidence. Subsequent inventory was run inside the Android shell. Some attempts to use `adb shell 'command'` returned host-side `adb: permission denied`; direct interactive `adb shell` subsequently worked. For repeatability, use the interactive shell unless a specific command form is verified.

## Phase 1 — prove ARMv6 userspace before rooting

Use a statically linked BusyBox binary whose build is verified as ARMv6-compatible. Do not install it system-wide.

Before transfer, record package source, exact package/version, architecture, static-link evidence, and SHA-256. Then use `/data/local/tmp/busybox` and run it without root:

    /data/local/tmp/busybox --help
    /data/local/tmp/busybox uname -a
    /data/local/tmp/busybox awk 'BEGIN { print 1.5 * 2.5 }'

First verify that `/data/local/tmp` is writable and executable by the shell user. The floating-point expression is an early compatibility probe; the phone's actual `/proc/cpuinfo` remains authoritative for CPU/VFP feature reporting. The reported Android ABI `armeabi` is not by itself a test of a candidate binary's requirements.

Record exact binary checksum and test output. If execution fails, diagnose the exact error before rooting. Do not infer compatibility solely from an `armhf` package label.

## Phase 2 — firmware-matched rooting gate

Before applying any root package:

1. read `ro.build.display.id` from the actual phone;
2. identify the exact firmware/build target expected by the rooting method;
3. verify root package provenance;
4. verify its checksum;
5. keep the stock recovery path available.

Current observed build gate: `GINGERBREAD.JPLC1` / PDA `S5360JPLC1`. Do not use a random historical root ZIP merely because it is labeled for GT-S5360.

Odin is not part of the first pass. It remains a later recovery/firmware tool if a separate experiment requires it.

## Phase 3 — preservation after root

Immediately after successful root access, re-check `mount`, `/proc/mtd`, `getprop`, and `/proc/cmdline` without modifying them.

For EFS preservation, first use the actual partition/mount information reported by the phone. Do not assume a path such as `/efs` is correct. Any EFS backup must be read-only, copied to external storage, and hashed.

## Phase 4 — choose the root filesystem location

Preferred candidate path A:

    /data/local/alpine

Fallback candidate path B:

    loop-backed Linux image on removable SD storage

Use the path supported by the actual filesystem, free-space, permission, and device evidence. The current inventory identifies RFS on `/data`, VFAT on the removable SD, and root-owned loop nodes under `/dev/block`; it does not yet establish that either route is usable for mounting a Linux rootfs.

Do not make CWM partitioning or repartitioning part of the main path.

## Phase 5 — unpack a minimal Linux root filesystem

Start with an ARMv6-compatible Alpine minirootfs only after confirming the selected release's CPU compatibility. Select it from official Alpine release/download information available at experiment time.

Record the exact release, architecture, checksum, and source. Do not hard-code an unverified future release in this document. If the selected userspace produces an explicit compatibility failure such as Illegal instruction or Function not implemented, stop and diagnose before evaluating an older release.

The initial rootfs should contain only what is needed for chroot, networking, package management, and SSH.

## Phase 6 — chroot

Bind or mount only the interfaces required by the Linux userspace, initially `/dev`, `/dev/pts`, `/proc`, and `/sys` as supported by the actual Android 2.6.35 environment.

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

This track reaches its first milestone when Android still boots normally, the Linux rootfs can be entered reproducibly with chroot, `/proc`, `/sys`, `/dev` and required networking are functional, package management works sufficiently, sshd starts reliably, and the Mac can connect.

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
