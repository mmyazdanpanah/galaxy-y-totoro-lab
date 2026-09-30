# galaxy-y-totoro-lab

Research, preservation, reconstruction, and modernization of the Samsung Galaxy Y GT-S5360 (totoro).

## Project goal

The long-term goal is to launch a new operating system on the physical Totoro, ideally booting a Linux environment without Android. A Linux userspace inside Android through chroot is a valuable, lower-risk intermediate milestone, but is not an independent OS boot.

## Current strategy

Use the fastest reliable path without unnecessary complexity: preserve the working stock Android kernel and boot chain, complete a read-only capability inventory, establish a credible recovery baseline, and validate the smallest compatible Linux userspace on the device. Native boot remains an evidence-gated objective; speculative kernel or mainline work must not block practical progress. No distribution or boot strategy is assumed to be final.

## Verified milestones

- Physical GT-S5360, Android 2.3.6 JPLC1, Linux 2.6.35.7 and root access verified.
- Basic static BusyBox chroot execution verified.
- Loop-backed ext2 creation, attachment, read-write mount, file I/O and clean teardown verified.
- ELF execution from the loop mount, network connectivity, SSH integration, recovery partition identity, and a usable full-firmware restore route remain unverified.

See STATUS.md for current gates and 08_MODERNIZATION/plan.md for the simplified implementation plan.

## Documentation map

- STATUS.md — current state, open gates and next action
- AGENTS.md — preservation and experiment rules
- 01_PRESERVATION/ — specimen evidence and acquisition
- 02_FIRMWARE/ — stock firmware history and reconstruction
- 03_PARTITIONS/ — live partition evidence
- 06_KERNEL/ and 07_BUILD/ — kernel archaeology and reproducible builds
- 08_MODERNIZATION/plan.md — strategy and milestones
- 08_MODERNIZATION/android-chroot-linux.md — active userspace procedure and evidence
- 09_EXPERIMENTS/ — experiment records
- 10_RESEARCH/ — historical and technical research

## Guiding rule

**Observe → preserve → smallest reversible step → test → record.**