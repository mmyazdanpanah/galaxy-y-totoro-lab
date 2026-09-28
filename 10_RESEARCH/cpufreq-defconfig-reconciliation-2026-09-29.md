# CPUFreq Source-to-Binary Reconciliation and Totoro Defconfig Comparison — 2026-09-29

## Scope

This pass reconciles the preserved Totoro CPUFreq/AVS binary evidence against the first-party Samsung OSS source and performs an exact symbol-level comparison of all five Totoro defconfigs.

Primary source:

- Samsung-OSS-Kernels/android_kernel_samsung_bcm21553
- branch: `gt-s5360_gb_opensource`
- source HEAD: `179772dd` — `Initial import from Samsung opensource package`

The comparison was performed from the five repository defconfig files as text, parsing both `CONFIG_X=value` and `# CONFIG_X is not set` forms. No local nested-repository modifications were changed.

## 1. CPUFreq source model

The key source files are:

- `kernel/common/arch/arm/plat-bcmap/include/plat/bcm_cpufreq_drv.h`
- `kernel/common/arch/arm/plat-bcmap/bcm_cpufreq.c`
- `kernel/common/arch/arm/mach-bcm215xx/device.c`
- `kernel/common/arch/arm/mach-bcm215xx/cpufreq_bcm21553.c`
- `kernel/common/arch/arm/mach-bcm215xx/include/mach/bcm21553_cpufreq_gov.h`

The first important result is structural:

`struct bcm_freq_tbl` is exactly two 32-bit fields:

```
struct bcm_freq_tbl {
    u32 cpu_freq;      /* in MHz */
    u32 cpu_voltage;   /* in uV */
};
```

Therefore one table record is 8 bytes.

The Samsung BCM21553 platform source constructs this table in `device.c` as exactly two entries:

```
FTBL_INIT(BCM_CORE_CLK_NORMAL / 1000, 1200000),
FTBL_INIT(BCM_CORECLK_TURBO / 1000, 1360000),
```

with:

- normal = 312 MHz / 1,200,000 uV
- turbo = 832 MHz / 1,360,000 uV

The driver converts each source MHz value to cpufreq kHz by multiplying by 1000. The voltage lookup converts the selected kHz value back to MHz before looking up the table.

The governor separately exposes only the normal/turbo pair:

- normal: 312,000 kHz
- turbo: 832,000 kHz

The source therefore gives a very precise historical interpretation of the Samsung OSS CPUFreq path.

## 2. AVS interaction

The same `device.c` contains AVS silicon-bin voltage definitions. The AVS callback can replace the normal/turbo voltages at runtime.

The source defines:

| Silicon bin | Normal | Turbo |
|---|---:|---:|
| slow | 1.30 V | 1.36 V |
| typical | 1.24 V | 1.30 V |
| fast | 1.18 V | 1.22 V |

These are runtime voltage adjustments to the two-entry CPUFreq table; they do not add additional frequency states.

This distinction matters: the Samsung OSS source has a two-frequency CPUFreq table even though the preserved binary evidence previously identified a six-record sequence.

## 3. Reconciliation with the preserved six-record binary object

The preserved decompressed Totoro kernel was previously mapped to an enclosing object at `0xc0813e08`, with a count of 6 and a six-record sequence at `0xc0813fb8`:

| Entry | Binary field 0 | Binary field 1 |
|---:|---:|---:|
| 0 | 156 | 1,160,000 |
| 1 | 312 | 1,200,000 |
| 2 | 468 | 1,200,000 |
| 3 | 624 | 1,220,000 |
| 4 | 832 | 1,300,000 |
| 5 | 1124 | 1,320,000 |

Six records × 8 bytes = 48 bytes, so the record width is compatible with the source `struct bcm_freq_tbl` shape.

But the content does **not** match the Samsung OSS source table:

- Samsung OSS source: exactly 2 entries, 312 and 832.
- Preserved binary candidate: 6 entries, 156/312/468/624/832/1124.
- Samsung OSS source's default voltage pair is 1.20 V / 1.36 V.
- The preserved six-record sequence contains 1.16/1.20/1.20/1.22/1.30/1.32 V.

