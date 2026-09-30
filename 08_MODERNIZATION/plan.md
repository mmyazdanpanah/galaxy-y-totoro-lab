# Galaxy Y Modernization Plan

## Goal

**Turn the Totoro into something useful and cool.**

The preferred technical destination is a useful Linux-based environment that can boot independently of Android. But we should not make the project long or complicated just to satisfy one architecture. If an Android-assisted, hybrid, or other simpler approach gives the physical Totoro a genuinely good user experience, that is also a successful outcome.

## Fast, reliable strategy

Keep the working stock Android kernel and boot chain unchanged while validating the simplest useful Linux userspace. Do not select a distribution or native-boot design by assumption. Add complexity only when a measured blocker requires it.

    Preserve and inventory
            ↓
    Small compatibility test
            ↓
    Minimal Linux userspace
            ↓
    Useful / cool Totoro experience
            ↓
    Independent native boot if practical and justified

Native boot is the preferred technical destination, not a reason to delay a useful result.

## Track A — practical Linux userspace (active)

1. **Read-only D1 inventory.** Capture full CPU identity, /proc/meminfo, kernel details, partition information, filesystem support, relevant device/sysfs interfaces, and carefully selected logs. Use interactive ADB. Redact sensitive identifiers.
2. **Offline candidate check.** Inspect a small executable for ARM ISA, EABI/float ABI, linkage, interpreter, libc and kernel requirements. Verify provenance and checksum.
3. **Minimal live test.** Execute one verified binary in a reversible writable location. Test ELF execution from loop-backed ext2 explicitly; prior loop evidence proves filesystem read/write and clean teardown only.
4. **Small rootfs.** If the binary passes, construct the smallest useful rootfs. Measure extracted size first and preserve /data headroom. Keep the SD card as transfer/storage unless execution constraints are resolved.
5. **Chroot integration.** Add only required /proc, /sys, /dev and terminal support. Test shell and basic utilities before services.
6. **Network and access.** Validate connectivity, then establish SSH or another reliable host access path. USB/ADB forwarding is an option to test, not an assumed capability.
7. **Milestone A.** Get a reproducible, useful Linux environment running on the real Totoro.
8. **User-experience checkpoint.** If the device already feels useful and cool, pause and evaluate before adding complexity.

## Track B — independent/native boot (preferred destination, gated)

The stock-kernel/custom-ramdisk approach is a credible hypothesis because it may avoid a kernel rebuild, but it is not proven. First establish boot/recovery partition layout, stock image format, bootloader acceptance behavior, early-boot debugging, and a credible recovery route. Do not flash a test image until those gates are reviewed and explicit approval is given.

A rebuilt Samsung kernel is a later option only if required drivers or kernel features block the stock-kernel route. Mainline porting, kexec, multiboot and second-device experiments remain alternatives to assess only where they offer concrete benefit; they are not default parallel workstreams.

## Track C — simpler alternatives

Keep open any Android-assisted, hybrid, or other architecture that can make the physical Totoro genuinely useful without unnecessary engineering. It must still be clearly documented so we know what is independent and what depends on Android.

A practical success is better than an impressive but unusable technical demo.

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

**Make the Totoro useful first. Keep the path short. Observe → preserve → smallest reversible step → test → record.**

No PIT write, repartitioning, EFS/modem write, userdata wipe, bootloader replacement, or boot/recovery flash without a separate reviewed decision and explicit approval. Preserve original artifacts and document failed tests.