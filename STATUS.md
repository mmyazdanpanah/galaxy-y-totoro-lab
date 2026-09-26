# Status

**Phase:** MUSEUM / initial preservation

**Device:** Samsung Galaxy Y GT-S5360 (`totoro`)

## Known state

- Android 2.3.6
- PDA: `S5360JPLC1`
- CSC: `S5360OJPLC1`
- Current baseband: `S5360XXLK3`
- Build: `GINGERBREAD.JPLC1`
- Kernel: 2.6.35.7 / `dpi@DELL161 #1`

## Next safe action

Capture read-only evidence from the handset and computer before any modification:

1. Verify USB/ADB/Heimdall availability.
2. Enter Download Mode only when ready; entering it is non-destructive.
3. Extract the live PIT from the specimen.
4. Hash and archive the PIT.
5. Identify the exact stock firmware lineage and obtain matching firmware artifacts.
6. Document recovery/download-mode screens photographically.

## Do not

- Do not repartition.
- Do not flash firmware or recovery yet.
- Do not erase or format EFS.
- Do not publish IMEI, serial number, or other device-specific identifiers.
- Do not assume a web-sourced PIT matches this specimen until compared with a live PIT.

## Known firmware lineage

The observed PDA/CSC correspond to the JPLC1/OJPLC1 Middle East/Arabic stock family. The historically matching package uses modem `S5360XXLC1`; the specimen currently reports `S5360XXLK3`. The reason for that combination is not yet established.

## Open questions

- What is the specimen's live PIT?
- What are the exact partition names and sizes?
- Which firmware package was originally installed?
- Why does the current modem differ from the matching JPLC1 package?
- What historical recovery/kernel sequence can be reconstructed reproducibly?
