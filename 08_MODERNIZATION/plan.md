# Galaxy Y Modernization Plan

## Goal

Launch a new operating system on the physical Samsung Galaxy Y GT-S5360 (Totoro), ultimately booting a Linux environment without Android. A Linux userspace inside Android through chroot is an intermediate milestone, not independent OS boot.

## Fast, reliable strategy

Keep the working stock Android kernel and boot chain unchanged while validating the simplest useful Linux userspace. Do not select a distribution or native-boot design by assumption. Add complexity only when a measured compatibility or capability blocker requires it.

    Preserve and inventory
            ↓
    Small binary compatibility test
            ↓
    Minimal Android-hosted Linux userspace (chroot)
            ↓
    Networking / USB-ADB access
            ↓
    Useful Linux userspace milestone

    In parallel, after recovery evidence:
    Stock boot-chain and ramdisk feasibility research
            ↓
    Controlled native-boot plan (not yet authorized)

## Track A — practical Android-hosted Linux (active)

1. **Read-only D1 inventory.** Capture full CPU identity, /proc/meminfo, kernel details, partition information, filesystem support, relevant device/sysfs interfaces, and carefully selected logs. Use interactive ADB. Redact sensitive identifiers.
2. **Offline candidate check.** Inspect a small executable for ARM ISA, EABI/float ABI, linkage, interpreter, libc and kernel requirements. Verify provenance and checksum.
3. **Minimal live test.** Execute one verified binary in a reversible writable location. Test ELF execution from loop-backed ext2 explicitly; prior loop evidence proves filesystem read/write and clean teardown only.
4. **Small rootfs.** If the binary passes, construct the smallest useful rootfs. Measure extracted size first and preserve /data headroom. Keep the SD card as transfer/storage unless execution constraints are resolved.
5. **Chroot integration.** Add only required /proc, /sys, /dev and terminal support. Test shell and basic utilities before services.
6. **Network and access.** Validate connectivity, then establish SSH or another reliable host access path. USB/ADB forwarding is an option to test, not an assumed capability.
7. **Milestone A.** Android boots normally; the Linux userspace enters reproducibly through chroot; basic utilities and host access work.

## Track B — independent/native boot (research, gated)

The stock-kernel/custom-ramdisk approach is a credible hypothesis because it may avoid a kernel rebuild, but it is not proven. First establish boot/recovery partition layout, stock image format, bootloader acceptance behavior, early-boot debugging, and a credible recovery route. Do not flash a test image until those gates are reviewed and explicit approval is given.

A rebuilt Samsung kernel is a later option only if required drivers or kernel features block the stock-kernel route. Mainline porting, kexec, multiboot and second-device experiments remain alternatives to assess only where they offer concrete benefit; they are not default parallel workstreams.

## Current evidence

- Physical device: GT-S5360 / BCM21553, Android 2.3.6 JPLC1, Linux 2.6.35.7.
- Root access and basic BusyBox chroot execution are verified.
- Loop-backed ext2 creation, attach, read-write mount, file I/O and clean teardown are verified.
- /data had approximately 162–163 MiB free at the last recorded measurement; this is not a current live measurement.
- SD VFAT was observed mounted noexec.
- Candidate binary compatibility, ELF execution from loop ext2, networking, SSH, recovery partition identity and a tested restore path remain open.

## Recovery and preservation gates

Before boot-critical changes, map partition nodes to names from device evidence and cross-check against PIT; acquire and hash read-only boot/recovery/EFS evidence using a verified method; keep EFS private and never write it; obtain a matching full-firmware restoration set; document a host-side restore runbook; and require explicit approval for any write.

A partition dump or Download Mode availability alone is not proof of a working restoration route. A no-op flash is a write and must not be described as read-only validation.

## Safety and working method

**Observe → preserve → smallest reversible step → test → record.**

No PIT write, repartitioning, EFS/modem write, userdata wipe, bootloader replacement, or boot/recovery flash without a separate reviewed decision and explicit approval. Preserve original artifacts and document failed tests.