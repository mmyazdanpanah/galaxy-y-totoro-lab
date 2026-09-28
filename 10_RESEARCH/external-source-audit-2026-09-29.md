# External Source Audit — Galaxy Y Totoro Deep Research Pass

Date: 2026-09-29
Scope: all URLs supplied in the 2026-09-29 source list.
Purpose: distinguish actionable evidence from historical/contextual material and record access limitations.

## Coverage result

All supplied URLs were individually processed through direct retrieval where possible and targeted search fallback where direct retrieval failed. Sources that were blocked, redirected incorrectly, or returned unrelated crawler content are explicitly marked below and are not treated as fully verified evidence.

The most actionable additions from this pass are:

1. An independent physical GT-S5360 investigation documents BCM21553 ThunderbirdEDN31, ARMv6 CPU details, a large BCM-specific /proc/devices inventory, LZMA kernel compression, and kernel relocation geometry consistent with 0x81608000.
2. The Watson kernel demonstrates that MTD support on Gingerbread was achieved on Totoro, while exposing concrete limitations around radio/EFS and recovery.
3. Merruk Technology provides a Totoro-specific build/compression workflow and names the totoro_brcm21553_05_defconfig.
4. The Totoro device-tree repository is a historical reference for separating common BCM21553 and Totoro-specific hardware description.
5. Broadcom/Mesa material confirms the BCM21553 GPU is related to the VideoCore family and that Broadcom released a 21553 specification plus an Android graphics-driver snapshot; this is useful for future GPU archaeology but does not establish modern mainline V3D support.
6. Samsung Broadcom bootloader work on a later device provides a concrete UART/S-BOOT reverse-engineering methodology: early UART, bootloader interruption, environment inspection, kernel-loading analysis, and RAM payload execution. It is methodology, not Totoro-specific proof.
7. postmarketOS has now removed ARMv6/armhf from current development, so postmarketOS is no longer a straightforward current target for Totoro; an independently maintained ARMv6 userspace path would be required.
8. Heimdall remains the relevant open-source Samsung flashing transport and documents the Loke/Odin protocol boundary.
9. The source set reinforces that the first experiment should remain a minimal, reversible kernel/ramdisk diagnostic boot rather than a large userspace or mainline conversion.

## Source-by-source audit

### Directly relevant Totoro / BCM21553

- Linux Magazine Android-to-Linux article — inspected. Generic Linux-on-Android/chroot material; low direct engineering value.
- Watson XDA development thread — inspected. High-value Watson thread; confirms MTD-on-Gingerbread experiment, OC/GPU/DVFS modifications, and reported radio/EFS and recovery limitations.
- sonickles9/watson-kernel-totoro — inspected. High-value Totoro kernel source/reference.
- Raspberry Pi Broadcom source-release article — inspected. Broadcom VideoCore source-release history.
- broadcomCM BCM21553 common kernel — inspected. BCM21553 common Samsung kernel lineage.
- 2021 Android 2.3.6 compilation article — inspected. Useful build/toolchain history, not hardware bring-up.
- cleverior BCM21553 kernel/Kernel path — direct crawler retrieval unavailable; targeted through search. Treat as mirror/source reference, not independent evidence unless locally checked.
- Watson issue #1 — inspected/targeted. Relevant project history; no stronger hardware fact established.
- francomor/kernel_b5510 — inspected. Galaxy Y Pro sibling kernel; useful BCM21553 comparison.
- CyanogenMod-ARMv6 BCM21553 cm-11.0 tree — inspected. Useful historical common-kernel lineage.
- Nura cooperve page — direct retrieval unavailable; targeted search did not produce a reliable page. Not independently verified.
- BCM21553 CM11 development thread page 14 — inspected/targeted. Historical driver/build context.
- CyanogenMod-ARMv6 BCM21553 common tree — inspected.
- BCM21553 Thunderbird driver thread — inspected/targeted. Historical driver identification.
- Broadcom drivers thread — inspected/targeted. Historical context; no new verified Totoro fact.
- Samsung kernel build tutorial — inspected/targeted. Generic build methodology.

### SoC / GPU / mainline

