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


## Root artifact checkpoint — 2026-09-29

The historical root package candidate has been acquired and hash-verified before any handset modification:

    update.zip
    size:   2,260,360 bytes
    MD5:    eac189609fd71de6bf053e7ff2636d7e
    SHA-1:  89108755e3cf1d6c298e60fc963881dacb3d313d
    SHA256: 3e4ebe31b908ea3a8750347f875f91493f550edd1cd2a3006293c45a41592a27

The ZIP passes `unzip -t`. Its payload contains `system/xbin/su`, `system/app/Superuser.apk`, `system/xbin/busybox`, `system/xbin/ssh`, and `system/xbin/sqlite3`, plus the Android update metadata and signing files.

The archive is compatible by historical target lineage with GT-S5360 Android 2.3.x, while exact JPLC1 package compatibility still requires inspection of the updater assertions and operations. Do not flash this package yet. The required next step is an offline audit of `updater-script` and `update-binary` for model assertions and to rule out formatting, repartitioning, boot/recovery, modem, or other unintended writes.

The separately tested official BusyBox 1.21.1 ARMv6 candidate remains the pre-root compatibility reference; the root ZIP's bundled BusyBox is not substituted for that provenance-controlled test.


## Root-package evidence gate — 2026-09-29

The historical `update.zip` candidate has now completed the offline updater-script/payload audit and a direct read-only destination check on the physical JPLC1 handset. The artifact remains hash-verified and passes ZIP integrity testing.

The updater script explicitly accepts `GT-S5360`. Active operations are system extraction plus 04755 permissions on `sqlite3`, `su`, `ssh`, and `busybox`, followed by unmounting `/system`. The format/mount lines are commented out. No active raw-image, program-execution, modem, boot/recovery, EFS, repartitioning, or partition-formatting operation was identified in the script. The bundled updater binary has broader recovery/BML capabilities, so the package remains subject to the firmware-matched gate.

Payload ELF inspection found static ARM EABI4 BusyBox and ARM EABI5 dynamic `ssh`, `sqlite3`, and `su` with interpreter `/system/bin/linker`. This is consistent at the ELF-header level with the ARMv6-era platform but is not execution proof.

The package metadata is not exact JPLC1 evidence: `pre-device=tass` and `post-build=google/passion/passion:2.3.3/GRI40/102588:user/release-keys`. The generic Android signing certificate is not treated as provenance proof.

Direct read-only checks from the interactive shell returned `No such file or directory` for all five package destination paths: `/system/xbin/su`, `/system/xbin/busybox`, `/system/xbin/ssh`, `/system/xbin/sqlite3`, and `/system/app/Superuser.apk`. No handset write has occurred.

**Disposition: do not flash yet.** The artifact audit is complete, but the final firmware-matched root gate remains open. Obtain and verify the exact stock JPLC1 firmware/recovery environment before any recovery installation attempt. Full evidence is recorded in `10_RESEARCH/root-package-evidence-gate-2026-09-29.md`.


## Post-root preservation checkpoint — 2026-09-29

Root is now verified on the physical JPLC1 handset and the required post-root read-only inventory is complete.

### Root verification

- Model: GT-S5360
- Build: `GINGERBREAD.JPLC1`
- ADB device: `0123456789ABCDEF device`
- `su -c id`: `uid=0(root) gid=0(root)`

### Direct JPLC1 storage evidence

`/proc/cmdline` reports the active `bcm_umi-nand` layout:

    bcm_boot       256 KiB
    loke         2,048 KiB
    loke_bk      2,048 KiB
    systemdata     256 KiB
    modem        12,800 KiB
    param_lfs     5,120 KiB
    boot          5,120 KiB
    boot_backup   5,120 KiB
    system      235,520 KiB
    cache        40,960 KiB
    userdata    201,984 KiB
    efs             256 KiB
    sysparm_dep     256 KiB
    umts_cal        256 KiB
    cal           1,024 KiB

`/proc/partitions` independently reports the corresponding BML devices:

    bml1 256
    bml2 2048
    bml3 2048
    bml4 256
    bml5 12800
    bml6 5120
    bml7 5120
    bml8 5120
    bml9 235520
    bml10 40960
    bml11 201984
    bml12 256
    bml13 256
    bml14 256
    bml15 1024

Live mounts confirm:

    /dev/stl9  -> /system      RFS  RO
    /dev/stl10 -> /cache       RFS  RW
    /dev/stl11 -> /data        RFS  RW
    /dev/stl6  -> /mnt/.lfs    j4fs RW
    mmcblk0p1  -> /mnt/sdcard  VFAT RW

The complete raw command outputs are preserved in `01_PRESERVATION/evidence/post-root-jplc1-device-inventory-2026-09-29.md`.

### Consequence for the chroot track

The active userspace-Linux experiment now has direct storage evidence from the actual rooted phone. In particular, `/data` is Samsung RFS rather than ext*, while the removable SD is VFAT. Rootfs placement and mount strategy must therefore be selected from observed permissions/capabilities rather than assumed Linux filesystem behavior.

The next gate is capability evaluation: verify the available static ARMv6 userspace tooling, confirm safe rootfs placement options, and only then build the smallest reversible chroot experiment.

Do not modify EFS, repartition, replace the boot/recovery images, or write BML/STL partitions as part of this userspace track.


## Chroot capability checkpoint — 2026-09-29

A direct test on the rooted physical JPLC1 handset has now confirmed basic chroot execution.

The temporary root tree was created with BusyBox under /data/local/tmp/totoro-chroot, and the known-good BusyBox v1.17.2 binary was copied to /bin/busybox. The test command returned:

    CHROOT_EXEC_OK
    uid=0 gid=0
    chroot_status=0

Therefore the current Android 2.6.35.7 environment supports the basic chroot operation required by this track. This is a direct runtime result, not an inference from BusyBox applet availability.

The Android standalone mkdir command previously reported Read-only file system; BusyBox mkdir subsequently succeeded. BusyBox cp also succeeded where standalone cp was unavailable. These command-environment differences are recorded to avoid misclassifying them as filesystem limitations.

BusyBox v1.17.2 exposes the required chroot, losetup, mount, umount, pivot_root, and filesystem/networking applets. Their presence does not yet prove loop or network functionality.

### Current capability state

- Root: confirmed.
- /data/local/tmp write/read: confirmed.
- SD write/read: confirmed.
- Shell-script execution under /data/local/tmp: confirmed.
- Static BusyBox execution: confirmed for installed BusyBox.
- Basic chroot: CONFIRMED.
- Loop-device attachment/mount: not yet tested.
- Network route/connectivity: not yet confirmed.
- /proc/self/ns: absent in current environment.
- Native/mainline boot: separate deferred research track.

The temporary test tree is intentionally retained until the next capability test. No mount or partition operation has been performed.

### Next gate

Test loop-device functionality using the smallest reversible operation possible. Do not alter /system, /data, EFS, BML/STL, PIT, boot, recovery, modem, or partition layout.
