# Roadmap

The roadmap is ordered around one goal: **turn the Totoro into something useful and cool.**

The active critical path is now the smallest safe Linux-userspace demonstration. Independent/native boot is preserved as a conditional research branch, not the default destination.

## Phase 0 — preservation and evidence

- [x] Record core physical/software identity and live partition evidence
- [x] Verify root access on the physical handset
- [x] Verify basic static BusyBox chroot execution
- [x] Verify loop-backed ext2 creation, RW mount, file I/O and clean teardown
- [ ] Capture only decision-critical D1/M1 read-only data still needed for later decisions
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

## Phase 2 — musl userspace — PASSED

- [x] Configure and build musl for ARMv6/ARM/soft-float
- [x] Build LLVM 22.1.8 ARM compiler-rt builtins
- [x] Produce and audit static musl ARMv6 ELF
- [x] Transfer over wireless ADB and verify SHA-256
- [x] Execute on physical Totoro with `STATUS=0`
- [x] **Milestone:** real static musl userspace executes on the stock kernel

## Phase 3 — dynamic userspace compatibility — PASSED

- [x] Configure musl for `arm-linux-gnueabi`
- [x] Build musl with ARMv6, ARM mode and soft-float settings
- [x] Build matching LLVM 22.1.8 ARM EABI compiler-rt builtins
- [x] Produce a dynamic musl ARMv6 executable and runtime
- [x] Audit ELF32/EABI/ARMv6/soft-float/`PT_INTERP`/dynamic linkage
- [x] Transfer the dynamic rootfs over wireless ADB
- [x] Verify executable, `libc.so`, loader and tarball SHA-256 on the physical device
- [x] Execute the dynamic musl binary on the physical Totoro
- [x] **Milestone:** one dynamic Linux userspace binary runs reliably on the stock kernel

Physical result:

```
TOTORO_DYNAMIC_MUSL_OK
armv6l
STATUS=0
```

Phase 4 physical acceptance evidence:

- Persistent image: `/mnt/sdcard/totoro-rootfs-phase4-01.ext2`, 32 MiB ext2.
- Pristine host image SHA-256: `4d84d6fb09d74b7049c0d0d77e8b59e1aba6f453c1bcf72eccaa5afde3ac6cc4`.
- Dynamic executable SHA-256: `48c56b75aeba489ec8b2101402dc8e3d5f5639c4271d4c34d5fda2d1ae7c0753`.
- Loader/libc SHA-256: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`.
- Two complete attach/mount/chroot/execute/unmount/detach cycles passed with `STATUS=0`.
- Final teardown checks reported `MOUNT_GONE` and `LOOP_DETACHED`.
- Post-cycle image SHA-256: `b2bec9c8ab005f9fe99aad752b724d7fa0a1c106fc0c8dbe583e2aaffcbdc4f0`.
- Host-side `e2fsck -fn` completed all five passes without structural errors.

The post-cycle hash is retained as a filesystem-state artifact; the pristine image hash remains the canonical build artifact.

## Phase 4 — persistent minimal rootfs — PASSED

- [x] Build the smallest persistent rootfs on the host; avoid package caches and unnecessary services
- [x] Store the persistent image on SD rather than consuming scarce internal RFS space
- [x] Use ext2 first because loop-backed ext2 is already proven on the handset
- [x] Prove dynamic ELF execution from the mounted ext2 image
- [x] Make the dynamic musl loader and `libc.so` part of the persistent rootfs
- [x] Validate controlled chroot entry and dynamic execution from the persistent rootfs
- [x] Validate repeatable attach/mount/chroot/exit/unmount/detach cycles
- [x] Never use `pivot_root`; verify mount/loop teardown
- [x] **Milestone:** repeatable entry/exit of a small Linux rootfs without altering boot-critical storage

## Phase 5 — first useful network service — NEXT

- [ ] Test a small HTTP server/service first because it directly supports the UI path
- [ ] Test Dropbear or another appropriately small SSH server if interactive access is useful
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