Therefore the earlier statement that the six-entry object was simply the Samsung OSS CPUFreq table must be downgraded. The stronger conclusion is:

> The preserved binary object has the same 8-byte frequency/voltage record shape as the Samsung CPUFreq source, and it is referenced by CPUFreq-related code, but its six-state contents are not the table present in the public Samsung OSS branch.

This is an important archaeology result rather than a failure. It tells us that the preserved kernel and the public Samsung OSS tree are not source-identical at this CPUFreq table.

The `1124` field consequently remains unresolved. It must not be treated as a stock Samsung OSS MHz value.

Historical community kernels for Galaxy Y/Totoro-family BCM21553 devices later advertised five or six selectable frequencies, showing that multi-state CPUFreq tables existed in the wider kernel ecosystem. That is corroborating context only; it does not identify the provenance of the preserved six-state object.

## 4. Consequence for the rebuild

The Samsung OSS source is still the correct primary historical rebuild baseline, but the first reproducible build should **not** be expected to reproduce the preserved six-entry CPUFreq object byte-for-byte.

The correct experiment is now:

1. build the Samsung OSS `05` configuration unchanged;
2. extract its resulting zImage;
3. compare the generated CPUFreq-related structures against the preserved kernel;
4. determine whether the six-state object belongs to a later Samsung-private/custom branch, a vendor patch, or another kernel lineage;
5. only then decide whether any six-state behavior is relevant to the modernization target.

No CPUFreq or AVS changes should be introduced in the first boot.

## 5. Exact five-defconfig comparison

Raw file sizes:

| Defconfig | Bytes |
|---|---:|
| 02B0 | 51,805 |
| 02B1 | 54,441 |
| 03 | 54,487 |
| 04 | 54,793 |
| 05 | 54,901 |

All five are Linux 2.6.35.7-era Totoro configurations.

### 5.1 Common architectural baseline

All five select:

- ARMv6 / ARM 32-bit v6K
- BCM215XX platform
- Totoro board
- BCM CPUFreq
- BCM21553 CPU idle
- SDRAM base `0x81600000`
- 3 GiB-style virtual split / 362 MiB kernel memory command-line reservation
- Samsung/Broadcom Android kernel infrastructure
- Broadcom framebuffer/display stack
- Broadcom V3D path
- BCM MMC
- Android binder/logger/timed-output infrastructure
- YAFFS/JFFS2/RFS-era storage support

Thus the five files are not five different SoC ports. They are closely related Totoro hardware/configuration variants.

### 5.2 02B0 → 02B1: major baseline transition

The principal architectural change is:

| Symbol | 02B0 | 02B1 |
|---|---|---|
| `CONFIG_ARCH_BCM21553_B0` | y | n |
| `CONFIG_ARCH_BCM21553_B1` | n | y |
| `CONFIG_BCM21553_B0_V3D_HACK` | y | absent |
| `CONFIG_MM_MEMPOOL_BASE_ADDR` | 0x94800000 | 0 |
| `CONFIG_TOUCHSCREEN_CYTTSP_CORE` | y | n |
| `CONFIG_TOUCHSCREEN_CYTTSP_I2C` | y | absent |
| `CONFIG_TOUCHSCREEN_TMA340` | n | y |
| `CONFIG_BCM_LCD_S6D04H0A01` | y | y |
| `CONFIG_BRCM_CNTIN` | n | y |
| `CONFIG_BCM21553_L2_EVCT` | absent | y |
| `CONFIG_BCM21553_V3D_SYNC_ENABLE` | absent | n |

There are also many generic networking/MTD/PPP symbols added between the two generated configurations. Those are configuration-surface changes, not evidence of a different physical board by themselves.

### 5.3 02B1 → 03

Only six parsed symbol differences occur:

| Symbol | 02B1 | 03 |
|---|---|---|
| `CONFIG_TOUCHSCREEN_SYNAPTICS_I2C_RMI` | n | absent |
| `CONFIG_TOUCHSCREEN_TMA340` | y | n |
| `CONFIG_BCM_LCD_SKIP_INIT` | n | y |
| `CONFIG_TOUCHSCREEN_TAT200A` | absent | n |
| `CONFIG_TOUCHSCREEN_MMS128` | absent | y |
| `CONFIG_TOUCHSCREEN_TMA140` | absent | y |

