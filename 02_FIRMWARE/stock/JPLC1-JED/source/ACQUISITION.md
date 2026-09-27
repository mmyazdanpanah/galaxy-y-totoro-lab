# JPLC1 Stock Firmware Acquisition

## Target

Model: Samsung Galaxy Y GT-S5360 (`totoro`)

Observed specimen:
- PDA: S5360JPLC1
- CSC: S5360OJPLC1
- Current modem: S5360XXLK3
- Android: 2.3.6

## Historical reconstruction target

Candidate:
`S5360JPLC1_S5360OJPLC1_S5360XXLC1_HOME.tar.md5`

Expected:
- PDA: S5360JPLC1
- CSC: S5360OJPLC1
- Modem: S5360XXLC1

## Acquisition rule

The firmware archive is treated as historical evidence.
It must not be flashed to the specimen merely because it matches
the observed PDA/CSC.

Record:
- source URL
- acquisition date
- archive filename
- archive size
- SHA-256
- embedded MD5, if present
- extracted filenames
- internal hashes

## Separation

The binary firmware archive is local evidence and is not committed
to the Git repository unless explicitly decided later.

The repository stores provenance, metadata, hashes, and inspection
results.
