# galaxy-y-totoro-lab

Research, preservation, reconstruction, and modernization of the Samsung Galaxy Y GT-S5360 (totoro).

## Project goal

**Ultimate goal: turn the Totoro into something useful and cool.**

The main technical target is a useful Linux-based environment that can eventually boot independently of Android. But we do not need to force one specific architecture or spend a long time chasing “pure Linux” if a simpler alternative gives the real phone a genuinely good user experience.

A chroot or Android-assisted Linux setup can therefore be a successful practical outcome. Independent Linux boot is the preferred technical destination, not a reason to make the project unnecessarily long or complicated.

## Current strategy

Use the shortest, safest and least complicated path to a useful device. Preserve the working stock Android kernel and boot chain, complete the read-only capability inventory, establish a credible recovery baseline, and validate the smallest compatible Linux userspace. Try native boot when the evidence supports it, but do not let speculative kernel or mainline work block practical progress.

Success means three things: it runs on the real Totoro, it is genuinely useful/fun to use, and we reached it without unnecessary risk or complexity.

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

**Make the Totoro useful first. Keep the path short. Observe → preserve → smallest reversible step → test → record.**