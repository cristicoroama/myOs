#include "../include/display.h"
#include "../include/string.h"
#include "../include/printf.h"

#define MMIO_BASE       0x3F000000
#define MBOX_BASE       (MMIO_BASE + 0xB880)
#define MBOX_READ       ((volatile unsigned int*)(MBOX_BASE + 0x00))
#define MBOX_STATUS     ((volatile unsigned int*)(MBOX_BASE + 0x18))
#define MBOX_WRITE      ((volatile unsigned int*)(MBOX_BASE + 0x20))
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000
#define MBOX_CH_PROP    8

static unsigned int*  framebuffer = (unsigned int*)0;
static int            fb_width    = 0;
static int            fb_height   = 0;
static int            cursor_x    = 0;
static int            cursor_y    = 0;
static unsigned int   fg_color    = COLOR_WHITE;
static unsigned int   bg_color    = COLOR_BLACK;

static unsigned char font_8x16[128][16] = {0};

static void font_init() {
    font_8x16['A'][0]  = 0x18;
    font_8x16['A'][1]  = 0x3C;
    font_8x16['A'][2]  = 0x66;
    font_8x16['A'][3]  = 0x66;
    font_8x16['A'][4]  = 0x7E;
    font_8x16['A'][5]  = 0x66;
    font_8x16['A'][6]  = 0x66;
    font_8x16['A'][7]  = 0x00;
}

static void mbox_write(unsigned int val, unsigned int channel) {
    while (*MBOX_STATUS & MBOX_FULL) {}
    *MBOX_WRITE = (val & ~0xF) | (channel & 0xF);
}

static unsigned int mbox_read(unsigned int channel) {
    while (1) {
        while (*MBOX_STATUS & MBOX_EMPTY) {}
        unsigned int val = *MBOX_READ;
        if ((val & 0xF) == channel) return val & ~0xF;
    }
}

void display_init() {
    volatile unsigned int __attribute__((aligned(16))) mbox[36];

    mbox[0]  = 35 * 4;
    mbox[1]  = 0;
    mbox[2]  = 0x00048003; mbox[3]  = 8; mbox[4]  = 0; mbox[5]  = SCREEN_WIDTH;  mbox[6]  = SCREEN_HEIGHT;
    mbox[7]  = 0x00048004; mbox[8]  = 8; mbox[9]  = 0; mbox[10] = SCREEN_WIDTH;  mbox[11] = SCREEN_HEIGHT;
    mbox[12] = 0x00048005; mbox[13] = 4; mbox[14] = 0; mbox[15] = 32;
    mbox[16] = 0x00048006; mbox[17] = 4; mbox[18] = 0; mbox[19] = 1;
    mbox[20] = 0x00040001; mbox[21] = 8; mbox[22] = 0; mbox[23] = 16; mbox[24] = 0;
    mbox[25] = 0;

    unsigned int addr = (unsigned int)(unsigned long)mbox;
    mbox_write(addr, MBOX_CH_PROP);
    mbox_read(MBOX_CH_PROP);

    if (mbox[23]) {
        framebuffer = (unsigned int*)(unsigned long)(mbox[23] & 0x3FFFFFFF);
        fb_width    = SCREEN_WIDTH;
        fb_height   = SCREEN_HEIGHT;
        font_init();
        display_clear(COLOR_BLACK);
        printf("[DISPLAY] Framebuffer: %dx%d @ %x\n", fb_width, fb_height, framebuffer);
    } else {
        printf("[DISPLAY] EROARE: framebuffer init esuat!\n");
    }
}

void display_draw_pixel(int x, int y, unsigned int color) {
    if (!framebuffer) return;
    if (x < 0 || x >= fb_width)  return;
    if (y < 0 || y >= fb_height) return;
    framebuffer[y * fb_width + x] = color;
}

void display_draw_rect(int x, int y, int w, int h, unsigned int color) {
    for (int dy = 0; dy < h; dy++)
        for (int dx = 0; dx < w; dx++)
            display_draw_pixel(x + dx, y + dy, color);
}

void display_clear(unsigned int color) {
    if (!framebuffer) return;
    for (int y = 0; y < fb_height; y++)
        for (int x = 0; x < fb_width; x++)
            framebuffer[y * fb_width + x] = color;
    cursor_x = 0;
    cursor_y = 0;
}

void display_set_color(unsigned int fg, unsigned int bg) {
    fg_color = fg;
    bg_color = bg;
}

void display_gotoxy(int x, int y) {
    cursor_x = x;
    cursor_y = y;
}

void display_putc(char c) {
    if (!framebuffer) return;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= ROWS) cursor_y = 0;
        return;
    }

    if (c == '\r') {
        cursor_x = 0;
        return;
    }

    int px = cursor_x * CHAR_WIDTH;
    int py = cursor_y * CHAR_HEIGHT;

    for (int row = 0; row < CHAR_HEIGHT; row++) {
        unsigned char bits = (c < 128) ? font_8x16[(int)c][row] : 0;
        for (int col = 0; col < CHAR_WIDTH; col++) {
            unsigned int color = (bits & (0x80 >> col)) ? fg_color : bg_color;
            display_draw_pixel(px + col, py + row, color);
        }
    }

    cursor_x++;
    if (cursor_x >= COLS) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= ROWS) cursor_y = 0;
    }
}

void display_puts(const char* s) {
    while (*s) display_putc(*s++);
}
