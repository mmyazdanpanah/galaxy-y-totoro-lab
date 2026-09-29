# Status

Phase: M0 complete; M1 preparation — Android-native userspace Linux reconnaissance

Latest handset evidence: 2026-09-29. The physical GT-S5360 was inspected through an interactive ADB shell. The active practical track remains Android-native Linux userspace; the Samsung OSS Totoro source baseline, CPUFreq/defconfig reconciliation, boot-image geometry, and external historical-source audit remain preserved as the separate native-boot research track.

The practical modernization path is split into two deliberately independent tracks:

1. **Android-native userspace Linux (active):** keep stock Android/kernel, introduce a minimal ARMv6-compatible Linux userspace through chroot, and reach it over SSH from the Mac.
2. **Native Linux boot (research/deferred):** reproducible Samsung kernel, boot-image reconstruction, UART/SBL work, and eventual mainline/hardware enablement.

## Active track — Android-native userspace Linux

Target:

    stock Android boots normally
          ↓
    read-only inventory
          ↓
    static ARMv6 BusyBox compatibility test
          ↓
    firmware-matched root gate
          ↓
    preservation / EFS evidence
          ↓
    Alpine-compatible ARMv6 rootfs
          ↓
    chroot
          ↓
    networking + sshd
          ↓
    userspace Linux running

### Current gates

1. **Phase 0 inventory captured from the actual phone.** Firmware, CPU, kernel, mounts, storage capacity, filesystem support, device nodes, and Android properties are recorded in `08_MODERNIZATION/android-chroot-linux.md`.
2. Static BusyBox ARMv6 compatibility must be tested before rooting. Confirm `/data/local/tmp` write/execute access and verify candidate architecture, CPU requirements, static-link status, provenance, and checksum first.
3. Root package must be matched to the phone's actual `ro.build.display.id` and checksum-verified.
4. Post-root preservation must inspect `/proc/mtd`, `mount`, `getprop`, and `/proc/cmdline` without modifying EFS.
5. Rootfs location is evidence-driven: `/data/local/alpine` is only a candidate because `/data` is RFS and had 163 MB free at inventory; a loop-backed SD image is a conditional fallback because loop nodes are root-owned and not exposed as `/dev/loop*`.
6. Alpine release is selected from official release information at experiment time and recorded with checksum and ARMv6 compatibility evidence.
7. Chroot and SSH are built incrementally, with separate interactive and SSH launch scripts.

### Confirmed physical phone inventory — 2026-09-29

- Model/product GT-S5360; board/device `totoro`; platform `bcm21553`.
- Android 2.3.6, build `GINGERBREAD.JPLC1`; PDA `S5360JPLC1`; CSC `S5360OJPLC1`; baseband `S5360XXLK3`.
- CPU ARMv6-compatible rev 5 (v6l), architecture 6TEJ, BCM21553 ThunderbirdEDN31; features include VFP and EDSP.
- Kernel `2.6.35.7`, GCC 4.4.3, `#1 PREEMPT`, dated Fri Mar 16 15:40:13 KST 2012, confirmed via `/proc/version`. `uname -a` was permission denied to the shell user.
- Android ABI property is `armeabi`; candidate binary compatibility still requires direct validation.
- ADB device was listed as `0123456789ABCDEF device`; interactive shell identity was UID/GID 2000 (`shell`).
- `/data` is `/dev/stl11` Samsung RFS, 189 MB total / 163 MB free at capture. `/system` and `/cache` are also RFS. Removable SD is VFAT at `/mnt/sdcard`.
- `/proc/filesystems` lists ext2/ext3 and other filesystems including RFS and j4fs.
- Loop block major 7 is listed in `/proc/devices`; `/dev/block/loop0`–`loop7` exist with root:root mode 0600, while `/dev/loop*` is absent. This proves device nodes exist, not that the shell can access them.
- `/data/local/tmp` exists and was empty. Write/execute access is not yet tested.
- Unprivileged `/proc/mtd` showed only its header and `/proc/cmdline` was permission denied; neither result establishes absence of flash partitions or boot arguments. Recheck after root, read-only.
- Build properties indicate `ro.secure=1`, `ro.debuggable=0`, build type `user`, and `release-keys`.

