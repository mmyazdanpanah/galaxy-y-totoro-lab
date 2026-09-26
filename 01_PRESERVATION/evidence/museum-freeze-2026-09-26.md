# Museum Freeze — 2026-09-26

This record closes the initial read-only preservation pass for the Samsung Galaxy Y GT-S5360 (`totoro`) specimen.

## Handset identity observed

- Model: Samsung Galaxy Y GT-S5360
- Codename: `totoro`
- Android: 2.3.6
- Build: `GINGERBREAD.JPLC1`
- PDA: `S5360JPLC1`
- CSC: `S5360OJPLC1`
- Current baseband: `S5360XXLK3`
- Kernel: `2.6.35.7 / dpi@DELL161 #1`

## Download Mode

Observed on the specimen:

```
ODIN MODE
Product name: GT-S5360
Custom Bin Down: No (0 count)
Current Bin: Samsung Official
```

No custom binary flash count was observed.

## USB observation

In Download Mode the host identified:

- Manufacturer: Samsung Electronics
- USB Vendor ID: `0x04e8`
- USB Product ID: `0x685d`
- USB Product Version: `0x0001`
- Link speed: 480 Mb/s

The USB descriptor serial is not treated as the handset's physical serial number and is intentionally not recorded here.

## Live PIT

A PIT was downloaded directly from the specimen using Heimdall v2.2.2 in Download Mode.

- Artifact: `totoro-live.pit`
- Size: 4096 bytes
- SHA-256: `06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538`
- Entries: 15
- PIT identifier reported by Heimdall: `toto`
- Storage/device type: OneNAND
- Partition block size/offset: 256 for all entries

The PIT was also independently inspected with `print-pit --no-reboot`; its 15-entry structure matches the preserved binary.

## Preservation boundary

During this pass:

- No firmware was flashed.
- No bootloader was flashed.
- No recovery was installed.
- No repartitioning was performed.
- No partition was erased or formatted.
- EFS was not read, written, erased, or formatted.
- The handset's IMEI and physical serial number are intentionally excluded from repository records.

## Museum state

The specimen remains in its observed stock Android 2.3.6 environment. The live PIT and its provenance are preserved. Further modification belongs to a later, explicitly documented laboratory phase.
