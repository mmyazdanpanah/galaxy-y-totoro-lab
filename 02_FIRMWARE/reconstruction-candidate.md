# Stock Firmware Reconstruction Candidate

## Exact observed lineage

The specimen reports:

- Model: GT-S5360
- PDA: `S5360JPLC1`
- CSC: `S5360OJPLC1`
- Android: 2.3.6
- Build: `GINGERBREAD.JPLC1`
- Current baseband: `S5360XXLK3`

## Candidate factory package

The strongest matching stock reconstruction candidate is:

`S5360JPLC1_S5360OJPLC1_S5360XXLC1_HOME.tar.md5`

The historical Galaxy Y Forum archive identifies this as an Arabic/Middle East release, Android 2.3.6, dated 2012-03-16, with CSC family including `THR` among other regional CSCs and Persian support.

A current firmware index independently lists `S5360JPLC1` with CSC `S5360OJPLC1` as an official Samsung firmware family for GT-S5360, confirming the PDA/CSC pairing and Android 2.3.6.

## Important divergence

The candidate package's matching modem is `S5360XXLC1`, while the specimen currently reports `S5360XXLK3`.

This is not evidence that the specimen originally shipped with XXLC1, nor evidence that XXLK3 is incorrect. It establishes that the handset currently combines the JPLC1/OJPLC1 PDA/CSC lineage with a later/different modem lineage.

Do not flash the candidate merely to make the versions match.

## Reconstruction rule

Treat the candidate package as a historical reconstruction artifact until provenance is established. The live handset remains the primary specimen.

Next efficient evidence target:

1. Read current Android properties through ADB, if available.
2. Preserve the resulting read-only property dump.
3. Compare software identifiers against the JPLC1 candidate.
4. Only then decide whether downloading the ~125 MB stock package adds enough evidence to justify the storage/time cost.

## Sources

- GALAXY Y FORUM: historical firmware package archive and regional/language metadata.
- SamFW: current firmware index for GT-S5360 JPLC1/OJPLC1.