No phone write operation has been performed. Rooting has not started. The initial inventory attempts run on the Mac are not handset evidence; the accepted inventory came from the interactive Android ADB shell. A later batch of quoted `adb shell '...'` calls returned host-side `adb: permission denied`, while direct interactive `adb shell` worked; use the interactive shell for repeatable capture until that discrepancy is explained.

## Preserved phone state

- Android 2.3.6
- PDA: S5360JPLC1
- CSC: S5360OJPLC1
- Current baseband: S5360XXLK3
- Build: GINGERBREAD.JPLC1
- Kernel: 2.6.35.7 / dpi@DELL161 #1
- Download Mode: Samsung Official
- Custom binary count: 0
- Live PIT SHA-256: 06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538

No firmware, bootloader, recovery, repartitioning, modem, system, userdata, or EFS write has been performed.

## Native-boot research checkpoint

The preserved decompressed Totoro kernel contains a six-entry CPUFreq-shaped object at 0xc0813fb8:

    156   1160000
    312   1200000
    468   1200000
    624   1220000
    832   1300000
    1124  1320000

Public Samsung OSS source defines the same 8-byte record shape but only two states: 312 MHz / 1.20 V and 832 MHz / 1.36 V. Therefore the preserved six-entry object is not the public Samsung OSS table; its provenance remains unresolved pending reproducible zImage comparison. The 1124 field must not be interpreted as MHz without proof.

The five Samsung Totoro defconfigs and their exact adjacent differences remain documented in `10_RESEARCH/cpufreq-defconfig-reconciliation-2026-09-29.md`.

## Native-boot safety boundary

The native-boot track remains **not ready for flashing or first hardware boot**. No CPUFreq/AVS modification, repartitioning, PIT write, modem work, EFS write, or bootloader replacement is authorized by the current plan.

## First practical hardware milestone

For the active chroot track, the first meaningful milestone is:

**userspace Linux running** — Android remains the booting system, a Linux rootfs enters reproducibly through chroot, networking works, and SSH access from the Mac is established.

This is intentionally lower-risk than replacing the Android boot path and can be evaluated independently of the native-boot research.

## Native-boot milestone estimate

The earlier estimate remains valid for the separate native-boot track: research/preparation is roughly 70–80% complete for a reversible diagnostic boot experiment, with reproducible build, zImage provenance, boot-image reconstruction, offline validation, and rollback verification remaining.

For the active chroot path, broad reconnaissance is now complete. The remaining preparation is the verified static BusyBox selection/test, followed by the firmware-matched root decision and preservation gate. A reliable userspace Linux milestone still depends on these tests and subsequent chroot/network/SSH integration.

## Next action

Verify `/data/local/tmp` write/execute access without root; inspect and verify an ARMv6-compatible statically linked BusyBox candidate (architecture, CPU requirements, provenance, checksum); then run the harmless pre-root compatibility checks. Do not root until the binary test and root-package provenance are reviewed.


## Root package acquisition checkpoint — 2026-09-29

The exact historical Galaxy Y root artifact has now been acquired and independently verified offline before any handset write:

- file: `update.zip`
- size: 2,260,360 bytes
- MD5: `eac189609fd71de6bf053e7ff2636d7e`
- SHA-1: `89108755e3cf1d6c298e60fc963881dacb3d313d`
- SHA-256: `3e4ebe31b908ea3a8750347f875f91493f550edd1cd2a3006293c45a41592a27`
- ZIP integrity test: passed
- archive contents: 11 entries including `system/xbin/su`, `system/app/Superuser.apk`, static BusyBox, SSH, sqlite3, updater-script, update-binary, and Android signing metadata.
- archive entry timestamps are consistent with the 2011 historical lineage; the acquisition mirror is not itself treated as proof of original provenance.

This is an **artifact-verification checkpoint, not a flashing approval**. The next gate is offline inspection of `META-INF/com/google/android/updater-script` and `update-binary`, including device assertions and a complete write/format/partition-operation audit. No phone write has occurred.

