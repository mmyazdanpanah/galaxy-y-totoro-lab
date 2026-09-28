# Samsung OSS Totoro Source Baseline — 2026-09-29

## Purpose

This note records the primary-source kernel evidence recovered from Samsung's public BCM21553 OSS tree for the GT-S5360 Galaxy Y (Totoro).

Source repository:

- `Samsung-OSS-Kernels/android_kernel_samsung_bcm21553`
- branch: `gt-s5360_gb_opensource`
- HEAD observed during this research pass: `179772dd`
- HEAD message: `Initial import from Samsung opensource package`

The local research checkout lives under `10_RESEARCH/work/samsung-bcm21553/` and remains intentionally outside the main repository history. The main repository records analysis and provenance, not the 610 MB source checkout.

## 1. Direct GT-S5360 build provenance

The Samsung source README explicitly gives the historical GT-S5360 Gingerbread build procedure:

```
cd kernel/common/
make bcm21553_totoro_05_defconfig
make
```

and identifies the resulting kernel as:

```
kernel/common/arch/arm/boot/zImage
```

The README also identifies the historical CodeSourcery Sourcery G++ Lite 2009q3-68 ARM EABI toolchain.

This is stronger than generic BCM21553 build documentation because the target is explicitly GT-S5360.

## 2. Totoro board variants

The source tree contains five Totoro-specific defconfigs:

- `bcm21553_totoro_02B0_defconfig`
- `bcm21553_totoro_02B1_defconfig`
- `bcm21553_totoro_03_defconfig`
- `bcm21553_totoro_04_defconfig`
- `bcm21553_totoro_05_defconfig`

The tree also contains:

- `board-totoro.c`
- `cpu-bcm21553.c`
- `cpufreq_bcm21553.c`
- `cpuidle_bcm21553.c`

This establishes that the Samsung source has both common BCM21553 support and explicit Totoro board support.

The existence of multiple Totoro configurations should not yet be interpreted as a confirmed mapping to retail hardware revisions. That mapping needs to be established from the configuration contents, board identifiers, or matching firmware/kernel provenance.

## 3. Board-level hardware evidence

`board-totoro.c` contains explicit Totoro platform configuration.

Storage:

- SDHC1 is configured as SDIO.
- SDHC2 is configured as eMMC when OneNAND support is not selected.
- SDHC3 is configured as removable SD.
- The source explicitly states that SDHC2 shares pin muxing with OneNAND and that OneNAND and SDHC2 cannot coexist.

Wireless:

- The board code contains BCM4325 Bluetooth/WLAN power/reset handling.
- WLAN uses a dedicated reset/power GPIO path and SDIO host interaction.

Input:

- The board code contains Totoro-specific key mappings and measured headset/key thresholds.

Display:

- The source includes BCM215XX display support and Totoro backlight configuration.

These are primary-source board facts and should take precedence over generic BCM21553 assumptions when reconstructing the historical device.

## 4. CPU / AP-CP architecture

`cpu-bcm21553.c` provides BCM21553-specific CPU initialization, including:

- interrupt-controller setup;
- GPIO-controller initialization;
- AP/CP shared-memory handling;
- IPC shared-memory initialization/clearing;
- communications-processor startup support;
- BCM21553 L2-cache initialization.

This confirms that the historical kernel is not merely a generic ARMv6 port. It contains platform-specific AP/CP and cache initialization for the BCM21553.

## 5. CPUFreq / DVFS — corrected source/binary interpretation

The Samsung tree contains a dedicated BCM21553 CPUFreq implementation. Its public source defines:

```
struct bcm_freq_tbl {
    u32 cpu_freq;      /* in MHz */
    u32 cpu_voltage;   /* in uV */
};
```

The Samsung platform source constructs exactly two CPUFreq states:

| State | Frequency | Voltage |
|---:|---:|---:|
| normal | 312 MHz | 1,200,000 uV |
| turbo | 832 MHz | 1,360,000 uV |

