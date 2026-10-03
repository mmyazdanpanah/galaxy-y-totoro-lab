# Totoro ↔ Mac Connectivity Architecture

Status: engineering research artifact
Date: 2026-10-03
Device: Samsung Galaxy Y GT-S5360 ("Totoro")
Known stock build: Android 2.3.6 JPLC1, Linux 2.6.35.7, root access

## Executive engineering conclusion

Totoro has substantially more usable communication paths than the current USB problem suggests. The engineering mistake would be trying to make one interface perform every job.

Recommended layered architecture:

    PRIMARY CONTROL
        Private Wi-Fi/IP
        -> ADB over TCP during Android bring-up
        -> SSH/Dropbear for the Linux userspace
        -> file transfer and diagnostics

    RECOVERY / PROVISIONING
        USB
        -> physical ADB
        -> possible USB networking/tethering
        -> recovery and deployment

    AUXILIARY CONTROL
        Bluetooth
        -> pairing / file exchange
        -> PAN only where the host OS actually supports it
        -> custom L2CAP transport only as a later research project

    WAN / INTERNET
        Dormitory Wi-Fi on Mac
        OR Totoro cellular data through APN
        OR a private router with upstream Internet

    REMOTE OVERLAY
        VPN or SSH reverse tunnel
        -> only after an underlying IP path exists

    OFFLINE RECOVERY
        microSD
        -> binaries, logs, rootfs, scripts, configuration

The key principle is:

    Separate Internet transport from Totoro control transport.

A single Mac Wi-Fi radio cannot normally maintain two independent Wi-Fi associations simultaneously. Therefore a permanent dual-path design needs a second physical interface, a private router, USB, or another independent transport.

## 1. Known Totoro capabilities

Project evidence and device documentation establish:

- Wi-Fi 802.11 b/g/n with hotspot functionality.
- Bluetooth 3.0 with A2DP.
- microUSB 2.0.
- GSM/GPRS/EDGE and 3G/HSDPA cellular data; the model is documented with HSDPA up to 7.2 Mbps.
- Android 2.3.x / 2.3.6 stock software.
- USB tethering and portable Wi-Fi hotspot.
- VPN configuration in stock Android.
- FM radio and GPS.
- Root access in the current project specimen.
- Earlier project sessions successfully used ADB over TCP at 172.20.10.2:5555.
- A later state showed 172.20.10.2 reachable by ICMP/ARP while TCP/5555 was refused. This proves that IP reachability and adbd listening are separate conditions.
- The project has a persistent SD-backed ext2 Linux userspace, native framebuffer access, touchscreen access, and root-controlled experimentation.

## 2. Channel engineering matrix

| Channel | IP-capable | ADB/SSH | Internet | Engineering role |
|---|---:|---:|---:|---|
| USB | Potentially, via USB tethering/networking | Yes | Yes, via tethering | Primary recovery/provisioning |
| Wi-Fi infrastructure | Yes | Yes | Yes | Primary normal network |
| Wi-Fi hotspot | Yes | Yes | Yes via Totoro cellular data if tethering works | Best isolated LAN experiment |
| Bluetooth PAN | Historically yes | If IP interface exists | Potentially | Legacy/conditional |
| Bluetooth L2CAP/RFCOMM | Not automatically | Custom transport possible | No | Auxiliary research channel |
| Cellular APN | Yes on Totoro | Indirectly | Yes | WAN/upstream Internet |
| VPN | Overlay | Yes through tunnel | Depends on lower path | Remote overlay |
| FM radio | No | No | No | Not a digital data channel |
| GPS | No | No | No | Location/time telemetry |
| microSD | No | No | No | Offline deployment/recovery |
| Audio jack | No native IP | Experimental | No | Experimental acoustic modem |
| UART/test pads | No native IP | Serial shell possible | No | Hardware recovery if identified |
| Camera | No | No | No | Optical side channel |
| NFC | No; not present on GT-S5360 | No | No | Not available |

## 3. USB

USB should remain the preferred physical recovery path even though the Mac currently does not detect Totoro.

Advantages:

- independent of Wi-Fi client isolation;
- physical access;
- ADB can be used to restore/configure wireless ADB;
- stock software documents USB tethering;
- potentially useful as a network interface through USB tethering/RNDIS, subject to exact JPLC1/macOS behavior.

Current project evidence of no device in macOS system_profiler/ioreg is a physical/enumeration diagnostic problem, not evidence that USB is unsuitable.

Role:

    USB = recovery + provisioning + high-confidence ADB

Do not change boot-critical storage merely to restore networking.

## 4. Wi-Fi

Wi-Fi is the strongest general-purpose network channel.

### Private LAN

    Internet
       |
    private router
       |
       +---- Mac
       |
       +---- Totoro

Reserve a fixed DHCP address for Totoro.

Then:

    adb connect <totoro-ip>:5555
    ssh root@<totoro-ip>

This is the cleanest long-term laboratory topology.

### Totoro hotspot

    cellular APN
        |
      Totoro
      Wi-Fi AP
        |
       Mac

