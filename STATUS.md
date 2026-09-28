# Status

Phase: M1-B evidence reconstruction → M1-C implementation preparation

Latest research pass: 2026-09-29 external-source audit, Samsung OSS Totoro source-baseline extraction, source-to-binary CPUFreq reconciliation, and exact five-defconfig comparison completed.

Device: Samsung Galaxy Y GT-S5360 (totoro)

## Preserved state

- Android 2.3.6
- PDA: S5360JPLC1
- CSC: S5360OJPLC1
- Current baseband: S5360XXLK3
- Build: GINGERBREAD.JPLC1
- Kernel: 2.6.35.7 / dpi@DELL161 #1
- Download Mode: Samsung Official
- Custom binary count: 0
- Live PIT SHA-256: 06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538

## AVS / CPUFreq archaeology checkpoint

The preserved decompressed Totoro kernel now provides strong binary evidence for a six-entry CPU operating-point structure.

The relevant object begins at virtual address `0xc0813e08`. It contains hardware/register descriptor records, function pointers, configuration fields, a state count, and the following unique six-entry sequence at `0xc0813fb8`:

| Entry | Frequency field | Voltage field |
|---:|---:|---:|
| 0 | 156 | 1,160,000 |
| 1 | 312 | 1,200,000 |
| 2 | 468 | 1,200,000 |
| 3 | 624 | 1,220,000 |
| 4 | 832 | 1,300,000 |
| 5 | 1124 | 1,320,000 |

The preceding object field `0xc0813f80 = 6` matches the number of entries.

The same object is still strongly implicated in CPUFreq by the previously recorded function-pointer/reference analysis. However, the public Samsung OSS source now establishes an important correction: its `struct bcm_freq_tbl` is an 8-byte `{cpu_freq, cpu_voltage}` record, but `device.c` contains only two states, 312 MHz/1.20 V and 832 MHz/1.36 V. AVS changes voltages for those two states; it does not add frequency states.

Therefore the preserved six-entry object is structurally compatible with the Samsung record shape but is not the table present in the public Samsung OSS branch. Its provenance must be established by a reproducible zImage comparison. The `1124` field remains unresolved and must not be interpreted as a stock MHz value.

The earlier AVS voltage triplets remain present:

- 1360 / 1360 / 1300 mV
- 1320 / 1300 / 1240 mV
- 1320 / 1220 / 1180 mV

Their exact labels remain unresolved.

## Exact five-defconfig comparison

All five Samsung OSS Totoro defconfigs were parsed directly. Exact adjacent symbol-difference counts are: 02B0→02B1 = 156, 02B1→03 = 6, 03→04 = 12, 04→05 = 8. The meaningful progression is B0/B0-V3D-hack → B1/L2-EVCT → touchscreen/LCD variants → V3D/BBMEM/Wi-Fi-reserved-memory → late Totoro F760/sensor/backlight/ILI9341 selections. Full details are in `10_RESEARCH/cpufreq-defconfig-reconciliation-2026-09-29.md`.

## External-source convergence

A deep audit of all supplied online sources independently corroborated several existing conclusions and added actionable evidence:

- Historical physical GT-S5360 work places the working kernel geometry at `0x81608000` and confirms LZMA kernel compression.
- The same investigation exposes a historical BCM21553 driver surface including `v3d`, `lcd`, `camera`, `bcm_*`, `hx170dec`, `h6270enc`, `mtd`, `bml`, and `stl`.
- Watson demonstrates historical MTD support on Gingerbread, but also reports radio/EFS and recovery limitations; MTD remains an alternative, not the first experiment.
- Merruk Technology supplies Totoro-specific build/compression tooling and `totoro_brcm21553_05_defconfig`.
- Current postmarketOS has dropped ARMv6/armhf package and cross-compiler support, so it should no longer be treated as the immediate userspace target.
- Later Samsung Broadcom bootloader research strengthens the case for a UART/SBL reconnaissance phase before invasive boot-chain work.

Source-access limitations and provenance are recorded explicitly in the external-source audit; inaccessible or incorrectly resolving URLs are not treated as verified evidence.

## Samsung OSS Totoro source checkpoint

