#ifndef DISPLAY_H
#define DISPLAY_H

#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   768
#define CHAR_WIDTH      8
#define CHAR_HEIGHT     16
#define COLS            (SCREEN_WIDTH  / CHAR_WIDTH)
#define ROWS            (SCREEN_HEIGHT / CHAR_HEIGHT)

#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0xFFFFFFFF
#define COLOR_RED       0xFFFF0000
#define COLOR_GREEN     0xFF00FF00
#define COLOR_BLUE      0xFF0000FF
#define COLOR_YELLOW    0xFFFFFF00
#define COLOR_CYAN      0xFF00FFFF

void display_init();
void display_clear(unsigned int color);
void display_putc(char c);
void display_puts(const char* s);
void display_set_color(unsigned int fg, unsigned int bg);
void display_gotoxy(int x, int y);
void display_draw_pixel(int x, int y, unsigned int color);
void display_draw_rect(int x, int y, int w, int h, unsigned int color);

#endif
