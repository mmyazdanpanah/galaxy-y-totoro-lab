# M1 — Minimal Linux Experiment

## Purpose

M1 is now the practical Android-native userspace experiment. It does not replace the Android kernel or boot image.

Immediate objective:

    Android boots normally
          ↓
    ARMv6 userspace compatibility proven
          ↓
    firmware-matched root
          ↓
    minimal Linux rootfs
          ↓
    chroot
          ↓
    network + SSH
          ↓
    userspace Linux running

The former native-boot M1 work is now a separate deferred track; its research records remain preserved.

## Gate 0 — phone inventory

Run the read-only Phase 0 commands from 08_MODERNIZATION/android-chroot-linux.md and preserve the complete output. No rooting or filesystem modification at this gate.

## Gate 1 — static BusyBox before root

Run a statically linked ARMv6-capable BusyBox from /data/local/tmp without root.

    busybox --help
    busybox uname -a
    busybox awk 'BEGIN { print 1.5 * 2.5 }'

Record exact package/version and checksum. Diagnose any execution failure before proceeding.

## Gate 2 — firmware-matched root

Use the actual ro.build.display.id from the phone to select the rooting method/package. Verify provenance and checksum before applying it.

Do not use a generic GT-S5360 root ZIP without build matching.

## Gate 3 — preserve after root

Immediately inspect mount, /proc/mtd, getprop, and /proc/cmdline.

EFS handling is evidence-driven. Do not assume /efs; do not write, erase, or format it.

## Gate 4 — rootfs storage

Try /data/local/alpine first.

If filesystem or permissions make this unsuitable, use a loop-backed image on removable SD. Do not introduce CWM partitioning or repartitioning unless a later separately approved experiment proves it necessary.

## Gate 5 — rootfs and chroot

Use an ARMv6-compatible Alpine userspace selected from official release information available at experiment time. Record exact release and checksum.

Create only the minimum required mount/bind environment for /dev, /dev/pts, /proc, and /sys.

Keep two launch scripts: interactive shell and SSH daemon. Avoid fragile all-purpose Android-shell argument handling.

## Gate 6 — networking and SSH

First prove network reachability from the chroot. Then install the minimum SSH server/client support needed for remote access.

Preferred connection path:

    Mac
      ↓
    adb forward tcp:2222
      ↓
    sshd in chroot

Use SSH keys where practical.

## M1 completion criterion

M1 succeeds when Android continues to boot normally; the Linux rootfs can be entered reproducibly; required /dev, /proc, /sys, and networking interfaces work; package management is usable enough; sshd starts reliably; and the Mac connects over the documented SSH path.

Milestone name: **userspace Linux running**.

## Failure / rollback

At every failure:

    observe → diagnose → smallest reversible fix → retest

If the phone baseline is endangered, stop and restore the known-good Android state. Do not improvise a repartition, EFS repair, modem operation, or bootloader change as a response to a userspace failure.

## Separate native-boot research

The Samsung OSS rebuild, CPUFreq provenance work, boot-image reconstruction, and UART/SBL research remain in the repository as a separate track. They are not prerequisites for M1 and should not be mixed into the first chroot experiment.