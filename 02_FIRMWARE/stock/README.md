# Stock Firmware Archive

This directory contains locally archived GT-S5360 stock-firmware acquisitions used for preservation, rollback research, and controlled modernization.

## Specimen

The physical specimen reports:

- Model: GT-S5360
- PDA/AP: S5360JPLC1
- Phone/Baseband: S5360XXLK3
- CSC: S5360OJPLC1
- Android: 2.3.6
- Build: GINGERBREAD.JPLC1

## Archived packages

### JPLC1-JED

Exact historical firmware identity of the specimen.

Region/source: JED.

The acquired archive is a signed Android update/system package and is not treated as a verified Odin rollback image.

### JPLF1-THR

JPLF1 regional firmware for THR.

The archived package is a single-file Odin HOME package.

It is retained as historical/regional evidence.

### JPLF1-XSG

JPLF1 regional firmware for XSG.

The archived FAC package contains separate PDA, MODEM, and CSC Odin payloads.

It is retained as a primary JPL-family rollback candidate pending payload-level verification.

### XXMK1-SER

XXMK1 regional firmware for SER.

The archived FAC package contains separate BOOT, PDA, MODEM, and CSC Odin payloads.

It is retained as the primary engineering rollback candidate pending payload-level verification.

## Integrity

Cryptographic identities of the four source ZIP archives are recorded in:

`SOURCE-HASHES.sha256`

The large firmware archives themselves are intentionally excluded from Git history. They remain local/offline acquisition artifacts.

## Safety

No package in this archive is considered flash-ready solely from filename, source listing, or ZIP structure.

Before experimental flashing:

1. Verify source archive integrity.
2. Inspect every Odin payload.
3. Verify appended Odin MD5 checksums.
4. Record payload manifests and hashes.
5. Preserve a second offline copy.
6. Freeze the rollback baseline.
7. Only then perform controlled flashing.
