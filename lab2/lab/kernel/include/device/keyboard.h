#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define MAX_KEYBUFFER_SIZE 256

void initKeyTable();

uint32_t getKeyCode();

char getChar(uint32_t code);

#endif
