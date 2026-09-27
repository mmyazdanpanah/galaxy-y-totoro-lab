# JPLF1-XSG Payload Inspection

Status: verified offline; no device modification performed.

Source archive:

`SAMFW.COM_GT-S5360_XSG_S5360JPLF1_fac.zip`

SHA-256:

`bc92745a884f2634feb9582c7c5bbae482c61d61c8f506f24b164e1861ff476c`

Archive contents:

- `PDA_S5360JPLF1_REV05.tar.md5`
- `MODEM_S5360XXLF1_REV05.tar.md5`
- `GT-S5360-MULTI-CSC-OJPLF1.tar.md5`
- `SS_DL.dll`

Verified TAR payload inventory:

### PDA

`PDA_S5360JPLF1_REV05.tar.md5`

- `Sbl.bin` — 1,306,624 bytes
- `boot.img` — 4,517,888 bytes
- `system.img` — 194,616,832 bytes
- `userdata.img` — 1,017,856 bytes
- `param.lfs` — 765,952 bytes

### MODEM

`MODEM_S5360XXLF1_REV05.tar.md5`

- `BcmCP.img` — 11,534,336 bytes

### CSC

`GT-S5360-MULTI-CSC-OJPLF1.tar.md5`

- `csc.rfs` — 11,372,544 bytes

## PIT correspondence

The payloads correspond directly to the following preserved live PIT partitions:

- `loke` → `Sbl.bin`
- `modem` → `BcmCP.img`
- `param_lfs` → `param.lfs`
- `boot` → `boot.img`
- `system` → `system.img`
- `cache` → `csc.rfs`
- `userdata` → `userdata.img`

No `BcmBoot.img` is present in this factory package. No mapping to the `bcm_boot` PIT partition is inferred from this inspection.

The original source archive remains unchanged.

Generated extraction and payload copies are local inspection artifacts and are intentionally excluded from Git.

No flashing, repartitioning, or modification of the phone was performed.
