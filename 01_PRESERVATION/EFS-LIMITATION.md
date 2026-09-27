# EFS Acquisition Limitation

Status: intentional preservation limitation; no phone modification performed.

Date: 2026-09-27

## Finding

No documented, verified method was identified for obtaining a raw EFS-equivalent backup from the stock GT-S5360 through Download Mode, Odin, or Heimdall without first gaining additional privileged access or modifying the device.

Historical Galaxy Y tooling identifies the EFS-equivalent data with the `bml15` partition. The documented Galaxy Y backup methods require root access and/or modified recovery tooling.

The preserved live PIT confirms an `efs` partition, but PIT acquisition exposes partition metadata only. It does not constitute an EFS readback.

Stock Odin firmware packages in the preserved set provide firmware payloads for restoration but do not provide a documented consumer EFS-readback operation.

## Preservation decision

The project will not:

- root the phone solely to obtain an EFS dump;
- flash CWM/TWRP solely to obtain an EFS dump;
- write or erase the EFS partition;
- guess a block-device mapping and perform a low-level read.

EFS remains preserved in situ on the original device.

## Baseline consequence

The rollback baseline is intentionally not described as a complete raw NAND backup.

The remaining limitation is:

`EFS raw image: not independently acquired`

All other preserved rollback artifacts remain independently verified.

## Exit condition

EFS acquisition research is closed unless a verified, non-destructive stock-device readback method is discovered.

The laboratory may proceed to the controlled OS experiment without modifying EFS.
