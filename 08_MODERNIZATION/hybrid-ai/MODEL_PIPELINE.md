# Totoro Hybrid AI — Model Pipeline

Status: proposed reproducible workflow; no model is approved for device deployment yet.

## Purpose

Keep training and model preparation on the Mac, deploying only small, audited artifacts to Totoro or iPhone. Separate model semantics from transport and application policy.

## Pipeline

    Task definition and data provenance
                 |
                 v
        Host training / baseline
                 |
                 v
       Evaluation and selection
                 |
                 v
       Quantization / operator reduction
                 |
                 v
       Versioned deployment export
                 |
                 v
       Host reference fixtures
                 |
                 v
       ARMv6 build / iOS Core ML package
                 |
                 v
       Binary and model audit (hashes)
                 |
                 v
       Device tests and measurements
                 |
                 v
       Versioned manifest and rollback

## Task and data definition

Every model needs a narrow task statement, user benefit, input/output schemas, limitations and fallback. Record data source, consent/permission where relevant, license, preprocessing and data splits. Avoid personal data in initial proof-of-concept work.

## Model manifest

Record stable model ID and semantic version; task/capability; source or training run; license and data provenance; input/output schema and preprocessing revision; architecture, parameter count and quantization; required operators and runtime ABI; target and resource budget; numerical tolerance; artifact filename, size and SHA-256; evaluation metrics, failure cases and creation date.

Do not include secrets, personal samples or private specimen identifiers.

## Host reference

Run deterministic fixtures through the Mac reference model before deployment. Store small, non-sensitive test vectors and expected outputs. Define tolerance for floating-point/quantized differences and reject conversions that exceed documented tolerance.

## Totoro artifact

Prefer a tiny task-specific model and minimal C runtime compiled for the verified ARMv6 ABI. Avoid modern Android ML APIs unless compatibility is demonstrated. Keep model data separate from executable code and validate hashes. Deploy through the established reversible userspace/removable-storage workflow; do not modify boot-critical partitions or stock Android components.

## iPhone artifact

Package for the selected Core ML/iOS target. Record conversion tooling, model specification, minimum OS, compute-unit configuration and device measurements. Verify on the physical iPhone SE (2020); simulator success is not physical acceptance.

## Mac service artifact

Manage Mac-hosted models independently of TLP. The service advertises capability IDs and model versions, validates requests, applies data-routing policy and returns bounded structured results. Model runtime changes must not require changing the device envelope.

## Release and rollback

Use immutable versioned artifact names. Never overwrite an accepted model/runtime. Retain the previous known-good package and manifest. Verify hashes before and after transfer where practical. A failed update must leave the previous version usable.

## Evaluation and acceptance

Measure task metrics and representative failures; compare outputs against host fixtures; record latency distributions, peak memory, artifact size, malformed-input behavior, disconnect/timeout/low-resource behavior and power/thermal impact where measurable.

- [ ] Task and capability narrowly defined.
- [ ] Data provenance and license recorded.
- [ ] Host baseline and fixtures reproducible.
- [ ] Conversion/quantization effects measured.
- [ ] Target ABI/runtime compatibility audited.
- [ ] Model and binary hashes recorded.
- [ ] Physical target execution verified.
- [ ] Latency, memory and failure behavior documented.
- [ ] Privacy/data-routing policy explicit.
- [ ] Rollback to prior accepted artifact tested or documented.

A model is not supported on Totoro until the physical-device checklist is satisfied.
