#include "../include/display.h"
#include "../include/string.h"
#include "../include/printf.h"
#include "../include/font.h"

#define MMIO_BASE       0x3F000000UL
#define MBOX_BASE       (MMIO_BASE + 0xB880)
#define MBOX_READ       ((volatile unsigned int*)(MBOX_BASE + 0x00))
#define MBOX_STATUS     ((volatile unsigned int*)(MBOX_BASE + 0x18))
#define MBOX_WRITE      ((volatile unsigned int*)(MBOX_BASE + 0x20))
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000
#define MBOX_CH_PROP    8

static volatile unsigned int __attribute__((aligned(16))) mbox[36];

static unsigned int*  framebuffer = (unsigned int*)0;
static int            fb_width    = 0;
static int            fb_height   = 0;
static int            fb_pitch    = 0;
static int            cursor_x    = 0;
static int            cursor_y    = 0;
static unsigned int   fg_color    = COLOR_WHITE;
static unsigned int   bg_color    = COLOR_BLACK;

static int mbox_call(unsigned int channel) {
    unsigned int addr = (unsigned int)((unsigned long)&mbox & ~0xF) | (channel & 0xF);
    while (*MBOX_STATUS & MBOX_FULL) {}
    *MBOX_WRITE = addr;
    while (1) {
        while (*MBOX_STATUS & MBOX_EMPTY) {}
        if (*MBOX_READ == addr) return mbox[1] == 0x80000000;
    }
}

void display_init() {
    mbox[0]  = 35 * 4;
    mbox[1]  = 0;
    mbox[2]  = 0x48003; mbox[3]  = 8; mbox[4]  = 8; mbox[5]  = SCREEN_WIDTH;  mbox[6]  = SCREEN_HEIGHT;
    mbox[7]  = 0x48004; mbox[8]  = 8; mbox[9]  = 8; mbox[10] = SCREEN_WIDTH;  mbox[11] = SCREEN_HEIGHT;
    mbox[12] = 0x48009; mbox[13] = 8; mbox[14] = 8; mbox[15] = 0; mbox[16] = 0;
    mbox[17] = 0x48005; mbox[18] = 4; mbox[19] = 4; mbox[20] = 32;
    mbox[21] = 0x48006; mbox[22] = 4; mbox[23] = 4; mbox[24] = 1;
    mbox[25] = 0x40001; mbox[26] = 8; mbox[27] = 8; mbox[28] = 4096; mbox[29] = 0;
    mbox[30] = 0x40008; mbox[31] = 4; mbox[32] = 4; mbox[33] = 0;
    mbox[34] = 0;

    if (mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0) {
        mbox[28] &= 0x3FFFFFFF;
        fb_width    = mbox[5];
        fb_height   = mbox[6];
        fb_pitch    = mbox[33];
        framebuffer = (unsigned int*)(unsigned long)mbox[28];
        display_clear(COLOR_BLACK);
        printf("[DISPLAY] %dx%d pitch=%d fb=%x\n", fb_width, fb_height, fb_pitch, framebuffer);
    } else {
        printf("[DISPLAY] EROARE init!\n");
    }
}

void display_draw_pixel(int x, int y, unsigned int color) {
    if (!framebuffer) return;
    if (x < 0 || x >= fb_width)  return;
    if (y < 0 || y >= fb_height) return;
    unsigned int* row = (unsigned int*)((unsigned char*)framebuffer + y * fb_pitch);
    row[x] = color;
}

void display_draw_rect(int x, int y, int w, int h, unsigned int color) {
    for (int dy = 0; dy < h; dy++)
        for (int dx = 0; dx < w; dx++)
            display_draw_pixel(x + dx, y + dy, color);
}

void display_clear(unsigned int color) {
    if (!framebuffer) return;
    bg_color = color;
    for (int y = 0; y < fb_height; y++)
        for (int x = 0; x < fb_width; x++)
            display_draw_pixel(x, y, color);
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

static void scroll() {
    if (!framebuffer) return;
    unsigned char* fb = (unsigned char*)framebuffer;
    for (int y = 0; y < fb_height - CHAR_HEIGHT; y++) {
        for (int x = 0; x < fb_pitch; x++) {
            fb[y * fb_pitch + x] = fb[(y + CHAR_HEIGHT) * fb_pitch + x];
        }
    }
    for (int y = fb_height - CHAR_HEIGHT; y < fb_height; y++)
        for (int x = 0; x < fb_width; x++)
            display_draw_pixel(x, y, bg_color);
}

void display_putc(char c) {
    if (!framebuffer) return;
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= ROWS) { scroll(); cursor_y = ROWS - 1; }
        return;
    }
    if (c == '\r') { cursor_x = 0; return; }
    if (c < 0 || c >= 128) c = '?';

    int px = cursor_x * CHAR_WIDTH;
    int py = cursor_y * CHAR_HEIGHT;

    for (int row = 0; row < CHAR_HEIGHT; row++) {
        unsigned char bits = font_8x16[(int)c][row];
        for (int col = 0; col < CHAR_WIDTH; col++) {
            unsigned int color = (bits & (0x80 >> col)) ? fg_color : bg_color;
            display_draw_pixel(px + col, py + row, color);
        }
    }

    cursor_x++;
    if (cursor_x >= COLS) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= ROWS) { scroll(); cursor_y = ROWS - 1; }
    }
}

void display_puts(const char* s) {
    while (*s) display_putc(*s++);
}
