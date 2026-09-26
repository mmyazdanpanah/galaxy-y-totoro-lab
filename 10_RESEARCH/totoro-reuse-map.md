# Totoro Reuse Map

Concrete historical material to reuse or consult before writing new Totoro code.

## 1. Samsung OSS kernel — PRIMARY

Repository: Samsung-OSS-Kernels/android_kernel_samsung_bcm21553

Use for:

- official BCM21553/Totoro baseline
- gt-s5360_gb_opensource
- bcm21553_totoro_05_defconfig
- board code
- original driver lineage
- reproducible kernel build

Decision: baseline; do not replace initially.

## 2. Watson kernel — BOOT/PACKAGING REFERENCE

Repository: sonickles9/watson-kernel-totoro

Useful for:

- historical Totoro kernel lineage
- preserved Totoro ramdisks
- Gingerbread/ICS/KitKat packaging material
- Android Image Kitchen-related tooling
- practical boot-image construction evidence

Decision: inspect before inventing a new ramdisk or boot-image recipe.

## 3. Eve kernel — ALTERNATIVE KERNEL REFERENCE

Repository: zecn/eve_kernel

Useful for:

- independent Totoro kernel lineage
- historical configuration/patch comparison
- alternative ARM toolchain/build assumptions

Decision: reference only unless a concrete Samsung baseline problem appears.

## 4. CM9 / Totoro device tree — DEVICE CONFIGURATION

Repository: percy-g2/android_device_totoro

Useful for:

- BoardConfig.mk
- recovery configuration
- kernel/device relationships
- system properties
- historical device-specific integration

Decision: reference for board/device configuration; not a replacement Linux device tree.

## 5. Samsung/community vendor trees — HARDWARE INTEGRATION

Historical vendor trees such as spacecaker/android_vendor_samsung can provide:

- proprietary hardware integration clues
- firmware/library expectations
- device-specific configuration

Decision: reference only; do not import proprietary Android userspace wholesale.

## 6. AndroidARMv6 / CM11 — LATER HARDWARE EVIDENCE

Historical AndroidARMv6/CM11 work demonstrates that the community adapted BCM21553/Totoro hardware far beyond stock Gingerbread.

Useful for:

- hardware quirks
- Broadcom-specific work
- init/adbd history
- later Android-era driver integration

Decision: mine for hardware knowledge after basic Linux boot.

## 7. XDA — PROCEDURAL/HISTORICAL EVIDENCE

Use old Galaxy Y development threads for:

- recovery procedures
- kernel/ROM relationships
- known failure modes
- what was actually tested on hardware

Decision: use as corroborating community evidence, not as authoritative source code.

## 8. YouTube / Galaxy Y Archive — PROCEDURAL EVIDENCE

Useful for:

- physical flashing/recovery procedures
- historical UI and recovery behavior
- identifying old tools and workflows

Decision: procedural reference only. Verify technical claims against source code or stronger evidence.

## Reuse order

    Samsung source
         ↓
    Watson boot/ramdisk material
         ↓
    other Totoro kernels/device trees
         ↓
    AndroidARMv6 hardware evidence
         ↓
    XDA/YouTube troubleshooting evidence
         ↓
    new code only when no proven material exists

## Important qualification

Historical community material proves that components were used on Totoro; it does not automatically prove compatibility with the preserved specimen.

Every reused artifact must be classified as:

- verified against specimen
- historically demonstrated
- engineering reference
- unverified

That distinction remains part of the laboratory record.
