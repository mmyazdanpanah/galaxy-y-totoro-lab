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

