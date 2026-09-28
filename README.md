# galaxy-y-totoro-lab

Research, preservation, reconstruction, and modernization of the Samsung Galaxy Y GT-S5360 (totoro).

## Project direction

The laboratory follows a preservation-first, reuse-first modernization strategy.

There are now two independent modernization tracks:

    preserve specimen
          ↓
    Android-native Linux userspace  ← active practical path
          ↓
    userspace Linux running

    native Linux boot              ← research / deferred
          ↓
    reproducible kernel → boot image → controlled boot
          ↓
    later hardware/mainline work

The active path keeps the stock Android kernel and boot chain intact. It aims to run a minimal ARMv6-compatible Linux userspace inside Android through chroot and make it reachable over SSH from the Mac.

The native-boot path remains valuable research, but it is not a prerequisite for the first useful Linux milestone.

## Documentation map

- STATUS.md — current specimen and project state
- AGENTS.md — preservation and experiment rules
- 01_PRESERVATION/ — specimen evidence and acquisition
- 02_FIRMWARE/ — stock firmware history and reconstruction
- 03_PARTITIONS/ — live partition evidence
- 06_KERNEL/ — kernel archaeology
- 07_BUILD/ — reproducible build work
- 08_MODERNIZATION/plan.md — two-track engineering roadmap
- 08_MODERNIZATION/android-chroot-linux.md — active Android-native Linux/chroot procedure
- 09_EXPERIMENTS/ — experiment records
- 10_RESEARCH/modernization-history.md — historical modernization map

## Guiding rule

**Observe → preserve → smallest reversible step → test → record.**

No irreversible hardware change should be made merely to make the project look more modern. Evidence and reproducibility come first.
