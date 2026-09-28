# Status

Phase: MUSEUM baseline frozen → M1-A/M1-B evidence and implementation preparation

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

The preserved decompressed Totoro kernel now contains a confirmed structured AVS/CPUFreq data region at file offset `0x80bf80`. It contains three voltage triplets separated by `0xffffffff` sentinels, followed by six coherent frequency/voltage pairs:

- 156 MHz → 1160 mV
- 312 MHz → 1200 mV
- 468 MHz → 1200 mV
- 624 MHz → 1220 mV
- 832 MHz → 1300 mV
- 1124 → 1320 mV

The binary also contains Broadcom AVS/CPUFreq symbols and diagnostics for OTP silicon classification, FF/TT/SS voltage selection, normal/turbo regulator states, cpufreq table creation, and frequency/voltage transitions.

The `1124` value is **not yet classified as a confirmed exposed cpufreq operating point**. Its code-level consumer must be recovered first. Likewise, the exact semantic labels of the three preceding voltage triplets remain unresolved.

The current evidence supports reconstructing the historical binary implementation before modifying the later Watson `device.c`. See `10_RESEARCH/avs-cpufreq-binary-reconstruction.md`.

## Current modernization state

The archaeology phase has produced enough evidence to move directly toward a controlled Totoro boot experiment.

The shortest reliable route is:

    Samsung/known Totoro kernel
         ↓
    verified Totoro boot geometry
         ↓
    Watson Gingerbread ramdisk
         ↓
    kernel-only substitution
         ↓
    offline verification
         ↓
    controlled boot
         ↓
    minimal Linux userspace
         ↓
    Modern Totoro

The primary target remains M3 — Modern Totoro. Mainline Linux is optional.

## M1-B evidence lock

CM9 Totoro BoardConfig.mk independently records:

- BOARD_KERNEL_BASE = 0x81600000
- BOARD_KERNEL_PAGESIZE = 4096
- BOARD_PAGE_SIZE = 0x1000
- BOARD_KERNEL_CMDLINE = empty
- BOARD_BOOTIMAGE_PARTITION_SIZE = 5242880

This agrees with Samsung and Watson kernel evidence for the Totoro SDRAM base.

Watson GB provides a real Totoro Gingerbread ramdisk and a real unpack/repack workflow. Its Git repository does not contain the referenced boot.img because image files are ignored.

The preserved JPLC1 HOME firmware package contains no boot.img.

Therefore the project will not waste time trying to recover the missing stock boot.img from the JPLC1 package.

Remaining M1-B unknowns:

- kernel offset
- ramdisk offset
- tags offset
- exact historical mkbootimg invocation
- complete compatible boot header

These must be recovered from historical build metadata or an actual compatible Totoro image. They must not be guessed.

## M1-A build environment

Historical Linux ARM EABI 4.4.3 material is available.

Preferred path:

1. contained Linux execution of the historical toolchain;
2. modern reproducible ARM cross-build if the historical host becomes the bottleneck;
3. source changes only for demonstrated compatibility failures.

Phone is not required for M1-A through M1-C.

## Reuse findings

Existing Totoro projects are treated as a parts library:

- Samsung OSS → baseline kernel
- Watson → Gingerbread ramdisk and boot packaging
- CM9 Totoro → concrete device/boot configuration
- AndroidARMv6/CM11 → later hardware/userspace evidence
- other historical Totoro kernels → alternative implementation evidence

See:

- 08_MODERNIZATION/plan.md
- 08_MODERNIZATION/M1-boot-experiment.md
- 10_RESEARCH/totoro-reuse-map.md

## Next actions

1. Recover complete boot-image parameters from historical Totoro build metadata or an actual compatible image.
2. Build Samsung's kernel with the simplest reproducible toolchain path.
3. Reconstruct and unpack a compatible Totoro boot image offline.
4. Substitute only the reproducible kernel.
5. Verify the resulting image structurally.
6. Only then prepare the controlled phone boot.

## Safety boundary

No firmware, bootloader, recovery, or repartitioning operation has been performed.

EFS has not been read, written, erased, or formatted.

Do not write PIT, modem, system, userdata, or EFS during M1.

The Museum baseline remains frozen.

## Known firmware lineage

The observed PDA/CSC correspond to the JPLC1/OJPLC1 Middle East/Arabic stock family. The historically matching package uses modem S5360XXLC1; the specimen currently reports S5360XXLK3. The reason for that combination is not established and is not required for the current M1 path.
