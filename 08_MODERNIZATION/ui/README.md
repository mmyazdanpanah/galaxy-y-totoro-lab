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