This is the best immediate experiment if a working SIM/data plan is available: the Mac gets a local path to Totoro and can potentially use Totoro's cellular data for Internet access.

### Dormitory Wi-Fi

The Mac currently has:

    en0 = Wi-Fi
    172.30.38.9

The dormitory network may isolate clients. If so, both devices can have Internet while being unable to talk directly.

A failed ping alone does not prove isolation; test the actual Totoro address and TCP/5555.

## 5. Bluetooth

Totoro has Bluetooth 3.0 and the stock manual documents Bluetooth pairing/data exchange.

### Bluetooth PAN

Historically macOS exposed Bluetooth PAN as a normal network service. Apple removed the built-in Bluetooth PAN networking surfaces from macOS Monterey-era releases. Therefore a modern Mac should not be treated as if Bluetooth PAN were guaranteed.

If the Mac's network services do not show Bluetooth PAN, do not make PAN the foundation of the project.

### Custom Bluetooth transport

Bluetooth remains interesting because classic Bluetooth can expose L2CAP channels. A custom Mac application could pair with Totoro and implement a small framed transport:

    Mac app
      |
    IOBluetooth
      |
    L2CAP
      |
    Totoro Bluetooth stack
      |
    custom framing
      |
    command/file protocol

This is feasible as a research project but is substantially more work than Wi-Fi or USB.

Role:

    Bluetooth = auxiliary low-bandwidth control/discovery
    not the first-line ADB network

## 6. APN / cellular data

The Galaxy Y exposes Access Point Names under:

    Settings
      -> Wireless and network
      -> Mobile networks
      -> Access Point Names

APN configures Totoro's carrier packet-data connection. It does not itself create a private Mac-Totoro LAN.

Useful topology:

    carrier
       |
      APN
       |
    Totoro
       |
    Wi-Fi hotspot
       |
      Mac

This can provide both:

- Mac -> Totoro local communication;
- Mac -> Internet through Totoro's cellular data.

Carrier NAT normally makes direct inbound access to Totoro from the public Internet unsuitable. A private APN/static address is carrier-specific and unnecessary for the initial architecture.

Role:

    APN = Totoro WAN uplink

## 7. VPN

Stock Galaxy Y software includes VPN settings, including VPN type selection. Samsung documentation for this Android generation includes PPTP and L2TP/IPsec variants.

VPN is an overlay, not a physical transport.

It only becomes useful after one of these works:

    Wi-Fi
    USB networking
    Bluetooth IP networking
    cellular Internet

Potential remote topology:

    Totoro -> APN -> VPN endpoint -> Mac/network

For this project, VPN should be treated as a later remote-access layer. Old Android VPN implementations and older protocols should not become the foundation of local device control.

An SSH reverse tunnel may ultimately be simpler for remote development.

## 8. ADB

ADB is the bootstrap/debug transport.

The current project has already demonstrated:

    adb -s 172.20.10.2:5555 ...

The later state:

    ICMP        reachable
    ARP         reachable
    TCP 5555    connection refused
    adb         connection refused

means the network path can remain healthy while adbd is stopped/not listening.

For a stable setup:

1. Restore USB ADB if possible.
2. Use root access to configure TCP ADB.
3. Give Totoro a predictable local IP.
4. Verify adbd starts reliably.
5. Add a small Mac reconnect helper.
6. Transition routine Linux work to SSH/Dropbear.

Legacy Android 2.x ADB-over-TCP can be enabled with the rooted property/restart mechanism, but it is an old debug protocol. Never expose port 5555 to an untrusted network.

## 9. SSH / Dropbear

Once the SD-backed Linux userspace is reliable, SSH should become the normal Linux control protocol.

Target:

    Mac
      |
    IP transport
      |
    Totoro Linux
      |
    Dropbear/sshd
      |
    shell

Benefits:

- normal remote shell;
- file transfer;
- reverse tunnels;
- independent of Android's ADB implementation;
- fits the project's minimal Linux strategy.

Therefore:

    ADB = bootstrap/debug
    SSH = normal Linux operation

## 10. microSD

microSD is not a network channel, but it is an unusually valuable independent data plane for a 2011 device.

Use it for:

- ARMv6 binaries;
- rootfs;
- scripts;
- configuration;
- logs;
- framebuffer diagnostics;
- firmware evidence;
- offline recovery payloads.

Role:

    microSD = offline deployment + recovery + archival transport

## 11. FM, GPS, audio, camera, UART

### FM

The FM subsystem is not a practical digital Mac-Totoro link. Keep it outside the normal architecture.

### GPS

GPS is receive-only for our purposes. It can supply location/time telemetry but cannot provide a Mac control path.

### Audio jack

An acoustic modem is theoretically possible but would be slow, fragile, and unnecessary compared with USB/Wi-Fi/Bluetooth.

### Camera

An optical side channel could carry QR/configuration data, but is unsuitable for continuous control.

### UART/test pads

If later hardware archaeology identifies safe UART test pads, UART could become an excellent boot/recovery console. It must be verified electrically before use.

