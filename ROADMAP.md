# Roadmap

The roadmap is ordered around the fastest reliable path to launching a new OS on the physical Galaxy Y Totoro, ultimately without Android running. A chroot userspace is an intermediate milestone, not independent boot.

## Preservation and evidence

- [x] Record core physical/software identity and live partition evidence (2026-09-29)
- [x] Verify root access on the physical handset
- [x] Verify basic static BusyBox chroot execution
- [x] Verify loop-backed ext2 creation, RW mount, file I/O and clean teardown
- [ ] Complete reviewed read-only capability/partition inventory (D1)
- [ ] Resolve supported BML/STL node-to-partition mappings
- [ ] Acquire read-only boot/recovery/EFS preservation evidence after mapping
- [ ] Obtain and verify a matching full JPLC1 restore set and document a credible restore route
- [ ] Back up untracked preservation/research evidence to protected, separate storage

## Practical Linux userspace — active

- [ ] Offline-audit a minimal candidate executable for ISA, ABI, linkage, libc and kernel requirements
- [ ] Execute the smallest verified candidate on Totoro
- [ ] Prove ELF execution from loop-backed ext2
- [ ] Build a minimal rootfs sized against measured storage and memory
- [ ] Validate chroot shell and required /proc, /sys and /dev interfaces
- [ ] Validate networking
- [ ] Establish reliable host access, preferably through a tested USB/ADB path
- [ ] **Milestone:** reproducible Linux userspace available while stock Android remains bootable

## Independent/native OS boot — gated research

- [ ] Confirm boot/recovery partition identities and image structure
- [ ] Assess stock-kernel/custom-ramdisk feasibility and early-boot debugging
- [ ] Establish recovery readiness before any boot-critical write
- [ ] Prepare and review a minimal, reversible first-boot experiment
- [ ] **Milestone:** Linux userspace boots without Android, with a verified recovery path
- [ ] Consider a rebuilt kernel only if the stock kernel is a demonstrated blocker
- [ ] Consider mainline, kexec, multiboot or hardware debug only where evidence justifies added complexity

## Historical research (non-blocking)

- [ ] Preserve Samsung BCM21553 source and defconfig reconciliation
- [ ] Continue CPUFreq/AVS provenance analysis
- [ ] Preserve historical recovery, ROM and custom-kernel references
- [ ] Document platform limits and reproducible findings

## Execution principle

**Observe → preserve → smallest reversible step → test → record.** Avoid parallel speculative work, unnecessary toolchains, broad distributions, and risky device changes without evidence-based need and explicit approval.