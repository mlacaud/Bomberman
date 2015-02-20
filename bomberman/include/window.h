#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>

void window_create(int width, int height);
void window_free();

void window_refresh();

// display a SDL_Surface at location (x,y)
void window_display_image(SDL_Surface* surface, int x, int y);

// set every pixel of the window to white
void window_clear();

// Display a picture if the player win
void game_finish();

// Display a picture if the player loose
void game_over();

#endif /* WINDOW_H_ */
