# Status

Phase: M0/M1 — Android-native userspace Linux reconnaissance and preparation

Latest research pass: 2026-09-29. The Samsung OSS Totoro source baseline, CPUFreq/defconfig reconciliation, boot-image geometry, and external historical-source audit remain preserved as the native-boot research track.

The practical modernization path is now split into two deliberately independent tracks:

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

1. Phase 0 phone inventory has not yet been completed in this documentation checkpoint.
2. Static BusyBox ARMv6 compatibility must be tested before rooting.
3. Root package must be matched to the phone's actual ro.build.display.id and checksum-verified.
4. Post-root preservation must inspect /proc/mtd, mount, getprop, and /proc/cmdline without modifying EFS.
5. Rootfs location is evidence-driven: /data/local/alpine first, loop-backed SD image if necessary.
6. Alpine release is selected from official release information at experiment time and recorded with checksum.
7. Chroot and SSH are built incrementally, with separate interactive and SSH launch scripts.

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

The five Samsung Totoro defconfigs and their exact adjacent differences remain documented in 10_RESEARCH/cpufreq-defconfig-reconciliation-2026-09-29.md.

## Native-boot safety boundary

The native-boot track remains **not ready for flashing or first hardware boot**. No CPUFreq/AVS modification, repartitioning, PIT write, modem work, EFS write, or bootloader replacement is authorized by the current plan.

## First practical hardware milestone

For the active chroot track, the first meaningful milestone is no longer a kernel boot. It is:

**userspace Linux running** — Android remains the booting system, a Linux rootfs enters reproducibly through chroot, networking works, and SSH access from the Mac is established.

This is intentionally lower-risk than replacing the Android boot path and can be evaluated independently of the native-boot research.

## Native-boot milestone estimate

The earlier estimate remains valid for the separate native-boot track: research/preparation is roughly 70–80% complete for a reversible diagnostic boot experiment, with reproducible build, zImage provenance, boot-image reconstruction, offline validation, and rollback verification remaining.

For the active chroot path, the remaining work is narrower and starts with actual phone output rather than additional broad archaeology.

## Next action

Run the read-only Phase 0 inventory and preserve its complete output. Then perform the static BusyBox ARMv6 compatibility test. Do not root until those results are reviewed.