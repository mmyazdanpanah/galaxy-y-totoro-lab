# Agent Rules

This repository is a preservation-first laboratory notebook.

## Non-negotiable rules

1. **Preserve before modifying.**
2. Never flash, repartition, wipe, format, root, or replace recovery without an explicit experimental decision.
3. Treat undocumented hardware/software state as evidence, not assumption.
4. Record commands, tool versions, firmware identifiers, hashes, dates, and outcomes.
5. Never commit IMEI, serial number, authentication material, private keys, or other specimen-specific secrets.
6. Keep historical artifacts separate from modified/experimental artifacts.
7. Prefer read-only acquisition whenever possible.
8. When an experiment can brick or irreversibly alter the specimen, stop and require explicit confirmation.
9. Do not overwrite an original artifact with a modified one.
10. Failed experiments are data: document them rather than hiding them.

## Repository structure

- `00_SPECIMEN/` — physical and software identity
- `01_PRESERVATION/` — acquisition, hashes, evidence
- `02_FIRMWARE/` — stock and historical firmware
- `03_PARTITIONS/` — PIT, EFS documentation, partition research
- `04_RECOVERY/` — recovery research
- `05_ANDROID/` — Android/ROM history and experiments
- `06_KERNEL/` — kernel source and patches
- `07_BUILD/` — build/reproducibility work
- `08_MODERNIZATION/` — revival work
- `09_EXPERIMENTS/` — experiment logs
- `10_RESEARCH/` — historical/technical research
- `archive/` — retired material

## Evidence convention

Every acquired artifact should have provenance where possible:

- source
- acquisition date
- exact filename/version
- SHA-256
- acquisition method
- relationship to the specimen
- verification status
