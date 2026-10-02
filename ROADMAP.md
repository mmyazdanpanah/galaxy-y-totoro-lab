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

## Phase 1 — smallest Linux payload — PASSED

- [x] Offline-audit a host-built static ARMv6 ELF for ISA, EABI, linkage and interpreter
- [x] Execute the verified static payload from `/data/local/tmp`
- [x] Verify native Linux syscall execution
- [x] Verify real filesystem round-trip I/O
- [x] Record exact output and exit status
- [x] **Milestone:** a verified custom ARMv6 Linux userspace payload executes on the real Totoro

Physical-device evidence includes:
- `totoro-exit42` → `STATUS=42`;
- `totoro-diag` → `TOToro native Linux diagnostic: Linux`, `STATUS=0`;
- `totoro-fsdiag` → `File round-trip: TOToro_FS_SYSCALL_OK`, `Kernel: Linux`, `STATUS=0`;
- temporary filesystem test file confirmed absent after cleanup.

## Phase 2 — musl userspace — PASSED

- [x] Configure and build musl for ARMv6/ARM/soft-float
- [x] Build LLVM 22.1.8 ARM compiler-rt builtins
- [x] Produce and audit static musl ARMv6 ELF
- [x] Transfer over wireless ADB and verify SHA-256
- [x] Execute on physical Totoro with `STATUS=0`
- [x] **Milestone:** real static musl userspace executes on the stock kernel

## Phase 3 — dynamic userspace compatibility — NEXT

- [x] Configure musl for `arm-linux-gnueabi`
- [x] Build musl with ARMv6, ARM mode and soft-float settings
- [x] Install a host-side musl sysroot
- [x] Compile a C test object against the musl sysroot
- [ ] Supply the matching ARM EABI compiler-runtime builtins
- [ ] Produce one minimal static musl-linked ARMv6 executable
- [ ] Offline-audit ELF32/EABI/ARMv6/soft-float/static/`PT_INTERP`
- [ ] Execute the static musl binary on the physical Totoro
- [ ] Test one dynamic musl binary and diagnose loader/libc/kernel compatibility
- [ ] **Milestone:** one dynamic Linux userspace binary runs reliably

Current blocker: Homebrew LLVM does not ship the required ARM compiler-runtime archive in the installed package. Matching LLVM 22.1.8 compiler-rt source is being fetched in `/tmp/compiler-rt-totoro` for an exact-version ARM EABI builtins build.

## Phase 4 — persistent minimal rootfs

- [ ] Build the smallest rootfs on the host; avoid package caches and unnecessary services
- [ ] Store the persistent image on SD rather than consuming scarce internal RFS space
- [ ] Use ext2 first because loop-backed ext2 is already proven on the handset
- [ ] Prove ELF execution from the mounted image
- [ ] Validate minimal chroot shell and only required /proc, /sys and /dev access
- [ ] Never use `pivot_root`; treat global bind mounts carefully and verify teardown
- [ ] **Milestone:** repeatable enter/exit of a small Linux rootfs without altering boot-critical storage

## Phase 5 — first useful network service

- [ ] Test Dropbear or another appropriately small SSH server
- [ ] Test `adb forward` as an early host-access path
- [ ] Test Wi-Fi/SSH as the preferred untethered path if networking is confirmed
- [ ] Add one small service: local dashboard, archive/status service, network utility or Hermes-oriented client
- [ ] **Milestone:** a real person can use the Totoro for a concrete task

## Phase 6 — real UI milestone

- [ ] Serve a small dashboard from the Totoro
- [ ] Open it using the existing Android browser
- [ ] Show live kernel/uptime/memory/network state
- [ ] Add at least one useful touch interaction
- [ ] **Milestone:** the physical phone presents and accepts a real touch-usable UI

## Phase 7 — useful/cool checkpoint

- [ ] Test the experience end-to-end
- [ ] Preserve the working state if it is already useful/cool
- [ ] Document the concrete limitation before adding complexity

## Phase 8 — richer userspace fallback

- [ ] Test the experience on the physical device, not just from the host
- [ ] Decide whether it is already useful/cool
- [ ] If yes, stop and preserve the working state
- [ ] If no, document the concrete limitation before adding complexity

## Phase 8 — richer userspace fallback

- [ ] If a richer environment is needed, test a purpose-built Buildroot/uClibc-ng or otherwise kernel-constrained rootfs
- [ ] Keep the rootfs small and ARMv6-specific
- [ ] Add only tools justified by the chosen use case
- [ ] Re-test memory and stability after each major addition

## Phase 9 — conditional native-boot research

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
