# Totoro Hybrid AI Architecture

Status: proposed architecture. Implementation and physical-device acceptance are pending.

## Purpose

Develop Totoro into a useful, computer-like historical artifact that participates in a modern distributed AI system without requiring the original Galaxy Y to run a contemporary large language model.

- **Totoro — Samsung Galaxy Y GT-S5360:** physical interface, device state, event capture and bounded local logic.
- **Mac:** primary AI host for local language models, orchestration, durable memory, development and evaluation.
- **iPhone SE (2020), optional:** mobile perception and connectivity node using supported iOS capabilities and explicit user-mediated interactions.

This is a distributed application, not a claim that intelligence is located in one device or that devices share unrestricted access.

## Constraints and evidence

The repository's verified baseline is an ARMv6-era device running Android 2.3.6 and Linux 2.6.35.7, with constrained memory and storage. Android-assisted native Linux userspace, persistent SD-backed ext2, and framebuffer/touch access have been demonstrated. A stable network service and complete native UI/session ownership remain separate engineering tasks.

Do not assume current Android ML frameworks, modern model formats, CPU instructions or memory budgets are compatible. Each binary and model requires specific compatibility and physical testing.

## Logical architecture

    MAC — PRIMARY AI HOST
    +--------------------------------------+
    | Task router / policy / agent runtime |
    | Local LLM and other inference        |
    | Durable memory / indexed knowledge   |
    | Model build, audit and evaluation    |
    | Totoro Link host service             |
    +-------------------+------------------+
                        |
                Totoro Link protocol
                        |
          +-------------+-------------+
          |                           |
    TOTORO — EDGE NODE           IPHONE — OPTIONAL
    +------------------+         +------------------+
    | UI / interaction |         | Camera / sensors |
    | Local state/logs |         | Speech / Core ML |
    | Local rules      |         | Mobile gateway   |
    | Tiny classifiers |         | Link companion   |
    | Link client      |         | User permissions |
    +------------------+         +------------------+

The protocol is independent of physical transport. A node advertises only capabilities verified on that device and software build. The Mac is the default coordinator, but Totoro must not require it for basic local behavior.

## Responsibilities

### Totoro
- Present a compact interface suited to 240×320 and constrained input.
- Maintain local identity, configuration, bounded event log and connection state.
- Execute a small allowlist of local commands.
- Perform deterministic rules and, after a native runtime is proven, small quantized inference.
- Queue eligible events for later synchronization.
- Reject malformed, oversized, stale or unauthorized commands.

Totoro must not store Mac credentials in plaintext, expose an unauthenticated shell, or depend on permanent connectivity.

### Mac
- Host Totoro Link and task routing.
- Run local LLMs and larger inference workloads where resources permit.
- Maintain long-term memory with explicit retention/deletion controls.
- Build, convert, validate and benchmark tiny models.
- Route tasks based on capability, privacy, connectivity and latency.
- Return bounded, display-appropriate results.

Expose a narrow application API, not a general-purpose remote shell.

### iPhone SE (2020)
- Optionally provide camera/audio capture, supported speech processing, Core ML inference and mobile connectivity.
- Communicate through a dedicated iOS companion or user-mediated share/export flow.
- Relay only user-permitted messages and respect iOS background/network behavior.
- Remain optional for the first milestone.

## Task placement

| Task | Default execution | Fallback |
|---|---|---|
| UI, status, command allowlist | Totoro | Remain local |
| Event timestamping and bounded logs | Totoro | Queue until sync |
| Simple rules | Totoro | No remote dependency |
| Tiny sensor classifier | Totoro after validation | Mac |
| Speech and image perception | iPhone or Mac | Text/manual input |
| Language reasoning and synthesis | Mac | Report unavailable offline |
| Training and conversion | Mac | Not performed on Totoro |
| Long-term knowledge retrieval | Mac | Limited local cache |

The router must not silently transmit sensitive content. Each task class needs a declared data policy.

## Operating modes

1. Standalone: local UI, status, rules and event capture.
2. USB development: message exchange over a verified USB/ADB path.
3. Local network: authenticated exchange over a trusted LAN.
4. iPhone-mediated: optional user-permitted relay.
5. Disconnected: local functions continue; queued events sync later.

Mode changes must be observable in UI and logs.

## Failure and safety behavior

Peer loss must not crash the UI or corrupt local queues. Requests need unique IDs, bounded payloads, deadlines and explicit status. Retries must be idempotent or protected against duplicate execution. Remote actions are allowlisted and authenticated. Never execute arbitrary received shell commands. Avoid public exposure of legacy services, minimize sensitive logging, and retain ADB/recovery access during development.

## Implementation gates

- H0: Mac-only protocol/schema tests, including malformed, oversized, duplicate and unauthorized requests.
- H1: physical Totoro ping/status over a verified transport.
- H2: bounded event queue and exactly-once-effect synchronization through deduplication.
- H3: one harmless authenticated remote task with timeout/error handling.
- H4: one measured tiny local classifier.
- H5: optional foreground iPhone bridge.
- H6: policy-driven task routing and offline fallback.

## Non-goals

Running a modern LLM on the original Galaxy Y is not a prerequisite. No boot replacement, arbitrary remote shell, required cloud service or mandatory iPhone dependency is introduced.

## Decision

Implement transport-independent Totoro Link and a Mac-hosted service first. Keep the iPhone bridge optional and defer Totoro inference until communication and runtime constraints are measured. This path is incremental, testable and reversible.
