# XXMK1-SER Payload Inspection

Status: verified offline; no device modification performed.

Source archive:

`SAMFW.COM_GT-S5360_SER_S5360XXMK1_fac.zip`

SHA-256:

`38225c75b5edc04a97db8ea6930e76c6edf207e53071c214da45ac184c7ef392`

Archive contents:

- `BOOT_S5360XXMK1_REV05.tar.md5`
- `PDA_S5360XXMK1_REV05.tar.md5`
- `MODEM_S5360XXMK1_REV05.tar.md5`
- `GT-S5360-MULTI-CSC-QXENA1.tar.md5`
- `SS_DL.dll`
- `_FirmwareInfo_Samfw.com.txt`

Verified TAR payload inventory:

### BOOT

`BOOT_S5360XXMK1_REV05.tar.md5`

- `BcmBoot.img` — 97,460 bytes

### PDA

`PDA_S5360XXMK1_REV05.tar.md5`

- `Sbl.bin` — 1,306,624 bytes
- `boot.img` — 4,546,560 bytes
- `system.img` — 219,983,872 bytes
- `userdata.img` — 1,016,320 bytes
- `param.lfs` — 765,952 bytes

### MODEM

`MODEM_S5360XXMK1_REV05.tar.md5`

- `BcmCP.img` — 11,534,336 bytes

### CSC

`GT-S5360-MULTI-CSC-QXENA1.tar.md5`

- `csc.rfs` — 9,012,224 bytes

## PIT correspondence

The payloads correspond to the following preserved live PIT partitions:

- `bcm_boot` → `BcmBoot.img`
- `loke` → `Sbl.bin`
- `modem` → `BcmCP.img`
- `param_lfs` → `param.lfs`
- `boot` → `boot.img`
- `system` → `system.img`
- `cache` → `csc.rfs`
- `userdata` → `userdata.img`

This package provides explicit payload coverage for all major image-bearing partitions identified in the preserved live PIT.

The original source archive remains unchanged.

Generated extraction artifacts are local inspection data and are intentionally excluded from Git.

No flashing, repartitioning, or modification of the phone was performed.
