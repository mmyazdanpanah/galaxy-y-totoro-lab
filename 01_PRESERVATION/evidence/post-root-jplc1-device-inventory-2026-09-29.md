# Post-Root JPLC1 Device Evidence — 2026-09-29

## Scope

This checkpoint preserves the first read-only inventory captured immediately after the successful stock-recovery root installation on the physical Samsung Galaxy Y GT-S5360 (totoro), firmware JPLC1.

No additional write, flash, repartition, format, EFS change, modem change, boot-image change, or recovery-image change was performed during this inventory.

## Root state

The phone was already verified rooted before this inventory:

- Model: GT-S5360
- Build: GINGERBREAD.JPLC1
- ADB device: `0123456789ABCDEF device`
- `su -c id`: `uid=0(root) gid=0(root)`

## /proc/mtd

Command:

```sh
su -c 'cat /proc/mtd'
```

Observed output:

```text
dev:    size   erasesize  name
```

The Android kernel exposes the Samsung BML/STL layer rather than a populated conventional `/proc/mtd` table. This empty/header-only result is therefore not evidence that NAND partitions are absent.

## /proc/cmdline

Command:

```sh
su -c 'cat /proc/cmdline'
```

Observed output:

```text
console=ttyS0,115200n8 mem=362M kmemleak=off root=/dev/ram0 rw androidboot.console=ttyS0 mtdparts=bcm_umi-nand:256K@0K(bcm_boot)ro,2048K@256K(loke)ro,2048K@2304K(loke_bk)ro,256K@4352K(systemdata)ro,12800K@4608K(modem)ro,5120K@17408K(param_lfs)rw,5120K@22528K(boot)ro,5120K@27648K(boot_backup)ro,235520K@32768K(system)rw,40960K@268288K(cache)rw,201984K@309248K(userdata)rw,256K@511232K(efs)rw,256K@511488K(sysparm_dep)ro,256K@511744K(umts_cal)ro,1024K@512000K(cal)r BOOT_MODE=0 loglevel=0 BOOT_FOTA=0 DEBUG_LEVEL=LOW
```

### Kernel command-line NAND map

| Partition | Offset | Size | Flag |
|---|---:|---:|---|
| bcm_boot | 0 KiB | 256 KiB | ro |
| loke | 256 KiB | 2,048 KiB | ro |
| loke_bk | 2,304 KiB | 2,048 KiB | ro |
| systemdata | 4,352 KiB | 256 KiB | ro |
| modem | 4,608 KiB | 12,800 KiB | ro |
| param_lfs | 17,408 KiB | 5,120 KiB | rw |
| boot | 22,528 KiB | 5,120 KiB | ro |
| boot_backup | 27,648 KiB | 5,120 KiB | ro |
| system | 32,768 KiB | 235,520 KiB | rw |
| cache | 268,288 KiB | 40,960 KiB | rw |
| userdata | 309,248 KiB | 201,984 KiB | rw |
| efs | 511,232 KiB | 256 KiB | rw |
| sysparm_dep | 511,488 KiB | 256 KiB | ro |
| umts_cal | 511,744 KiB | 256 KiB | ro |
| cal | 512,000 KiB | 1,024 KiB | r |

These are direct JPLC1 runtime observations from the phone's own kernel command line.

## Live mounts

Command:

```sh
su -c 'mount'
```

Observed output:

```text
rootfs / rootfs ro,relatime 0 0
tmpfs /dev tmpfs rw,relatime,mode=755 0 0
devpts /dev/pts devpts rw,relatime,mode=600 0 0
proc /proc proc rw,relatime 0 0
sysfs /sys sysfs rw,relatime 0 0
tmpfs /mnt/asec tmpfs rw,relatime,mode=755,gid=1000 0 0
tmpfs /mnt/obb tmpfs rw,relatime,mode=755,gid=1000 0 0
/dev/stl9 /system rfs ro,relatime,vfat,log_off,check=no,gid/uid/rwx,iocharset=cp437 0 0
/dev/stl10 /cache rfs rw,nosuid,nodev,relatime,vfat,llw,gid/uid/rwx,iocharset=cp437 0 0
/dev/stl6 /mnt/.lfs j4fs rw,relatime 0 0
/dev/stl11 /data rfs rw,nosuid,nodev,relatime,vfat,llw,check=no,gid/uid/rwx,iocharset=cp437 0 0
/dev/block/vold/179:1 /mnt/sdcard vfat rw,dirsync,nosuid,nodev,noexec,relatime,uid=1000,gid=1015,fmask=0702,dmask=0702,allow_utime=0020,codepage=437,iocharset=iso8859-1,shortname=mixed,utf8,errors=remount-ro 0 0
/dev/block/vold/179:1 /mnt/secure/asec vfat rw,dirsync,nosuid,nodev,noexec,relatime,uid=1000,gid=1015,fmask=0702,dmask=0702,allow_utime=0020,codepage=437,iocharset=iso8859-1,shortname=mixed,utf8,errors=remount-ro 0 0
tmpfs /mnt/sdcard/.android_secure tmpfs ro,relatime,size=0k,mode=000 0 0
```

## /proc/partitions

Command:

```sh
su -c 'cat /proc/partitions'
```

Observed output:

```text
major minor  #blocks  name

 179        0   15196160 mmcblk0
 179        1   15195136 mmcblk0p1
 137        0     513024 bml0/c
 137        1        256 bml1
 137        2       2048 bml2
 137        3       2048 bml3
 137        4        256 bml4
 137        5      12800 bml5
 137        6       5120 bml6
 137        7       5120 bml7
 137        8       5120 bml8
 137        9     235520 bml9
 137       10      40960 bml10
 137       11     201984 bml11
 137       12        256 bml12
 137       13        256 bml13
 137       14        256 bml14
 137       15       1024 bml15
 138        6       1280 stl6
 138        9     227840 stl9
 138       10      36864 stl10
 138       11     194816 stl11
```

## Correlation

The following mappings are directly established:

| Kernel partition | BML | STL | Live mount |
|---|---:|---:|---|
| boot | bml7 | — | — |
| boot_backup | bml8 | — | — |
| system | bml9 | stl9 | /system |
| cache | bml10 | stl10 | /cache |
| userdata | bml11 | stl11 | /data |
| efs | bml12 | — | — |
| sysparm_dep | bml13 | — | — |
| umts_cal | bml14 | — | — |
| cal | bml15 | — | — |

The BML numbering and sizes match the kernel command-line partition map exactly. STL reports smaller logical filesystem capacities for the RFS-backed system/cache/userdata volumes, as expected from the Samsung BML/STL layer.

The live device therefore directly establishes the following practical mappings:

- `/dev/stl9` → `/system` → RFS, read-only
- `/dev/stl10` → `/cache` → RFS, read-write
- `/dev/stl11` → `/data` → RFS, read-write
- `/dev/stl6` → `/mnt/.lfs` → j4fs, read-write
- `/dev/block/vold/179:1` → `/mnt/sdcard` → VFAT, read-write

## Interpretation

This is a completed post-root, read-only device checkpoint.

It provides direct JPLC1 evidence for the NAND partition map and the active Android filesystem topology. It also establishes the storage constraints for the next userspace-Linux phase: the Android root filesystem uses Samsung RFS, `/data` is RFS, and removable SD is VFAT.

No inference about unmounted BML roles beyond the kernel command-line names is required for the active chroot track.

## Safety boundary

The following remain unchanged and must remain unchanged during the next phase:

- no PIT/repartition operation
- no bootloader write
- no boot/recovery image replacement
- no modem/radio write
- no EFS write or format
- no userdata wipe
- no direct BML/STL write

The next phase is capability testing and rootfs design, not flashing.

## Checkpoint status

**POST-ROOT READ-ONLY INVENTORY: PASSED**

**ROOT GATE: PASSED**

**NEXT: ARMv6 userspace/rootfs capability evaluation**