The verified artifact, extracted updater binary, and pre-root baseline are being organized under the repository's preservation/research work areas. Raw research binaries remain work artifacts unless explicitly committed.


## Root-package evidence gate — 2026-09-29

The offline audit of the historical `update.zip` candidate is now complete. The exact artifact remains hash-verified (2,260,360 bytes; MD5 `eac189609fd71de6bf053e7ff2636d7e`; SHA-1 `89108755e3cf1d6c298e60fc963881dacb3d313d`; SHA-256 `3e4ebe31b908ea3a8750347f875f91493f550edd1cd2a3006293c45a41592a27`) and passes ZIP integrity testing.

The active updater script explicitly accepts `GT-S5360`. Its active operations are limited to extracting the package's `system` tree, assigning 04755 permissions to `sqlite3`, `su`, `ssh`, and `busybox`, then unmounting `/system`. The script's format/mount lines are commented out. No active raw-image, program-execution, modem, boot, recovery, EFS, repartitioning, or partition-formatting operation was identified. The bundled updater binary nevertheless has broader recovery/BML capabilities; this is recorded separately and is not treated as proof that those capabilities are invoked by the current script.

Offline payload inspection reports ARM EABI4 static BusyBox and ARM EABI5 dynamically linked `ssh`, `sqlite3`, and `su` using `/system/bin/linker`. These headers are not obviously incompatible with the ARMv6 handset, but they are not a substitute for execution testing.

The package metadata is not JPLC1-specific: `pre-device=tass` and `post-build=google/passion/passion:2.3.3/GRI40/102588:user/release-keys`. Its generic Android signing certificate is likewise not independent provenance proof.

A direct read-only interactive ADB check found all five package destination paths absent on the current handset: `/system/xbin/su`, `/system/xbin/busybox`, `/system/xbin/ssh`, `/system/xbin/sqlite3`, and `/system/app/Superuser.apk`. No phone write has occurred.

**Root-package disposition: do not flash yet.** The package has passed the offline artifact audit but has not passed the final firmware-matched root gate. The remaining safe preparation step is to obtain and independently verify the exact stock JPLC1 firmware/recovery environment and compare its recovery/update assumptions with the actual handset. Detailed evidence is recorded in `10_RESEARCH/root-package-evidence-gate-2026-09-29.md`.


## Root installation readiness checkpoint — 2026-09-29

The final pre-install checks have now passed on the user's local workstation.

- Git root was verified as the exact project repository: `/Users/mostafa/Workspace/03_Projects/Engineering/galaxy-y-totoro-lab`.
- Branch is `main`, tracking `origin/main`.
- No tracked local changes are present. The four untracked paths remain intentional research/preservation work areas and were not altered:
  - `01_PRESERVATION/evidence/pre-root-baseline/`
  - `10_RESEARCH/work/TotoroBuild.sparseimage`
  - `10_RESEARCH/work/root-research/`
  - `10_RESEARCH/work/samsung-bcm21553/`
- The SD-card copy of `update.zip` was re-hashed immediately before installation preparation.
- SD-card package SHA-256: `3e4ebe31b908ea3a8750347f875f91493f550edd1cd2a3006293c45a41592a27`.
- Package size: 2,260,360 bytes.

The package therefore remains byte-identical to the offline-verified artifact. The planned handset operation is limited to applying this historical update package from stock recovery. No Odin firmware flash, PIT/repartition operation, boot/recovery replacement, modem change, CSC change, EFS write, userdata wipe, or format is part of this step.

The recovery/restore assessment remains conservative: exact JPLC1 recovery-binary identity has not been proven, and a complete JPLC1 firmware restore package has not been established. However, the actual JPLC1 handset mount topology matches the preserved stock Totoro recovery layout (`/system -> /dev/stl9`, `/cache -> /dev/stl10`, `/data -> /dev/stl11`, `/mnt/.lfs -> /dev/stl6`), while the verified updater-script performs only a system-tree extraction, four 04755 permission assignments, and a system unmount. This is sufficient for the narrowly scoped root experiment, but not evidence of a complete device-recovery guarantee.

