# AVS / CPUFreq Binary Reconstruction — Totoro JPLC1

**Status:** Read-only reverse-engineering checkpoint; historical AVS/CPUFreq data structure identified, consumer function unresolved.  
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

Runtime diagnostic strings identify separate Core LPM, Normal, and Turbo regulator paths and explicit CPU voltage/frequency changes.

These strings establish the presence of a Broadcom AVS + CPUFreq/DVFS implementation. They do not, by themselves, establish the exact source implementation.

## Recovered structured data

At file offset `0x80bf80`, the decompressed kernel contains a structured region beginning with:

```
0x80bf80  0x00000006
0x80bf84  0x00000000
0x80bf88  0xffffffff
0x80bf8c  1360000
0x80bf90  1360000
0x80bf94  1300000
0x80bf98  0xffffffff
0x80bf9c  1320000
0x80bfa0  1300000
0x80bfa4  1240000
0x80bfa8  0xffffffff
0x80bfac  1320000
0x80bfb0  1220000
0x80bfb4  1180000

0x80bfb8  156
0x80bfbc  1160000
0x80bfc0  312
0x80bfc4  1200000
0x80bfc8  468
0x80bfcc  1200000
0x80bfd0  624
0x80bfd4  1220000
0x80bfd8  832
0x80bfdc  1300000
0x80bfe0  1124
0x80bfe4  1320000
```

The trailing six pairs form a highly coherent frequency/voltage table:

| Entry | Frequency value | Voltage |
|---:|---:|---:|
| 0 | 156 MHz | 1160 mV |
| 1 | 312 MHz | 1200 mV |
| 2 | 468 MHz | 1200 mV |
| 3 | 624 MHz | 1220 mV |
| 4 | 832 MHz | 1300 mV |
| 5 | 1124 | 1320 mV |

The first 11 values before that table form three voltage triplets separated by `0xffffffff` sentinels:

```
1360  1360  1300
1320  1300  1240
1320  1220  1180
```

Their exact semantic labels remain unresolved, but their correspondence with the binary's FF/TT/SS and normal/turbo AVS strings makes silicon/process-corner characterization a strong hypothesis.

**Important:** the `1124` value is not yet classified as a user-visible cpufreq operating point. It may be a PLL/divider/internal frequency representation. Code-level consumption must confirm this.

## Addressing evidence

Multiple references encode `0xc080c000`, and the observed kernel data around file offset `0x80c000` is therefore consistent with a file-to-kernel-virtual mapping of:

```
virtual address = file offset + 0xc0000000
```

for this region.

The same search did **not** find direct references to `0xc080bf80`, `0xc080bfb8`, `0xc080bfe8`, or `0xc080c000` as simple pointers to the table start/interior, except for references to `0xc080c000`.

This means the AVS structure should not be assumed to be reached through a simple global pointer to `0x80bf80`. PC-relative access, an enclosing structure, linker-generated tables, or another indirection remain possible.

The large pointer table around file offset `0x630a10` contains repeated kernel virtual addresses beginning with:

```
c080ba80
c080c000
c080e000
c080e740
c080f560
c0810118
c0810318
...
```

This appears to be a linker/kernel address table or section-related structure and should not be conflated with the AVS table merely because it contains `c080c000`.

## Kallsyms evidence

The decompressed kernel contains the string `_stext` and extensive kallsyms-related material, including:

- `kallsyms`
- `kernel/kallsyms.c`
- `kallsyms_on_each_symbol`
- `kallsyms_lookup_name`

The region around file offset `0x773e00` is clearly populated with kernel symbol/name strings such as `__module_`, `module_get`, `module_put`, `__print_symbol`, and `kallsyms_lookup_name`. It is therefore not itself the executable code for kallsyms.

The Broadcom driver names are present as strings, but simple searches for direct pointers to their string addresses did not recover function references. This is consistent with the possibility that the embedded kallsyms representation is compressed/indirect rather than a plain pointer-to-name table.

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
CPU frequency/voltage table
   ↓
cpufreq table setup
   ↓
clock + regulator transition
```

The exact C structures, function addresses, and state-selection logic remain to be reconstructed from code.

The later Watson `device.c` must remain unchanged until this historical binary implementation is recovered sufficiently to distinguish confirmed behavior from later reconstruction.

## Next verification

1. Establish the exact kernel virtual/file mapping around the AVS data.
2. Identify code that consumes the distinctive voltage triplets or six frequency entries.
3. Recover the enclosing structure and its field offsets.
4. Recover the actual addresses of `bcm_create_cpufreqs_table`, `bcm_cpufreq_set_speed`, and `_bcm_avs_get_silicon_type` from kallsyms or executable references.
5. Confirm whether `1124` is an operating frequency or an internal PLL/divider value.
6. Only then compare the recovered behavior against Watson source.

No source modification, boot-image modification, flashing, or repartitioning is implied by this record.
