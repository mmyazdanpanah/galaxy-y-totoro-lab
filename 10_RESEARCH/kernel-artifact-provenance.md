# Kernel Artifact Provenance — Totoro

**Status:** Artifact format and digest verified; source, configuration, and compiler attribution unresolved.  
**Scope:** Read-only inspection of the preserved `totoro-real-kernel.bin` and the associated Watson kernel research. This note does not establish that the artifact was built from any particular checkout.

## Artifact identity

Observed artifact path in the offline lab volume:

```
/Volumes/TotoroBuild/totoro-real-kernel.bin
```

Recorded inspection results:

| Property | Verified result |
|---|---|
| File identification | Linux kernel ARM boot executable, zImage, little-endian |
| File size | 2,828,168 bytes |
| SHA-256 | `251531c44c2763f2b58d36b02941d3b202a3987800a66f7990dabc9d6e59aefd` |
| ARM zImage magic | `0x016f2818` at offset `0x24` (bytes `18 28 6f 01`) |

The zImage magic and file identification support classifying this file as an ARM zImage. They do not identify the source repository, commit, kernel configuration, compiler, build host, or intended boot-image/ramdisk pairing.

## Source and build attribution

**Unresolved — do not attribute this artifact to Watson, Samsung OSS, CM9/CM11, or a named toolchain without further evidence.**

The associated Watson research checkout is:

- Repository: `sonickles9/watson-kernel-totoro`
- Local research path: `/Volumes/TotoroBuild/watson-kernel-totoro`
- Recorded checkout HEAD: `80d0e2db Update .gitignore`
- Kernel source version in that checkout: Linux `2.6.35.14`

The Watson checkout contains historical Totoro configurations and packaging scripts, but the inspected checkout did not contain a built `zImage`, `Image`, or `*.ko` artifact at the searched locations. The presence of related source or configuration is not proof that it produced `totoro-real-kernel.bin`.

The preserved binary inspection did not recover compiler/version strings. Their absence from a compressed zImage is not evidence of which compiler was used.

The Watson automation scripts refer to an ARM EABI Linaro 4.6.2 toolchain at a historical path, while the source Makefile has a 4.4.3 default. A preserved 4.4.3 toolchain was identified as a 32-bit Linux ELF executable and is not directly executable as a native macOS binary. These facts describe available historical build references only; they do not establish the compiler used for this artifact.

## Configuration and hardware qualification

Watson includes several Totoro configurations, including `bcm21553_totoro_02B0_defconfig`, `_02B1`, `_03`, `_04`, `_05`, `_samurai`, and `cyanogenmod_totoro_defconfig`, as well as later `.config.mtd` and `.config.sec` files.

These configurations differ in hardware assumptions and kernel options. The exact physical LCD/touch revision of the preserved handset has not been established by a runtime hardware readout. No specific Watson config should therefore be assigned to the binary based only on its filename, nearby files, or general device compatibility.

## Repository synchronization checkpoint

A local read-only Git check, reported by the operator, established:

- Git root: `/Users/mostafa/Workspace/03_Projects/Engineering/galaxy-y-totoro-lab`
- Remote: `origin → https://github.com/mmyazdanpanah/galaxy-y-totoro-lab.git`
- Branch: `main`
- Local and `origin/main` both at `3bae115` (`Document intentional EFS backup limitation`)
- Ahead/behind count after fetching `origin main`: `0 0`
- Only reported untracked path: `10_RESEARCH/work/`

This is a checkpoint, not a permanent guarantee of synchronization. The untracked work directory was intentionally left untouched. No claim is made here about the contents of that directory.

## Evidence classification

- **Artifact observation:** file type, byte size, SHA-256, and zImage magic as recorded from read-only inspection.
- **Repository evidence:** Watson source/config/script facts describe the inspected historical checkout.
- **Engineering inference:** the binary is a plausible kernel artifact for further offline comparison.
- **Unverified:** exact source commit/tree, config, compiler and version, build command/environment, and original pairing with a ramdisk or boot image.

## Next verification steps

1. Preserve the binary unchanged and verify its SHA-256 again whenever it is copied or analyzed.
2. Compare its embedded kernel version and identifiable symbols/configuration against candidate source trees, without treating a match in isolation as proof of provenance.
3. If a candidate build is performed, record the exact source commit, config hash, toolchain identity, build command, environment, and resulting artifact hash.
4. Keep this artifact as a reference until a reproducible build can be independently compared with it.

No flashing, repacking, source attribution, or modification of the preserved binary is implied by this record.
