# Status

Phase: M1-B evidence reconstruction → M1-C implementation preparation

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

The same object contains `0xc0813f24 = 0xc005e3a8`, the address of `cpufreq_bcm_list_states()`, while `cpufreq_bcm_list_states()` itself loads the object address `0xc0813e08`. This is strong evidence that the structure is directly involved in the stock Broadcom CPUFreq implementation.

The complete six-pair sequence occurs exactly once in the decompressed kernel. Absolute references to the first entry occur at `0xc0638cd4` and `0xc08153c0`; the individual interior entries are not referenced as standalone absolute pointers. This is consistent with the table being consumed through its enclosing structure rather than by independent pointers.

The semantic interpretation is now substantially stronger than the earlier "candidate table" classification, but the exact C field layout and the question of whether all six entries are exposed by the runtime cpufreq policy still require code-level confirmation. In particular, `1124` must still be explained: it may be an exposed operating frequency or an internal PLL/divider representation.

The earlier AVS voltage triplets remain present:

- 1360 / 1360 / 1300 mV
- 1320 / 1300 / 1240 mV
- 1320 / 1220 / 1180 mV

Their exact labels remain unresolved.

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

1. Recover/confirm the exact historical boot packaging path sufficiently for a safe test image.
2. Rebuild the Samsung Totoro kernel reproducibly and compare its zImage against preserved evidence.
3. Finish the CPUFreq/AVS code-level reconstruction, especially the exact state-structure layout and `1124` semantics.
4. Construct the first offline test boot image.
5. Unpack/repack/check the result and record hashes.
6. Verify rollback/recovery procedure.
7. Only then perform the first controlled phone boot.

## Safety boundary

No firmware, bootloader, recovery, or repartitioning operation has been performed.

EFS has not been read, written, erased, or formatted.

Do not write PIT, modem, system, userdata, or EFS during M1.

The Museum baseline remains frozen.

## Estimate

For the first meaningful hardware milestone — a reversible, diagnostic Totoro boot using a reproducible kernel and verified boot structure — the project is roughly 70–80% complete in research/preparation terms.

The remaining work is concentrated rather than broad: build reproducibility, boot-image reconstruction, final AVS/CPUFreq interpretation, offline validation, and recovery verification.

For a genuinely useful modern Linux system after that first boot, substantially more work remains. Hardware enablement (storage, framebuffer/display, input, USB, Wi-Fi/networking, audio, battery, suspend/resume and possibly Bluetooth) is a separate engineering phase.

The project should therefore be considered **close to the first controlled hardware experiment, but not yet ready to flash or boot the phone**.
