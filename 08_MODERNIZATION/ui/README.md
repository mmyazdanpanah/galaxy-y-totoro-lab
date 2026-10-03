# Totoro Native UI — first interactive display

This is the first deliberately small interactive Linux UI for the physical Samsung Galaxy Y GT-S5360 (totoro).

Design:
- ARMv6 userspace;
- /dev/graphics/fb0 direct mmap;
- 240x320, 32-bpp framebuffer;
- Samsung/Broadcom LCDFB_IOCTL_UPDATE_LCD (0x46ff) dirty-row refresh;
- /dev/input/event4 touchscreen input;
- one touchable control;
- no boot-critical storage access.

This is a physical-device bring-up artifact, not yet an independent Linux graphical session. Android may redraw over the framebuffer because Android still owns the display.

## Build

Use the already-established Totoro musl toolchain:

    cd /Users/mostafa/Workspace/03_Projects/Engineering/galaxy-y-totoro-lab/08_MODERNIZATION/ui

    LLVM=/opt/homebrew/opt/llvm
    LLD=/opt/homebrew/opt/lld@21/bin/ld.lld
    MUSL=/tmp/totoro-musl-build/sysroot
    CRT=/tmp/compiler-rt-totoro/build-builtins-totoro/lib/linux/libclang_rt.builtins-arm.a

    "$LLVM/bin/clang"       --target=arm-linux-gnueabi       -march=armv6 -marm -mfloat-abi=soft       -O2 -fno-stack-protector -fno-pie       -nostdinc -isystem "$MUSL/include" -static       -fuse-ld="$LLD" -Wl,-e,_start       -o totoro-native-ui totoro-native-ui.c       "$MUSL/lib/crt1.o" "$MUSL/lib/crti.o" "$MUSL/lib/crtn.o"       "$CRT" "$MUSL/lib/libc.a"

Then audit:

    /opt/homebrew/opt/llvm/bin/llvm-readelf -h -l -A totoro-native-ui
    shasum -a 256 totoro-native-ui

Confirm ELF32/ARM/EABI5/ARMv6/static/no PT_INTERP.

## Transfer and run

    adb devices
    adb -s 172.20.10.2:5555 push totoro-native-ui /data/local/tmp/totoro-native-ui
    adb -s 172.20.10.2:5555 shell chmod 755 /data/local/tmp/totoro-native-ui
    adb -s 172.20.10.2:5555 shell su -c /data/local/tmp/totoro-native-ui

Expected startup:

    TOTORO_NATIVE_UI
    FB 240x320 virtual_y=640 bpp=32 stride=960 yoffset=...
    TOUCH /dev/input/event4

Touch the middle button. Expected console output:

    TOUCH x=<x> y=<y> state=ON

Touch again to toggle OFF.

## Acceptance gate

Pass only when:
1. The custom screen is visibly rendered.
2. The touch control responds.
3. The console reports matching X/Y coordinates.
4. Several toggles work without a crash.
5. Android returns normally after exit.

Record the binary SHA-256 and terminal output before changing the program.

## Safety

The program accesses only /dev/graphics/fb0 and /dev/input/event4. It does not write PIT, BML/STL, EFS, modem, boot/recovery, userdata, or kernel images.

Do not run it together with other framebuffer writers.


## U1 physical acceptance — deterministic native UI

U1 is physically accepted on the Samsung Galaxy Y GT-S5360 (totoro).

The milestone combines deterministic framebuffer selection, native framebuffer rendering, LCD refresh, exclusive touchscreen ownership, touch-state handling, and safe recovery to Android.

### Hardware acceptance

- `/dev/graphics/fb0` opened successfully.
- Framebuffer: `240x320`, `32 bpp`.
- Virtual framebuffer: `240x640`.
- Line length / stride: `960` bytes.
- `mmap()` of the framebuffer succeeded.
- `FBIOPAN_DISPLAY` was verified independently for both framebuffer pages.
- Page 0 (`yoffset=0`) and page 1 (`yoffset=320`) were both physically visible on the LCD.
- U1 explicitly selects page 0 before the first native render.
- `LCDFB_IOCTL_UPDATE_LCD` (`0x46ff`) successfully refreshes the native display.
- `/dev/input/event4` (`sec_touchscreen`) is used for touchscreen input.
- `EVIOCGRAB` successfully gives the native UI exclusive touchscreen ownership.
- Multiple physical button presses produced native ON/OFF state transitions.
- Exiting with `Ctrl-C` released the touchscreen grab and Android touchscreen operation returned normally.

