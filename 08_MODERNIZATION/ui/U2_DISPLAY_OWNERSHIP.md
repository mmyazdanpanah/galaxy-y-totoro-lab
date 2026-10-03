# U2 Display Ownership Investigation

Status: attribution phase substantially complete; resume at V5.

## Finding

The stock Android display stack can modify the Totoro framebuffer while a native framebuffer test is idle. Runtime ptrace tracing now directly identifies the active compositor thread and its framebuffer configuration calls.

## Runtime target

Tested stock build:

    SurfaceFlinger TID: 1484
    system_server TGID: 1477
    zygote PID: 1307
    framebuffer fd: 23
    framebuffer: /dev/graphics/fb0

SurfaceFlinger is a thread inside system_server on this build; stopping a separate init service named surfaceflinger did not eliminate the observed framebuffer interference.

## Passive evidence

A passive test wrote unique patterns once, selected a page once, and then performed no framebuffer writes or page pans during observation. The physical display changed while both framebuffer pages changed and yoffset later changed from 0 to 320.

Representative run:

    initial yoffset=0
    page0 checksum=0x3c5dd9c5
    page1 checksum=0xdf5bbfc5

    T+46619 ms: page0 -> 0x2597e67a
    T+46619 ms: page1 -> 0x9ec5b647
    T+46806 ms: yoffset 0 -> 320

This proves external framebuffer/display activity but does not identify the writer by itself.

## Ptrace V3

V3 attached to TID 1484 and observed ARM EABI syscall 54 (ioctl):

    fd=23
    request=0x4601
    request=FBIOPUT_VSCREENINFO

The argument contained a 240x320 visible mode with a 240x640 virtual framebuffer and yoffset alternating between 0 and 320. The ioctl returned 0. No 0x4606 FBIOPAN_DISPLAY or 0x46ff Samsung/private LCD ioctl was observed in the relevant trace.

## Ptrace V4

V4 retained the ioctl arguments and performed an observational PTRACE_PEEKDATA read of the same fb_var_screeninfo structure after successful return.

Observed sequence:

    BEFORE offset=0,320
    result=0
    AFTER  offset=0,320

    BEFORE offset=0,0
    result=0
    AFTER  offset=0,0

    BEFORE offset=0,320
    result=0
    AFTER  offset=0,320

Therefore the requested page configuration is accepted by the kernel and remains represented in the structure after the successful ioctl.

This is strong runtime evidence that SurfaceFlinger actively drives fb0 page configuration using FBIOPUT_VSCREENINFO on this build.

It is intentionally not phrased as proof that every LCD memory transition is caused by that ioctl: the exact framebuffer memory-write/composition path remains open.

## Why V5 exists

A later V4 invocation produced repeated ptrace SIGSTOP messages before reaching the first traced ioctl. The V4 stop/termination control was therefore considered too noisy for the next experiment.

V5 is the next tool and is designed to:

- trace only FBIOPUT_VSCREENINFO (0x4601);
- retain entry fd/request/argument values correctly;
- read before/after yoffset observationally;
- use a bounded observation window;
- avoid synthetic SIGSTOP termination of the target;
- detach cleanly.

V5 has been prepared and built locally, but no V5 physical acceptance run has been completed yet.

## ADB transport state at pause

After the V4 work, the phone remained reachable at 172.20.10.2:

    ICMP: 0% packet loss
    ARP: 172.20.10.2 -> b0:df:3a:fa:b3:b7
    TCP 5555: Connection refused
    adb connect: Connection refused

This is consistent with the phone remaining reachable while adbd is not listening on TCP/5555. USB ADB is currently unavailable on the Mac and no terminal/root shell is available directly on the phone.

Recovery boundary: use a normal Android reboot only. Do not use recovery/download mode, flashing, factory reset, partition changes, bootloader changes, PIT/EFS/modem changes, or kernel replacement for this transport issue.

## Current U2 state

    Framebuffer access/rendering                 PASS
    Page switching                              PASS
    Touch discovery/grab                        PASS
    Passive stability                           FAIL
    External Android display interference       CONFIRMED
    SurfaceFlinger TID attribution              CONFIRMED
    fb0 descriptor attribution                  CONFIRMED
    FBIOPUT_VSCREENINFO attribution             CONFIRMED
    Post-ioctl yoffset observation              CONFIRMED
    Exact framebuffer memory-write path         OPEN
    Display-session ownership mechanism         OPEN
    LVGL integration                            DEFERRED

## Next session

1. Restore ADB after a normal Android reboot.
2. Verify Android/system_server health.
3. Re-identify the current SurfaceFlinger TID; do not assume 1484 survives reboot.
4. Run V5 for the bounded observation window.
5. If V5 is clean, proceed to the reversible display/session ownership experiment.
6. Re-run the passive framebuffer stability test before integrating LVGL.