This is strong evidence that 02B1 and 03 are closely related configurations whose meaningful hardware delta is primarily touchscreen/display initialization.

### 5.4 03 → 04

The meaningful changes are:

| Symbol | 03 | 04 |
|---|---|---|
| `CONFIG_TOUCHSCREEN_MMS128` | y | absent |
| `CONFIG_BCM_LCD_S6D04H0A01` | y | n |
| `CONFIG_BCM_LCD_SKIP_INIT` | y | n |
| `CONFIG_ARGB8888` | n | y |
| `CONFIG_BRCM_V3D_OPT` | absent | y |
| `CONFIG_BMEM` | absent | y |
| `CONFIG_BMEM_WRAP` | absent | y |
| `CONFIG_BROADCOM_WIFI_RESERVED_MEM` | absent | y |
| `CONFIG_TOUCHSCREEN_MMS128_REV04` | absent | y |
| `CONFIG_TOUCHSCREEN_F760` | absent | n |
| `CONFIG_BCM_LCD_ILI9341_BOE` | absent | y |
| `CONFIG_DPRAM` | absent | n |

This is the first clear shift toward a different display/GPU memory configuration.

### 5.5 04 → 05

The final variant changes are compact:

| Symbol | 04 | 05 |
|---|---|---|
| `CONFIG_TOUCHSCREEN_MMS128_REV04` | y | n |
| `CONFIG_TOUCHSCREEN_F760` | n | y |
| `CONFIG_BCM_LCD_ILI9341_BOE` | y | n |
| `CONFIG_ARGB8888` | y | n |
| `CONFIG_SENSORS_TOTORO` | absent | y |
| `CONFIG_BACKLIGHT_TOTORO` | absent | y |
| `CONFIG_BCM_LCD_ILI9341_BOE_REV05` | absent | y |
| `CONFIG_BCM_LCD_ILI9341_CPT` | absent | y |

The `05` configuration is therefore not merely a renamed copy of `04`: it changes touchscreen, LCD, pixel-format, sensor, and backlight selections.

## 6. Exact comparison totals

The parsed symbol-level comparison gives:

- 02B0 vs 02B1: 156 differing symbols
- 02B1 vs 03: 6 differing symbols
- 03 vs 04: 12 differing symbols
- 04 vs 05: 8 differing symbols

The large 02B0→02B1 delta is dominated by the transition from the older B0 configuration and a broader/generated configuration surface. The later configurations are much more tightly related.

The exact comparison therefore supports treating `05` as the direct historical build target named by Samsung's README, while retaining `02B0` as an earlier BCM21553 B0 hardware/configuration lineage and `02B1`→`04` as intermediate B1 variants.

It still does **not** prove which configuration exactly matches the preserved JPLC1 phone. That requires binary/config provenance correlation.

## 7. New archaeology conclusion

Two previous assumptions are now refined:

1. The Samsung OSS CPUFreq implementation is a **two-state** 312/832 MHz source table, not a six-state table.
2. The preserved six-state binary object is a **related but non-identical CPUFreq structure** whose provenance must be established by zImage/build comparison.

The five defconfigs also reveal that the main variant evolution is concentrated in SoC revision, touchscreen, LCD, V3D-memory, and late Totoro sensor/backlight configuration rather than in the basic CPU/storage architecture.

This makes the next build experiment substantially more precise: build `05` unchanged, then use the resulting zImage as the source-controlled reference against the preserved kernel.

## 8. Next step

The next archaeology/implementation step is no longer broad source hunting. It is a reproducible build-and-diff experiment:

```
Samsung OSS 179772dd
        |
        +-- bcm21553_totoro_05_defconfig
        |
        +-- historical toolchain
        |
        v
reproducible zImage
        |
        +-- boot image structural comparison
        +-- symbol/string comparison
        +-- CPUFreq object comparison
        +-- compressed kernel/hash comparison
        v
source ↔ binary provenance map
```

Only after that comparison should the project decide whether the preserved six-state CPUFreq object comes from a Samsung-private build, a later vendor branch, or a custom kernel lineage.
