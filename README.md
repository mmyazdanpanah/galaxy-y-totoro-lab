# galaxy-y-totoro-lab

Research, preservation, reconstruction, and modernization of the Samsung Galaxy Y GT-S5360 (totoro).

## Project identity

**Primary working identity:** **A tiny, native ARMv6 pocket computer built from a Samsung Galaxy Y.**

**Optional product identity:** **Totoro-Pocket-PC** — a compact name for the eventual physical Linux-computing experience.

These names describe an aspirational product direction, not a claim that the device is already an independent Linux computer. The present implementation remains Android-assisted and reversible.

The UI direction is deliberately broader than a terminal or server. The long-term target is a **tiny desktop-like Linux experience** adapted to the Totoro's 240×320 display, touchscreen, limited memory and ARMv6 CPU. We will borrow mature Linux infrastructure and lessons from existing projects such as postmarketOS and historical Linux-on-phone work where useful, while keeping Totoro-specific hardware integration and the minimal shell under our control.

The intended progression is:
`verified Linux userspace → native framebuffer/touch UI → tiny desktop-like shell → useful pocket-PC applications`.

## Project goal

**Ultimate goal: turn the Totoro into something useful, cool, and recognizably computer-like.**

The active strategy is now deliberately outcome-first: preserve the working Android system and stock kernel, prove the smallest compatible Linux userspace, and reach a useful physical-device experience as quickly and safely as possible.

An Android-assisted Linux environment, a focused static Linux payload, a hybrid service, or another reversible design is a successful outcome if it makes the real Totoro genuinely useful or fun. Independent Linux boot is no longer the active critical path; it is a conditional research branch to reopen only if a demonstrated limitation requires it.

## Identity and UI principles

- **Pocket PC first:** design for small-screen computing rather than trying to imitate a modern smartphone.
- **Desktop experience, miniature:** provide familiar concepts such as a launcher, task/application screens, files, terminal, settings/tools and status information, while keeping rendering and interaction lightweight.
- **Borrow infrastructure, not identity:** reuse established Linux interfaces, libraries, driver knowledge and lessons from postmarketOS and other historical projects; do not unnecessarily reproduce an entire distribution.
- **Totoro-native integration:** framebuffer, touchscreen, display geometry, input behavior and hardware constraints are treated as first-class design inputs.
- **Incremental graphics stack:** begin with direct framebuffer rendering, then introduce a small graphics/UI layer only when repeated needs justify it. A conventional desktop/compositor stack remains optional and conditional.
- **Preservation remains mandatory:** the pocket-PC identity does not justify boot-critical writes, repartitioning or replacing the stock Android environment prematurely.

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

## Current physical milestone

As of 2026-10-02, the project has crossed the important boundary from “Linux payload execution” into **physical-device interaction**: a persistent SD-backed ext2 Linux userspace is verified, native framebuffer writes have produced a visible LCD update, and the next engineering target is touch-driven native UI.

This makes a pocket-PC direction technically meaningful rather than purely conceptual. The first UI should therefore be treated as the beginning of the product layer, while keeping the underlying Linux substrate minimal and evidence-driven.

## Hybrid AI architecture direction

The project now also documents an optional, reversible **Totoro + Mac hybrid AI** direction, with the iPhone SE (2020) as an optional mobile perception and relay node. Totoro remains an offline-capable physical interface and edge node; the Mac hosts larger inference, orchestration and durable knowledge services. The iPhone is not a required dependency.

The architecture is a proposal, not a verified implementation. Communication and AI work must not displace the active physical-device UI/network-service gates or justify boot-critical changes. Start with protocol tests and a harmless ping/status exchange; add local inference only after the target runtime and resource budget are measured.

See [Hybrid AI documentation](08_MODERNIZATION/hybrid-ai/README.md) for the architecture, Totoro Link protocol, transport analysis, iPhone bridge, edge-AI strategy and model pipeline.

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
- 08_MODERNIZATION/hybrid-ai/ — proposed distributed AI architecture and implementation specifications
- 09_EXPERIMENTS/ — experiment records
- 10_RESEARCH/ — historical and technical research
- 99_SANDBOX/ — raw external reviews and exploratory material

## Guiding rule

**Make the Totoro useful first. Keep the path short. Observe → preserve → smallest reversible step → test → record.**
