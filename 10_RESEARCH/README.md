# Research

Technical and historical research supporting the Totoro laboratory.

## Current tracks

- modernization-history.md — broader mobile Linux/Android modernization history.
- totoro-reuse-map.md — concrete reusable Totoro projects and what each contributes.
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