### U1 acceptance run

Physical device output:

    TOTORO_NATIVE_UI
    FB 240x320 virtual_y=640 bpp=32 stride=960 yoffset=0
    TOUCH /dev/input/event4
    TOUCH x=152 y=221 state=ON
    TOUCH x=144 y=230 state=OFF
    TOUCH x=137 y=227 state=ON
    TOUCH x=148 y=185 state=OFF
    TOUCH x=150 y=222 state=ON
    TOUCH x=146 y=226 state=OFF
    TOUCH x=148 y=227 state=ON
    TOUCH x=142 y=225 state=OFF
    TOUCH x=140 y=225 state=ON
    TOUCH x=144 y=222 state=OFF
    TOUCH x=140 y=224 state=ON
    TOUCH x=131 y=225 state=OFF
    TOUCH x=129 y=224 state=ON
    TOUCH x=127 y=225 state=OFF
    TOUCH x=120 y=225 state=ON
    TOUCH x=173 y=224 state=OFF
    TOUCH x=185 y=231 state=ON
    TOUCH x=80 y=225 state=OFF
    TOUCH x=93 y=229 state=ON

The U1 integration binary was rebuilt for ARMv6 and verified as a static ARM EABI5 ELF32 executable.

U1 integration binary SHA-256:

    10fed21ef56fe63859dd82008c9b3bd078a0e61daa9bc91f76c36d92a4799b56

### U1 milestone status

    U1.0 framebuffer access                  PASS
    U1.1 touchscreen event path              PASS
    U1.2 framebuffer page switching          PASS
    U1.3 deterministic native rendering      PASS
    U1.4 native rendering + touch            PASS
    U1.5 exclusive touchscreen ownership     PASS
    U1.6 safe recovery to Android            PASS

### Evidence binaries

Independent framebuffer pan diagnostic:

    SHA-256:
    6f808566e6d1c9aa18b9247f93c62c457daab4c338f157b5cd82bc4afa742748

Framebuffer two-page physical rendering diagnostic:

    SHA-256:
    3ef6e98748a4d3078a3cf6faf264b2a35efbe3bc0b61f9f7f15397f0d84e56f4

These diagnostics established the physical framebuffer/page behavior before the combined U1 integration.

### Safety boundary

U1 does not modify bootloader state, PIT, BML/STL partitioning, EFS, modem state, recovery, kernel images, or Android system files. The native UI operates through the existing framebuffer and touchscreen device interfaces and exits back to Android without persistent system modification.

U1 is now closed. Further work should move to U2 rather than expanding the framebuffer proof-of-concept.


## U2.0 display/session ownership investigation

U2.0 physical diagnostics established that framebuffer access, rendering throughput, page switching, and touchscreen ownership are functional, but the stock Android display stack can still modify the framebuffer while the native test is idle.

### Combined diagnostic measurements

Representative physical measurements on the GT-S5360:

    FB 240x320 virtual=240x640
    FB bpp=32 stride=960
    BENCH fill32: ~195 MiB/s, ~1.50 ms/frame
    BENCH RGB565 -> 2x32: ~2.34–2.45 ms/frame
    BENCH page-switch: ~11.4–12.0 ms/pan
    Touch device: /dev/input/event4 (sec_touchscreen)
    EVIOCGRAB: PASS

These measurements are performance evidence for the current device/toolchain, not fixed architectural limits. The 120x160 logical / 2x scale path remains a candidate and must be benchmarked in the eventual UI implementation.

### Passive ownership test — decisive evidence

The passive test was designed to remove our own framebuffer activity from the observation window:

- unique patterns were written to both virtual framebuffer pages during setup;
- page 0 was selected once;
- during the 60-second observation interval the test performed no framebuffer writes;
- during the observation interval the test performed no FBIOPAN_DISPLAY calls;
- touchscreen interaction was not required.

Observed physical run:

    PASSIVE initial yoffset=0
    PASSIVE page0 checksum: 0x3c5dd9c5
    PASSIVE page1 checksum: 0xdf5bbfc5

At approximately 46.619 seconds:

    PAGE0 CONTENT CHANGE: 0x3c5dd9c5 -> 0x2597e67a
    PAGE1 CONTENT CHANGE: 0xdf5bbfc5 -> 0x9ec5b647

At approximately 46.806 seconds:

    YOFFSET CHANGE: 0 -> 320

