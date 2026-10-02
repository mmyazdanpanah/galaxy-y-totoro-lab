# Galaxy Y Modernization Plan

## Goal

**Turn the Totoro into something useful and cool.**

The project is outcome-first. A Linux userspace running beside Android, a focused static Linux payload, a hybrid service, or another reversible design is a successful result if it makes the physical Totoro genuinely useful or fun.

Independent Linux boot remains interesting, but it is **not the active critical path**. We only reopen it when a concrete requirement cannot be met by the Android-assisted design.

## Integrated strategy

The five external reviews have been consolidated in `integrated-external-review-2026-09-30.md`. The project has now converted the first major recommendation into physical-device evidence:

    Preserve working Android
            ↓
    Decision-critical read-only baseline
            ↓
    Static ARMv6 ELF test                    ← PASSED
            ↓
    Dynamic musl compatibility test         ← ACTIVE
            ↓
    Small SD-backed ext2 rootfs
            ↓
    SSH / small useful service
            ↓
    Physical-device UX checkpoint
            ↓
    Richer userspace only if justified
            ↓
    Native boot only if a concrete blocker requires it

## Track A — active Linux userspace

### A1. Decision-critical baseline

Capture only what is needed to interpret the next experiment:
- exact CPU identity if not already tied to primary handset evidence;
- actual available memory;
- network interfaces/state;
- pty availability;
- relevant mount flags.

Use interactive ADB. Redact specimen-sensitive identifiers before sharing or committing.

### A2. Static ARMv6 payload — PASSED

A host-built static ARMv6 Linux EABI payload has now executed on the physical Totoro from `/data/local/tmp`.

Verified:
- ELF32 little-endian ARM;
- EABI5;
- ARMv6 / `arm1136jf-s` attributes;
- no `PT_INTERP`;
- static linkage;
- native execution against the stock 2.6.35.7 kernel;
- ordinary Linux syscalls;
- real filesystem round-trip I/O.

Representative results:
- `totoro-exit42` returned status 42;
- `totoro-diag` returned status 0 after Linux diagnostic syscalls;
- `totoro-fsdiag` returned status 0 after file create/write/read/unlink.

This is the first decisive Linux-userspace milestone.

### A3. Musl compatibility — STATIC PATH PASSED

The host-built static musl ARMv6 path is now physically verified. The audited artifact `totoro-musl-test` has SHA-256 `15c6fb0828a2933d64270b33d585879b99e01d59e0f00fa81fc0cd6962aec4d5` and executed on the physical Totoro with `MUSL_ARMV6_OK` and `STATUS=0`.

### A4. Dynamic musl compatibility — NEXT

The host-side musl environment is now built far enough to isolate the remaining dependency.

Completed:
- configured musl for `arm-linux-gnueabi`;
- built with ARMv6, ARM mode and soft-float;
- installed a dedicated musl sysroot;
- compiled a C test object against that sysroot;
- reached direct LLD linking of musl `libc.a`.

Current issue:
- the direct LLD link reports missing ARM EABI compiler-runtime helpers such as `__aeabi_dmul`, `__aeabi_uidiv` and related symbols;
- the installed Homebrew LLVM package does not contain the needed ARM builtins archive;
- this is classified as a host-side compiler-runtime integration issue, not as a handset compatibility failure.

Next:
1. build matching LLVM 22.1.8 compiler-rt ARM builtins in `/tmp`;
2. link one minimal static musl ARMv6 test;
3. audit ELF attributes;
4. push over wireless ADB;
5. execute on the physical Totoro;
6. only then test a dynamic musl loader.

### A4. Persistent minimal rootfs

Only after the musl compatibility gate:
- build on the host;
- keep it minimal;
- put the persistent image on the SD card;
- use ext2 first because it is already proven;
- do not rely on the VFAT mount's execution semantics; prove execution from the separate ext2 mount;
- validate clean attach/mount/chroot/exit/unmount/detach.

Never use `pivot_root`. Avoid a second `devpts` mount unless a later, evidence-based test demonstrates a real need.

### A6. Access and service

First test a small SSH implementation such as Dropbear. `adb forward` may be used as an early tethered access method; Wi-Fi/SSH is preferred if networking is confirmed.

Then implement exactly one useful service, for example:
- local status/dashboard;
- archive/capture endpoint;
- small network diagnostic service;
- Hermes-oriented client/bridge;
- offline knowledge service.

Do not install a broad package set before choosing the service.

### A7. Real UI milestone

