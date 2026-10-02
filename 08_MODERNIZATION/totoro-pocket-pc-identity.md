# Totoro-Pocket-PC Identity

## Working identity

**A tiny, native ARMv6 pocket computer built from a Samsung Galaxy Y.**

**Optional product name:** **Totoro-Pocket-PC**

This identity describes the intended product direction of the research project. It does not imply that the handset already boots an independent Linux system.

## Why this identity now

The project has crossed several important physical-device thresholds:

- native ARMv6 static Linux userspace execution;
- dynamic musl ARMv6 execution on the stock kernel;
- persistent SD-backed ext2 rootfs execution;
- repeatable attach/mount/chroot/execute/teardown;
- direct native framebuffer mapping;
- a verified Samsung LCD update ioctl;
- a visible physical LCD update from native userspace;
- a known touchscreen input path at `/dev/input/event4`.

These results mean the project can reasonably begin designing the **product layer** rather than treating Linux execution as the entire objective.

## Product concept

Totoro-Pocket-PC is envisioned as a miniature Linux computer whose interface is closer to a tiny desktop than to a conventional Android application.

Possible first-level experience:

- launcher/home screen;
- status bar or status panel;
- Files;
- Terminal;
- Tools/System;
- Settings;
- About;
- small focused applications;
- touch navigation and back/home behavior.

The design should respect the actual hardware:

- 240×320 physical display;
- ARMv6-class CPU;
- limited RAM;
- small storage budget;
- touchscreen input;
- stock 2.6.35.7 kernel during the current implementation phase.

## Borrow vs. build

### Borrow

Use established Linux technology and accumulated research wherever it reduces unnecessary work:

- Linux input-event interfaces;
- framebuffer conventions;
- embedded graphics techniques;
- fonts and text rendering approaches;
- small ARMv6-compatible libraries;
- postmarketOS lessons;
- historical Galaxy Y/Linux-phone work;
- other constrained embedded Linux UI patterns.

### Build

Keep the Totoro-specific experience under project control:

- framebuffer/LCD integration;
- touchscreen coordinate handling;
- hit testing and navigation;
- tiny graphics layer;
- shell/state model;
- resource budgets;
- visual language;
- pocket-PC application set.

The goal is therefore neither to invent Linux again nor to transplant an entire existing distribution.

## UI architecture

The preferred progression is:

```
Applications
    ↓
Totoro-Pocket-PC Shell
    ↓
Tiny graphics/UI layer
    ↓
Framebuffer + touchscreen
    ↓
ARMv6 Linux userspace
    ↓
Stock kernel / Android hardware plumbing
```

Implementation stages:

1. **U0 — framebuffer proof:** PASSED.
2. **U1 — interactive native screen:** NEXT.
3. **U2 — reusable graphics primitives:** planned.
4. **U3 — desktop-like shell:** planned.
5. **U4 — pocket-PC applications:** planned.
6. **U5 — conventional GUI stack:** conditional, only if evidence justifies its resource cost.

## Design principle

> **Borrow the infrastructure; build the Totoro experience.**

A desktop-like experience does not require a desktop distribution. The first shell can be a tiny native ARMv6 program with direct framebuffer rendering and touchscreen input. More sophisticated libraries or a conventional compositor/window system can be evaluated later if they provide a measurable benefit.

## Preservation boundary

The identity does not change the project's safety rules.

The current product path remains reversible and Android-assisted. It does not require:

- PIT writes;
- repartitioning;
- EFS/modem writes;
- boot/recovery flashing;
- replacing the stock kernel;
- userdata wiping.

Independent Linux boot remains a conditional research branch, to be reopened only when a demonstrated product requirement cannot be satisfied by the reversible path.

## Success definition

The project succeeds when the physical Galaxy Y behaves like a coherent little computer rather than merely proving that Linux binaries can execute.

The first meaningful product milestone is therefore:

**A touch-driven, native, desktop-like Totoro shell running on the real LCD, backed by the verified ARMv6 Linux userspace.**
