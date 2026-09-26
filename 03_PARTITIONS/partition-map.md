# Live Partition Map

Source: live PIT downloaded directly from the specimen on 2026-09-26.

PIT artifact:
- SHA-256: `06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538`
- Size: 4096 bytes
- Entries: 15
- Storage/device type reported by Heimdall: OneNAND
- Partition block size/offset: 256 for all entries
- PIT identifier reported by Heimdall: `toto`

| # | Partition | Binary | Attributes | Blocks | Image/file field |
|---:|---|---|---|---:|---|
| 1 | bcm_boot | AP | 0x00 — Read-Only | 1 | BcmBoot.img |
| 2 | loke | AP | 0x01 — Read-Only | 8 | sbl.bin |
| 3 | loke_bk | AP | 0x02 — Read-Only | 8 | — |
| 4 | systemdata | AP | 0x03 — Read-Only | 1 | totoro.pit |
| 5 | modem | CP | 0x04 — Read-Only | 50 | BcmCP.img |
| 6 | param_lfs | AP | 0x15 — STL Read-Only | 20 | param.lfs |
| 7 | boot | AP | 0x05 — Read-Only | 20 | boot.img |
| 8 | boot_backup | AP | 0x06 — Read-Only | 20 | — |
| 9 | system | AP | 0x14 — STL Read-Only | 920 | system.img |
| 10 | cache | AP | 0x16 — STL Read-Only | 160 | csc.rfs |
| 11 | userdata | AP | 0x17 — STL Read-Only | 789 | userdata.img |
| 12 | efs | AP | 0x07 — Read/Write | 1 | — |
| 13 | sysparm_dep | AP | 0x08 — Read-Only | 1 | sysparm_dep.img |
| 14 | umts_cal | AP | 0x09 — Read-Only | 1 | HEDGE_NVRAM8_RF_LE.bin |
| 15 | cal | AP | 0x0a — Read/Write | 4 | — |

This is the live partition inventory of the specimen, not a reconstructed or assumed map from a web-hosted PIT.

The Heimdall `print-pit --no-reboot` output independently reproduced the same 15-entry structure as the preserved binary PIT. No write, flash, repartition, or EFS operation was performed during acquisition.

Sensitive identifiers such as IMEI and physical serial number are intentionally excluded.
