# Totoro Rollback Baseline

Status: frozen offline baseline; no device modification performed.

Date: 2026-09-27

## Verified stock firmware

| Package | SHA-256 |
|---|---|
| JPLC1-JED | `4a39cffef9c553114a52e25989cc8ed4ef42a6e9301e858c3d9b7aadde00bc53` |
| JPLF1-THR | `e58161ec05b491d34dc1ec1a3aba65cfe0a3c59c78f31afee459b9fee7acad7f` |
| JPLF1-XSG | `bc92745a884f2634feb9582c7c5bbae482c61d61c8f506f24b164e1861ff476c` |
| XXMK1-SER | `38225c75b5edc04a97db8ea6930e76c6edf207e53071c214da45ac184c7ef392` |

## Live PIT

File: `totoro-live.pit`

SHA-256:

`06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538`

The PIT was acquired from the preserved device before any flashing or repartitioning.

## Independent physical copy

A second copy of the four verified firmware archives and live PIT was created on:

`/dev/disk6` — external physical USB device

Mounted at:

`/Volumes/NO NAME/Totoro-Rollback-2026-09`

All five copied files were independently SHA-256 verified against the primary copies.

## EFS status

The live PIT identifies an `efs` partition.

EFS was not modified during preservation.

No independent raw EFS image has been acquired.

Therefore this baseline does not claim a complete EFS backup. EFS remains preserved in place on the device.

## Device modification status

No flashing performed.

No repartitioning performed.

No rooting performed.

No recovery modification performed.

No EFS modification performed.

## Rollback status

The verified stock firmware set and live PIT provide a documented rollback baseline.

XXMK1-SER provides explicit payload coverage for the major image-bearing partitions identified in the preserved live PIT, including `bcm_boot`.

The EFS partition remains the principal unbacked device-resident component.

## Backup integrity

Primary source hashes and independent USB-copy hashes match for all five preserved files.

Generated inspection artifacts are not part of the rollback copy.