**Current state: ready for the controlled stock-recovery installation step; phone write has not yet been performed.**


## Root installation success checkpoint — 2026-09-29

The controlled stock-recovery root installation has now completed successfully on the actual GT-S5360 JPLC1 handset.

Recovery reported:

    installing su and Superuser.apk
    installing OK
    by yodeput
    Install from sdcard complete

Post-reboot ADB verification is conclusive:

- Model: GT-S5360
- Build: GINGERBREAD.JPLC1
- ADB device: `0123456789ABCDEF device`
- `adb shell su -c id`: `uid=0(root) gid=0(root)`
- `/system/xbin/su`: present, root:root, mode 04755
- `/system/xbin/busybox`: present, root:root, mode 04755
- `/system/xbin/ssh`: present, root:root, mode 04755
- `/system/xbin/sqlite3`: present, root:root, mode 04755
- `/system/app/Superuser.apk`: present, root:root, mode 0644

This proves the historical package was accepted by the stock recovery and that the handset now provides working UID 0 through `su`. No firmware, bootloader, recovery, modem, repartitioning, userdata wipe, or EFS operation was performed.

**Root gate: PASSED.** The next phase is read-only post-root preservation and capability inventory before introducing any Linux rootfs/chroot. Do not modify EFS or flash additional firmware at this stage.


## Post-root read-only preservation checkpoint — 2026-09-29

The first post-root read-only inventory on the physical GT-S5360 JPLC1 handset is complete and preserved in `01_PRESERVATION/evidence/post-root-jplc1-device-inventory-2026-09-29.md`.

Direct runtime evidence:

- `su -c id` returns `uid=0(root) gid=0(root)`.
- `/proc/mtd` is header-only; this is consistent with the Samsung BML/STL storage architecture and is not treated as evidence that NAND is absent.
- `/proc/cmdline` directly exposes the JPLC1 `bcm_umi-nand` partition map, including named `boot`, `boot_backup`, `system`, `cache`, `userdata`, `efs`, and calibration partitions.
- `/proc/partitions` directly exposes `bml1`–`bml15`, `stl6`, `stl9`, `stl10`, and `stl11`.
- Live mounts directly confirm `/dev/stl9 -> /system` (RFS, RO), `/dev/stl10 -> /cache` (RFS, RW), `/dev/stl11 -> /data` (RFS, RW), `/dev/stl6 -> /mnt/.lfs` (j4fs, RW), and the removable SD VFAT mount.
- The BML sizes match the kernel command-line partition sizes exactly; STL exposes the expected smaller logical capacities for the RFS-backed filesystems.

The post-root preservation gate is therefore **PASSED**.

No additional phone write was performed during this inventory. The next work is capability evaluation and rootfs design for the Android-native Linux userspace track. Native boot remains a separate research track.


## Chroot capability checkpoint — 2026-09-29

Direct post-root testing on the physical GT-S5360 JPLC1 handset has now confirmed basic chroot execution.

Observed with BusyBox v1.17.2:

    /system/xbin/busybox chroot /data/local/tmp/totoro-chroot /bin/busybox sh -c 'echo CHROOT_EXEC_OK; /bin/busybox id'

Result:

    CHROOT_EXEC_OK
    uid=0 gid=0
    chroot_status=0

The test root was created under /data/local/tmp using BusyBox mkdir and cp; the copied BusyBox is root-owned and executable. This confirms that the rooted Android environment can change root to a separate directory tree and execute a static ARM userspace binary as UID 0.

The earlier Android mkdir -p failure (Read-only file system) is not evidence that /data is read-only: the same directory was subsequently created successfully with BusyBox mkdir, and file write/read/remove tests on /data/local/tmp and /mnt/sdcard both passed.

BusyBox v1.17.2 provides chroot, losetup, mount, umount, pivot_root, mke2fs, and networking applets. Applet availability is not treated as proof of kernel/device capability.