The first UI target is a browser-rendered local dashboard rather than a native Linux desktop. A tiny HTTP service on the Totoro will expose live device state and at least one useful touch interaction through Android's existing browser.

Acceptance criteria:
- service runs on the physical Totoro;
- phone's own browser opens the local page;
- page displays live Totoro information;
- at least one control performs a real action or changes useful state;
- touch interaction works;
- no boot-critical storage is touched.

If this is already useful/cool, preserve it before adding complexity.

### A8. Useful/cool checkpoint

Use the physical phone.

If the experience is already useful/cool, stop. Preserve the working image, scripts and evidence before doing anything more ambitious.

## Track B — richer userspace fallback

If the desired use case needs more programs than the minimal userspace can provide:

1. Prefer a purpose-built rootfs.
2. Consider Buildroot/uClibc-ng or a tightly constrained musl build for the actual ARMv6/kernel combination.
3. Keep the rootfs small and host-built.
4. Add only tools justified by the use case.

Do not assume Alpine, Debian, or another distro will work solely because an architecture label appears compatible. Test the exact binaries.

## Track C — independent/native boot, conditional

This track is parked until Track A/B demonstrates a concrete blocker.

If reopened, require:
1. exact boot/recovery partition and image identity;
2. verified restore inputs;
3. written host-side recovery runbook;
4. early-boot debug path;
5. explicit user approval for any boot-critical write.

Only after those gates may stock-kernel/custom-ramdisk work be considered. A rebuilt kernel, mainline, kexec, multiboot, UART/SBL work or ROM swap requires its own evidence and justification.

## Useful alternative outcomes

Keep these available without making them dependencies:
- retro/offline entertainment;
- pocket terminal/reference device;
- local web dashboard;
- archive/capture device;
- small network utility;
- Hermes-oriented thin client.

A useful Android-only outcome is still a success if it is genuinely useful/cool.

## Current evidence

### Verified on the physical Totoro

- GT-S5360 / BCM21553, Android 2.3.6 JPLC1, Linux 2.6.35.7;
- root access;
- basic static BusyBox chroot execution;
- loop-backed ext2 creation, attachment, RW mount, file I/O and clean teardown;
- custom host-built static ARMv6 Linux EABI ELF execution from `/data/local/tmp`;
- native Linux syscall execution;
- real filesystem round-trip I/O;
- no boot-chain or stock-kernel modification from these experiments.

### Verified on the host

- Homebrew LLVM 22.1.8 targeting `arm-linux-gnueabi`;
- musl configured and built for ARMv6/ARM/soft-float;
- installed musl sysroot;
- C compilation against musl succeeds;
- direct LLD link reaches musl `libc.a` and exposes the remaining ARM EABI compiler-runtime dependency.

### Still open

- ARM compiler-runtime builtins integration;
- static musl binary execution on Totoro;
- dynamic userspace compatibility;
- ELF execution from loop-mounted ext2;
- actual available RAM under normal Android load;
- networking and SSH;
- complete recovery readiness and restore procedure.

## Recovery and preservation gates

The low-risk userspace path does not require raw partition writes.

Before any boot-critical change:
- cross-check node identity against primary partition evidence and PIT;
- verify the exact input images;
- verify a credible restore set and procedure;
- keep EFS private and never write it;
- document rollback and failure handling;
- require explicit approval.

Do not treat a review claim as proof of recovery readiness. A dump alone is not a tested restoration route. A no-op flash is still a write.

## Safety rules for the active path

- No PIT writes.
- No BML/STL raw writes.
- No EFS/modem writes.
- No boot/recovery flashing.
- No repartitioning.
- No userdata wipe.
- No `pivot_root`.
- No unnecessary second `devpts` mount.
- Tear down loop mounts before SD removal.
- Verify mounts and loop state after teardown.
- Prefer removable storage for persistent Linux data.

## Stop conditions

Stop and diagnose rather than switching approaches blindly when:
- an ELF returns `Illegal instruction`;
- the loader reports `not found`;
- the binary returns `Exec format error`;
- a syscall fails with `ENOSYS`;
- memory pressure becomes unstable;
- networking or pty assumptions fail.

The exact failure determines the next branch.

## Immediate next action

**Finish compiler-rt integration.**

Then run:

1. build one minimal static musl-linked ARMv6 executable;
2. audit ELF32/EABI/ARMv6/soft-float/static/`PT_INTERP`;
3. push over wireless ADB;
4. execute on the physical Totoro and record exact output/status;
5. use the result to decide the dynamic loader test.

Do not build Alpine/Debian or a full rootfs yet.