- dissonant BCM215xx mainline hub — inspected/targeted. Architecture/reverse-engineering context.
- dissonant BCM215xx introduction — inspected/targeted.
- PhoneDB BCM21553 device URL — direct timeout; processor page found as fallback. It identifies BCM21553 as 32-bit ARMv6, single-core ARM1136 and lists OpenGL ES 2.0 and integrated modem characteristics. Secondary evidence.
- Broadcom 21553 GPU specification — inspected/targeted. Important future GPU reference.
- anholt VideoCore device list — inspected. Historical lack of mainline BCM21553 graphics support.
- viethoang9 BCM21553 kernel — inspected. Additional kernel lineage.
- Codeberg BCM21553 discovery page — inspected as discovery index; no stronger evidence than underlying repositories.
- Mesa V3D documentation — inspected. Explicitly notes the public BCM21553 specification and Android graphics-driver snapshot; current V3D documentation concerns newer generations.

### Bootloader / flashing / low-level methodology

- prototype-U/Ace-i — inspected/targeted. Early BCM21553 U-Boot/Ace-i work; relevant UART/clock clues.
- Replicant issue 2076 — inspected/targeted. U-Boot-from-boot.img precedent.
- Samsung bootloader exploit discussion — inspected as methodology reference only.
- UnsignedChad Note20 ABL/Odin research — inspected. Modern Odin/LOKE reverse-engineering methodology, not a Totoro exploit.
- XDA Loke alpha URL — currently resolves to unrelated XDA content; not verified and not used as evidence.
- Heimdall — inspected. Confirms Samsung flashing and Loke/Odin protocol boundary.
- Replicant UART page — direct page currently protected by Anubis; not independently readable in this pass. Do not treat this exact page as verified.
- XDA jig/JTAG URL — currently resolves to unrelated XDA content; not verified.

### Linux userspace / archives

- postmarketOS ARMhf removal announcement — supplied URL does not crawl cleanly, but the official postmarketOS announcement was found through search at the current edge announcement path. It states that ARMv6/armhf package builds and cross-compilers are being dropped.
- pmaports issue 4615 — direct issue retrieval failed; the official announcement corroborates the ARMv6 removal. Exact issue page not independently verified.
- Nura cooperve page — not independently verified.
- AFHArchive — inspected. Historical AndroidFileHost preservation/index; useful for artifact recovery, not hardware facts.
- SamFW XXMK1 — direct retrieval failed. Treat as firmware discovery link until the actual package is independently downloaded and hashed.
- Merruk Technology — inspected. Highly useful Totoro build tooling; documents totoro_brcm21553_05_defconfig, preparation/compression stages, stock/Merruk ramdisk choices, and boot-image output.
- NachiketNamjoshi/totoro_device_tree — inspected. Historical Totoro device tree.

## New engineering implications

### Boot geometry
Historical physical-device evidence points to a kernel relocation/load geometry centered on 0x81608000, matching the project's existing boot-chain reconstruction. This is independent corroboration, not proof of the exact Samsung stock mkbootimg invocation.

### LZMA
The physical GT-S5360 investigation found the kernel compressed with LZMA (0x5d header) and traced it to Linux decompress_unlzma.c. This independently supports the project's raw-LZMA/zImage analysis.

### Historical driver surface
The physical device exposed v3d, lcd, camera, bcm_gps, bcm_kril, bcm_rpc, bcm_alsa_*, hx170dec, h6270enc, memalloc, rtc, mtd, bml, stl, and other BCM-specific interfaces. Preserve these names as a driver-archaeology checklist.

### MTD
Watson demonstrates MTD support on Gingerbread. Reported limitations included radio/calling/EFS-related problems and recovery instability. MTD is therefore a documented fallback, not a reason to alter the preserved baseline now.

### postmarketOS
Current postmarketOS no longer provides ARMv6/armhf package/cross-compiler infrastructure. A future Linux userspace experiment should therefore plan for a self-maintained ARMv6-capable userspace or another reproducible legacy userspace.

### UART/SBL
Later Samsung Broadcom work demonstrates a low-risk research pattern: recover console output, understand the bootloader's kernel-loading behavior, then use a minimal diagnostic payload. For Totoro, investigate whether its SBL exposes comparable behavior before invasive boot-chain changes.

## Recommended next work

1. Add kernel notes for the independently corroborated 0x81608000 geometry and LZMA path.
2. Add a BCM21553 historical driver inventory to kernel archaeology.
3. Add a bootloader/UART research track to the modernization plan.
4. Keep MTD documented as an alternative rather than the immediate path.
5. Update the postmarketOS roadmap for the 2026 ARMv6/armhf removal.
6. Continue toward the first reversible diagnostic boot, not a full mainline conversion.

## Evidence discipline

No source should be treated as proof merely because it appears in the list. Directly verified pages, search-confirmed fallbacks, and inaccessible/incorrect URLs are deliberately distinguished. Preserved device evidence and reproducible local binaries remain higher-confidence evidence than third-party claims.
