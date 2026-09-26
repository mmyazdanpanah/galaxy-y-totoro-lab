# Galaxy Y Modernization Plan

## Objective

Modernize the Samsung Galaxy Y GT-S5360 (totoro) into a useful, reproducible Linux-based system while preserving the original specimen and avoiding unnecessary reinvention.

The target is not "make a 2011 phone behave like a modern Android phone." The target is a small, maintainable Linux computer/phone built around hardware that still works.

## Core strategy

Reuse the existing hardware layer → isolate it → replace the generic userspace → progressively replace hardware-specific pieces only when useful.

For Totoro, the fastest reliable route is:

    GT-S5360 / Totoro
        ↓
    existing/proven boot path
        ↓
    existing Totoro downstream kernel
        ↓
    existing hardware drivers
        ↓
    minimal Linux userspace
        ↓
    Alpine / postmarketOS
        ↓
    lightweight interface + SSH/network tools
        ↓
    selective mainline work, only where justified

Do not make a fully mainline kernel the first milestone.

## Phase 0 — Preserve the Museum baseline

Status: complete.

The read-only preservation pass is frozen. The observed stock environment, Download Mode state, live PIT, partition metadata, and provenance are documented.

The baseline must remain untouched while modernization is researched.

See STATUS.md, 01_PRESERVATION/, and 03_PARTITIONS/partition-map.md.

## Phase 1 — Totoro archaeology

Status: next.

Determine exactly how much existing work can be reused before writing new code.

Audit:

- GT-S5360 / Galaxy Y / totoro
- BCM21553 / bcm21553
- ARM11 / ARMv6 constraints
- Samsung kernel source and historical releases
- community Totoro kernels
- device trees / board files
- boot image layout and kernel command line
- recovery and boot-chain constraints
- framebuffer/display support
- touchscreen/input drivers
- USB
- storage / OneNAND
- Wi-Fi / Bluetooth
- audio
- battery / charging / suspend
- modem and camera
- historical CyanogenMod / LineageOS work
- Replicant
- Halium / Ubuntu Touch
- postmarketOS / Alpine
- any existing upstream Linux / mainline BCM21553 work

The output should be an evidence-backed Totoro hardware and kernel reuse map, not a speculative design.

## Phase 2 — Prove the smallest Linux boot

Goal:

    bootloader → Totoro kernel → init → shell

Use the existing/proven Totoro kernel first.

Build the smallest reproducible test image possible:

- kernel
- initramfs
- BusyBox
- shell
- basic diagnostics

Do not add a graphical stack yet.

Success criterion: a repeatable Linux boot with enough console/debug access to diagnose hardware.

## Phase 3 — Replace the prototype userspace with Alpine/postmarketOS

Once the minimal Linux boot is proven:

- use Alpine Linux as the lightweight base
- use postmarketOS infrastructure where it reduces device-specific work
- preserve the existing Totoro kernel and drivers initially
- add OpenSSH and standard Linux tooling
- make the root filesystem reproducible

This is the first real modernization target.

## Phase 4 — Bring up hardware in dependency order

Priority:

1. CPU / RAM
2. storage
3. USB
4. framebuffer / display
5. touchscreen / buttons
6. networking
7. Wi-Fi
8. audio
9. battery / charging
10. suspend / resume
11. Bluetooth
12. camera
13. modem

The ordering intentionally prioritizes a useful Linux computer over telephony completeness.

Camera and modem work are not prerequisites for an interesting Linux system.

## Phase 5 — Lightweight interface

The hardware is extremely constrained by modern standards.

Avoid:

- modern desktop environments
- large graphical stacks
- unnecessary background services
- heavyweight browsers

Prefer a framebuffer-oriented or otherwise minimal interface.

The interface should be treated as part of the hardware constraint, not as an attempt to reproduce a contemporary smartphone UI.

## Phase 6 — Mainline feasibility audit

Only after a working downstream Linux system exists, compare each subsystem against upstream Linux.

Create a matrix:

| Subsystem | Existing Totoro support | Mainline support | Reuse cost | Mainline cost | Decision |
|---|---|---|---|---|---|
| CPU / SoC | TBD | TBD | TBD | TBD | TBD |
| storage | TBD | TBD | TBD | TBD | TBD |
| USB | TBD | TBD | TBD | TBD | TBD |
| GPIO | TBD | TBD | TBD | TBD | TBD |
| display | TBD | TBD | TBD | TBD | TBD |
| touchscreen | TBD | TBD | TBD | TBD | TBD |
| Wi-Fi | TBD | TBD | TBD | TBD | TBD |
| audio | TBD | TBD | TBD | TBD | TBD |
| power | TBD | TBD | TBD | TBD | TBD |
| Bluetooth | TBD | TBD | TBD | TBD | TBD |
| camera | TBD | TBD | TBD | TBD | TBD |
| modem | TBD | TBD | TBD | TBD | TBD |

This prevents "mainline" from becoming an ideological requirement.

## Phase 7 — Selective upstreaming

If individual mainline drivers are mature and useful, replace downstream components one at a time.

A hybrid result is valid:

    mostly mainline
    + a small compatibility layer
    + a few unavoidable legacy components

There is no benefit in rewriting working hardware support merely to reach a nominal 100% mainline state.

## Phase 8 — Pure mainline, only if justified

A fully upstream Totoro kernel becomes a separate research goal.

Only pursue it if the Phase 6 audit shows that the missing pieces are tractable and the result provides a real maintenance or capability benefit.

The first pure-mainline milestone is simply:

    mainline kernel → boots on Totoro

Then:

    storage → USB → display → touch → networking

Everything beyond that is optional.

## Milestones

### M0 — Evidence

Exact hardware, boot chain, kernel lineage, partition layout, and reusable historical work are known.

### M1 — Linux boot

A reproducible Totoro kernel + minimal userspace boots.

### M2 — Linux computer

Storage + USB + display + input + networking work.

### M3 — Modern Totoro

Alpine/postmarketOS userspace, lightweight interface, reproducible build, documented experiments.

M3 is the primary success condition.

### M4 — Upstream Totoro

Mainline Linux supports enough of the device to replace the downstream kernel.

M4 is optional.

## Decision rule

    Can the existing Totoro kernel boot?
             │
        ┌────┴────┐
        │         │
       NO        YES
        │         │
     repair     minimal Linux
     kernel        │
                   ↓
             Alpine / pmOS
                   │
             useful system?
               │       │
              NO      YES
               │       │
            diagnose  modernize
                       │
                       ↓
               mainline audit
                  │       │
               feasible  expensive
                  │       │
               mainline  keep proven
                 POC     downstream
                  │       │
                  └───┬───┘
                      ↓
                   compare

## Explicitly out of scope for the first implementation

- Treble / GSI / DSU
- GKI
- forcing a modern Android release onto the device
- modern LineageOS as the first route
- replacing the bootloader without a demonstrated need
- repartitioning
- camera-first development
- modem-first development
- rewriting the entire kernel
- large experimental patch stacks

These techniques were important in later mobile Linux/Android history, but the Galaxy Y predates the hardware/software architecture they were designed around.

## Working principle

Simple fix first. Alternative second. Reinvention last.

Every proposed change should answer:

1. What existing work can we reuse?
2. Is the required fix small and reversible?
3. If not, is there a proven alternative?
4. What new capability does the change buy us?
5. Can the experiment be reproduced and rolled back?

The project should move forward by evidence, not by ambition.
