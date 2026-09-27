# JPLC1 Firmware Acquisition Record

## Status

JPLC1 is the exact historical firmware identity recovered from the physical GT-S5360 specimen, but no genuine public multi-file BOOT/PDA/MODEM/CSC Odin package for this build has been located.

The public archive research performed for this project consistently located JPLC1 as a single-file Odin HOME package:

`S5360JPLC1_S5360OJPLC1_S5360XXLC1_HOME.tar.md5`

SamFW categorizes the JPLC1 package as a One Files firmware. Historical references also describe loading the single HOME.tar.md5 package into Odin's PDA/AP slot.

## Specimen match

The physical phone reports:

- Model: GT-S5360
- PDA/AP: S5360JPLC1
- Phone/Baseband: S5360XXLK3
- CSC: S5360OJPLC1
- Build: GINGERBREAD.JPLC1

Build properties recovered from the acquired JPLC1 package were consistent with the specimen, including:

- ro.build.PDA=S5360JPLC1
- ro.build.host=DELL161
- build date: Fri Mar 16 15:45:06 KST 2012

## Important distinction

The file previously acquired under the JPLC1 source directory was inspected and determined to be a signed Android update/system package containing META-INF, updater-script, update-binary, system/, CSC data, and related Android update contents.

It is NOT being treated as a verified Odin rollback image.

This file must not be flashed as if it were the genuine JPLC1 HOME.tar.md5 package.

## Research conclusion

The project has not found a genuine public multi-file JPLC1 Odin package.

This is an archive-research conclusion, not a claim that no such package ever existed privately or in an unindexed archive.

## Rollback decision

JPLC1 remains the historical identity baseline.

The primary rollback candidate for modernization is a later official GT-S5360 Android 2.3.6 Full Files package, currently targeted at S5360XXMK1, because its BOOT/PDA/MODEM/CSC components can be inspected independently before any flash operation.

JPLC1 remains useful as historical firmware evidence and may be revisited later if a genuine HOME.tar.md5 package can be acquired and independently verified.

## Safety rule

No firmware from this record is to be flashed merely because its filename resembles the specimen firmware.

Rollback requires:

1. Genuine Odin-compatible package
2. Integrity verification
3. Payload inspection
4. Offline second copy
5. Explicit rollback baseline freeze
6. Only then experimental flashing
