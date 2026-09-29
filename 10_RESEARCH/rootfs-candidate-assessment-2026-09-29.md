# Totoro Rootfs Candidate Assessment — 2026-09-29

## Scope

This assessment selects and constrains the next rootfs experiment for the physical Samsung Galaxy Y GT-S5360 JPLC1.

Observed platform constraints:

- CPU: Broadcom BCM21553 / ARM11, ARMv6-compatible, VFP present.
- Android: 2.3.6 JPLC1.
- Kernel: Linux 2.6.35.7.
- Android ABI property: `armeabi`.
- `/data`: Samsung RFS, read-write; approximately 162.1 MiB free at the latest loop-capability capture.
- Loop-backed ext2: directly proven on the physical handset.
- SD: VFAT, mounted `noexec`.
- Basic chroot: directly proven.
- Modern namespace interface: `/proc/self/ns` was not exposed in the observed environment.
- RAM: public specifications commonly report approximately 290 MiB; this is treated as contextual hardware information rather than a substitute for an on-device `/proc/meminfo` capture.

The objective is an Android-hosted userspace Linux environment, not a native Linux boot.

## Candidate matrix

| Candidate | ARMv6 CPU fit | ABI fit | 2.6.35 kernel risk | Footprint | Assessment |
|---|---|---|---|---|---|
| Alpine 3.22.6 armhf minirootfs | Official Alpine armhf port explicitly targets ARMv6 | hard-float; handset has VFP, but direct binary test is still required | **High/uncertain**: current musl documentation says Linux >=2.6.39 is needed for POSIX-conformant behaviour; older kernels may work with varying non-conformance | Official minirootfs archive is about 3 MiB compressed | **Primary candidate for direct compatibility experiment, not yet approved for full deployment** |
| Older Alpine armhf (3.19.x / 3.18.x) | Official armhf releases exist and remain archived | hard-float; same ABI question | Potentially better historical fit than current userspace, but kernel compatibility still requires testing | About 3 MiB compressed minirootfs archives | **Fallback candidates if current Alpine fails** |
| Debian armel | Designed for older 32-bit ARM and is much closer to Totoro's ISA baseline | soft-float EABI; ABI-compatible directionally with ARMv6 | Userspace may work under the Android kernel, but current Debian support is constrained and the distribution is much heavier than a minimal Alpine tree | Larger practical footprint than Alpine; exact minimal tree must be constructed | **Fallback/research candidate, not first deployment target** |
| Custom BusyBox + musl/minimal libc tree | Can be built exactly for ARMv6 and the required float ABI | Can be controlled precisely | Can minimize syscall surface and avoid unsupported services | Potentially smallest footprint | **Best escape hatch if distro rootfs compatibility fails; requires more engineering** |
| postmarketOS rootfs | ARMhf ecosystem exists | ARMhf | Its project/device status is aimed at native boot and the Totoro page warns about future ARMhf support concerns | Not optimized for this chroot experiment | **Not selected for the Android-hosted first rootfs** |

## Alpine finding

Alpine officially lists `armhf` as a 32-bit ARM port for ARMv6 devices and has maintained an armhf minirootfs. The current v3.22 release tree contains `alpine-minirootfs-3.22.6-armhf.tar.gz`, approximately 3 MiB compressed, with SHA-256 and GPG sidecar files. This makes Alpine unusually attractive for a constrained Totoro rootfs. Official Alpine documentation also describes the minirootfs specifically for containers and minimal chroots.

However, the kernel boundary is the major unresolved issue. Current musl documentation says Linux kernel >=2.6.39 is necessary for POSIX-conformant behaviour and that older kernels may work with varying degrees of non-conformance. Totoro's 2.6.35.7 kernel is below that boundary. Therefore, the project must not treat current Alpine/musl compatibility as established merely because the CPU architecture matches.

The correct next experiment is a small, offline-verified Alpine armhf binary/rootfs compatibility probe, not immediate full deployment.

## Why Debian armel is not the first choice

Debian's armel port targets older 32-bit ARM processors, whereas current Debian armhf requires ARMv7/VFPv3 and therefore does not fit Totoro. Current Debian documentation also states that trixie is the last release for armel and that regular armel support is being retired/restricted.

An armel userspace is therefore technically interesting as a conservative ISA/ABI fallback, but it is less attractive for this project because the current distribution lifecycle and package/kernel assumptions are increasingly constrained. A tiny Debian armel userspace may still be useful if Alpine's musl/kernel interaction fails.

## RAM and storage constraints

Public Galaxy Y specifications commonly report about 290 MiB RAM. Alpine's own requirements page currently lists 256 MiB as a generic minimum to start an armhf system, while explicitly warning that its non-x86 figures are work in progress. This leaves very little margin once stock Android remains resident.

Therefore:

- no desktop environment;
- no full Alpine installation;
- no unnecessary OpenRC services;
- no large package set;
- no compiler/toolchain on the phone;
- no package cache retained unless needed;
- SSH should be introduced only after the base chroot is stable.

The measured `/data` free space of approximately 162.1 MiB is the harder storage constraint. A first rootfs image should be deliberately small enough to leave substantial headroom for Android and experiment artifacts.

## Storage strategy

The loop-backed ext2 experiment is now directly proven on the handset. This is the preferred Linux-filesystem mechanism for the first rootfs image because it avoids requiring Linux-native semantics from the underlying RFS filesystem.

The SD card remains useful for transferring the rootfs archive/image, but its live VFAT mount is `noexec`. Do not execute the Linux userspace directly from the SD filesystem.

Proposed first image envelope:

- start with a small ext2 image;
- keep the initial rootfs well below the available `/data` capacity;
- leave meaningful free space for Android and rollback;
- expand only after the base chroot has been proven.

The exact image size should be selected after measuring the extracted candidate rootfs rather than guessing.

## Compatibility gates before deployment

The selected Alpine 3.22.6 armhf archive must first be downloaded and verified offline using its official SHA-256 and GPG metadata. Then inspect the rootfs ELF binaries and identify:

- ARM ISA requirements;
- EABI/float ABI;
- dynamic interpreter;
- libc version;
- use of instructions beyond ARMv6/VFP;
- likely kernel syscall requirements.

After offline inspection, perform the smallest live test possible on Totoro:

1. copy one verified candidate executable into `/data/local/tmp`;
2. execute it without changing `/system`;
3. record the exact error or success;
4. only then unpack the full rootfs.

A failure such as `Illegal instruction`, missing dynamic loader, unsupported syscall, or VFP/ABI error is a diagnosis boundary, not a reason to bypass the compatibility gate.

## Current disposition

**Primary experiment candidate:** Alpine v3.22.6 armhf minirootfs.

**Status:** architecture/storage candidate selected for testing; full deployment is not yet approved.

**Fallback order for investigation:** older Alpine armhf release → purpose-built ARMv6 musl/BusyBox rootfs → minimal Debian armel userspace.

The deciding evidence will come from actual executable tests against the physical 2.6.35.7 Totoro kernel, not from the architecture label alone.

## Sources

- Alpine architecture matrix: https://wiki.alpinelinux.org/wiki/Architecture
- Alpine downloads: https://www.alpinelinux.org/downloads/
- Alpine v3.22 armhf release directory: https://dl-cdn.alpinelinux.org/alpine/v3.22/releases/armhf/
- Alpine requirements: https://wiki.alpinelinux.org/wiki/Requirements
- musl supported platforms: https://wiki.musl-libc.org/supported-platforms
- Debian ARM ports: https://www.debian.org/ports/arm/
- Debian supported ARM hardware: https://www.debian.org/releases/trixie/arm64/ch02s01.en.html

