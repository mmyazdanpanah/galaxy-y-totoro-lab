# Status

**Current stage:** verified native ARMv6 userspace execution → musl toolchain integration.

**Project goal:** **turn the Totoro into something useful and cool.** A useful Android-assisted Linux environment or other reversible hybrid outcome is fully successful. Independent Linux boot is now a conditional research branch, not the active critical path.

**Working strategy:** preserve the known-working Android system and stock kernel; add complexity only when a measured blocker requires it; prefer evidence-producing experiments that write only to `/data/local/tmp` or removable storage.

## Verified physical-device state — 2026-10-01

- GT-S5360 / totoro / BCM21553; Android 2.3.6 GINGERBREAD.JPLC1; PDA S5360JPLC1; CSC S5360OJPLC1; baseband S5360XXLK3.
- CPU reports ARMv6-compatible architecture 6TEJ with VFP and EDSP. Exact implementer/part/variant/revision remains a primary-evidence item unless the corresponding handset transcript is linked and reviewed.
- Linux 2.6.35.7, GCC 4.4.3, PREEMPT, build dated 2012-03-16.
- Root verified: `su -c id` returned `uid=0(root)`.
- Basic static BusyBox chroot execution verified as UID 0.
- Loop-backed ext2 creation, attachment through `/dev/block/loop0`, read-write mount, file I/O, unmount, detach and cleanup verified.
- `/data` is Samsung RFS, read-write, with approximately 162–163 MiB free at the recorded capture. SD is VFAT and was observed mounted `noexec`.
- Root installation added five files under `/system`; the device is not byte-for-byte stock at the system-file level.
- No boot, recovery, modem, EFS, PIT, repartitioning, or userdata wipe/write has been performed. The root installation did modify `/system`.
- Live PIT SHA-256: `06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538`.

## Major new verified milestone — static musl userspace on physical Totoro

A real host-built, statically linked musl ARMv6 Linux userspace program executed successfully on the physical Galaxy Y.

Artifact: `totoro-musl-test`  
SHA-256: `15c6fb0828a2933d64270b33d585879b99e01d59e0f00fa81fc0cd6962aec4d5`  
Size: 57,916 bytes

The ELF was audited as ARMv6/EABI5, soft-float, statically linked, with no `PT_INTERP` or dynamic section. It was transferred over TCP ADB at `172.20.10.2:5555`; the device hash matched exactly before execution.

Physical result:

```
TOToro musl userspace
Kernel: Linux
Release: 2.6.35.7
Machine: armv6l
MUSL_ARMV6_OK
STATUS=0
```

This verifies native musl libc startup/runtime, malloc/free, string operations, uname and clean process exit on the stock Linux 2.6.35.7 kernel. No boot-critical storage, kernel, or boot-chain changes were made.

## Current gates

- M1 — decision-critical baseline: remaining narrow evidence only.
- M2 — static ARMv6 ELF: PASSED.
- M3 — static musl userspace: PASSED.
- M4 — dynamic musl compatibility: NEXT.
- M5 — persistent SD-backed ext2 rootfs.
- M6 — minimal network service.
- M7 — real UI milestone: local web dashboard rendered by the Totoro's existing Android browser, with touch interaction.
- M8 — useful/cool checkpoint and preservation.
- Native boot remains conditional on a concrete user-facing blocker.

## Immediate path to a real UI

The near-term target is deliberately not a Linux desktop. Build one dynamic musl test, then a minimal persistent ext2 userspace, then a tiny HTTP service. The phone's existing Android browser can render the service locally, giving us a genuine physical-screen/touch UI without first solving GPU, framebuffer, window-system, or desktop-stack problems.

## Major new verified milestone — native ARMv6 Linux userspace

The project has now passed the original decisive static-userspace gate on the physical Totoro.

### Verified: custom ARMv6 ELF execution

A host-built, statically linked ARMv6 Linux EABI executable was built with Homebrew LLVM/LLD, inspected, transferred over wireless ADB and executed on the real handset.

Verified properties included:

- ELF32 little-endian ARM executable;
- EABI5;
- ARMv6 / `arm1136jf-s` attributes;
- no `PT_INTERP`;
- statically linked;
- execution directly from `/data/local/tmp`.

Observed physical-device result:

`totoro-exit42` → `STATUS=42`.

A second diagnostic payload successfully exercised Linux `write`, `uname`, `getuid`, `getpid` and `exit` syscalls and returned `STATUS=0`.

A third payload performed real filesystem round-trip I/O using `open`, `write`, `read`, `close`, `uname` and `unlink` and returned:

`File round-trip: TOToro_FS_SYSCALL_OK`  
`Kernel: Linux`  
`STATUS=0`

The temporary test file was then confirmed absent.

Therefore the following are now **verified on the physical device**:

- ARMv6 ELF execution;
- Linux syscall execution;
- real filesystem I/O from a custom native payload;
- temporary-file lifecycle;
- stock kernel unchanged;
- boot chain untouched.

This moves the project past the former M2 gate.

## Current musl toolchain gate

The host-side musl cross-build is now in progress.

Verified on the Mac:

- Homebrew LLVM 22.1.8 can target `arm-linux-gnueabi`;
- musl configured successfully for `arm-linux-gnueabi`;
- musl built successfully with ARMv6, ARM mode and soft-float settings;
- musl installed into `/tmp/totoro-toolchain/arm-linux-gnueabi/usr`;
- static `libc.a`, startup objects and headers are present.

The first direct musl link reached the musl archive but failed only because the LLVM ARM EABI compiler-runtime helpers were not present:

`__aeabi_dcmpeq`, `__aeabi_dmul`, `__aeabi_uidiv`, `__aeabi_dadd`, `__aeabi_dsub`, `__aeabi_d2uiz`, `__aeabi_ui2d`, `__aeabi_uldivmod`, and related symbols.

This is currently classified as a **host toolchain integration gap**, not a Totoro compatibility failure. C compilation against the musl sysroot succeeds and LLD consumes `libc.a`.

The matching LLVM 22.1.8 compiler-rt source is currently being fetched into `/tmp/compiler-rt-totoro`. This is host-side scratch work and is intentionally outside the project repository.

## Current gates

1. **M1 — decision-critical read-only baseline:** capture any still-unresolved exact CPU identity, actual available RAM, network state/interfaces, pty availability and relevant mount flags. Do not turn this into broad archaeology.
2. **M2 — static ARMv6 ELF:** **PASSED.** Custom ARMv6 static ELF execution and filesystem syscalls are verified on the physical Totoro.
3. **M3 — dynamic userspace:** in progress. Build one real static musl-linked ARMv6 binary first, then use it as the foundation for a dynamic compatibility test.
4. **M4 — persistent rootfs:** not started as a new milestone; use the already-proven SD loop/ext2 path only after M3 is characterized.
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

**Finish the musl compiler-runtime integration, then build one minimal static musl-linked ARMv6 executable.**

The next technical sequence is:

**compiler-rt ARM EABI builtins → static musl-linked ARMv6 test → ELF audit → wireless ADB push → physical Totoro execution → record exact output/status → then dynamic loader test.**

Do not build Alpine/Debian or a full rootfs yet.

Historical checkpoints below this section are superseded where they conflict with the current status.