A first-party Samsung OSS kernel tree for GT-S5360 has now been verified at branch `gt-s5360_gb_opensource`, HEAD `179772dd` (`Initial import from Samsung opensource package`). The tree explicitly contains five Totoro defconfigs (`02B0`, `02B1`, `03`, `04`, `05`), `board-totoro.c`, `cpu-bcm21553.c`, `cpufreq_bcm21553.c`, and `cpuidle_bcm21553.c`.

The Samsung README explicitly gives `make bcm21553_totoro_05_defconfig` followed by `make`, producing `arch/arm/boot/zImage`. The board source directly documents SDHC1/SDHC2/SDHC3 roles, the OneNAND/eMMC pin-mux constraint, BCM4325 WLAN/BT power/reset handling, and Totoro-specific input/display configuration. The CPU source documents BCM21553 AP/CP shared-memory, interrupt/GPIO, cache, and communications-processor initialization.

The `05` defconfig also enables historical Broadcom display/multimedia paths including `CONFIG_FB_BCM`, `CONFIG_FB_BCM_215XX`, `CONFIG_BCM_DSS`, `CONFIG_BCM215XX_DSS`, and `CONFIG_BRCM_V3D`. This confirms historical Samsung V3D integration; it does not imply modern mainline V3D support.

The source-level CPUFreq implementation now provides a direct primary-source track for reconciling the preserved six-entry binary operating-point structure. The `1124` field remains unresolved and must not be interpreted as MHz until the source conversion logic is mapped.

The nested Samsung source checkout remains external research material and is intentionally not vendored into the main repository.

## Boot-chain evidence

Independent evidence converges on the Totoro boot geometry:

- base: `0x81600000`
- kernel address: `0x81608000`
- page size: 4096
- tags: `0x81600100`
- verified community Totoro boot image: kernel 2,828,168 bytes; ramdisk 2,142,869 bytes

The remaining historical packaging unknowns are the exact stock mkbootimg invocation, stock cmdline/board fields, and complete provenance of the stock boot image. The community image is usable as structural evidence and as an offline packaging reference, but is not claimed to be Samsung stock.

## Current modernization state

The project is now at the transition from archaeology to controlled implementation.

Target loop:

    preserved baseline
         ↓
    reproducible Samsung kernel
         ↓
    verified Totoro boot geometry
         ↓
    offline boot-image reconstruction
         ↓
    structural/hash verification
         ↓
    controlled first phone boot
         ↓
    diagnostics
         ↓
    minimal Linux userspace

The first phone experiment should not yet attempt overclocking, AVS modification, repartitioning, modem work, or a large userspace migration.

## Remaining M1 gates

1. Rebuild the Samsung Totoro kernel reproducibly using the historical `05` configuration/toolchain path.
2. Compare the resulting zImage against the preserved kernel, including the CPUFreq object and configuration-sensitive symbols.
3. Recover/confirm the exact-enough historical boot packaging path for a safe test image.
4. Establish a Totoro-specific UART/SBL reconnaissance plan.
5. Construct the first offline test boot image.
6. Unpack/repack/check the result and record hashes.
7. Verify rollback/recovery procedure.
8. Only then perform the first controlled phone boot.

## Safety boundary

No firmware, bootloader, recovery, or repartitioning operation has been performed.

EFS has not been read, written, erased, or formatted.

Do not write PIT, modem, system, userdata, or EFS during M1.

The Museum baseline remains frozen.

## Estimate

For the first meaningful hardware milestone — a reversible, diagnostic Totoro boot using a reproducible kernel and verified boot structure — the project is roughly 70–80% complete in research/preparation terms.

The remaining work is concentrated rather than broad: reproducible build, zImage provenance comparison, boot-image reconstruction, offline validation, and recovery verification.

For a genuinely useful modern Linux system after that first boot, substantially more work remains. Hardware enablement (storage, framebuffer/display, input, USB, Wi-Fi/networking, audio, battery, suspend/resume and possibly Bluetooth) is a separate engineering phase.

The project should therefore be considered **close to the first controlled hardware experiment, but not yet ready to flash or boot the phone**. The new source audit does not change that safety boundary; it makes the next reconnaissance and implementation steps better specified.
