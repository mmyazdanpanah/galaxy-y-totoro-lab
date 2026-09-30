# Android-Hosted Linux on Totoro

## Purpose

This document is the active implementation track for turning the Totoro into something useful and cool without replacing Android or the stock kernel.

The immediate target is not a “Linux distribution.” It is a **verified ARMv6 Linux userspace capability**, followed by the smallest persistent rootfs and one useful service. Independent Linux boot is conditional and outside the active critical path.

See `integrated-external-review-2026-09-30.md` for the five-review synthesis.

## Verified handset baseline

- GT-S5360 / totoro / BCM21553.
- Android 2.3.6, GINGERBREAD.JPLC1; PDA S5360JPLC1; CSC S5360OJPLC1; baseband S5360XXLK3.
- CPU reports ARMv6-compatible architecture 6TEJ with VFP and EDSP. Exact implementer/part/variant/revision is still treated as unresolved unless tied to primary handset evidence.
- Linux 2.6.35.7, GCC 4.4.3, PREEMPT, build dated 2012-03-16.
- Root verified through `su -c id` → `uid=0(root)`.
- Basic static BusyBox chroot execution verified; output included `CHROOT_EXEC_OK` and `uid=0 gid=0`.
- `/data` is Samsung RFS, RW, with approximately 162–163 MiB free at the latest documented measurement.
- SD is VFAT and was observed mounted `noexec`.
- `/proc/self/ns` was not exposed in the observed environment; this alone does not establish that all namespace functionality is impossible.

## Verified loop-backed ext2 capability

A 4 MiB image was created under `/data/local/tmp`, formatted ext2, attached using `/dev/block/loop0`, mounted read-write, and successfully used for file write/read. It was then cleanly unmounted and detached, and temporary artifacts were removed.

This proves loop-backed ext2 creation, attachment, RW mounting, file I/O and teardown on the actual handset.

It does **not** prove execution of an ELF binary from that mount. The earlier script attempt was invalid because of its BusyBox shebang invocation.

## Integrated engineering direction

The review synthesis changes the order of work:

1. **Decision-critical read-only baseline**
   - exact CPU identity if unresolved;
   - actual available RAM;
   - network state/interfaces;
   - pty availability;
   - relevant mount flags.

2. **Static ARMv6 payload**
   - audit provenance/checksum and ELF attributes on the host;
   - execute from `/data/local/tmp`;
   - capture exact result.

3. **Dynamic musl payload**
   - test one exact binary from a reversible location;
   - distinguish loader, ABI, syscall/libc and memory failures.

4. **Persistent SD rootfs**
   - build on the host;
   - use ext2 first;
   - prove ELF execution from the mounted image;
   - keep the rootfs minimal.

5. **Chroot integration**
   - expose only the required `/proc`, `/sys`, `/dev` interfaces;
   - never use `pivot_root`;
   - do not add a second `devpts` mount without evidence;
   - verify complete teardown.

6. **Access and service**
   - test Dropbear/SSH;
   - test `adb forward` as an early access path;
   - test Wi-Fi/SSH when networking is confirmed;
   - add one useful service.

7. **UX checkpoint**
   - use the physical device;
   - stop if it is already useful/cool.

## Candidate end states

The active implementation should optimize for one of these, chosen after the basic payload works:

- pocket Linux SSH node;
- local browser dashboard;
- offline reference/knowledge device;
- archive/capture utility;
- small network diagnostic tool;
- Hermes-oriented thin client;
- retro/offline Android experience as an independent quick win.

The Linux environment does not need a desktop, compiler, package cache or broad distribution to count as successful.

## Compatibility rules

Do not accept a distro architecture label as proof.

For every candidate executable record:
- provenance and checksum;
- ARM architecture/ISA attributes;
- EABI/float ABI;
- dynamic interpreter, if any;
- static vs dynamic linkage;
- libc;
- relevant kernel requirements.

Interpret failures literally:
- `Illegal instruction` → investigate ISA/instruction-set mismatch;
- `Exec format error` → investigate ELF/ABI/architecture;
- `not found` for an existing dynamic ELF → investigate interpreter/loader;
- `ENOSYS`/syscall failures → investigate kernel/libc mismatch;
- segfault/OOM → investigate binary compatibility and memory pressure.

Do not substitute another distribution until the failure class is understood.

## Storage policy

Use:
1. `/data/local/tmp` for the first small payloads;
2. SD-backed loop image for persistent rootfs;
3. internal `/data` only where its limited free space is justified.

The SD VFAT mount being `noexec` is not itself a conclusion about an ext2 loop mount. Prove execution on the separate ext2 mount.

Ext2 is the first filesystem candidate because it has already been demonstrated on the handset. Ext3 is an alternative only if a measured requirement appears.

## Minimal chroot safety model

The Android kernel and global mount table remain shared.

Therefore:
- do not use `pivot_root`;
- avoid assumptions about mount namespaces;
- bind only interfaces that are actually required;
- never create a second `devpts` by default;
- tear down `/proc`, `/sys`, `/dev` bindings and the rootfs mount in a controlled order;
- detach the loop device only after all mounts are gone;
- verify with mount/loop listings before removing the SD card.

## Preservation boundary

Nothing in this active path requires:
- PIT writes;
- BML/STL writes;
- EFS/modem writes;
- boot/recovery flashing;
- repartitioning;
- userdata wipe.

Any future boot-critical operation needs a separate written plan, verified inputs, recovery assessment and explicit approval.

Review-reported claims about exact CPU part, restore-set completeness or EFS readback are not promoted to project facts until primary evidence is linked and reviewed.

## Immediate procedure

**Step 1 — baseline:** capture only the decision-critical read-only data.

**Step 2 — static payload:** host-side audit, push to `/data/local/tmp`, execute, record.

**Step 3 — dynamic payload:** if Step 2 passes, run one dynamic musl test and record.

**Step 4 — persistent rootfs:** only after Steps 2–3, build the smallest SD/ext2 image and test chroot execution.

**Step 5 — usefulness:** add Dropbear or one small service and demonstrate a real task.

The first success target is therefore **not “boot Linux.” It is “run useful Linux code safely on the real Totoro.”**