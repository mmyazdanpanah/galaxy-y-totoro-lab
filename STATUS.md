# Status

**Current stage:** Phase 4 persistent SD-backed ext2 rootfs verified on the physical Totoro → Phase 5 network service / native framebuffer UI bring-up.

**Project identity:** **A tiny, native ARMv6 pocket computer built from a Samsung Galaxy Y.**  
**Optional product identity:** **Totoro-Pocket-PC.**

This is an aspirational product identity, not a claim of independent Linux boot. The present implementation is still Android-assisted and reversible. The intended UX direction is a **tiny desktop-like Linux experience** adapted to the Totoro's 240×320 touchscreen and severe resource limits.

**Project goal:** **turn the Totoro into something useful, cool, and computer-like.** A useful Android-assisted Linux environment or other reversible hybrid outcome is fully successful. Independent Linux boot is now a conditional research branch, not the active critical path.

**Working strategy:** preserve the known-working Android system and stock kernel; add complexity only when a measured blocker requires it; prefer evidence-producing experiments that write only to `/data/local/tmp` or removable storage.

## Verified physical-device state — 2026-10-02

- GT-S5360 / totoro / BCM21553; Android 2.3.6 GINGERBREAD.JPLC1; PDA S5360JPLC1; CSC S5360OJPLC1; baseband S5360XXLK3.
- Linux 2.6.35.7, GCC 4.4.3, PREEMPT, build dated 2012-03-16.
- Root verified: `su -c id` returned `uid=0(root)`.
- Basic static BusyBox chroot execution verified as UID 0.
- Loop-backed ext2 creation, attachment through `/dev/block/loop0`, read-write mount, file I/O, unmount, detach and cleanup verified.
- `/data` is Samsung RFS and read-write; SD is VFAT and was observed mounted `noexec`.
- Root installation added five files under `/system`; the device is not byte-for-byte stock at the system-file level.
- No boot, recovery, modem, EFS, PIT, repartitioning, or userdata wipe/write has been performed. The root installation did modify `/system`.
- Live PIT SHA-256: `06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538`.

## Major verified milestone — static musl userspace on physical Totoro

A real host-built, statically linked musl ARMv6 Linux userspace program executed successfully on the physical Galaxy Y.

Artifact: `totoro-musl-test`  
SHA-256: `15c6fb0828a2933d64270b33d585879b99e01d59e0f00fa81fc0cd6962aec4d5`  
Size: 57,916 bytes

Physical result:

```
TOToro musl userspace
Kernel: Linux
Release: 2.6.35.7
Machine: armv6l
MUSL_ARMV6_OK
STATUS=0
```

This verifies native musl libc startup/runtime, malloc/free, string operations, uname and clean process exit on the stock Linux 2.6.35.7 kernel.

## Major verified milestone — Phase 3 dynamic userspace compatibility PASSED

On 2026-10-02, a host-built dynamically linked ARMv6 musl executable was transferred to the physical Totoro, independently hash-verified on-device, and executed successfully inside an isolated chroot rootfs under `/data/local/tmp`.

Dynamic test artifact: `totoro-musl-dynamic-test`  
SHA-256: `48c56b75aeba489ec8b2101402dc8e3d5f5639c4271d4c34d5fda2d1ae7c0753`

Dynamic musl runtime:
- `libc.so` SHA-256: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`
- `ld-musl-arm.so.1` SHA-256: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`
- Both are the same verified musl dynamic runtime image.
- Rootfs tarball SHA-256: `55e575f42372123f90b8e6a3c73196a087c51cea3e0f62f7b015caf6102c7017`.

Physical result:

```
TOTORO_DYNAMIC_MUSL_OK
armv6l
STATUS=0
```

This verifies, on the stock kernel:
- dynamic ARMv6 ELF loading;
- `PT_INTERP` execution through `/lib/ld-musl-arm.so.1`;
- musl dynamic relocation/runtime;
- shared `libc.so` execution;
- Linux syscall use through dynamic musl userspace;
- clean process exit.