Current practical gate: BASIC CHROOT EXECUTION PASSED. Loop-backed storage and network operation remain unverified. No mount, loop attachment, BML/STL write, EFS operation, repartitioning, or boot/recovery modification has been performed during this checkpoint.

Next: perform a reversible loop-device capability test, then choose between a directory rootfs and filesystem-image rootfs from observed results.


## Loop-backed storage checkpoint — 2026-09-29

Direct testing on the physical rooted JPLC1 handset has now validated loop-backed ext2 storage.

- A 4 MiB regular file was created under `/data/local/tmp` with BusyBox `dd`.
- `mke2fs` successfully created an ext2 filesystem in that file.
- `losetup -f` identified a free loop device; Android exposes the usable node as `/dev/block/loop0`, not `/dev/loop0`.
- `losetup /dev/block/loop0 /data/local/tmp/totoro-loop-test.img` returned status 0.
- `mount -t ext2 /dev/block/loop0 /data/local/tmp/totoro-loop-mnt` returned status 0.
- The live mount was explicitly `rw`: `/dev/block/loop0 on /data/local/tmp/totoro-loop-mnt type ext2 (rw,relatime,errors=continue)`.
- A file was written and read back successfully inside the mounted filesystem.
- The filesystem was unmounted, the loop device detached, and the image/mountpoint removed successfully.

This proves a reversible filesystem-image mechanism on the actual handset. It does **not** yet prove execution of an ELF binary from the loop-mounted filesystem; the shell-script execution attempt used a BusyBox shebang incorrectly and is not treated as an execution test.

Detailed evidence: `01_PRESERVATION/evidence/loop-backed-ext2-capability-2026-09-29.md`.

## Rootfs candidate assessment checkpoint — 2026-09-29

The rootfs selection has been narrowed using the live Totoro constraints.

**Primary compatibility experiment:** Alpine Linux v3.22.6 `armhf` minirootfs. Alpine's official architecture matrix explicitly describes its `armhf` port as 32-bit ARM for ARMv6 devices, and the official v3.22 armhf release directory contains the 3.22.6 minirootfs with checksum/GPG sidecars. The archive is approximately 3 MiB compressed. citeturn0search0turn1search0

The primary unresolved risk is the old kernel. Totoro runs Linux 2.6.35.7. Current musl documentation states that Linux >=2.6.39 is necessary for POSIX-conformant behaviour; older kernels may work with varying non-conformance. Therefore the architecture match does **not** establish current Alpine/musl compatibility. citeturn3search0

The fallback investigation order is:
1. older Alpine `armhf` release;
2. a purpose-built ARMv6 musl/BusyBox tree with tightly controlled syscall and ABI requirements;
3. minimal Debian `armel` userspace.

Current Debian `armhf` is ARMv7-oriented and therefore not a Totoro target. Debian `armel` is the older-ARM alternative, but current Debian documentation says trixie is the last armel release and support is being restricted, so it is a fallback rather than the first deployment target. citeturn0search7turn0search12

Storage is also tight: the latest measured `/data` free space was about 162.1 MiB. Public Galaxy Y specifications commonly report about 290 MiB RAM, while Alpine's current requirements page lists 256 MiB as a generic armhf starting point and warns that its non-x86 figures are work in progress. This leaves little margin while stock Android remains resident. citeturn2search6turn1search7

Accordingly, the first real rootfs must be minimal: no desktop, no compiler toolchain, no unnecessary daemons, and no retained package cache. The image size will be chosen from the actual extracted rootfs size rather than guessed.

Detailed assessment: `10_RESEARCH/rootfs-candidate-assessment-2026-09-29.md`.

## Current active gate

The next step is **offline verification and minimal live compatibility testing of Alpine v3.22.6 armhf**, not full rootfs deployment. Verify the official archive SHA-256/GPG metadata, inspect representative ELF binaries for ARM ISA/ABI/interpreter requirements, then run the smallest verified candidate executable on the phone. A failure such as Illegal instruction, missing loader, ABI error, or unsupported syscall is a diagnosis boundary.

The native/mainline boot track remains separate and unchanged.
