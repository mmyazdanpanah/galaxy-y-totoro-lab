# Totoro Hybrid AI — Architecture Package

Status: proposed architecture; no hybrid link or AI inference acceptance is claimed yet.

This package defines a reversible path to a distributed AI system around the physical Samsung Galaxy Y GT-S5360 (Totoro), with a Mac as primary AI host and an iPhone SE (2020) as an optional mobile perception and gateway node.

## Design decision

Treat Totoro as an offline-capable physical interface and edge node, not as a host for a modern conversational LLM. The Mac provides larger-model inference, orchestration and durable knowledge services. The iPhone participates only through explicitly implemented, permissioned interfaces. Communication is independent of transport and AI runtime.

## Documents

- TOTORO_HYBRID_AI_ARCHITECTURE.md — nodes, responsibilities, task placement and failure behavior.
- TOTORO_LINK_PROTOCOL.md — transport-independent messages, identity, validation and versioning.
- TRANSPORT_LAYER_ANALYSIS.md — USB/ADB, Wi-Fi/IP, Bluetooth and future options.
- IPHONE_BRIDGE_ARCHITECTURE.md — iOS constraints, bridge patterns and role boundaries.
- EDGE_AI_STRATEGY.md — realistic on-device inference scope and alternatives.
- MODEL_PIPELINE.md — train, convert, audit, deploy and measure.

## Current project context

Verified physical milestones include Android-assisted ARMv6 Linux userspace execution, persistent SD-backed ext2 use, and framebuffer/touchscreen experiments. A stable network service and complete native UI/session ownership remain separate engineering tasks. This package is architectural planning, not evidence that communication, the iPhone bridge or inference already works.

## First implementation gate

Implement a minimal Totoro-to-Mac request/response path over the first available safe transport. Begin with host-side protocol tests and a harmless ping/status exchange. Do not couple this gate to model inference or modify boot-critical storage.

## Preservation and security

Follow repository AGENT rules. Keep experiments reversible; do not write boot/recovery/PIT/EFS/modem/raw partition state. Do not expose credentials or private specimen data in logs. Authenticate peers, allowlist harmless operations, and treat all device-provided payloads as untrusted.
