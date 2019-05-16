#ifndef __VGA_H__
#define __VGA_H__

#define MAX_ROW 25
#define MAX_COL 80

void initVga();
void clearScreen();
void updateCursor(int row, int col);
void scrollScreen();

#endif