The BCM21553 governor also defines normal=312 MHz and turbo=832 MHz. AVS can replace the voltages for those two states according to silicon bin; it does not add additional frequency states.

The preserved decompressed Totoro kernel contains a previously identified six-record sequence at `0xc0813fb8` inside an object at `0xc0813e08`:

| Entry | Frequency field | Voltage field |
|---:|---:|---:|
| 0 | 156 | 1,160,000 |
| 1 | 312 | 1,200,000 |
| 2 | 468 | 1,200,000 |
| 3 | 624 | 1,220,000 |
| 4 | 832 | 1,300,000 |
| 5 | 1124 | 1,320,000 |

Six 8-byte records are structurally compatible with the public `bcm_freq_tbl` shape, but the six-state contents do not match the two-state Samsung OSS table. The earlier interpretation that the preserved six-state object was simply the public Samsung OSS CPUFreq table is therefore retired.

The stronger conclusion is that the preserved binary and public Samsung source share a related CPUFreq record shape and implementation lineage, while the preserved six-state table comes from a different source/configuration lineage. The `1124` value remains unresolved and must not be treated as a stock Samsung OSS MHz value.

The complete reconciliation and exact five-defconfig comparison are recorded in `10_RESEARCH/cpufreq-defconfig-reconciliation-2026-09-29.md`.

No CPUFreq or AVS modification is appropriate for the first boot.

## 6. Historical display / multimedia surface

The `bcm21553_totoro_05_defconfig` contains historical Broadcom display and multimedia selections including:

- `CONFIG_FB_BCM=y`
- `CONFIG_FB_BCM_215XX=y`
- `CONFIG_BCM_DSS=y`
- `CONFIG_BCM215XX_DSS=y`
- Totoro backlight support
- Broadcom Hantro/graphics wrapper components
- `CONFIG_BRCM_V3D=y`
- Broadcom camera HAL/acquisition support

This is primary-source evidence that the historical Samsung Totoro kernel enabled a Broadcom V3D path.

It does not establish that a modern mainline Linux V3D driver exists for BCM21553. The historical Samsung driver and modern mainline support remain separate questions.

## 7. Storage/filesystem implications

The historical configuration includes Broadcom MMC support and legacy Android/flash-oriented filesystems and mechanisms, including YAFFS/JFFS2/RFS-related support.

The board source's explicit OneNAND/eMMC mux constraint is particularly relevant to the project's existing MTD/BML/STL archaeology.

The historical MTD path remains a fallback. It should not be enabled or tested on the preserved phone until the first reversible boot path is established.

## 8. Boot geometry cross-check

The Samsung source remains consistent with the previously reconstructed Totoro boot geometry:

- SDRAM base: `0x81600000`
- zImage relocation/load address: `0x81608000`
- boot image page size: 4096 bytes

The real historical boot image independently contains `kernel_addr = 0x81608000`.

The agreement between Samsung source and preserved binary evidence is now a three-way convergence:

1. Samsung kernel source;
2. historical community Totoro boot image;
3. independent physical-device investigation.

This substantially increases confidence in the address reconstruction, while leaving the exact stock Samsung mkbootimg invocation unresolved.

## 9. What this source does not prove

The Samsung OSS tree does not by itself prove:

- the exact bootloader/SBL implementation used by the preserved phone;
- the exact stock mkbootimg command;
- that every Totoro defconfig corresponds to a different retail revision;
- modern mainline Linux support for all historical peripherals;
- that a rebuilt zImage will boot unchanged on the preserved device.

Those remain experimental or provenance questions.

## 10. Research consequence

The next implementation sequence is now:

```
Samsung OSS source
       +
preserved Totoro binary evidence
       ↓
reproducible historical kernel build
       ↓
zImage comparison
       ↓
boot-image reconstruction
       ↓
offline structural/hash verification
       ↓
UART/SBL reconnaissance where safely accessible
       ↓
minimal reversible phone boot
```

The source tree should remain a referenced external primary source. Do not vendor the nested Git repository into `galaxy-y-totoro-lab`.
