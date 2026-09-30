# galaxy-y-totoro-lab

Research, preservation, reconstruction, and modernization of the Samsung Galaxy Y GT-S5360 (totoro).

## Project goal

**Ultimate goal: turn the Totoro into something useful and cool.**

The active strategy is now deliberately outcome-first: preserve the working Android system and stock kernel, prove the smallest compatible Linux userspace, and reach a useful physical-device experience as quickly and safely as possible.

An Android-assisted Linux environment, a focused static Linux payload, a hybrid service, or another reversible design is a successful outcome if it makes the real Totoro genuinely useful or fun. Independent Linux boot is no longer the active critical path; it is a conditional research branch to reopen only if a demonstrated limitation requires it.

## Current strategy

The reviews in `08_MODERNIZATION/integrated-external-review-2026-09-30.md` converge on a shorter sequence:

1. Capture only the decision-critical read-only baseline.
2. Audit one verified static ARMv6 ELF on the host.
3. Execute it from `/data/local/tmp`.
4. Test one dynamic musl-based ARMv6 binary.
5. Use the already-proven SD-backed loop/ext2 mechanism for a persistent minimal rootfs.
6. Add a reliable access path such as Dropbear/SSH.
7. Build one genuinely useful service or experience.
8. Stop and reassess once the Totoro is already useful.

Do not select a large distribution, rebuild the kernel, port mainline Linux, repartition, or flash boot-critical regions merely because those are technically interesting.

## Verified milestones

- Physical GT-S5360, Android 2.3.6 JPLC1, Linux 2.6.35.7 and root access verified.
- Basic static BusyBox chroot execution verified.
- Loop-backed ext2 creation, attachment, read-write mount, file I/O and clean teardown verified.
- ELF execution from the loop mount, dynamic Linux userspace compatibility, networking/SSH integration, recovery partition identity, and a fully verified restore route remain open unless supported by primary evidence.

## Documentation map

- STATUS.md — current state, gates and immediate next action
- AGENTS.md — preservation and experiment rules
- 01_PRESERVATION/ — specimen evidence and acquisition
- 02_FIRMWARE/ — stock firmware history and reconstruction
- 03_PARTITIONS/ — live partition evidence
- 06_KERNEL/ and 07_BUILD/ — kernel archaeology and reproducible builds
- 08_MODERNIZATION/plan.md — main strategy and milestones
- 08_MODERNIZATION/android-chroot-linux.md — active userspace procedure and evidence
- 08_MODERNIZATION/integrated-external-review-2026-09-30.md — integrated analysis of the five external reviews
- 09_EXPERIMENTS/ — experiment records
- 10_RESEARCH/ — historical and technical research
- 99_SANDBOX/ — raw external reviews and exploratory material

## Guiding rule

**Make the Totoro useful first. Keep the path short. Observe → preserve → smallest reversible step → test → record.**