The test did not modify boot-critical storage, the kernel, boot chain, NAND partitioning, EFS, PIT, recovery, or modem. The temporary dynamic rootfs remains under `/data/local/tmp/totoro-dynamic-test-01`.

## Major verified milestone — Phase 4 persistent SD-backed ext2 rootfs PASSED

On 2026-10-02, the physical Totoro successfully mounted and executed a dynamically linked ARMv6 musl program from a persistent SD-backed ext2 filesystem, using the stock Linux 2.6.35.7 kernel.

Persistent image:
- path on device: `/mnt/sdcard/totoro-rootfs-phase4-01.ext2`;
- size: 33,554,432 bytes (32 MiB);
- filesystem: ext2;
- loop device: `/dev/block/loop0`;
- mountpoint: `/data/local/tmp/totoro-rootfs-phase4-mnt`.

Host-built image SHA-256 before device use: `4d84d6fb09d74b7049c0d0d77e8b59e1aba6f453c1bcf72eccaa5afde3ac6cc4`.

The mounted executable, loader and libc were independently hash-verified on the device:
- executable: `48c56b75aeba489ec8b2101402dc8e3d5f5639c4271d4c34d5fda2d1ae7c0753`;
- `libc.so`: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`;
- `ld-musl-arm.so.1`: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`.

Physical execution from the mounted ext2 root, via chroot, produced:

```
TOTORO_DYNAMIC_MUSL_OK
armv6l
STATUS=0
```

A second complete attach → mount → hash verification → chroot execution → unmount → loop detach cycle also passed with `STATUS=0`. Both teardown checks reported `MOUNT_GONE` and `LOOP_DETACHED`.

The image SHA-256 changed after writable device mounts, from `4d84d6fb09d74b7049c0d0d77e8b59e1aba6f453c1bcf72eccaa5afde3ac6cc4` to `b2bec9c8ab005f9fe99aad752b724d7fa0a1c106fc0c8dbe583e2aaffcbdc4f0`. The post-cycle image was pulled back to the host and passed `e2fsck -fn` through all five filesystem-check passes with no structural errors. The original hash remains the canonical pristine build artifact; the post-cycle hash records the writable filesystem state after physical use.

This milestone demonstrates a persistent, removable Linux userspace substrate without modifying boot-critical storage, the kernel, boot chain, NAND partitioning, EFS, PIT, recovery or modem.


## Major verified milestone — native framebuffer/LCD update path PASSED

On 2026-10-02, the physical Totoro demonstrated a working native userspace path from framebuffer memory to the actual LCD panel.

Verified display state:
- `/proc/fb`: `0 LCDfb`;
- `/dev/graphics/fb0`;
- physical mode 240×320;
- virtual framebuffer 240×640;
- 32 bits per pixel;
- 960-byte stride;
- framebuffer mapping size 614,400 bytes.

Samsung BCM21553 kernel source tracing established the exact update mechanism: `lcdfb.c` exposes `.fb_ioctl = lcdfb_ioctl`; `LCDFB_IOCTL_UPDATE_LCD` is `0x46ff`; the ioctl accepts `LCD_DirtyRows_t { top, bottom }` and calls `lcd_dirty_rows()`; the lower driver uses the DMA/write-combine framebuffer and Broadcom LCD controller update path. No separate userspace cache flush is required by this traced path. The physical device has no `/dev/lcd` node, so the framebuffer private ioctl is the relevant tested interface.

A minimal static ARMv6 program opened and mapped `fb0`, backed up page 0, wrote an opaque white 240×320 pattern, issued `0x46ff` for rows 0–319, then restored the original page with the same ioctl.

Physical result:

```
OPEN_OK fd=9
MMAP_OK length=614400
BACKUP_PAGE0_OK
WHITE_PATTERN_WRITTEN
DIRTY_ROWS top=0 bottom=319
UPDATE_IOCTL_OK
WHITE_VISIBLE_FOR_5_SECONDS
RESTORING_PAGE0
RESTORE_IOCTL_OK
STATUS=0
```

