# Totoro Link Protocol (TLP) — v0.1

Status: proposal for implementation and testing; not yet deployed on the physical Totoro.

## Scope and requirements

TLP is a transport-independent application message format for Totoro, Mac and an optional iPhone companion. USB, TCP/IP, Bluetooth or a relay are transports; they do not change message semantics. Version 0.1 is request/response oriented. Streaming, arbitrary commands and generic file transfer are out of scope.

Messages are small UTF-8 JSON envelopes with explicit version, ID, sender, recipient, type and timestamp. Receivers enforce size, depth, type and field limits, authenticate peers, authorize capabilities and fail closed on unsupported protocol versions.

## Example request

    {
      "protocol": "tlp/0.1",
      "id": "tot-000042",
      "kind": "request",
      "from": "totoro",
      "to": "mac",
      "sent_at": "2026-10-03T12:00:00Z",
      "capability": "device.status",
      "deadline_ms": 5000,
      "payload": {"detail": "basic"}
    }

## Example response

    {
      "protocol": "tlp/0.1",
      "id": "tot-000042",
      "kind": "response",
      "from": "mac",
      "to": "totoro",
      "sent_at": "2026-10-03T12:00:01Z",
      "status": "ok",
      "payload": {"device": "totoro", "link": "connected"}
    }

Examples are schema illustrations, not credentials or proof of implemented capability.

## Envelope fields

| Field | Required | Meaning |
|---|---|---|
| protocol | yes | Exact version, initially tlp/0.1 |
| id | yes | Unique correlation ID |
| kind | yes | request, response, event or error |
| from / to | yes | Registered logical node identifiers |
| sent_at | yes | UTC timestamp; tolerate clock skew |
| capability | request/event | Allowlisted capability |
| deadline_ms | request | Maximum useful processing window |
| status | response/error | ok, rejected, unsupported, timeout or failed |
| payload | yes | Capability-specific bounded object |

Validate nesting depth, string lengths, types, required fields and total encoded size before processing.

## Initial capability registry

- system.ping — harmless reachability test.
- device.status — return explicitly permitted basic status.
- event.append — submit a bounded, non-sensitive event.
- event.sync — synchronize queued event IDs.
- ai.summarize_status — request a short Mac-generated status summary.
- ai.classify — later, a named model/task request with a strict input schema.

Advertise a capability only after implementation and testing. Unknown capabilities return unsupported. Remote shell, arbitrary paths and generic code execution are prohibited.

## Stream framing

For stream transports such as TCP, use a 4-byte unsigned big-endian length prefix followed by exactly that many UTF-8 JSON bytes. The proposed initial maximum frame is 16 KiB. Reject zero-length, oversized, truncated and invalid UTF-8 frames. Message-oriented transports carry one complete envelope per message. Never assume TCP packet boundaries are message boundaries.

## Delivery and idempotency

IDs are unique within a sender's retained request window. Receivers retain recent completed IDs and deduplicate retries. Events use stable IDs and remain queued until acknowledged. Queue size and retention are bounded; overflow must be visible. Non-idempotent actions are excluded from v0.1 and would require replay protection and confirmation.

## Security

Pair peers explicitly; a claimed sender field is not authentication. Bind authenticated session identity to envelope identity and authorize capabilities per peer. Use TLS on supported modern endpoints. For a legacy endpoint, document TLS limitations and use a trusted link with a carefully designed application-level authentication mechanism where feasible; never commit or hard-code secrets. Apply replay windows, deadlines, rate limits and strict parsing. Never pass payload text to a shell, SQL statement, filesystem path or interpreter.

## Error example

    {
      "protocol": "tlp/0.1",
      "id": "tot-000042",
      "kind": "error",
      "from": "mac",
      "to": "totoro",
      "sent_at": "2026-10-03T12:00:02Z",
      "status": "rejected",
      "payload": {"code": "CAPABILITY_NOT_ALLOWED"}
    }

Errors must not expose stack traces, credentials or private specimen information.

## Required tests

Valid round-trip; unknown capability; unsupported version; malformed JSON; invalid UTF-8; missing fields; oversized frame; timeout; duplicate ID; replay; unauthorized peer; interrupted connection; and reconnect. Start with a Mac reference implementation and simulated Totoro client, then implement the smallest compatible physical client and record compiler, ABI, binary hash, memory use and device output.

Unknown optional fields may be ignored. Unknown required capabilities or versions must fail closed. Maintain shared fixtures across implementations.
