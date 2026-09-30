# Android-Hosted Linux on Totoro

## Purpose

This document tracks the fastest reliable route to a useful Linux userspace on the physical Samsung Galaxy Y GT-S5360 while keeping the working Android system and stock kernel intact.

The immediate target is a minimal Linux root filesystem entered through Android chroot, with a reliable shell and eventually host access. This is **not** independent OS boot. Launching Linux without Android is a separate, gated native-boot objective.

## Verified handset baseline

- GT-S5360 / totoro / BCM21553.
- Android 2.3.6, GINGERBREAD.JPLC1; PDA S5360JPLC1; CSC S5360OJPLC1; baseband S5360XXLK3.
- CPU reports ARMv6-compatible architecture 6TEJ, VFP and EDSP. Full implementer/part/variant/revision capture remains a D1 item; avoid inferring a specific core model from v6l alone.
- Linux 2.6.35.7, GCC 4.4.3, PREEMPT, build dated 2012-03-16.
- Root verified through su -c id → uid=0(root).
- Basic static BusyBox chroot execution verified; output included CHROOT_EXEC_OK and uid=0 gid=0.
- /data is Samsung RFS, RW, with approximately 162–163 MiB free at the latest documented measurement.
- SD is VFAT and was observed mounted noexec.
- /proc/self/ns was not exposed in the observed environment; this alone does not establish that all namespace functionality is impossible.

Detailed observations: 01_PRESERVATION/evidence/post-root-jplc1-device-inventory-2026-09-29.md.

## Verified loop-backed ext2 capability

A 4 MiB image was created under /data/local/tmp, formatted ext2, attached using /dev/block/loop0, mounted read-write, and successfully used for file write/read. It was then cleanly unmounted and detached, and temporary artifacts were removed.

This proves loop-backed ext2 creation, attachment, RW mounting, file I/O and teardown on the actual handset. It does **not** prove execution of an ELF binary from that mount. The earlier script attempt was invalid because of its BusyBox shebang invocation.

Full procedure and outputs: 01_PRESERVATION/evidence/loop-backed-ext2-capability-2026-09-29.md.

## Current stage: D1 read-only inventory

Before further implementation, capture a compact read-only capability and partition inventory from an interactive ADB shell. This refines the earlier plan because dump targets must be mapped before acquiring partition images.

Capture, where available:
- /proc/cpuinfo, /proc/meminfo, /proc/version, /proc/cmdline;
- /proc/partitions, /proc/filesystems, /proc/devices, /proc/iomem, /proc/fb;
- relevant /dev/block, /sys/class/graphics, /sys/class/android_usb, power-supply and kernel-interface metadata;
- /proc/config.gz only if present, plus relevant, reviewed kernel log excerpts.

Use interactive ADB, the repeatable method in prior testing. Capture output privately on the development computer. Review and redact serial numbers, IMEI, credentials, keys, network identifiers and other specimen-specific sensitive values before sharing or committing. Avoid indiscriminate publication of full getprop or dmesg output.

Interpretation cautions:
- A missing /proc/config.gz does not prove a feature is disabled.
- A sysfs directory, device node, or configuration string does not prove that a capability works.
- Map BML/STL nodes to named partitions only where evidence supports the mapping; leave unresolved entries unknown.
- Do not run stop, mount, attach loop devices, write files, or flash as part of D1.

D1 deliverable: reviewed transcript, measured memory, full CPU identity, provisional node-to-partition table, observed debug interfaces, and explicit unresolved questions.

## Next practical gates

1. Complete D1 without state changes.
2. In parallel, perform offline inspection of a small candidate executable: checksum/provenance, ARM ISA, EABI/float ABI, linkage/interpreter, libc and likely kernel requirements.
3. After review, acquire preservation evidence only using confirmed partition identities and suitable read-only methods. Establish a realistic recovery route; dumps alone are not a restore procedure.
4. Execute the smallest verified candidate binary in a reversible location, including an explicit ELF-from-loop-ext2 test.
5. If it passes, create a deliberately small rootfs sized from actual measurements, then validate chroot mounts, shell, networking and host access one step at a time.

## Architecture policy

No distribution or boot method is pre-approved. Choose the simplest candidate that passes actual CPU, ABI, kernel and storage tests. Avoid full installations, desktops, compilers, unnecessary daemons and package caches. If a candidate fails, stop at the exact failure and diagnose before switching approaches.

The stock-kernel/custom-ramdisk method remains a plausible native-boot hypothesis, but must wait for partition/image identification, debug-path assessment and recovery confidence. Kernel rebuilds, mainline work, kexec and multiboot should be pursued only if evidence shows they are necessary or materially reduce risk.

## Safety boundary

No boot/recovery flash, PIT or partition-table write, BML/STL write, EFS/modem write, repartitioning, or userdata wipe is part of the current stage. Any future state-changing experiment requires a separate written plan, verified inputs, rollback assessment and explicit approval.

## Current status

**Passed:** root, basic chroot execution, loop-backed ext2 RW mount/file I/O/clean teardown.

**Open:** D1 inventory refresh, complete partition-node mapping, recovery partition identity, verified full JPLC1 restore set and restore procedure, candidate ELF compatibility, ELF execution from loop ext2, networking and SSH.

**Immediate next action:** run D1 as a strictly read-only interactive ADB capture; review the redacted output before planning dumps or implementation.