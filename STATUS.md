# Status

**Current stage:** integrated external-review strategy → smallest Linux-userspace compatibility gate.

**Project goal:** **turn the Totoro into something useful and cool.** A useful Android-assisted Linux environment or other reversible hybrid outcome is fully successful. Independent Linux boot is now a conditional research branch, not the active critical path.

**Working strategy:** preserve the known-working Android system and stock kernel; add complexity only when a measured blocker requires it; prefer evidence-producing experiments that write only to `/data/local/tmp` or removable storage.

## Verified physical-device state — 2026-09-29

- GT-S5360 / totoro / BCM21553; Android 2.3.6 GINGERBREAD.JPLC1; PDA S5360JPLC1; CSC S5360OJPLC1; baseband S5360XXLK3.
- CPU reports ARMv6-compatible architecture 6TEJ with VFP and EDSP. Exact implementer/part/variant/revision remains a primary-evidence item unless the corresponding handset transcript is linked and reviewed.
- Linux 2.6.35.7, GCC 4.4.3, PREEMPT, build dated 2012-03-16.
- Root verified: `su -c id` returned `uid=0(root)`.
- Basic static BusyBox chroot execution verified as UID 0.
- Loop-backed ext2 creation, attachment through `/dev/block/loop0`, read-write mount, file I/O, unmount, detach and cleanup verified.
- The loop test does **not** prove ELF execution from the mounted image.
- `/data` is Samsung RFS, read-write, with approximately 162–163 MiB free at the recorded capture. SD is VFAT and was observed mounted `noexec`.
- Root installation added five files under `/system`; the device is not byte-for-byte stock at the system-file level.
- No boot, recovery, modem, EFS, PIT, repartitioning, or userdata wipe/write has been performed. The root installation did modify `/system`.
- Live PIT SHA-256: `06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538`.

## Integrated review result

Five independent reviews were consolidated in `08_MODERNIZATION/integrated-external-review-2026-09-30.md`.

The strongest common findings are:

- Android-assisted Linux is the shortest practical active architecture.
- A small static ARMv6 ELF test should precede any full rootfs or distribution work.
- A dynamic musl-based test should follow the static test and answer the real userspace compatibility question.
- The already-proven SD loop/ext2 mechanism is preferable to consuming scarce internal RFS space for the persistent rootfs.
- Dropbear/SSH or a small web service can provide the first genuinely useful Linux experience.
- Native boot, custom kernel, mainline, UART/SBL, CPUFreq/AVS and similar work should be preserved but removed from the critical path.
- Review-reported claims about exact CPU part, restore-set completeness, EFS readback, community ROMs, or distro support remain qualified until tied to primary evidence.

## Current gates

1. **M1 — decision-critical read-only baseline:** capture exact CPU identity if still unresolved, actual available RAM, network state/interfaces, pty availability and relevant mount flags. Do not turn this into broad archaeology.
2. **M2 — static ARMv6 ELF:** offline-audit provenance, ISA/EABI/float ABI, linkage and interpreter; execute one verified static binary from `/data/local/tmp`.
3. **M3 — dynamic userspace:** execute one dynamic musl-based ARMv6 binary from a reversible location. Diagnose the exact failure mode before changing candidates.
4. **M4 — persistent rootfs:** only after M2/M3, use the already-proven SD loop/ext2 path for a small host-built rootfs and repeatable chroot.
5. **M5 — access/service:** Dropbear/SSH and one useful service or browser-accessible local dashboard.
6. **M6 — useful/cool checkpoint:** stop and reassess when the physical Totoro is genuinely useful or fun.
7. **M7 — conditional fallback:** Buildroot/uClibc-ng or another purpose-built rootfs only if richer userspace is still needed and the chosen distribution path fails.
8. **M8 — native boot reconsideration:** reopen only if a documented user-facing requirement cannot be met by the Android-assisted design.

## Recovery readiness

Recovery readiness remains separate from the active low-risk path.

Established: root and stock Android operation; live PIT hash; documented partition-map evidence from the post-root inventory; boot-critical regions reported untouched.

Not yet promoted to “verified”: exact recovery partition/image identity, a fully verified matching JPLC1 restore set, a rehearsed host-side restoration procedure, and any claim that EFS can be safely read back. Review statements about these items are not sufficient by themselves.

Before any boot-critical write, require verified matching inputs, node-to-partition cross-checking, a written restoration runbook, and explicit approval. A no-op flash is still a write.

## Active safety boundary

The userspace path must not write BML/STL raw nodes, PIT, EFS/modem, boot/recovery partitions or userdata.

For chroot work:
- never use `pivot_root`;
- do not create a second `devpts` mount unless later evidence proves it is required and safe;
- always tear down loop mounts before SD removal;
- verify mount and loop state after teardown;
- keep persistent rootfs storage on removable media when practical.

## Immediate next action

**Do not build a full distribution yet.**

Run the smallest compatibility gate:

**read-only baseline → host-side static ARMv6 ELF audit → execute one static binary from `/data/local/tmp` → record exact output and exit status.**

If that succeeds, immediately test one dynamic musl binary. Only then build the smallest persistent rootfs.

Historical checkpoints below this section are superseded where they conflict with the current status.