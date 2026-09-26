# Status

Phase: MUSEUM baseline frozen → M1-A build/reuse preparation

Device: Samsung Galaxy Y GT-S5360 (totoro)

## Preserved state

- Android 2.3.6
- PDA: S5360JPLC1
- CSC: S5360OJPLC1
- Current baseband: S5360XXLK3
- Build: GINGERBREAD.JPLC1
- Kernel: 2.6.35.7 / dpi@DELL161 #1
- Download Mode: Samsung Official
- Custom binary count: 0
- Live PIT SHA-256: 06d5b4f05588fa8f06d29f46c7e5056002fdfcbd15901520d651b4ee87a9a538

## Current modernization state

The broad archaeology phase is now sufficiently complete to start implementation.

The practical route is:

    Samsung kernel
         ↓
    known Totoro boot/ramdisk material
         ↓
    controlled Linux boot
         ↓
    minimal Linux computer
         ↓
    Alpine / postmarketOS userspace
         ↓
    useful lightweight system
         ↓
    mainline audit

The primary target is M3 — Modern Totoro. Mainline Linux is optional.

## M1-A finding

Samsung's published BCM21553 source contains:

- gt-s5360_gb_opensource
- bcm21553_totoro_05_defconfig
- Totoro board support

Historical Android prebuilt material contains ARM EABI 4.4.3 toolchains for Darwin and Linux hosts.

The Darwin toolchain is Mach-O i386, so current macOS cannot execute it. The next step is to run the Linux-hosted historical toolchain inside a contained Linux environment.

Phone: not required for M1-A through M1-C.

## Community reuse finding

Existing Totoro projects provide reusable historical evidence for:

- custom kernels
- ramdisks
- boot-image packaging
- recovery
- device trees
- vendor integration
- later AndroidARMv6 hardware support

See:

- 10_RESEARCH/totoro-reuse-map.md
- 08_MODERNIZATION/plan.md
- 08_MODERNIZATION/M1-boot-experiment.md

The old community work is a parts library, not a requirement to reproduce old Android ROMs.

## Next actions

1. Execute historical Linux ARM EABI 4.4.3 in a contained Linux environment.
2. Build Samsung's kernel unchanged.
3. Record zImage size/hash and build provenance.
4. Inspect known Totoro boot/ramdisk material.
5. Verify boot-image construction without touching the phone.
6. Only then prepare the first controlled boot experiment.

## Safety boundary

No firmware, bootloader, recovery, or repartitioning operation has been performed.

EFS has not been read, written, erased, or formatted.

Do not write PIT, modem, system, userdata, or EFS during M1.

The Museum baseline remains frozen.

## Known firmware lineage

The observed PDA/CSC correspond to the JPLC1/OJPLC1 Middle East/Arabic stock family. The historically matching package uses modem S5360XXLC1; the specimen currently reports S5360XXLK3. The reason for that combination is not yet established.
