# Galaxy Y Modernization Plan

## Goal

**Turn the Totoro into something useful and cool.**

The project is outcome-first. A Linux userspace running beside Android, a focused static Linux payload, a hybrid service, or another reversible design is a successful result if it makes the physical Totoro genuinely useful or fun.

Independent Linux boot remains interesting, but it is **not the active critical path**. We only reopen it when a concrete requirement cannot be met by the Android-assisted design.

## Integrated strategy

The active path is now:

    Preserve working Android
            ↓
    Decision-critical read-only baseline
            ↓
    Static ARMv6 ELF test                    ← PASSED
            ↓
    Static musl userspace                    ← PASSED
            ↓
    Dynamic musl compatibility               ← PASSED
            ↓
    Persistent SD-backed ext2 rootfs         ← PASSED
            ↓
    Network service                          ← ACTIVE
            ↓
    Native framebuffer + touchscreen         ← DISPLAY PASSED
            ↓
    Interactive native UI                    ← NEXT
            ↓
    Useful/cool checkpoint
            ↓
    Richer userspace only if justified
            ↓
    Native boot only if a concrete blocker requires it

## Track A — active Linux userspace

### A1. Decision-critical baseline

Capture only what is needed to interpret the next experiment:
- exact CPU identity if still needed;
- actual available memory;
- network interfaces/state;
- pty availability;
- relevant mount flags.

Use interactive ADB. Redact specimen-sensitive identifiers before sharing or committing.

### A2. Static ARMv6 payload — PASSED

A host-built static ARMv6 Linux EABI payload has executed on the physical Totoro from `/data/local/tmp`.

### A3. Static musl userspace — PASSED

The host-built static musl ARMv6 path is physically verified. The audited artifact `totoro-musl-test` has SHA-256 `15c6fb0828a2933d64270b33d585879b99e01d59e0f00fa81fc0cd6962aec4d5` and executed on the physical Totoro with `MUSL_ARMV6_OK` and `STATUS=0`.

### A4. Dynamic musl compatibility — PASSED

The dynamic musl runtime is now physically verified on the stock Totoro kernel.

Verified artifact:
- `totoro-musl-dynamic-test` SHA-256: `48c56b75aeba489ec8b2101402dc8e3d5f5639c4271d4c34d5fda2d1ae7c0753`;
- `libc.so` SHA-256: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`;
- `ld-musl-arm.so.1` SHA-256: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`;
- rootfs tarball SHA-256: `55e575f42372123f90b8e6a3c73196a087c51cea3e0f62f7b015caf6102c7017`.

Physical result:

```
TOTORO_DYNAMIC_MUSL_OK
armv6l
STATUS=0
```

This proves that the stock 2.6.35.7 kernel can load the ARMv6 dynamic ELF, invoke the musl dynamic loader, relocate the runtime, resolve shared musl libc, execute Linux syscalls and exit cleanly.

No boot-critical storage, kernel, boot chain, NAND partitioning, EFS, PIT, recovery or modem was modified by this test.

### A5. Persistent minimal rootfs — PASSED

The persistent rootfs milestone is now physically verified on the handset.

A 32 MiB ext2 image stored on the removable SD card was attached through `/dev/block/loop0`, mounted, entered with a controlled chroot, and used to execute the verified dynamically linked ARMv6 musl test. The same image completed two full attach/mount/chroot/execute/unmount/detach cycles.

Requirements:
- host-built and reproducible;
- dynamic musl runtime included;
- no package caches or unnecessary services;
- minimal shell plus only required runtime files;
- ext2 image stored on removable media;
- execution proven from the mounted ext2 filesystem, not from VFAT;
- repeatable attach → mount → chroot → service → exit → unmount → detach;
- no `pivot_root`;
- no boot-critical writes.

Acceptance evidence:

- pristine image SHA-256: `4d84d6fb09d74b7049c0d0d77e8b59e1aba6f453c1bcf72eccaa5afde3ac6cc4`;
- dynamic executable SHA-256: `48c56b75aeba489ec8b2101402dc8e3d5f5639c4271d4c34d5fda2d1ae7c0753`;
- loader/libc SHA-256: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`;
- physical result: `TOTORO_DYNAMIC_MUSL_OK`, `armv6l`, `STATUS=0`;
- two successful attach → mount → chroot → execute → unmount → detach cycles;
- post-cycle image SHA-256: `b2bec9c8ab005f9fe99aad752b724d7fa0a1c106fc0c8dbe583e2aaffcbdc4f0`;
- post-cycle host-side `e2fsck -fn`: all five passes completed without structural errors.

The post-cycle hash is retained as a filesystem-state artifact; the pristine image hash remains the canonical build artifact.

**The Totoro can repeatedly enter and leave a persistent SD-backed Linux rootfs while Android remains intact.**

### A6. Access and service

After A5 is stable:
- prefer a tiny HTTP service as the first service because it directly supports the physical UI path;
- test `adb forward` as an early host-access method;
- test Dropbear/SSH if interactive access is useful;
- use Wi-Fi/SSH as the preferred untethered path if networking remains stable.

Choose exactly one useful service before adding packages.

### A7. Real UI milestone

The first UI target is a browser-rendered local dashboard rather than a native Linux desktop.

Acceptance criteria:
- service runs on the physical Totoro;
- phone's own Android browser opens the local page;
- page displays live Totoro information;
- at least one control performs a real action or changes useful state;
- touch interaction works;
- no boot-critical storage is touched.

If this is already useful/cool, preserve it before adding complexity.


### A7. Native framebuffer and touchscreen — DISPLAY PATH PASSED

The Totoro's stock Android kernel exposes a working native framebuffer and touchscreen interface. Verified display state is 240×320 physical, 240×640 virtual, 32 bpp, with a 614,400-byte mapped framebuffer. Touchscreen input is available at `/dev/input/event4` with X/Y ranges corresponding to the display.

Samsung BCM21553 source tracing established the update mechanism: `lcdfb.c` provides `.fb_ioctl = lcdfb_ioctl`; `LCDFB_IOCTL_UPDATE_LCD = 0x46ff`; userspace passes `LCD_DirtyRows_t { top, bottom }`; the driver invokes `lcd_dirty_rows()`; and the lower LCD controller uses the DMA/write-combine framebuffer and Broadcom update path. The physical device has no `/dev/lcd` node, so the tested framebuffer ioctl is the relevant interface.

The direct dirty-row test wrote an opaque white 240×320 page and issued the ioctl. The physical LCD visibly flashed white before Android redrew its own display. Device-verified artifact SHA-256: `2e1cbb523761ff88a39253bfe6f5f5af460649d6f8d8019ec8c608cd21ca5c2d`.

**This proves custom native userspace pixels can reach the physical LCD on the stock Totoro.** It does not yet prove an independent Linux graphical session; Android still owns and redraws the display.

### A8. Interactive native UI — NEXT

Build the smallest real UI on top of the proven display and input path:
- ARMv6 native userspace;
- 240×320 framebuffer renderer;
- direct mmap writes;
- `0x46ff` dirty-row refresh;
- `/dev/input/event4` touch decoding;
- one stable screen and one or two touchable controls;
- no boot-critical writes.

If Android immediately redraws over the UI, document that behavior and then decide whether tighter Android cooperation or a more independent display ownership model is justified.

### A9. Useful/cool checkpoint

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

Only after those gates may stock-kernel/custom-ramdisk work be considered.

## Current evidence

### Verified on the physical Totoro

- GT-S5360 / BCM21553, Android 2.3.6 JPLC1, Linux 2.6.35.7;
- root access;
- basic static BusyBox chroot execution;
- loop-backed ext2 creation, attachment, RW mount, file I/O and clean teardown;
- custom host-built static ARMv6 Linux EABI ELF execution;
- static musl userspace execution;
- dynamic musl userspace execution;
- native Linux syscall execution;
- real filesystem round-trip I/O;
- dynamic executable, loader and libc hashes independently verified on-device;
- no boot-chain or stock-kernel modification from these experiments.

### Still open

- tiny HTTP/network service on the persistent rootfs;
- interactive native framebuffer/touch UI;
- useful/cool checkpoint;
- actual available RAM under normal Android load;
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

## Immediate next action

**Build and test the first tiny network service on the persistent rootfs.**

The next technical sequence is:

1. Keep the verified persistent ext2 image as the Phase 4 baseline.
2. Add only the files required for one tiny network service.
3. Prove the service runs inside the persistent rootfs.
4. Prove host/device network reachability.
5. Prefer a tiny HTTP service because it directly supports the browser UI path.
6. Preserve the working state before adding further packages.

Do not touch boot-critical storage, NAND raw nodes, PIT, EFS, modem, boot/recovery partitions, or repartitioning.
