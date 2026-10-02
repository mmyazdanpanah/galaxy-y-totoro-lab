#ifndef TOTORO_LINUX_UAPI_H
#define TOTORO_LINUX_UAPI_H

#include <stdint.h>

/*
 * Linux 2.6-era framebuffer userspace ABI.
 *
 * These definitions intentionally mirror the BCM21553 kernel's
 * linux/fb.h userspace-visible structures. Do not replace with
 * host/macOS framebuffer definitions.
 */

#define FBIOGET_VSCREENINFO 0x4600
#define FBIOGET_FSCREENINFO 0x4602

struct fb_bitfield {
    uint32_t offset;
    uint32_t length;
    uint32_t msb_right;
};

struct fb_fix_screeninfo {
    char id[16];
    unsigned long smem_start;
    uint32_t smem_len;
    uint32_t type;
    uint32_t type_aux;
    uint32_t visual;
    uint16_t xpanstep;
    uint16_t ypanstep;
    uint16_t ywrapstep;
    uint32_t line_length;
    unsigned long mmio_start;
    uint32_t mmio_len;
    uint32_t accel;
    uint16_t reserved[3];
};

struct fb_var_screeninfo {
    uint32_t xres;
    uint32_t yres;
    uint32_t xres_virtual;
    uint32_t yres_virtual;
    uint32_t xoffset;
    uint32_t yoffset;

    uint32_t bits_per_pixel;
    uint32_t grayscale;

    struct fb_bitfield red;
    struct fb_bitfield green;
    struct fb_bitfield blue;
    struct fb_bitfield transp;

    uint32_t nonstd;
    uint32_t activate;

    uint32_t height;
    uint32_t width;

    uint32_t accel_flags;

    uint32_t pixclock;
    uint32_t left_margin;
    uint32_t right_margin;
    uint32_t upper_margin;
    uint32_t lower_margin;
    uint32_t hsync_len;
    uint32_t vsync_len;

    uint32_t sync;
    uint32_t vmode;
    uint32_t rotate;

    uint32_t colorspace;
    uint32_t reserved[4];
};

/*
 * Linux input-event ABI on the Totoro's 32-bit ARM kernel.
 */
struct input_event {
    long tv_sec;
    long tv_usec;
    uint16_t type;
    uint16_t code;
    int32_t value;
};

#define EV_SYN              0x00
#define EV_ABS              0x03

#define SYN_REPORT          0x00

#define ABS_MT_TOUCH_MAJOR  0x30
#define ABS_MT_TOUCH_MINOR  0x32
#define ABS_MT_POSITION_X   0x35
#define ABS_MT_POSITION_Y   0x36

#endif
