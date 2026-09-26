# Live Partition Map

Source: live PIT downloaded directly from the specimen on 2026-09-26.

PIT artifact:
- SHA-256: `06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538`
- Size: 4096 bytes
- Entries: 15

| # | Partition | Attributes | Image/file field |
|---:|---|---|---|
| 1 | bcm_boot | 0x00 | BcmBoot.img |
| 2 | loke | 0x01 | sbl.bin |
| 3 | loke_bk | 0x02 | — |
| 4 | systemdata | 0x03 | totoro.pit |
| 5 | modem | 0x04 | BcmCP.img |
| 6 | param_lfs | 0x15 | param.lfs |
| 7 | boot | 0x05 | boot.img |
| 8 | boot_backup | 0x06 | — |
| 9 | system | 0x14 | system.img |
| 10 | cache | 0x16 | csc.rfs |
| 11 | userdata | 0x17 | userdata.img |
| 12 | efs | 0x07 | — |
| 13 | sysparm_dep | 0x08 | sysparm_dep.img |
| 14 | umts_cal | 0x09 | HEDGE_NVRAM8_RF_LE.bin |
| 15 | cal | 0x0a | — |

This is the live partition inventory of the specimen, not a reconstructed or assumed map from a web-hosted PIT.

Sensitive identifiers such as IMEI and physical serial number are intentionally excluded.
