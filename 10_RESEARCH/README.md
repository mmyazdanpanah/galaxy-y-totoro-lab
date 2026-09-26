# Research

Technical and historical research supporting the Totoro laboratory.

## Current research tracks

- modernization-history.md — historical map of mobile Linux/Android modernization and the implementation lessons relevant to Totoro.
- ../08_MODERNIZATION/plan.md — current engineering roadmap.

## Research rule

Separate:

1. Observed specimen evidence
2. Historical evidence
3. Community/project evidence
4. Engineering inference
5. Unverified hypotheses

Never promote a web-sourced claim into specimen evidence without provenance comparison.

## Current architectural conclusion

For this 2011-era device, the fastest reliable route is:

    existing Totoro hardware support
            ↓
    minimal Linux
            ↓
    Alpine / postmarketOS
            ↓
    lightweight usable system
            ↓
    mainline feasibility audit
            ↓
    selective upstreaming

This is a working hypothesis, not a completed hardware audit.

## Next research question

What exact kernel, board support, boot parameters, drivers, and historical Totoro Linux work can be reused with the least new code?

That question should be answered before substantial modernization code is written.
