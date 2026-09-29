# Root Package Evidence Gate — 2026-09-29

## Scope

This checkpoint records the offline audit of the historical `update.zip` candidate and the corresponding read-only checks against the physical GT-S5360 JPLC1 handset. No handset write has occurred.

## Verified artifact

- File: `update.zip`
- Size: 2,260,360 bytes
- MD5: `eac189609fd71de6bf053e7ff2636d7e`
- SHA-1: `89108755e3cf1d6c298e60fc963881dacb3d313d`
- SHA-256: `3e4ebe31b908ea3a8750347f875f91493f550edd1cd2a3006293c45a41592a27`
- ZIP integrity: passed
- Entries: 11

The package contains:

- `system/xbin/su`
- `system/xbin/busybox`
- `system/xbin/ssh`
- `system/xbin/sqlite3`
- `system/app/Superuser.apk`
- Android update metadata, manifest, signature, and updater binary.

The artifact is preserved under `10_RESEARCH/work/root-research/original-lineage/`. Raw research binaries remain work artifacts unless explicitly committed.

## Updater-script audit

The active updater script asserts that at least one product/device property equals an accepted historical model, including `GT-S5360`.

The active operations are:

1. `package_extract_dir("system", "/system")`
2. `set_perm(0, 0, 04755, "/system/xbin/sqlite3")`
3. `set_perm(0, 0, 04755, "/system/xbin/su")`
4. `set_perm(0, 0, 04755, "/system/xbin/ssh")`
5. `set_perm(0, 0, 04755, "/system/xbin/busybox")`
6. `unmount("/system")`

The `format("MTD", "system")` and `mount("MTD", "system", "/system")` lines are commented out and therefore are not active script operations.

No active `write_raw_image`, `run_program`, modem, boot, recovery, EFS, repartitioning, or partition-formatting command was identified in the updater script.

### Important distinction

The bundled `update-binary` is a recovery updater executable with broader capabilities than this particular script uses. Offline string/symbol inspection shows support for formatting, mounting, deleting, package extraction, raw image writing, program execution, and Samsung BML/STL-related operations. Those capabilities are not by themselves evidence that the current updater script will invoke them.

## Payload architecture

Offline `file` inspection:

| Payload | Architecture / linkage | SHA-256 |
|---|---|---|
| busybox | ARM 32-bit EABI4, statically linked, stripped | `f765c41eae0a56c67574ee191371fe95de287d5edb3e527d465fd973c351b4cf` |
| ssh | ARM 32-bit EABI5, dynamically linked, interpreter `/system/bin/linker` | `2e28f8c6ee0158c7e32427330d1a5b28e47d64a4607d49c081fb8ecf3000c77e` |
| sqlite3 | ARM 32-bit EABI5, dynamically linked, interpreter `/system/bin/linker` | `1eb31a54668452b1ce89699f839850259b97a2b89ca8c1ec2c81fffadd519643` |
| su | ARM 32-bit EABI5, dynamically linked, interpreter `/system/bin/linker` | `b37a4b1c1abfe6e87cc8b81cf0b2158447d0b5e9e4cf429a990033e689c73c96` |
| Superuser.apk | Android/JAR package | `cf56038e2580cff4307cddd762a2596f96894bd33bdc327eb7b5781435e3d91f` |

The ARM payloads are not obviously incompatible with the ARMv6 handset at the ELF-header level. This is compatibility evidence, not proof of successful execution.

## Metadata and provenance caveat

`META-INF/com/android/metadata` contains:

- `pre-device=tass`
- `post-build=google/passion/passion:2.3.3/GRI40/102588:user/release-keys`
- `post-timestamp=0`

These values do not correspond to the target GT-S5360 JPLC1 Android 2.3.6 build. The package therefore remains a generalized/repackaged historical root artifact rather than exact JPLC1-specific proof.

The certificate is an Android signing certificate with:

- Subject: Android / Mountain View, California
- Issuer: same Android identity
- Not Before: Feb 29 2008

This generic certificate identity is not treated as independent provenance proof.

## Physical handset destination check

The interactive ADB shell remained:

`uid=2000(shell)`

The following direct read-only checks returned `No such file or directory`:

- `/system/xbin/su`
- `/system/xbin/busybox`
- `/system/xbin/ssh`
- `/system/xbin/sqlite3`
- `/system/app/Superuser.apk`

This is now a meaningful destination check; unlike an earlier shell attempt that produced permission-denied ambiguity, the individual `ls -l` calls returned explicit absence for these paths.

The combined `ls -ld` command used in the same capture was malformed for the phone's old `ls` implementation (`-ld` was interpreted as a filename). That does not invalidate the individual target-file results.

