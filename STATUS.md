# Status

**Current stage:** read-only capability inventory and recovery-risk reduction, followed by the smallest userspace compatibility test.

**Project goal:** launch a new OS on the physical Samsung Galaxy Y GT-S5360 (Totoro), ultimately without Android running. A chroot Linux userspace hosted by Android is an intermediate milestone, not independent OS boot.

**Working strategy:** pursue the fastest reliable route, preserve the known-working stock system, and add complexity only when a measured blocker requires it. The stock-kernel/custom-ramdisk route is a hypothesis for native boot, not a proven or approved implementation.

## Verified physical-device state — 2026-09-29

- GT-S5360 / totoro / BCM21553; Android 2.3.6 GINGERBREAD.JPLC1; PDA S5360JPLC1; CSC S5360OJPLC1; baseband S5360XXLK3.
- CPU reports ARMv6-compatible architecture 6TEJ with VFP and EDSP. Full implementer/part/variant/revision details remain a capture item; do not infer a specific core model from v6l alone.
- Linux 2.6.35.7, GCC 4.4.3, PREEMPT, build dated 2012-03-16.
- Root verified: su -c id returned uid=0(root).
- Basic static BusyBox chroot execution verified as UID 0.
- Loop-backed ext2 creation, attachment through /dev/block/loop0, read-write mount, file I/O, unmount, detach and cleanup verified.
- The loop test does not prove ELF execution from the mounted image.
- /data is Samsung RFS, read-write, with approximately 162–163 MiB free at the recorded capture. SD is VFAT and was observed mounted noexec.
- Root installation added five files under /system. The device is therefore not byte-for-byte stock at the system-file level; record this as a known post-root delta.
- No boot, recovery, modem, EFS, PIT, repartitioning, or userdata wipe/write has been performed. The root installation did modify /system.
- Live PIT SHA-256: 06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538.

Detailed handset and loop evidence is linked from 08_MODERNIZATION/android-chroot-linux.md and 01_PRESERVATION/evidence/. Some review conclusions rely on STATUS summaries rather than independent inspection of every raw evidence file; treat those as documented claims until raw output is reviewed.

## Current gates

1. **D1 — read-only capability and partition inventory: NEXT.** Capture CPU identity, memory, kernel and partition information, relevant filesystem/device metadata, available framebuffer/USB interfaces, power-supply metadata, and reviewed logs. Use interactive ADB. Redact sensitive values before sharing or committing. A missing /proc/config.gz does not prove a feature is disabled; a device node or sysfs entry does not prove functionality.
2. **D2 — preservation and recovery evidence: AFTER D1.** Establish exact partition-node mappings first. Then acquire carefully scoped read-only boot/recovery/EFS evidence and hashes. EFS must never be written or published. A dump alone is not a tested restoration path.
3. **Offline compatibility audit.** Inspect a small candidate executable for ISA, EABI/float ABI, interpreter/libc and likely kernel requirements. A distribution architecture label alone is insufficient.
4. **Small live executable test.** Run only a verified minimal candidate in a reversible location. Record exact output and errors. ELF execution from loop ext2 remains unverified.
5. **Minimal chroot integration.** After compatibility passes, build a small rootfs, then test required interfaces, networking and SSH incrementally.

Offline audit can proceed in parallel with D1. Do not perform a recovery-slot test, flash, partition write, or other boot-critical modification as part of D1.

## Recovery readiness

Established: root and stock Android operation; live PIT hash; documented partition-map evidence from the post-root inventory; boot-critical regions reported untouched.

Not established: independently verified full JPLC1 restore set; definitive recovery partition identity and image structure; complete named mapping of relevant BML/STL nodes; raw EFS backup; rehearsed host-side restoration procedure. Download Mode availability alone does not guarantee recovery.

Before any boot-critical write, require verified matching images, a node-to-partition map cross-checked against PIT evidence, a written restoration runbook, and an explicit experimental decision. A no-op flash is still a write, not a read-only rehearsal.

## Simplified technical direction

- **Active practical path:** retain Android and the stock kernel; prove a minimal Linux userspace binary, then a small chroot and USB/ADB-assisted access.
- **Native-boot research:** investigate stock-kernel/custom-ramdisk feasibility only after partition identity, image structure, debug path and recovery confidence are established.
- **Avoid for now:** kernel rebuilds, mainline porting, overclock/AVS work, repartitioning, large distributions, unnecessary services and broad package installations.

## Next action

Run the strictly read-only D1 inventory from an interactive ADB shell and save the transcript privately on the development computer. Redact serial, IMEI, credentials, keys, network identifiers and other sensitive values before sharing. Return the transcript and a provisional partition-node map; mark unresolved mappings as unknown. Review D1 before planning any dump or state-changing experiment.

## Historical checkpoints

Earlier entries in this status history described pre-root or pre-loop-test states. They are superseded by this current summary. In particular, statements that rooting has not occurred, that no /system write has occurred, or that loop-backed ext2 is untested are stale and must not be used as current status.