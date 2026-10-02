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
