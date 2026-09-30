# Galaxy Y Modernization Plan

## Goal

**Turn the Totoro into something useful and cool.**

The project is outcome-first. A Linux userspace running beside Android, a focused static Linux payload, a hybrid service, or another reversible design is a successful result if it makes the physical Totoro genuinely useful or fun.

Independent Linux boot remains interesting, but it is **not the active critical path**. We only reopen it when a concrete requirement cannot be met by the Android-assisted design.

## Integrated strategy

The five external reviews have been consolidated in `integrated-external-review-2026-09-30.md`. Their useful common ground is:

    Preserve working Android
            ↓
    Decision-critical read-only baseline
            ↓
    Static ARMv6 ELF test
            ↓
    Dynamic musl compatibility test
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

This ordering maximizes information gained per experiment while keeping rollback simple.

## Track A — active Linux userspace

### A1. Decision-critical baseline

Capture only what is needed to interpret the next experiment:
- exact CPU identity if not already tied to primary handset evidence;
- actual available memory;
- network interfaces/state;
- pty availability;
- relevant mount flags.

Use interactive ADB. Redact specimen-sensitive identifiers before sharing or committing.

### A2. Static ARMv6 payload

On the host:
- verify provenance and checksum;
- inspect ARM architecture, EABI/float ABI, linkage and interpreter;
- ensure the binary is actually built for the Totoro's ARMv6 capabilities.

On the phone:
- push to `/data/local/tmp`;
- execute as root;
- record exact stdout/stderr and exit status.

This is the first decisive Linux-userspace test. Do not build a full rootfs before it passes.

### A3. Dynamic userspace

After a static payload passes, test one dynamic musl-based ARMv6 binary.

The purpose is to distinguish:
- static ELF execution;
- dynamic loader compatibility;
- libc/kernel syscall compatibility;
- ABI problems;
- memory pressure.

If it fails, diagnose the exact failure before changing distributions.

### A4. Persistent minimal rootfs

Only after A2/A3:
- build on the host;
- keep it minimal;
- put the persistent image on the SD card;
- use ext2 first because it is already proven;
- do not rely on the VFAT mount's execution semantics; prove execution from the separate ext2 mount;
- validate clean attach/mount/chroot/exit/unmount/detach.

Never use `pivot_root`. Avoid a second `devpts` mount unless a later, evidence-based test demonstrates a real need.

### A5. Access and service

First test a small SSH implementation such as Dropbear. `adb forward` may be used as an early tethered access method; Wi-Fi/SSH is preferred if networking is confirmed.

Then implement exactly one useful service, for example:
- local status/dashboard;
- archive/capture endpoint;
- small network diagnostic service;
- Hermes-oriented client/bridge;
- offline knowledge service.

Do not install a broad package set before choosing the service.

### A6. UX checkpoint

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

Only after those gates may stock-kernel/custom-ramdisk work be considered. A rebuilt kernel, mainline port, kexec, multiboot, UART/SBL work or ROM swap requires its own evidence and justification.

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

Verified:
- GT-S5360 / BCM21553, Android 2.3.6 JPLC1, Linux 2.6.35.7;
- root access;
- basic static BusyBox chroot execution;
- loop-backed ext2 creation, attachment, RW mount, file I/O and clean teardown.

Still open:
- exact CPU part identity tied to primary evidence, if not already captured;
- static custom ARMv6 ELF execution;
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

**Do not build Alpine/Debian or a full rootfs yet.**

Run:

1. decision-critical read-only baseline;
2. host-side static ARMv6 ELF audit;
3. execute one static binary from `/data/local/tmp`;
4. record exact output and exit status.

If it passes, run one dynamic musl compatibility test. Only after that build the smallest persistent rootfs.