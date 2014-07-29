#ifndef GRAPHICS_H
#define GRAPHICS_H

uint8_t window_line; // Current window line

/* Initialize graphics
 * returns 1 if successful, 0 otherwise */
int init_gfx();

//Render the row number stored in the LY register
void draw_row();

void output_screen();


#endif /* GRAPHICS_H */

