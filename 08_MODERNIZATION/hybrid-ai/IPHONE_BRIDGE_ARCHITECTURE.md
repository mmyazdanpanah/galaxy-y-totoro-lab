# iPhone SE (2020) Bridge Architecture

Status: optional architecture proposal; no iOS companion or physical bridge is implemented yet.

## Role

The iPhone SE (2020) can complement Totoro with modern mobile compute, camera, microphone, supported Core ML/Vision capabilities and current networking. It is an optional perception and relay node, not an unrestricted background server or a required dependency.

## Interaction patterns

### Foreground companion — initial target
A user opens a dedicated iOS app, pairs it with Totoro/Mac and explicitly initiates a task. The app captures permitted input, runs an available on-device model or relays a request to the Mac, then returns a bounded result. This is observable and avoids depending on continuous background execution.

### Local-network bridge
When connected to a trusted network, the app communicates with the Mac service and, if routing permits, Totoro. Handle local-network privacy permission and app lifecycle behavior explicitly.

### User-mediated handoff
Where direct connectivity is unavailable, use explicit share/export/import or a QR/text payload for small non-sensitive messages. This is a fallback, not a transparent always-on bridge.

### Background operation — conditional
Background networking and Bluetooth depend on iOS APIs, declared capabilities, scheduling, power state and permissions. Do not promise continuous relay or arbitrary background execution. Prototype foreground behavior first, then test background modes against current platform documentation and the physical phone.

## Responsibilities

- Capture camera/audio only after clear user action and permission.
- Run only models supported by the selected iOS deployment target.
- Return structured results rather than unbounded transcripts/media by default.
- Relay TLP envelopes while preserving sender, relay identity and correlation IDs.
- Display connection, permission, task and data-sharing state.
- Provide controls to stop capture, revoke pairing and clear retained data.

## Data flow

    Totoro / User
         |
         v
    iPhone companion
         |----> local Core ML / Vision / supported speech processing
         |
         +----> authenticated Totoro Link request to Mac
         |
         v
    bounded result + provenance
         |
         v
    Totoro UI or Mac task record

The iPhone must not impersonate Totoro. The Mac distinguishes the authenticated iPhone peer from a relayed Totoro message.

## Pairing and trust

Pairing is user-initiated and confirmed on relevant devices. Store credentials in platform-appropriate protected storage and never commit secrets. Revoke pairing explicitly. Authorize capabilities per peer; permission to submit a classification must not imply device-control rights.

## Example tasks

Image capture and classification; short audio capture and supported transcription; forwarding text prompts to the Mac; relaying Totoro events; and offline perception where a compatible model is installed. Verify each API/model against the actual iOS target before claiming support.

## Privacy and failure behavior

Respect camera, microphone, local-network and Bluetooth permissions. Make capture/transmission visible. Minimize retention and avoid logging raw media or personal content. State whether processing occurs on iPhone, Mac or an external service. Handle app suspension, disconnect and reconnect without data loss or false success. Cloud services are not required for core operation.

## Implementation gates

I0 shared TLP data model and fixtures. I1 foreground ping/status to Mac. I2 user-triggered task and bounded result. I3 one measured Core ML/Vision task. I4 optional Totoro event relay with deduplication. I5 background behavior only if required by a concrete use case.

Acceptance requires physical iPhone testing of pairing, permissions, authentication, task completion, timeout, suspension, reconnect, deletion and privacy behavior.