The phone display visibly changed at the same time as the framebuffer checksum transition; the user observed an Android/display-generated abstract image replacing the native test pattern. Both virtual pages changed before the visible-page offset changed.

Final passive result:

    checks: 339
    yoffset changes: 1
    page0 content changes: 1
    page1 content changes: 1
    metadata changes: 0
    final yoffset: 320

This is the cleanest U2 evidence so far: an external display/framebuffer owner is modifying framebuffer memory and subsequently changing the visible virtual page even while the native test is not writing or panning the framebuffer.

The exact writer is not yet attributed. The evidence does not by itself prove that a particular Android process, gralloc, SurfaceFlinger component, V3D path, or kernel thread performed the memory writes.

### Android display-stack attribution evidence

Read-only inspection of stock Android shows that system_server (PID 1477) holds four persistent descriptors for:

    /dev/graphics/fb0

It also holds descriptors for:

    /dev/v3d
    /dev/gememalloc
    /dev/input/event4

The four fb0 descriptors were present both before and during the passive test. This shows that Android already had persistent framebuffer access; it was not caused by the native test opening fb0.

The pulled gralloc.default.so contains explicit framebuffer mapping code:

    fb_device_open
    mapFrameBufferLocked
    /dev/graphics/fb%u
    /dev/fb%u
    mmap
    ioctl
    FBIOPUT_VSCREENINFO
    page flipping not supported (yres_virtual=%d, requested=%d)

Its exported symbols include:

    fb_device_open
    mapFrameBufferLocked
    mapBuffer
    gralloc_lock
    gralloc_unlock

This establishes that the stock gralloc implementation can map and manage the framebuffer, but it does not by itself identify the writer responsible for the passive-test transition.

The pulled libsurfaceflinger.so exposes the Android compositor/display path, including:

    SurfaceFlinger::postFramebuffer()
    SurfaceFlinger::handlePageFlip()
    SurfaceFlinger::lockPageFlip()
    SurfaceFlinger::unlockPageFlip()
    DisplayHardware::flip()
    DisplayHardware::getCurrentBufferIndex()
    DisplayHardware::getDisplayBufferAddress()
    DisplayHardware::compositionRequest()
    DisplayHardware::compositionComplete()
    V3D_Compose::Create()

It also references:

    FramebufferNativeWindow::setUpdateRectangle()
    FramebufferNativeWindow::compositionComplete()
    FramebufferNativeWindow::getCurrentBufferIndex()

libsurfaceflinger.so depends on the expected legacy Android graphics stack:

    libhardware.so
    libEGL.so
    libGLESv1_CM.so
    libui.so
    libpixelflinger.so
    libsurfaceflinger_client.so

The earlier source/binary evidence also shows the Samsung/Broadcom LCD path exposes the private dirty-row ioctl 0x46ff. The current binary inspection did not find a literal 0x46ff string in libsurfaceflinger.so; absence of a string is not evidence that the ioctl is unused.

### Current U2.0 status

    Framebuffer mmap/access                 PASS
    32-bit framebuffer throughput           PASS
    RGB565 -> 2x32 rendering benchmark     PASS
    Page switching                          PASS
    Touch discovery                         PASS
    EVIOCGRAB                               PASS
    Passive framebuffer stability           FAIL
    Spontaneous framebuffer content change CONFIRMED
    Spontaneous yoffset change              CONFIRMED
    External display ownership interference CONFIRMED
    Exact writer identity                  NOT YET ATTRIBUTED

### Engineering consequence

U2 should not proceed by blindly adding more UI toolkit code while Android still owns a competing display path.

The next step is a reversible, read-only attribution phase followed by a minimal display/session ownership mechanism. The preferred sequence is:

1. identify the minimum Android display component(s) that must stop drawing;
2. establish a deterministic Totoro display session;
3. verify framebuffer stability again with the passive test;
4. re-run touch ownership and Android restoration tests;
5. only then integrate the reusable Totoro HAL and LVGL 8.3 backend.

Do not kill system_server, zygote, or other Android process groups blindly. Do not change boot/recovery/PIT/EFS/modem/kernel state. Preserve ADB/SSH recovery throughout.

### U2 safety boundary

The U2 diagnostics remain non-persistent bring-up tools. They use the existing framebuffer and touchscreen interfaces and do not modify bootloader state, PIT, BML/STL partitioning, EFS, modem state, recovery, kernel images, or Android system files.