## Final read-only partition mapping

The final live-device mapping pass was completed on the physical JPLC1 handset.

### Directly observed

- `bml0/c`: 513,024 KiB
- `bml1`: 256 KiB
- `bml2`: 2,048 KiB
- `bml3`: 2,048 KiB
- `bml4`: 256 KiB
- `bml5`: 12,800 KiB
- `bml6`: 5,120 KiB
- `bml7`: 5,120 KiB
- `bml8`: 5,120 KiB
- `bml9`: 235,520 KiB
- `bml10`: 40,960 KiB
- `bml11`: 201,984 KiB
- `bml12`: 256 KiB
- `bml13`: 256 KiB
- `bml14`: 256 KiB
- `bml15`: 1,024 KiB

The handset exposes corresponding `stl1` through `stl15` device nodes, with `stl6`, `stl9`, `stl10`, and `stl11` actively mounted.

### Confirmed filesystem mappings

- `stl6` → `/mnt/.lfs` using j4fs
- `stl9` → `/system` using RFS, read-only
- `stl10` → `/cache` using RFS, read-write
- `stl11` → `/data` using RFS, read-write
- `mmcblk0p1` → `/mnt/sdcard` using VFAT

The live `/proc/partitions` output reports no conventional MTD partitions, while the Samsung BML/STL device layer is fully present. The empty `/proc/mtd` output is therefore not evidence that the NAND partitioning is absent.

### Kernel/recovery cross-check

The JPLF1 SBL inspection independently contains explicit strings for:

- `kernel partition`
- `recovery partition`
- `loke partition`
- `pit partition`
- `Cannot find the recovery partition!`
- `Cannot find the kernel partition!`
- `Kernel read success from kernel partition`
- `KERNEL`
- `loke`
- `loke_bk`

This independently confirms that the Samsung SBL model has distinct kernel and recovery partition concepts. The live JPLC1 device's `bml7` and `bml8` are therefore retained as the kernel/recovery candidates, but their exact roles are **not promoted to JPLC1-specific proof** until an exact JPLC1 PIT or equivalent partition-table evidence is archived and decoded.

Likewise, no exact role is asserted here for `bml3`, `bml4`, `bml5`, or `bml12`–`bml15` beyond their observed existence and sizes.

The attempted sysfs and recursive recovery-string searches were blocked by the unprivileged Android shell's permission restrictions and are not used as evidence.

## Decision gate

### Established

- Artifact hash and ZIP integrity are verified.
- GT-S5360 is explicitly accepted by the updater assertion.
- Active updater-script operations are understood.
- No active formatting, repartitioning, modem, boot/recovery, or EFS operation was found in the script.
- Payload ELF architecture is consistent with this ARMv6-era platform at the header level.
- Target payload paths are currently absent.
- The live handset's BML/STL partition architecture is directly observed.
- `/system`, `/cache`, `/data`, and `/mnt/.lfs` mappings are directly confirmed.
- JPLF1 SBL independently confirms separate kernel/recovery partition concepts.
- No phone write has occurred.

### Still unresolved

- Exact compatibility with `GINGERBREAD.JPLC1` / `S5360JPLC1` is not proven.
- Package metadata identifies `tass` / `passion` rather than JPLC1.
- The recovery updater binary has broad low-level capabilities even though the present script does not invoke the dangerous operations.
- Exact JPLC1 PIT/recovery partition provenance has not yet been independently archived and decoded.
- Exact roles of the remaining unmounted BML partitions are not yet proven from JPLC1-specific partition-table evidence.

### Current disposition

**Root experiment may now be prepared, but no direct partition or boot-chain write is authorized.**

The verified historical `update.zip` remains a candidate for a stock-recovery `Apply update from SD` experiment only. Do not use Odin, PIT flashing, modem flashing, bootloader flashing, boot-image flashing, recovery-image flashing, or direct BML/STL writes as part of this experiment.

The first experiment remains intentionally narrow: use the existing stock recovery to apply the verified package, then verify whether the expected five payload files appear with the expected setuid permissions and whether root access is actually obtained.

If the package fails its device/build assertion or otherwise aborts before installation, stop and preserve the recovery output; do not bypass assertions or substitute a different binary.

No PIT, EFS, modem, bootloader, userdata, partition format, or boot-image write is authorized by this checkpoint.

## Next milestone

After a successful root experiment, preserve the rooted JPLC1 state and move directly to the ARMv6 Linux userspace/chroot track. Native Linux boot remains a separate research track and does not block the first practical Linux userspace milestone.
