# Research

Technical and historical research supporting the Totoro laboratory.

## Current tracks

- modernization-history.md — broader mobile Linux/Android modernization history.
- totoro-reuse-map.md — concrete reusable Totoro projects and what each contributes.
- kernel-artifact-provenance.md — verified zImage artifact facts and explicit source/config/compiler provenance limits.
- ../08_MODERNIZATION/plan.md — current engineering roadmap.

## Evidence classes

Keep separate:

1. observed specimen evidence
2. historical evidence
3. community/project evidence
4. engineering inference
5. unverified hypothesis

Never promote web-sourced material into specimen evidence without provenance comparison.

## Current conclusion

The existing Totoro ecosystem is large enough that new implementation should begin only after extracting reusable pieces.

    Samsung source
          +
    community kernel/ramdisk work
          ↓
    reproducible kernel
          ↓
    known boot path
          ↓
    minimal Linux
          ↓
    Alpine / postmarketOS userspace
          ↓
    useful system

Mainline work comes only after a useful downstream-based system exists.

## Research stopping rule

Do not expand archaeology unless it answers a concrete implementation question.

For each blocker:

    existing solution?
        ↓ yes → reuse
        ↓ no
    small fix?
        ↓ yes → fix
        ↓ no
    proven alternative?
        ↓ yes → adapt
        ↓ no → new implementation


## Root-package evidence — 2026-09-29

A historical GT-S5360 root `update.zip` candidate has been preserved as a hash-identified research artifact. Verified facts:

- size: 2,260,360 bytes
- MD5: `eac189609fd71de6bf053e7ff2636d7e`
- SHA-1: `89108755e3cf1d6c298e60fc963881dacb3d313d`
- SHA-256: `3e4ebe31b908ea3a8750347f875f91493f550edd1cd2a3006293c45a41592a27`
- ZIP integrity: passed
- payload includes `su`, `Superuser.apk`, BusyBox, SSH, and sqlite3.

The package is not yet classified as safe to install. The next research action is static inspection of `META-INF/com/google/android/updater-script` and `update-binary` to establish exact device assertions and all filesystem/partition operations. Provenance, package compatibility, and safety of operations must remain separate evidence classes.