## 12. Permanent connection hierarchy

    Tier 0  microSD
            offline recovery/deployment

    Tier 1  USB
            physical recovery / ADB / provisioning

    Tier 2  private Wi-Fi/IP
            normal network

    Tier 3  SSH/Dropbear
            Linux application/control protocol

    Tier 4  Bluetooth
            auxiliary transport

    Tier 5  APN
            Internet upstream

    Tier 6  VPN / reverse SSH
            remote overlay

    Tier 7  FM/GPS/audio/optical/UART
            specialized research channels

This is an allocation of engineering responsibilities, not a claim that every channel has the same performance characteristics.

## 13. Recommended project topology

Long-term:

                 DORM INTERNET
                       |
                 [PRIVATE ROUTER]
                    /       \\
                   /         \\
                Mac           Totoro
                 |               |
              Wi-Fi           Wi-Fi
                 |               |
                 +---- TCP/IP --+
                       |
                 ADB / SSH / SCP

                         +
                         |
                    USB recovery
                         |
                         +---- ADB / provisioning

                         +
                         |
                    Bluetooth
                         |
                  auxiliary control

                         +
                         |
                    microSD
                         |
                offline recovery

The private router creates a stable Totoro laboratory subnet while its upstream connection can provide Internet. This avoids depending on dormitory client-to-client routing.

If a router is not available, the temporary topology is:

    Totoro cellular APN
          |
    Totoro Wi-Fi hotspot
          |
         Mac

or:

    Mac dorm Wi-Fi
          |
      independent
      second transport
          |
        Totoro

## 14. Decision gates

### N1 — Wi-Fi local reachability

Pass when:

- Mac reaches Totoro;
- TCP/5555 opens when adbd is listening;
- chosen Internet path remains usable.

### N2 — persistent ADB

Pass when:

- Totoro retains intended network settings after reboot;
- adbd starts as intended;
- Mac reconnects without manual intervention.

### N3 — SSH

Pass when:

- Linux userspace starts;
- Dropbear/SSH starts;
- Mac executes a shell and transfers files.

### N4 — dual-path operation

Pass when:

- Internet remains on the chosen upstream;
- Totoro remains reachable on the control path;
- changing Internet transport does not destroy Totoro control.

### N5 — recovery

Pass when:

- USB is available when repaired;
- microSD contains known-good recovery/deployment payload;
- Wi-Fi loss does not make Totoro unrecoverable.

## 15. Immediate experiment sequence

When returning to Totoro:

1. Enable Portable Wi-Fi Hotspot.
2. Connect Mac to Totoro.
3. Record both IP addresses and subnet.
4. Test local reachability.
5. Test TCP/5555.
6. If 5555 is refused, diagnose adbd separately rather than blaming Wi-Fi.
7. Pair Totoro and Mac over Bluetooth.
8. Check whether macOS exposes a Bluetooth PAN service.
9. Test Totoro mobile data/APN separately.
10. Test whether Mac Internet works through Totoro's hotspot.
11. Diagnose USB enumeration independently.
12. Once one IP path is stable, deploy SSH/Dropbear in the Linux userspace.

## 16. Engineering position

Totoro should be treated as a small networked computer with multiple independent transports, not as a phone with one connection method.

The clean division is:

    Wi-Fi       = normal network
    USB         = physical recovery
    ADB         = bootstrap/debug
    SSH         = Linux control
    Bluetooth   = auxiliary transport
    APN         = Internet upstream
    VPN/reverse SSH = remote overlay
    microSD     = offline recovery
    FM/GPS/etc. = specialized capabilities

For the current dormitory environment, the highest-value experiment is not trying to make one Wi-Fi radio associate with two access points. It is establishing a dedicated Totoro control path while leaving the Mac's existing Internet path untouched.

Practical candidate order:

    1. Totoro hotspot + Totoro cellular data
    2. Private router with dormitory upstream
    3. USB restoration
    4. Bluetooth auxiliary transport
    5. VPN/remote overlay

A permanent Totoro lab should ultimately have at least two independent recovery/control paths: Wi-Fi/IP for normal work and USB or microSD for recovery.

## Sources

- Samsung Galaxy Y GT-S5360 support:
  https://www.samsung.com/hk_en/support/model/GT-S5360MAATGY/
- Samsung Galaxy Y manual, including Bluetooth, Wi-Fi, USB tethering, mobile-network sharing and VPN:
  https://www.manualslib.com/manual/261236/Samsung-Galaxy-Y-Gt-S5360.html
- Galaxy Y APN procedure:
  https://deviceguides.vodafone.ie/samsung/galaxy-y/explore/set-up-your-phone-for-internet/
- Galaxy Y USB tethering procedure:
  https://deviceguides.vodafone.ie/samsung/galaxy-y/change-settings/use-tethering/
- Galaxy Y specifications:
  https://www.gsmarena.com/samsung_galaxy_y_s5360-4117.php
- Apple Network Extension routing:
  https://developer.apple.com/documentation/networkextension/routing-your-vpn-network-traffic
- Project evidence:
  08_MODERNIZATION/ui/README.md
  08_MODERNIZATION/ui/U2_DISPLAY_OWNERSHIP.md
  99_SANDBOX/00_LOGS/TERMINAL_RESULTS
