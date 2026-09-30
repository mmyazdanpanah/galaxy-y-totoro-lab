# Roadmap

The roadmap is ordered around one goal: **turn the Totoro into something useful and cool.**

The active critical path is now the smallest safe Linux-userspace demonstration. Independent/native boot is preserved as a conditional research branch, not the default destination.

## Phase 0 — preservation and evidence

- [x] Record core physical/software identity and live partition evidence (2026-09-29)
- [x] Verify root access on the physical handset
- [x] Verify basic static BusyBox chroot execution
- [x] Verify loop-backed ext2 creation, RW mount, file I/O and clean teardown
- [ ] Capture only decision-critical D1/M1 read-only data: exact CPU identity if unresolved, actual RAM, network state, pty and mount flags
- [ ] Verify any remaining BML/STL mapping or recovery claims against primary evidence
- [ ] Obtain and verify a matching full JPLC1 restore set and credible restore procedure before any boot-critical experiment
- [ ] Back up untracked preservation/research evidence to protected separate storage

## Phase 1 — smallest Linux payload

- [ ] Offline-audit one static ARMv6 ELF for provenance, ISA, EABI/float ABI, linkage and interpreter
- [ ] Execute the verified static payload from `/data/local/tmp`
- [ ] Record exact output, exit status and failure mode if any
- [ ] **Milestone:** a verified custom ARMv6 Linux userspace payload executes on the real Totoro

## Phase 2 — dynamic userspace

- [ ] Offline-audit one dynamic musl-based ARMv6 binary
- [ ] Execute it from a reversible location
- [ ] Determine whether loader/libc/kernel compatibility is sufficient
- [ ] If it fails, diagnose the exact class: ISA, ABI, loader, syscall/libc or memory
- [ ] **Milestone:** one dynamic Linux userspace binary runs reliably

## Phase 3 — persistent minimal rootfs

- [ ] Build the smallest rootfs on the host; avoid package caches and unnecessary services
- [ ] Store the persistent image on SD rather than consuming scarce internal RFS space
- [ ] Use ext2 first because loop-backed ext2 is already proven on the handset
- [ ] Prove ELF execution from the mounted image
- [ ] Validate minimal chroot shell and only required /proc, /sys and /dev access
- [ ] Never use `pivot_root`; treat global bind mounts carefully and verify teardown
- [ ] **Milestone:** repeatable enter/exit of a small Linux rootfs without altering boot-critical storage

## Phase 4 — first useful service

- [ ] Test Dropbear or another appropriately small SSH server
- [ ] Test `adb forward` as an early host-access path
- [ ] Test Wi-Fi/SSH as the preferred untethered path if networking is confirmed
- [ ] Add one small service: local dashboard, archive/status service, network utility or Hermes-oriented client
- [ ] **Milestone:** a real person can use the Totoro for a concrete task

## Phase 5 — UX checkpoint

- [ ] Test the experience on the physical device, not just from the host
- [ ] Decide whether it is already useful/cool
- [ ] If yes, stop and preserve the working state
- [ ] If no, document the concrete limitation before adding complexity

## Phase 6 — richer userspace fallback

- [ ] If a richer environment is needed, test a purpose-built Buildroot/uClibc-ng or otherwise kernel-constrained rootfs
- [ ] Keep the rootfs small and ARMv6-specific
- [ ] Add only tools justified by the chosen use case
- [ ] Re-test memory and stability after each major addition

## Phase 7 — conditional native-boot research

Only reopen this phase if a concrete requirement cannot be met by the Android-assisted design.

- [ ] Confirm boot/recovery partition identities and image structure
- [ ] Establish a credible restoration procedure
- [ ] Establish an early-boot debug path
- [ ] Assess stock-kernel/custom-ramdisk feasibility
- [ ] Consider a rebuilt kernel only if the stock kernel is a demonstrated blocker
- [ ] Consider mainline/kexec/multiboot only if a specific benefit justifies the added risk and work
- [ ] Require explicit approval before any boot-critical write

## Historical research — non-blocking

- [ ] Preserve Samsung BCM21553 source and defconfig reconciliation
- [ ] Preserve CPUFreq/AVS provenance analysis
- [ ] Preserve historical recovery, ROM and custom-kernel references
- [ ] Document platform limits and reproducible findings

## Execution principle

**Observe → preserve → smallest reversible step → test → record → use the device.**

Do not let speculative native-boot research outrun the next useful physical-device demonstration.