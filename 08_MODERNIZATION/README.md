# Modernization

Modernize the Galaxy Y without pretending it is a modern smartphone.

## Strategy

The current engineering strategy is documented in [plan.md](plan.md):

1. freeze and preserve the specimen
2. audit existing Totoro kernel/hardware work
3. prove a minimal Linux boot
4. move to Alpine/postmarketOS userspace
5. bring up useful hardware in dependency order
6. audit mainline feasibility
7. upstream individual components only when useful
8. pursue a fully mainline kernel only if justified

## Success target

**M3 — Modern Totoro:** a reproducible Alpine/postmarketOS-based Linux system with a lightweight usable interface and documented hardware support.

M4 — Upstream Totoro is optional.

## Current research priorities

- exact historical Totoro kernel lineage
- BCM21553 support
- boot image and kernel command line
- reusable community kernels and device trees
- existing postmarketOS/Totoro work
- existing upstream/mainline BCM21553 work
- minimum kernel + initramfs + BusyBox boot
