# Modernization

Modernize the Galaxy Y by reusing the large body of existing Totoro work before writing new code.

## Current route

    Samsung kernel
         ↓
    proven Totoro boot/ramdisk
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

## Primary target

M3 — Modern Totoro: a reproducible, lightweight Linux system with useful hardware support and a maintainable userspace.

A fully mainline kernel is optional.

## Current phase

M1-A — reproduce the Samsung kernel.

The source, Totoro defconfig, and historical ARM EABI 4.4.3 toolchain family have been identified. The remaining host issue is execution of the old i386 compiler, so the next step is a contained Linux build environment.

Phone: not required yet.

## New reuse strategy

Before implementing anything, consult:

- 10_RESEARCH/totoro-reuse-map.md
- Samsung BCM21553 source
- historical Totoro kernel/ramdisk projects
- device trees and vendor trees
- historical AndroidARMv6 work

The first boot image should reuse a known Totoro ramdisk rather than inventing a new one.

## Working rule

Simple fix first. Alternative second. Reinvention last.
