# Status

**Phase:** MUSEUM / preservation baseline frozen

**Device:** Samsung Galaxy Y GT-S5360 (`totoro`)

## Preserved state

- Android 2.3.6
- PDA: `S5360JPLC1`
- CSC: `S5360OJPLC1`
- Current baseband: `S5360XXLK3`
- Build: `GINGERBREAD.JPLC1`
- Kernel: 2.6.35.7 / `dpi@DELL161 #1`
- Download Mode observed as `Samsung Official`, custom binary count 0
- Live PIT downloaded and independently inspected
- Live PIT SHA-256: `06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538`

## Preservation result

The initial read-only preservation pass is complete. The specimen's observed identity, Download Mode state, USB identity, live PIT provenance, and partition metadata are documented.

See:
- `01_PRESERVATION/evidence/live-pit-2026-09-26.md`
- `01_PRESERVATION/evidence/museum-freeze-2026-09-26.md`
- `03_PARTITIONS/partition-map.md`

## Laboratory boundary

No firmware, bootloader, recovery, or repartitioning operation has been performed.

EFS has not been read, written, erased, or formatted.

The next phase may investigate historical firmware reconstruction and reproducible laboratory upgrades, but the Museum baseline should remain frozen.

## Known firmware lineage

The observed PDA/CSC correspond to the JPLC1/OJPLC1 Middle East/Arabic stock family. The historically matching package uses modem `S5360XXLC1`; the specimen currently reports `S5360XXLK3`. The reason for that combination is not yet established.

## Open questions

- Which exact stock firmware package was originally installed?
- Why does the current modem differ from the matching JPLC1 package?
- What historical recovery/kernel sequence can be reconstructed reproducibly?
- Which artifacts can be obtained and verified without modifying the Museum baseline?

## Do not

- Do not repartition.
- Do not flash firmware or recovery during the Museum phase.
- Do not erase or format EFS.
- Do not publish IMEI, physical serial number, or other device-specific identifiers.
- Do not treat web-sourced artifacts as specimen evidence without provenance comparison.
