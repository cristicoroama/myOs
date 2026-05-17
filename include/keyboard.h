#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEY_ENTER   '\r'
#define KEY_BACKSP  127
#define KEY_UP      0x41
#define KEY_DOWN    0x42
#define KEY_RIGHT   0x43
#define KEY_LEFT    0x44
#define KEY_ESC     0x1B

void keyboard_init();
char keyboard_getc();
int  keyboard_available();

#endif
