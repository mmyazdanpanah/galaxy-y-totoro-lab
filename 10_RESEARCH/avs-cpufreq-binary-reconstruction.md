# AVS / CPUFreq Binary Reconstruction — Totoro JPLC1

**Status:** Strong structured CPUFreq/AVS object identified; exact consumer semantics still being reconstructed.  
**Scope:** Preserved decompressed Totoro kernel image `totoro-real-decompressed.bin`. No source modification or flashing is implied.

## Evidence identity

The analyzed kernel is the preserved real Totoro kernel artifact:

- SHA-256: `251531c44c2763f2b58d36b02941d3b202a3987800a66f7990dabc9d6e59aefd`
- Source zImage artifact: `totoro-real-kernel.bin`
- Decompressed analysis image: `totoro-real-decompressed.bin`
- Kernel load geometry previously established: page size 4096; kernel address `0x81608000`; ramdisk address `0x82600000`; tags `0x81600100`.

## Driver evidence

The binary contains Broadcom AVS/CPUFreq diagnostic and symbol strings including:

- `bcm_avs_drv_probe`
- `_bcm_avs_get_silicon_type`
- `bcm_avs.silicon_otp_val`
- `bcm_avs.silicon_type`
- `bcm_create_cpufreqs_table`
- `bcm_cpufreq_set_speed`
- `bcm_cpufreq_get_speed`
- `bcm_cpufreq_init`
- `bcm_cpufreq_verify_speed`
- `bcm_cpufreq_exit`
- `cpufreq_bcm_dvfs_enable`
- `cpufreq_bcm_dvfs_disable`

The binary also contains AVS voltage-selection strings:

- `nm2_ff_voltage_turbo`
- `nm2_tt_voltage_turbo`
- `nm2_ss_voltage_turbo`
- `nm2_ff_voltage_normal`
- `nm2_tt_voltage_normal`
- `nm2_ss_voltage_normal`
- `nm_ff_voltage`
- `nm_tt_voltage`
- `nm_ss_voltage`
- `avs_otp_threshold_sstt`
- `avs_otp_threshold_ttff`

These strings establish the presence of a Broadcom AVS + CPUFreq/DVFS implementation, including silicon/process-corner selection. They do not by themselves prove the source-level data structure.

## Confirmed CPUFreq/AVS data structure

A much stronger result was obtained by searching the complete decompressed kernel for the six candidate frequency/voltage pairs.

The exact 48-byte sequence occurs **once**:

```
0xc0813fb8:
    156   1160000
    312   1200000
    468   1200000
    624   1220000
    832   1300000
    1124  1320000
```

The surrounding object begins at `0xc0813e08`.

Immediately before the table:

```
0xc0813f80 = 6
```

which matches the number of entries.

The object also contains:

```
0xc0813f24 = 0xc005e3a8
```

and `0xc005e3a8` is the entry address of `cpufreq_bcm_list_states()`.

Conversely, the literal pool used by `cpufreq_bcm_list_states()` contains:

```
0xc005e820 = 0xc0813e08
```

Therefore the relationship is bidirectional at the binary-structure level:

```
cpufreq_bcm_list_states()
        │
        └── loads object 0xc0813e08
                         │
                         ├── state count = 6
                         │
                         └── six frequency/voltage entries
```

This is strong evidence that the object belongs to the stock Broadcom CPUFreq implementation rather than being an unrelated calibration table.

## Enclosing structure observations

The object beginning at `0xc0813e08` starts with repeated hardware/register descriptors:

```
f88800d8, string pointer, 1, flags
f88800dc, string pointer, 2, flags
f88800e0, string pointer, 3, flags
f88800e4, string pointer, 4, flags
...
```

Later fields include:

```
0xc0813f10 = 1
0xc0813f14 = 0xc005dec4
0xc0813f24 = 0xc005e3a8
0xc0813f38 = 0x00010101
0xc0813f3c = 0xf88ce000
0xc0813f80 = 6
```

This looks like a vendor-specific platform/CPUFreq configuration object containing register descriptors, callbacks, state metadata and operating-point data.

The exact C structure should not yet be reconstructed solely from these observations.

## Exact table values

| Entry | Frequency field | Voltage field | Provisional interpretation |
|---:|---:|---:|---|
| 0 | 156 | 1,160,000 | likely 156 MHz / 1160 mV |
| 1 | 312 | 1,200,000 | likely 312 MHz / 1200 mV |
| 2 | 468 | 1,200,000 | likely 468 MHz / 1200 mV |
| 3 | 624 | 1,220,000 | likely 624 MHz / 1220 mV |
| 4 | 832 | 1,300,000 | likely 832 MHz / 1300 mV |
| 5 | 1124 | 1,320,000 | frequency semantics unresolved |

The voltage fields are unambiguous integer values consistent with microvolts. The frequency fields are structurally paired with them, but the precise unit/representation of the final `1124` value still requires code-level confirmation.

## Address/reference evidence

Absolute 32-bit references to the enclosing object `0xc0813e08` occur at:

```
0xc000f1c4
0xc000f524
0xc005dbe8
0xc005dea8
0xc005dfdc
0xc005e370
0xc005e820
0xc005ed4c
0xc0638ca4
0xc078ed80
```

The first table entry address `0xc0813fb8` has absolute references at:

```
0xc0638cd4
0xc08153c0
```

The interior entries do not appear as standalone absolute pointers. This is consistent with access through the enclosing structure or indexed table arithmetic.

The six-entry sequence itself occurs only once in the kernel.

## AVS characterization data

A separate region still exists at file offset `0x80bf80`:

```
0x80bf80  6
0x80bf88  ffffffff
0x80bf8c  1360000
0x80bf90  1360000
0x80bf94  1300000
0x80bf98  ffffffff
0x80bf9c  1320000
0x80bfa0  1300000
0x80bfa4  1240000
0x80bfa8  ffffffff
0x80bfac  1320000
0x80bfb0  1220000
0x80bfb4  1180000
```

These triplets remain provisionally associated with FF/TT/SS silicon/process-corner characterization because the binary contains matching AVS selection strings. Their exact labels are not yet proven.

Do not conflate this separate characterization region with the newly identified CPUFreq object at `0xc0813e08`.

## Current interpretation

The strongest current model is:

```
OTP value
   ↓
silicon-type classification
   ↓
FF / TT / SS voltage selection
   ↓
normal / turbo voltage state
   ↓
CPUFreq configuration object
   ↓
state count + frequency/voltage entries
   ↓
cpufreq table setup
   ↓
clock + regulator transition
```

The binary now gives strong evidence for the CPUFreq configuration object and its six entries. The remaining work is to recover the exact state-building code and determine whether the runtime policy exposes all six entries.

## Next verification

1. Disassemble the functions referencing `0xc0813e08`, especially the code around `0xc005dec4`, `0xc005e3a8`, and `0xc005f048`.
2. Track accesses to object offsets `0x178` and `0x1b0`.
3. Recover the exact state-array construction and filtering logic.
4. Determine the semantic representation of `1124`.
5. Recover the historical boot packaging invocation.
6. Build and validate the first reproducible kernel/boot image offline.
7. Only after offline validation and rollback verification, prepare the controlled phone experiment.

No source modification, boot-image modification, flashing, or repartitioning is implied by this record.
