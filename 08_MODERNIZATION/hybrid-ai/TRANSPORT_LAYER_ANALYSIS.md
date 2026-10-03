# Totoro Link — Transport Layer Analysis

Status: engineering assessment. Availability, stability and throughput require measurement on the physical GT-S5360 and selected Mac/iPhone configurations.

## Decision summary

Implement a transport-independent protocol and proceed in this order: USB/ADB for development when physically available; Wi-Fi/IP for routine local connectivity after a service is proven; an optional foreground iPhone relay; Bluetooth only after compatibility is verified. APN, VPN and cellular are not direct application protocols.

## Comparison

| Channel | Potential role | Advantages | Constraints / unknowns | Priority |
|---|---|---|---|---|
| USB + ADB | Development, shell, transfer, forwarding | Wired and debuggable | USB enumeration/ADB currently needs verification | First when available |
| Wi-Fi + TCP/IP | Direct Totoro–Mac service | Flexible; supports HTTP/TCP/WebSocket | Legacy Wi-Fi/security, setup, sleep and power behavior | Primary daily path |
| Bluetooth Classic | Small commands or relay | Wireless, modest data needs | Profiles, pairing and iOS compatibility require tests | Experimental |
| Bluetooth LE | Telemetry/control | Low power on suitable hardware | BLE support on this exact phone must not be assumed | Research only |
| iPhone hotspot | Local IP network | Portable network gateway | Band/security compatibility and routing vary | Optional |
| iPhone app relay | Mobile gateway | Modern iOS networking and compute | Permissions, lifecycle and background limits | Optional |
| APN/cellular | Wide-area IP | Potential remote connectivity | Carrier/band/service availability; inbound reachability not guaranteed | Later |
| VPN | Secure routed overlay | Useful for remote IP access | Requires functioning IP and compatible client/endpoint | Later |
| Audio signaling | Experimental data channel | Avoids conventional data interfaces | Slow, noisy and operationally awkward | Not planned |
| SD-card handoff | Offline exchange | No live link required | Manual and delayed | Fallback |

## USB / ADB

ADB is valuable for diagnostics, artifact transfer and port forwarding when the physical data path works. Project diagnostics have previously observed IP reachability with TCP/5555 refusing connections, and USB enumeration has also been unavailable. Therefore ADB is not assumed operational.

Diagnose in layers: cable/port and device mode; host USB enumeration; device/host ADB compatibility; daemon/listener state. Do not use flashing or recovery mode as a connectivity shortcut.

## Wi-Fi / IP

Wi-Fi is the preferred eventual transport. Begin with a small HTTP request/response API. Add WebSocket only if interactive bidirectional updates are useful. MQTT is optional and should be adopted only if publish/subscribe needs justify a broker.

Prefer Totoro initiating outbound connections to the Mac on a trusted local network. Do not expose a legacy service directly to the public internet. Pair peers, authenticate sessions, restrict capabilities and bound payloads. Measure association reliability, address changes, round-trip latency, reconnect time, idle power and Android sleep behavior.

## Bluetooth

Bluetooth may suit presence or small messages, but it is not selected until the exact Galaxy Y firmware exposes a usable compatible profile/API. Do not infer BLE support from modern Android documentation. iOS may require a supported accessory profile or dedicated companion app. Test pairing, reconnect, foreground/background behavior and throughput before selecting it.

## APN, hotspot and VPN

- APN configures carrier packet-data access; it does not define TLP or guarantee inbound reachability.
- Hotspot/tethering may place Totoro and Mac on a shared local network if device and security compatibility permit.
- VPN provides an IP overlay after ordinary IP connectivity works; it is not a substitute for a functioning link.
- Cellular service depends on radio bands, carrier support and provisioning. It is not required for the hybrid design.

## Transport policy

Use only explicitly configured, verified transports. Report unavailable, disconnected, connecting, connected or degraded state; authenticated peer; last successful exchange; latency/failure metrics; payload limit; and whether the path is local or relayed. Use bounded retries/backoff and never silently downgrade to an unauthenticated channel.

## Acceptance sequence

T0 host-only protocol tests. T1 USB/ADB ping/status if restored. T2 local Wi-Fi ping/status and reconnect tests. T3 bounded event sync and deduplication. T4 foreground iPhone relay. T5 Bluetooth feasibility only for a concrete use case. T6 remote access/VPN only after local security and reliability gates pass.

Record hardware/firmware, software versions, topology, duration, successful and failed exchanges, latency, reconnect behavior and security limitations.

## Recommendation

Build TLP once, then implement USB/ADB and Wi-Fi as interchangeable transports. Keep iPhone relay optional. Defer APN, VPN and Bluetooth until the local service is stable and their complexity is justified.