Most importantly, the physical LCD visibly flashed white for less than one second before Android redrew its normal UI. This is direct physical evidence that custom native userspace can modify the framebuffer and trigger the real LCD controller/panel update path on the stock Totoro.

Artifact: `fb-direct-dirty` device-verified SHA-256: `2e1cbb523761ff88a39253bfe6f5f5af460649d6f8d8019ec8c608cd21ca5c2d`.

This does not yet establish an independent Linux graphical session; Android continues to own/redraw the display. The next goal is a small interactive native UI using the proven framebuffer and touchscreen paths.

## Product-direction milestone

The project has now demonstrated the three foundations needed for a future Totoro-Pocket-PC experience:

1. **Compute:** native ARMv6 static and dynamic musl Linux userspace execution.
2. **Persistent environment:** repeatable SD-backed ext2 rootfs attach → mount → chroot → execute → teardown.
3. **Physical UI substrate:** direct native framebuffer access has visibly updated the real LCD; touchscreen input is available through `/dev/input/event4`.

The next product-layer milestone is therefore **not a full Linux distribution**. It is the first tiny native graphical shell: framebuffer renderer + touch input + a desktop-like launcher/state model.

The desired progression is:

```
Linux userspace
     ↓
framebuffer + touchscreen
     ↓
minimal graphics primitives
     ↓
Totoro UI layer
     ↓
desktop-like pocket shell
     ↓
Files / Terminal / Tools / Settings / small apps
```

Existing Linux-phone projects, including postmarketOS and historical Galaxy Y work, should be used as technical references and sources of reusable ideas where compatible. We should not copy an entire desktop stack before the Totoro hardware and performance evidence justifies it.

## Current gates

- M1 — decision-critical baseline: remaining narrow evidence only.
- M2 — static ARMv6 ELF: PASSED.
- M3 — static musl userspace: PASSED.
- M4 — dynamic musl compatibility: **PASSED — Phase 3 complete.**
- M5 — persistent SD-backed ext2 rootfs: **PASSED — Phase 4 complete.**
- M6 — minimal network service: **NEXT / in progress**.
- M7 — native framebuffer display path: **PASSED on physical LCD**.
- M8 — interactive native UI: **NEXT**.
- M9 — useful/cool checkpoint and preservation.
- Native boot remains conditional on a concrete user-facing blocker.

## Phase 4 acceptance evidence

- persistent SD-backed ext2 image: 32 MiB;
- pristine image SHA-256: `4d84d6fb09d74b7049c0d0d77e8b59e1aba6f453c1bcf72eccaa5afde3ac6cc4`;
- physical executable SHA-256: `48c56b75aeba489ec8b2101402dc8e3d5f5639c4271d4c34d5fda2d1ae7c0753`;
- physical dynamic loader/libc SHA-256: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`;
- two successful attach/mount/chroot/execute/teardown cycles;
- execution result: `TOTORO_DYNAMIC_MUSL_OK`, `armv6l`, `STATUS=0`;
- post-cycle image SHA-256: `b2bec9c8ab005f9fe99aad752b724d7fa0a1c106fc0c8dbe583e2aaffcbdc4f0`;
- post-cycle host-side `e2fsck -fn`: all five passes completed without structural errors.

## Immediate path

The next engineering task is the **first tiny interactive native UI**, now explicitly treated as the first product-layer step toward Totoro-Pocket-PC: a small ARMv6 userspace renderer that writes the verified framebuffer and responds to `/dev/input/event4` touch events. The network-service path remains useful in parallel as a remote/debug/control channel.

## Phase 3 acceptance evidence

The dynamic compatibility gate is considered complete because the exact host-built artifacts were hash-verified on the physical device before execution and produced the expected result:

- executable: `48c56b75aeba489ec8b2101402dc8e3d5f5639c4271d4c34d5fda2d1ae7c0753`;
- `libc.so`: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`;
- loader: `6a86294f527a1ad6539ac7643badd1986cfca012f5edfe52a541be3da00a4191`;
- result: `TOTORO_DYNAMIC_MUSL_OK`, `armv6l`, `STATUS=0`.

Historical checkpoints below this section are superseded where they conflict with the current status.
